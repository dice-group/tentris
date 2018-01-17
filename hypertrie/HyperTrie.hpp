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
    explicit HyperTrie(uint8_t depth);

    /**
     * Depth is the length of the keys.
     */
    uint8_t depth;

    /**
     * Sum of all entries.
     */
    T leafsum{};

    /**
     * Number of entries.
     */
    uint64_t leafcount{};

private:
    /**
     * edges_by_pos: subkey_pos -> edges;<br/>
     * edges: key_part -> subtrie OR value
     */
    vector<map<uint64_t, variant<HyperTrie *, T>> *> edges_by_pos =
            vector<map<uint64_t, variant<HyperTrie *, T>> *>(depth);

    /**
     * Get a pointer to the edges map by subkey_pos. If the map doesn't exist so far it is created.
     * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
     * @return a pointer to a map that maps the used key_parts at subkey_pos of a subkey to sub-hypertries or T entries.
     */
    map<uint64_t, variant<HyperTrie *, T>> *getOrCreateEdges(uint8_t subkey_pos) noexcept {
        auto edges = this->edges_by_pos[subkey_pos];
        // if the map for subkey_pos doesn't exist create it.
        if (edges == nullptr) {
            edges = new map<uint64_t, variant<HyperTrie *, T>>();
            this->edges_by_pos[subkey_pos] = edges;
        }
        return edges;
    }

    /**
     * Get pointer to edges map by subkey_pos.
     * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
     * @return optional if it exists: a map that maps the used key_parts at pos of a key to sub-hypertries or T entries
     */
    optional<map<uint64_t, variant<HyperTrie *, T>> *> getEdges(uint8_t subkey_pos) noexcept {
        map<uint64_t, variant<HyperTrie *, T>> *edges = this->edges_by_pos[subkey_pos];
        // if the map for subkey_pos doesn't exist create it.
        if (edges == nullptr)
            return std::nullopt;
        else
            return {edges};
    }

    /**
     * Add a child or a value to the hypertrie.
     * @param pos The key position where the child should be added.
     * @param key_part The key_part to be used at the key position pos.
     * @param value The sub-hypertrie or T to be set.
     * @return Optional Difference between old and new value if this-> depth is 1. TODO!
     */
    variant<HyperTrie *, T> setChild(uint8_t pos, uint64_t key_part, optional<variant<HyperTrie *, T>> &value) {
        auto edge = getOrCreateEdges(pos);

        // depth > 1 means it is an inner node. So a sub-HyperTrie is added not a T value.
        if (this->depth > 1) {
            // no value was passed. So create a new sub HyperTrie of depth this->depth -1 and add it.
            if (not value) {
                variant<HyperTrie *, T> child{new HyperTrie(this->depth - uint8_t(1))};
                (*edge)[key_part] = child;
                return child;
            } // a value was passed. Add it.
            else {
                (*edge)[key_part] = *value;
                return *value;
            }
        } // depth == 1 means it is a leaf node. So Values are directly assigned to key_parts
        else {
            // look current entry up.
            auto old_value_ = (*edge).find(key_part);
            // write new value
            (*edge)[key_part] = *value;

            // if old value didn't exist return new value
            if (old_value_ == (*edge).end()) {
                return *value;
            } // if old value existed return difference.
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
     * @param subkey_pos The key position where to look for the child.
     * @param key_part The key_part identifing the child.
     * @return Optional the child if it exists. If this->depth is 1 the child is of value T otherwise it is a HyperTrie.
     */
    optional<variant<HyperTrie *, T>> getChild(uint8_t subkey_pos, uint64_t key_part) {
        optional<map<uint64_t, variant<HyperTrie *, T>> *> edges_ = getEdges(subkey_pos);
        // check if HyperTrie is emtpy.
        if (edges_) {
            map<uint64_t, variant<HyperTrie *, T>> *edges = *edges_;
            // find child
            auto child_ = edges->find(key_part);
            // return it if it exist
            if (child_ != edges->end()) {
                return {child_->second};
            }
        }
        // in any other case return that no child exists
        return std::nullopt;
    }

    void delChild(uint8_t &pos, uint64_t key_part) {
        optional<map<uint64_t, variant<HyperTrie *, T>> *> edges_ = getEdges(pos);
        throw "Not yet implemented.";
    }

public:

    /**
     * Get an value or SubTrie by a key.
     * @param key Vector of uint64 coordinates.
     * @return a SubTrie or a value depending on the length of the key.
     */
    optional<variant<HyperTrie *, T>> get(vector<uint64_t> &key) {
        // TODO: optimize access order
        HyperTrie *current_trie = this;
        for (uint8_t pos = 0; pos < key.size(); pos++) {
            const optional<variant<HyperTrie *, T>> &child_ = current_trie->getChild(uint8_t(0), key[pos]);
            // child exists
            if (child_) {
                if (pos != key.size() - 1) { // leaf
                    current_trie = std::get<HyperTrie *>(*child_);
                } else { // inner node
                    return {*child_};
                }
            } else
                return std::nullopt;
        }
        // if key is empty return this
        return {variant<HyperTrie *, T>{current_trie}};
    }

private:
    /**
     * Inserts and links the HyperTrie nodes recursively.
     * @param key key to save the value for
     * @param new_value the new value to be safed.
     * @param has_old_value if there was an value for this key before.
     * @param value_diff difference beween former value and current value
     * @param finished_subtries map of finished sub HyperTries
     * @param pos_calc PosCalc object for the current sub HyperTrie
     */
    void set_rek(vector<uint64_t> &key, T &new_value, bool has_old_value, T &value_diff,
                 std::unordered_map<std::vector<bool>, HyperTrie *> &finished_subtries, PosCalc *pos_calc) {
        // update this node
        this->leafsum += value_diff;
        this->leafcount += not has_old_value;

        // add it to the finished ( means updated ) nodes.
        finished_subtries[pos_calc->removed_positions] = this;

        // subtrie has only one position left: insert value
        if (pos_calc->subkey_length == 1) {
            uint64_t key_part = key[pos_calc->subkey_to_key_pos(0)];

            optional<variant<HyperTrie *, T>> value_ = optional < variant<HyperTrie *, T>>
            { variant<HyperTrie *, T>{new_value}};

            this->setChild(0, key_part, value_);
        } else { // depth > 1 -> inner node
            // a child must be set or updated for every subkey_pos available.
            for (uint8_t key_pos : pos_calc->subkey_to_key) {
                uint64_t key_part = key[key_pos];

                // get pos_calc for next child and check if it was already updated earlier.
                PosCalc *const next_pos_calc = pos_calc->use(key_pos);
                auto finished_child = finished_subtries.find(next_pos_calc->removed_positions);

                // get the child at the current position.
                optional<variant<HyperTrie *, T>> child_ = getChild(pos_calc->key_to_subkey_pos(key_pos), key_part);

                // the child exists ...
                if (child_) {
                    // ... and the subtrie starting with the child was not already finished:
                    if (finished_child == finished_subtries.end()) {
                        // call this function for the child
                        HyperTrie *child = std::get<HyperTrie *>(*child_);
                        child->set_rek(key, new_value, has_old_value, value_diff, finished_subtries, next_pos_calc);

                    }

                } else { // the child does not exist ...
                    // ... and the subtrie starting with the child was already finished:
                    if (finished_child != finished_subtries.end()) {
                        // set the child at this node
                        variant<HyperTrie *, T> value{finished_child->second};
                        optional<variant<HyperTrie *, T>> value_{value};
                        this->setChild(pos_calc->key_to_subkey_pos(key_pos), key_part, value_);

                    } else { // ... and the subtrie starting with the child was not already finished:
                        // set a new child and call this function for the child
                        optional<variant<HyperTrie *, T>> value_ = std::nullopt;
                        variant<HyperTrie *, T> new_child = this->setChild(pos_calc->key_to_subkey_pos(key_pos),
                                                                           key_part, value_);
                        HyperTrie *child = std::get<HyperTrie *>(new_child);
                        child->set_rek(key, new_value, has_old_value, value_diff, finished_subtries, next_pos_calc);
                    }
                }
            }
        }
    }

public:
    /**
     * Set a value for a key.
     * @param key key to the value
     * @param value value to be set
     */
    void set(vector<uint64_t> &key, T &value) {
        if (key.size() != this->depth) {
            throw "Key length must match HyperTrie->depth";
        }
        // check if there is already another value for this key.
        optional<variant<HyperTrie *, T>> oldValue_ = get(key);
        bool has_old_value = false;
        T oldValue;
        if (oldValue_) {
            has_old_value = true;
            oldValue = std::get<T>(*oldValue_);
            // when the value doesn't we don't need to do anything.
            if (oldValue == value) {
                return;
            }
        }

        // calculate value difference.
        T value_diff = has_old_value ? (value - oldValue) : value;

        // cache for already created sub HyperTries.
        std::unordered_map<std::vector<bool>, HyperTrie *> finished_subtries{};

        // get pos_calc for this.
        vector<bool> subkey_mask(key.size());
        PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

        // store key recursively
        set_rek(key, value, has_old_value, value_diff, finished_subtries, pos_calc);
    }

    void del(vector<uint64_t> &coords) {
        throw "Not yet implemented.";
    }

};


template<typename T>
HyperTrie<T>::HyperTrie(uint8_t depth) : depth(depth) {}


#endif //LIBSPARSETENSOR_HYPERTRIE_HPP
