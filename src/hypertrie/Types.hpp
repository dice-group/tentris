//
// Created by me on 2/2/18.
//

#ifndef SPARSETENSOR_HYPERTRIE_TYPES_HPP
#define SPARSETENSOR_HYPERTRIE_TYPES_HPP

#include <cstdint>
#include <vector>

namespace sparsetensor::hypertrie {

// TODO: document
    typedef uint8_t key_pos_t;

    typedef std::vector<bool> subkey_mask_t;

    typedef uint64_t key_part_t;
#define KEY_PART_MAX (UINT64_MAX)
#define KEY_PART_MIN (UINT64_MIN)
}

#endif //SPARSETENSOR_HYPERTRIE_TYPES_HPP
