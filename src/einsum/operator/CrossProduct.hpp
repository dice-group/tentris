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
        vector<Einsum<T>> predecessors{};
    public:
        explicit CrossProduct(const Subscript &subscript) :
                subscript(subscript) {
            const map<op_pos_t, Subscript> &sub_subscripts = subscript.getSubSubscripts();
            for (const auto &[op_id, sub_subscript] : sub_subscripts) {
                predecessors.push_back({sub_subscript});
            }
        }

        CrossProductTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) {
            shape_t result_shape = calcShape<T, HyperTrieTensor>(operands, subscript);

            vector<MapTensor<T> *> predecessor_results(predecessors.size());
            // TODO: make parallel
            // get results from all predecessors
            for (int i = 0; i < size(predecessors); ++i) {
                Einsum<T> &predecessor = predecessors.at(i);
                predecessor_results[i] = predecessor.getResult(operands);

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
