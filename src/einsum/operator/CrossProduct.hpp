#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include "Operator.hpp"
#include <vector>
#include <tuple>
#include "../../tensor/CrossProductTensor.hpp"

using std::vector;
using std::tuple;

using sparsetensor::tensor::CrossProductTensor;
using sparsetensor::einsum::operators::Operator;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class CrossProduct : public Operator<T> {
    protected:
        explicit CrossProduct(Subscript &subscript) : Operator<T>{subscript} {
            for (auto &sub_subscript_ : subscript.getSubSubscripts()) {
                predecessors.push_back({sub_subscript_.second});
            }
        }

        vector<Operator<T>> predecessors{};

    public:
        Tensor<T> *getResult(const vector<variant<Tensor<T> *, T>> &tensors) override {
            // TODO: calc shape
            vector<uint64_t> shape{};

            vector<Tensor<T> *> predecessor_results(predecessors.size());
            // TODO: make parallel
            // get results from all predecessors
            for (int i = 0; i < size(predecessors); ++i) {
                Operator<T> &predecessor = predecessors[i];
                predecessor_results[i] = predecessor.getResult(tensors[i]);
                if (predecessor_results[i]->nnz = 0) {
                    // TODO: determine shape
                    // TODO: when parallel -> cancel all other threads.
                    return new CrossProductTensor<T>(shape);
                }
            }
            return new CrossProductTensor<T>(predecessor_results, subscript);
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
