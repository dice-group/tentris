#ifndef LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP
#define LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP

#include "../einsum/Subscript.hpp"
#include "Tensor.hpp"
#include <tuple>

using sparsetensor::einsum::Subscript;
using std::tuple;
using std::vector;


namespace sparsetensor::tensor {
    template<typename T>
    class CrossProductTensor : public Tensor<T> {
    public:
        CrossProductTensor(vector<tuple<Tensor<T> *, Subscript>> &inputs, vector<uint64_t> shape)
                : Tensor<T>{shape}, inputs(inputs) {
            // TODO: initialize fields
        }

    private:
        vector<tuple<Tensor<T> *, Subscript>> inputs;
        vector<Tensor<T>> tensor;

    public:
        virtual T get(vector<uint64_t> &key) override {
            return {};
        }

        virtual void set(std::vector<uint64_t> &key, T &value) override {
            throw "Set not supported by CrossProductTensor.";
        }

        static CrossProductTensor *getZero(vector<uint64_t> shape) {
            return nullptr;
        }
    };

}
#endif //LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP
