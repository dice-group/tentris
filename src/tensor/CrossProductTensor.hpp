#ifndef LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP
#define LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP

#include <einsum/Subscript.hpp>
#include "Tensor.hpp"
#include "MapTensor.hpp"

namespace sparsetensor::tensor {

    template<typename T>
    class CrossProductTensor : public Tensor<T> {
    public:
        CrossProductTensor(const vector<tuple<MapTensor *, Subscript>> &inputs) : inputs(inputs) {
            // TODO: initialize fields
        }

    private:
        vector<tuple<MapTensor *, Subscript>> inputs;
        vector<Tensor<T>> tensor;

    public:
        T get(vector<uint64_t> &key) {
            return {};
        }

        void set(std::vector<uint64_t> &key, T &value) {
            throw "Set not supported by CrossProductTensor.";
        }

        static CrossProductTensor *getZero(vector<uint64_t> shape);
    };

}
#endif //LIBSPARSETENSOR_CROSSPRODUCTTENSOR_HPP
