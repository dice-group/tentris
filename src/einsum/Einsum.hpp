#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include "../tensor/HyperTrieTensor.hpp"
#include "../tensor/MapTensor.hpp"
#include "Subscript.hpp"
#include "operator/Operator.hpp"

typedef std::vector<uint8_t> raw_subscript;

template<typename T>
Tensor<T> *einsum(vector<HyperTrieTensor<T>> operands,
                  vector<raw_subscript> raw_operand_subscripts, raw_subscript raw_result_subscript) {

    Subscript subscript = Subscript{raw_operand_subscripts, raw_result_subscript}.optimize();

    Operator operator_tree = Operator<T>::buildOperatorGraph(subscript, operands);

    Tensor<T> *result = operator_tree.getResult(operands);

    return result;
}

#endif //LIBSPARSETENSOR_EINSUM_HPP
