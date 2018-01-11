//
// Created by me on 1/9/18.
//

#ifndef LIBSPARSETENSOR_HYPERTRIE_HPP
#define LIBSPARSETENSOR_HYPERTRIE_HPP


#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include "boost/variant.hpp"


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
    uint8_t total;
    map<uint64_t, variant<HyperTrie *, T>> *edges_by_pos[];
    T leafsum;
    uint64_t leafcount;

public:
    T get(vector<uint64_t> &coords) {
        return nullptr;
    }

    void set(vector<uint64_t> &coords, T &value) {

    }

    void del(vector<uint64_t> &coords) {

    }

private:
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
        auto edges = this->edges_by_pos[pos];
        // if the map for pos doesn't exist create it.
        if (edges == nullptr)
            return {};
        else
            return {edges};
    }

    /**
     * Add a child to the hypertrie.
     * @param pos The key position where the child should be added.
     * @param key_part The key_part to be used at the key position pos.
     * @param value The sub-hypertrie or T to be set.
     * @return Optional Difference between old and new value if this-> depth is 1.
     */
    optional<T> setChild(uint8_t &pos, u_int64_t key_part, optional<variant<HyperTrie *, T>> &value) {
        auto edge = get_or_create_edges(pos);
        if (this->depth > 1) {
            if (not value) {
                variant<HyperTrie *, T> child{new HyperTrie(this->depth - uint8_t(1))};
                (*edge)[key_part] = child;
            } else {
                (*edge)[key_part] = *value;
            }
            return std::nullopt;
        } else {
            auto old_value = (*edge)[key_part];
            (*edge)[key_part] = *value;

            if (old_value == nullptr)
                return *value;
            else
                return *value - std::get<T>(old_value);
        }
    }

    optional<variant<HyperTrie *, T>> getChild(uint8_t &pos, u_int64_t key_part) {
        optional<map<uint64_t, variant<HyperTrie *, T>> *> edges_ = get_edges(pos);
        if (edges_)
            return {(*edges_)[key_part]}; // TODO: does optional(nullptr) form a nullopt?
        else
            return {};
    }


};


template<typename T>
HyperTrie<T>::HyperTrie(uint8_t depth) : depth(depth) {}


#endif //LIBSPARSETENSOR_HYPERTRIE_HPP
