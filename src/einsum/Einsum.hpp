#ifndef SPARSETENSOR_EINSUM_EINSUM_HPP
#define SPARSETENSOR_EINSUM_EINSUM_HPP


#include "../tensor/HyperTrieTensor.hpp"
#include "../tensor/CrossProductTensor.hpp"
#include "Subscript.hpp"
#include "operator/CrossProduct.hpp"

using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::CrossProductTensor;
using sparsetensor::einsum::operators::CrossProduct;

namespace sparsetensor::einsum {


    template<typename T>
    CrossProductTensor<T> *einsum(const vector<HyperTrieTensor<T> *> operands,
                                  const vector<raw_subscript> raw_operand_subscripts,
                                  const raw_subscript raw_result_subscript) {

        Subscript subscript = Subscript{raw_operand_subscripts, raw_result_subscript};

        CrossProduct<T> operator_tree{subscript};

        CrossProductTensor<T> *result = operator_tree.getResult(operands);

        return result;
    }
}

#endif //SPARSETENSOR_EINSUM_EINSUM_HPP
