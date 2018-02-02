#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include "../tensor/HyperTrieTensor.hpp"
#include "../tensor/MapTensor.hpp"
#include "Subscript.hpp"
#include "operator/Operator.hpp"

using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::einsum::Operator::Operator;

namespace sparsetensor::einsum {


    template<typename T>
    Tensor<T> *einsum(vector<HyperTrieTensor<T>> operands,
                      vector<raw_subscript> raw_operand_subscripts, raw_subscript
                      raw_result_subscript) {

        Subscript subscript = Subscript{raw_operand_subscripts, raw_result_subscript}.optimize();

        Operator::Operator<T> operator_tree = Operator::Operator<T>::buildOperatorGraph(subscript, operands);

        Tensor<T> *result = operator_tree.getResult(operands);

        return
                result;
    }
}

#endif //LIBSPARSETENSOR_EINSUM_HPP
