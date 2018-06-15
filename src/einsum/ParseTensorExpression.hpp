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


    /**
     * Solves a Einstein Summation Convention on given Tensors.
     * @tparam T type of the values hold by processed Tensors (Tensor).
     * @param operands HyperTrieTensor Operands to process
     * @param raw_operand_subscripts labels of the Operands
     * @param raw_result_subscript result labels
     * @return a Tensor holding the result. The CrossProductTensor must be freed manually.
     */
    CrossProductTensor *einsum(const vector<HyperTrieTensor *> operands,
                               const vector<raw_subscript> raw_operand_subscripts,
                               const raw_subscript raw_result_subscript) {

        Subscript subscript = Subscript{raw_operand_subscripts, raw_result_subscript};

        // generate a operator tree from the subscript
        CrossProduct<T> operator_tree{subscript};

        // calculate and return the result.
        return operator_tree.getResult(operands);
    }
}

#endif //SPARSETENSOR_EINSUM_EINSUM_HPP
