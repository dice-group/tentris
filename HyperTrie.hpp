//
// Created by me on 1/9/18.
//

#ifndef LIBSPARSETENSOR_HYPERTRIE_HPP
#define LIBSPARSETENSOR_HYPERTRIE_HPP


#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include <unordered_map>
#include "boost/variant.hpp"
#include "PosCalc.hpp"


using std::vector;
using std::variant;
using std::map;
using std::optional;

template<typename T>
class HyperTrie {

public:
    HyperTrie(uint8_t depth);

private:
    uint8_t depth;
    T leafsum;
    uint64_t leafcount;
    vector<map<uint64_t, variant<HyperTrie *, T>> *> edges_by_pos = vector<map<uint64_t, variant<HyperTrie *, T>> *>(depth);
    // TODO: must get smaller

    /**
     * Get edges map by pos. If the map doesn't exist so far it is created.
     * @param pos position of the next coordinate. MUST be in range [0,self->depth).
     * @return a map that maps the used key_parts at pos of a key to sub-hypertries or T entries.
     */
    map<uint64_t, variant<HyperTrie *, T>> *get_or_create_edges(uint8_t pos) noexcept {
        auto edges = this->edges_by_pos[pos];
        // if the map for pos doesn't exist create it.
        if (edges == nullptr) {
            edges = new map<uint64_t, variant<HyperTrie *, T>>();
            this->edges_by_pos[pos] = edges;
        }
        return edges;
    }

    /**
     * Get edges map by pos.
     * @param pos position of the next coordinate. MUST be in range [0,self->depth).
     * @return optional if it exists: a map that maps the used key_parts at pos of a key to sub-hypertries or T entries
     */
    optional<map<uint64_t, variant<HyperTrie *, T>> *> get_edges(uint8_t pos) noexcept {
        map<uint64_t, variant<HyperTrie *, T>> *edges = this->edges_by_pos[pos];
        // if the map for pos doesn't exist create it.
        if (edges == nullptr)
            return std::nullopt;
        else
            return {edges};
    }

    /**
     * Add a child to the hypertrie.
     * @param pos The key position where the child should be added.
     * @param key_part The key_part to be used at the key position pos.
     * @param value The sub-hypertrie or T to be set.
     * @return Optional Difference between old and new value if this-> depth is 1. TODO!
     */
    variant<HyperTrie *, T> setChild(uint8_t pos, uint64_t key_part, optional<variant<HyperTrie *, T>> &value) {
        auto edge = get_or_create_edges(pos);
        if (this->depth > 1) {
            if (not value) {
                variant<HyperTrie *, T> child{new HyperTrie(this->depth - uint8_t(1))};
                (*edge)[key_part] = child;
                return child;
            } else {
                (*edge)[key_part] = *value;
                return *value;
            }
        } else {
            auto old_value_ = (*edge).find(key_part);
            (*edge)[key_part] = *value;

            if (old_value_ == (*edge).end())
                return *value;
            else {
                T new_value = std::get<T>(*value);
                T old_value = std::get<T>(old_value_->second);
                T value_diff = new_value - old_value;
                return {value_diff};
            }
        }
    }

    /**
     * Get a child by key_part position and key_part value.
     * @param pos The key position where to look for the child.
     * @param key_part The key_part identifing the child.
     * @return Optional the child if it exists. If this->depth is 1 the child is of value T otherwise it is a HyperTrie.
     */
    optional<variant<HyperTrie *, T>> getChild(uint8_t pos, uint64_t key_part) {
        optional<map<uint64_t, variant<HyperTrie *, T>> *> edges_ = get_edges(pos);
        if (edges_) {
            map<uint64_t, variant<HyperTrie *, T>> *edges = *edges_;
            auto child_ = edges->find(key_part); // TODO: fails here
            if (child_ != edges->end()) {
                return {child_->second};
            }
        }
        return std::nullopt;
    }

    void delChild(uint8_t &pos, uint64_t key_part) {
        optional<map<uint64_t, variant<HyperTrie *, T>> *> edges_ = get_edges(pos);
        if (edges_); // TODO: implement delete
    }

public:

    optional<variant<HyperTrie *, T>> get(vector<uint64_t> &coords) {
        // TODO: optimize access order
        HyperTrie *current_trie = this;
        for (uint8_t pos = 0; pos < coords.size(); pos++) {
            const optional<variant<HyperTrie *, T>> &child_ = current_trie->getChild(uint8_t(0), coords[pos]);
            if (child_) {
                if (pos != coords.size() - 1)
                    current_trie = std::get<HyperTrie *>(*child_);
                else
                    return std::get<T>(*child_);
            } else
                return {};
        }
        return {}; // TODO: remove
    }

private:

    void set_rek(vector<uint64_t> &coords, T &newValue, bool hasOldValue, T &leafSumDiff,
                 std::unordered_map<std::vector<bool>, HyperTrie *> &finished_subtries,
                 PosCalc *pos_calc) {
        this->leafsum += leafSumDiff;
        this->leafcount += not hasOldValue;

        finished_subtries[pos_calc->removed_positions] = this;

        // subtrie has only one position left: insert value
        if (pos_calc->subkey_length == 1) {
            uint64_t key_part = coords[pos_calc->subkey_to_key_pos(0)];

            optional<variant<HyperTrie *, T>> value_ = optional < variant<HyperTrie *, T>>
            { variant<HyperTrie *, T>{newValue}};

            this->setChild(0, key_part, value_);
        } else {
            for (uint8_t pos : pos_calc->subkey_to_key) {
                uint64_t key_part = coords[pos];
                PosCalc *const next_pos_calc = pos_calc->use(pos);
                auto finished_child = finished_subtries.find(next_pos_calc->removed_positions);

                optional<variant<HyperTrie *, T>> child_ = getChild(pos_calc->key_to_subkey_pos(pos), key_part);
                if (not child_) {
                    if (finished_child != finished_subtries.end()) {
                        variant<HyperTrie *, T> value{finished_child->second};
                        optional<variant<HyperTrie *, T>> value_{value};
                        this->setChild(pos_calc->key_to_subkey_pos(pos), key_part, value_);
                    } else {
                        optional<variant<HyperTrie *, T>> value_ = std::nullopt;
                        variant<HyperTrie *, T> new_child = this->setChild(pos_calc->key_to_subkey_pos(pos), key_part,
                                                                           value_);
                        HyperTrie *child = std::get<HyperTrie *>(new_child);
                        child->set_rek(coords, newValue, hasOldValue, leafSumDiff, finished_subtries, next_pos_calc);
                    }
                } else {
                    if (finished_child != finished_subtries.end()) {
                        HyperTrie *child = std::get<HyperTrie *>(*child_);
                        child->set_rek(coords, newValue, hasOldValue, leafSumDiff, finished_subtries, next_pos_calc);
                    }
                }
            }
        }
    }

public:
    void set(vector<uint64_t> &coords, T &value) {
        optional<variant<HyperTrie *, T>> oldValue_ = get(coords);
        bool hasOldValue = false;
        T oldValue;
        if (oldValue_) {
            hasOldValue = true;
            T oldValue = std::get<T>(*oldValue_);
        }

        T leafsumDiff = value ? hasOldValue : value - oldValue;

        std::unordered_map<std::vector<bool>, HyperTrie *> finished_subtries{};

        vector<bool> subkey_mask(coords.size());
        PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

        set_rek(coords, value, hasOldValue, leafsumDiff, finished_subtries, pos_calc);
    }

    void del(vector<uint64_t> &coords) {
        throw "Not yet implemented.";
    }

};


template<typename T>
HyperTrie<T>::HyperTrie(uint8_t depth) : depth(depth) {}


#endif //LIBSPARSETENSOR_HYPERTRIE_HPP
