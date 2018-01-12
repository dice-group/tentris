//
// Created by me on 11.01.18.
//

#ifndef LIBSPARSETENSOR_POSCALC_HPP
#define LIBSPARSETENSOR_POSCALC_HPP


#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include <unordered_map>
#include <stdint-gcc.h>
#include "boost/variant.hpp"
#include "HyperTrie.hpp"

using std::vector;
using std::map;

/**
 * Provides translation between position in superkeys with subkeys. (1,2,4) for example is a subkey of (1,2,3,4).
 * And the other way around.
 */
class PosCalc {
    template<typename T>
    friend
    class HyperTrie;

    /**
     * Holds all instances.
     */
    inline static map<vector<bool>, PosCalc *> instances{};

    /**
     * Private construcor.
     * @param key_length length of superkeys.
     */
    explicit PosCalc(const uint8_t &key_length) : key_length(key_length) {}

    /**
     * Length of superkeys.
     */
    uint8_t key_length;
    /**
     * Bitmask for position of the superkeys that are already used and now gone in the subkey.
     */
    vector<bool> used_positions = vector<bool>(key_length);
    /**
     * Difference between the superkeys's and subkeys's key position.
     */
    vector<uint8_t> super_to_sub_diff = vector<uint8_t>(key_length);

    /**
     * Cache for PosCalcs of subkeys that are 1 shorter.
     */
    vector<PosCalc *> next_pos_calcs = vector<PosCalc *>(key_length);

public:

    /**
     * Convert a superkey position to a subkey key position.
     * @param key_pos superkey position
     * @return subkey position
     */
    inline uint8_t key_to_trie_pos(const uint8_t key_pos) const {
        return key_pos - super_to_sub_diff[key_pos];
    }

    /**
     * Convert a subkey position to a superkey position.
     * @param subkey_pos subkey position
     * @return superkey position
     */
    inline uint8_t trie_to_key_pos(const uint8_t subkey_pos) const {
        return subkey_pos + super_to_sub_diff[subkey_pos];
    }

    /**
     * PosCalcs of subkeys that has one position less.
     * @param key_pos position of superkey to be removed.
     * @return PosCalc like this but without position key_pos.
     */
    inline PosCalc *used(const uint8_t key_pos) {
        PosCalc *child = next_pos_calcs.at(key_pos);
        if (child == nullptr) {
            vector<bool> next_used_pos = this->used_positions;
            next_used_pos[key_pos] = 1;
            child = getInstance(next_used_pos);
            next_pos_calcs[key_pos] = child;
        }
        return child;
    }

    /**
     * Get an instance for a vector of used positions.
     * @param used_pos bit vector o used positions.
     * @return an instance
     */
    static PosCalc *getInstance(const vector<bool> &used_pos) {
        auto instance_ = instances.find(used_pos);
        if (instance_ != instances.end()) {
            return instance_->second;
        } else {
            PosCalc *instance = new PosCalc(uint8_t(used_pos.size()));

            for (uint8_t key_pos = 0; key_pos < used_pos.size(); key_pos++) {

                if (used_pos[key_pos]) {
                    instance->used_positions[key_pos] = true;
                    for (uint8_t i = key_pos; i < instance->key_length; i++) {
                        instance->super_to_sub_diff[i] += 1;
                    }
                }
            }
            instances.insert_or_assign(used_pos, instance);
            return instance;
        }
    }

};


#endif //LIBSPARSETENSOR_POSCALC_HPP
