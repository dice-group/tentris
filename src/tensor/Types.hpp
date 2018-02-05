#ifndef SPARSETENSOR_TENSOR_TYPES_HPP
#define SPARSETENSOR_TENSOR_TYPES_HPP

#include <cstdint>
#include <vector>

namespace sparsetensor::tensor {

// TODO: document
    using shape_t = ::std::vector<uint64_t>;

    using key_pos_t = ::uint8_t;

    using key_part_t = ::uint64_t;

    using Key_t = ::std::vector<key_part_t>;

    #define KEY_PART_MAX (UINT64_MAX)

    #define KEY_PART_MIN (0)
}

#endif //SPARSETENSOR_TENSOR_TYPES_HPP
