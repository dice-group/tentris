//
// Created by me on 2/2/18.
//

#ifndef SPARSETENSOR_HYPERTRIE_TYPES_HPP
#define SPARSETENSOR_HYPERTRIE_TYPES_HPP

#include <cstdint>
#include <vector>

namespace sparsetensor::hypertrie {

// TODO: document
    using key_pos_t = ::uint8_t;

    using subkey_mask_t = ::std::vector<bool>;

    using key_part_t = ::uint64_t;

#define KEY_PART_MAX (UINT64_MAX)

#define KEY_PART_MIN (0)
}

#endif //SPARSETENSOR_HYPERTRIE_TYPES_HPP
