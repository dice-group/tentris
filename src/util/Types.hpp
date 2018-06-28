#ifndef SPARSETENSOR_UTIL_TYPES
#define SPARSETENSOR_UTIL_TYPES

#include <cstdint>
#include <vector>
#include <iostream>


namespace tnt::util::types {
    /**
     * Type for labels (uint8_t).
     */
    using label_t = uint8_t;

#define NO_LABEL (UINT8_MAX)

    /**
     * Type for the position of a operand (uint8_t).
     */
    using op_pos_t =uint8_t;

#define OP_POS_MAX (UINT8_MAX)

    /**
     * Type for the position of a label (uint8_t).
     */
    using label_pos_t = uint8_t;

    /**
     * A raw subscript is a vector of uint8s (uint8_t).
     */
    using raw_subscript = ::std::vector<uint8_t>;



    using subkey_mask_t = ::std::vector<bool>;

    using key_pos_t = ::uint8_t;

    using key_part_t = ::uint64_t;

    using Key_t = ::std::vector<key_part_t>;

#define KEY_PART_MAX (UINT64_MAX)

#define KEY_PART_MIN (0)
}

#endif //SPARSETENSOR_UTIL_TYPES
