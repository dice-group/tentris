#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include "Einsum.hpp"
#include <vector>
#include <tuple>
#include "../../tensor/CrossProductTensor.hpp"

using std::vector;
using std::tuple;

using sparsetensor::tensor::CrossProductTensor;
using sparsetensor::tensor::MapTensor;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class CrossProduct {
        const Subscript &subscript;
        const Subscript optimized_subscript;
        vector<Einsum<T>> predecessors{};
    public:
        explicit CrossProduct(const Subscript &subscript) :
                subscript(subscript),
                optimized_subscript(Subscript{subscript}.optimize()) {
            const map<op_pos_t, Subscript> &sub_subscripts = optimized_subscript.getSubSubscripts();
            for (const auto &[op_id, sub_subscript] : sub_subscripts) {
                predecessors.push_back({sub_subscript});
            }
        }

        CrossProductTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) { // todo: make this more narrow
            shape_t result_shape = calcShape<T, HyperTrieTensor>(operands, subscript);

            vector<MapTensor<T> *> predecessor_results(predecessors.size());
            // TODO: make parallel
            // get results from all predecessors
            for (int i = 0; i < size(predecessors); ++i) {
                Einsum<T> &predecessor = predecessors.at(i);
                vector<HyperTrieTensor<T> *> einsum_operands{};
                for (const op_pos_t &op_pos : predecessor.subscript.getOriginalOperandPoss()) {
                    einsum_operands.push_back(operands.at(op_pos));
                }
                predecessor_results[i] = predecessor.getResult(einsum_operands);

                if (predecessor_results[i]->nnz = 0) {
                    // TODO: when parallel -> cancel all other threads.
                    return new CrossProductTensor<T>(result_shape);
                }
            }
            return new CrossProductTensor<T>(predecessor_results, subscript);
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
