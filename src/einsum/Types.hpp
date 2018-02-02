#ifndef SPARSETENSOR_EINSUM_TYPES_HPP

#define SPARSETENSOR_EINSUM_TYPES_HPP

#include <cstdint>
#include <vector>


namespace sparsetensor::einsum {
/**
     * Type for labels (uint8_t).
     */
    typedef uint8_t label_t;
/**
 * Type for the position of a operand (uint8_t).
 */
    typedef uint8_t op_pos_t;
/**
 * Type for the position of a label (uint8_t).
 */
    typedef uint8_t label_pos_t;

    typedef std::vector<uint8_t> raw_subscript;
}

#endif //SPARSETENSOR_EINSUM_TYPES_HPP
