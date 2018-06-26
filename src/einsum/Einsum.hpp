#ifndef SPARSETENSOR_EINSUM_EINSUM_HPP
#define SPARSETENSOR_EINSUM_EINSUM_HPP


#include "../hypertrie/BoolHyperTrie.hpp"
#include "../tensor/CrossProductTensor.hpp"
#include "operator/CrossProduct.hpp"



namespace sparsetensor::operations {
    using sparsetensor::tensor::Tensor;
    using sparsetensor::tensor::BoolHyperTrieTensor;
    using sparsetensor::tensor::CrossProductTensor;
    using sparsetensor::operations::operators::CrossProduct;

    CrossProduct<

    /**
     * Solves a Einstein Summation Convention on given Tensors.
     * @tparam T type of the values hold by processed Tensors (Tensor).
     * @param operands HyperTrieTensor Operands to process
     * @param raw_operand_subscripts labels of the Operands
     * @param raw_result_subscript result labels
     * @return a Tensor holding the result. The CrossProductTensor must be freed manually.
     */
    CrossProductTensor *einsum(const vector<BoolHyperTrieTensor*> operands,
                                  const vector<raw_subscript> raw_operand_subscripts,
                                  const raw_subscript raw_result_subscript) {

        Subscript subscript = Subscript{raw_operand_subscripts, raw_result_subscript};

        // generate a operator tree from the subscript
        // TODO: cache
        CrossProduct<T> operator_tree{subscript};

        // calculate and return the result.
        return operator_tree.getResult(operands);
    }
}

#endif //SPARSETENSOR_EINSUM_EINSUM_HPP
