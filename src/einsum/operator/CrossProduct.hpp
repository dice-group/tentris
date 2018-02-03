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
            vector<tuple<variant<Tensor<T> *, T>, Subscript>> predecessor_results{};
            // TODO: make parallel
            for (int i = 0; i < size(predecessors); ++i) {
                predecessor_results.push_back({predecessors[i].getResult(), predecessors[i].subscript});
                if (predecessor_results[i].second.isZero()) {
                    // TODO: determine shape
                    // TODO: when parallel -> cancel all other threads.
                    return CrossProductTensor<T>::getZero(vector<uint64_t>{});
                }
            }
            vector<uint64_t> shape{}; // TODO: calc shape
            return new CrossProductTensor<T>(predecessor_results, shape);
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
