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
    std::vector<bool> previous_pos{key_length};
    std::vector<uint8_t> pos_diff{key_length};

    uint8_t key_to_trie_pos(uint8_t key_pos){
        return key_pos - pos_diff[key_pos];
    }

    uint8_t trie_to_key_pos(uint8_t trie_pos){

        return key_pos - pos_diff[key_pos];
    }

};


#endif //LIBSPARSETENSOR_POSCALC_HPP
