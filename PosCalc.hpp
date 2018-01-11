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
#include "boost/variant.hpp"

class PosCalc {
    uint8_t key_length;
    uint8_t trie_depth;
    std::vector<bool> previous_pos{key_length};
    std::vector<uint8_t> key_to_trie{key_length};
    std::vector<uint8_t> trie_to_key{trie_depth};

public:
    void used(uint8_t key_pos) {
        for (uint8_t i = key_pos + uint8_t(1); i < key_length; i++) {
            key_to_trie[i] -= 1;
        }
        trie_depth -= 1;
        for (uint8_t i = key_pos; i < trie_depth; i++) {
            key_to_trie[i] += 1;
        }
    }

    uint8_t key_to_trie_pos(uint8_t key_pos) {
        return key_to_trie[key_pos];
    }

    uint8_t trie_to_key_pos(uint8_t trie_pos) {
        return trie_to_key[trie_pos];
    }

};


#endif //LIBSPARSETENSOR_POSCALC_HPP
