#ifndef LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP
#define LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP

#include "../einsum/Subscript.hpp"
#include "Tensor.hpp"
#include <tuple>

using sparsetensor::einsum::Subscript;
//using sparsetensor::tensor::Tensor;
using std::tuple;
using std::vector;


namespace sparsetensor::tensor {

    template<typename T>
    class CrossProductTensor : public Tensor<T> {
    public:
        explicit CrossProductTensor(typename vector<tuple<Tensor<T> *, Subscript>> &inputs) : inputs(inputs) {
            // TODO: initialize fields
        }

    private:
        typename vector<tuple<Tensor<T> *, Subscript>> inputs;
        vector<Tensor<T>> tensor;

    public:
        virtual T get(vector<uint64_t> &key) override {
            return {};
        }

        virtual void set(std::vector<uint64_t> &key, T &value) override {
            throw "Set not supported by CrossProductTensor.";
        }

        static CrossProductTensor *getZero(vector<uint64_t> shape);
    };

}
#endif //LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP
