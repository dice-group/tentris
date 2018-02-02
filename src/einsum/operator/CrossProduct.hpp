#ifndef LIBSPARSETENSOR_CROSSPRODUCT_HPP
#define LIBSPARSETENSOR_CROSSPRODUCT_HPP


#include "Operator.hpp"
#include "Einsum.hpp"
#include <vector>
#include "../../tensor/CrossProductTensor.hpp"

using std::vector;

using sparsetensor::tensor::CrossProductTensor;

namespace sparsetensor::einsum::Operator {

    template<typename T>
    class CrossProduct : public Operator<T> {
    protected:
        CrossProduct(Subscript &subscript) : Operator(subscript) {
            for (auto &sub_subscript_ : subscript.getSubSubscripts()) {
                predecessors.push_back({sub_subscript_.second});
            }
        }

        vector<Einsum> predecessors{};

    public:
        CrossProductTensor <T> *getResult(vector<Tensor < T>>

        tensors) {
            vector<tuple<MapTensor *, Subscript>> predecessor_results{};
            // TODO: make parallel
            for (int i = 0; i < size(predecessors); ++i) {
                predecessor_results.push_back({predecessors[i].getResult(), predecessors[i].subscript});
                if (predecessor_results[i].second.isZero()) {
                    // TODO: determine shape
                    // TODO: when parallel -> cancel all other threads.
                    return CrossProductTensor::getZero(vector<uint64_t>{});
                }
            }
            return new CrossProductTensor<T>(predecessor_results);
        }
    };
}

#endif //LIBSPARSETENSOR_CROSSPRODUCT_HPP
