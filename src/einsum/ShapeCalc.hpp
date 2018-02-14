#ifndef SPARSETENSOR_EINSUM_SHAPECALC_HPP
#define SPARSETENSOR_EINSUM_SHAPECALC_HPP

#include "../tensor/Types.hpp"
#include "Subscript.hpp"

using sparsetensor::tensor::shape_t;

namespace sparsetensor::einsum {

    /**
     * Calculates the shape that will result from a einsum() on given operands.
     * @tparam T type of the values hold by processed Tensors (Tensor).
     * @tparam Tensor Tensor implementation. Must be subclass of sparsetensor::tensor::Tensor.
     * @param operands operands to einsum()
     * @param sc Subscript for einsum()
     * @return the shape of the result
     */
    template<typename T, template<typename> class Tensor>
    shape_t calcResultShape(const vector<Tensor<T> *> &operands, const Subscript &sc) {
        // TODO: check if all dimensions the the same label have the same size
        shape_t result_shape{};
        for (auto &&resultLabel : sc.getResultLabels()) {
            const unordered_map<label_t, vector<op_pos_t>> &map = sc.getOperandsWithLabel();
            const vector<op_pos_t> &op_poss_with_label = map.at(resultLabel);
            const op_pos_t &op_pos = op_poss_with_label.at(0);
            const vector<label_pos_t> &label_poss_in_op = sc.getLabelPossInOperand().at({op_pos, resultLabel});
            const label_pos_t &label_pos = label_poss_in_op.at(0);

            const uint64_t &dim_size = operands.at(op_pos)->shape.at(label_pos);
            result_shape.push_back(dim_size);
        }
        return result_shape;
    }

}

#endif //SPARSETENSOR_EINSUM_SHAPECALC_HPP
