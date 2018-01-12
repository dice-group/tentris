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

using std::vector;
using std::map;

class PosCalc {
    inline static map<vector<bool>, PosCalc> instances{};
    uint8_t key_length;
    std::vector<bool> used_pos{key_length};
    std::vector<uint8_t> key_to_trie{key_length};
    std::vector<uint8_t> trie_to_key{key_length};

public:
    PosCalc used(uint8_t key_pos) {
        auto next_used_pos = used_pos;
        next_used_pos[key_pos] = 1;
        return getInstance(used_pos);
    }

public:
    PosCalc(const uint8_t &key_length) : key_length(key_length) {}


    uint8_t key_to_trie_pos(uint8_t key_pos) {
        return key_to_trie[key_pos];
    }

    uint8_t trie_to_key_pos(uint8_t trie_pos) {
        return trie_to_key[trie_pos];
    }

    static PosCalc getInstance(std::vector<bool> &used_pos) {
        auto instance_ = instances.find(used_pos);
        if (instance_ != instances.end()) {
            return instance_->second;
        } else {
            PosCalc instance{uint8_t(used_pos.size())};

            for (uint8_t key_pos = 0; key_pos < used_pos.size(); key_pos++) {

                if (used_pos[key_pos]) {
                    for (uint8_t i = key_pos; i < instance.key_length; i++) {
                        instance.key_to_trie[i] -= 1;
                        instance.key_to_trie[i] += 1;
                    }
                }
            }
            instances[used_pos] = instance;
            return instance;
        }
    }

};


#endif //LIBSPARSETENSOR_POSCALC_HPP
