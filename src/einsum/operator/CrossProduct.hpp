#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include <vector>
#include <tuple>
#include "Operator.hpp"
#include "Einsum.hpp"
#include "../../tensor/CrossProductTensor.hpp"

using std::vector;
using std::tuple;

using sparsetensor::tensor::CrossProductTensor;
using sparsetensor::tensor::MapTensor;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class CrossProduct : public Operator<T, HyperTrieTensor, CrossProductTensor> {
    protected:
        const Subscript optimized_subscript;
        vector<Einsum<T>> predecessors{};
    public:
        explicit CrossProduct(const Subscript &subscript) :
                Operator<T, HyperTrieTensor, CrossProductTensor>{subscript},
                optimized_subscript(Subscript{subscript}.optimize()) {

            for (const auto &[op_id, sub_subscript] : optimized_subscript.getSubSubscripts()) {
                predecessors.push_back({sub_subscript});
            }
        }

        CrossProductTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) {
            shape_t result_shape = calcShape<T, HyperTrieTensor>(operands, this->subscript);

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

                if (predecessor_results[i]->nnz == 0) {
                    // TODO: when parallel -> cancel all other threads.
                    return new CrossProductTensor<T>(result_shape);
                }
            }
            return new CrossProductTensor<T>(predecessor_results, optimized_subscript);
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
