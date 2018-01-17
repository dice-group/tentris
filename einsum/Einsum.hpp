#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include "../tensor/HyperTrieTensor.hpp"
#include "../tensor/MapTensor.hpp"
#include "Subscript.hpp"
#include "operator/Operator.hpp"

typedef std::vector<uint8_t> raw_subscript;

template<typename T>
MapTensor<T> *einsum(vector<HyperTrieTensor<T>> operands,
                     vector<raw_subscript> raw_operand_subscripts, raw_subscript raw_result_subscript) {

    Subscript subscript = Subscript::optimized(raw_operand_subscripts, raw_result_subscript);

    Operator operator_tree = Operator::buildOperatorGraph(subscript, operands);

    MapTensor<T> *result = operator_tree.getResult();

    return result;
}

#endif //LIBSPARSETENSOR_EINSUM_HPP
