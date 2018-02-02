#ifndef LIBSPARSETENSOR_TENSOR_HPP
#define LIBSPARSETENSOR_TENSOR_HPP


#include <cstdint>
#include <vector>
#include <map>

using std::vector;

namespace sparsetensor::tensor {
    template<typename T>
    class Tensor {
    private:
    public:
        Tensor() {}

    public:
        uint8_t ndim;
        uint64_t nnz{};
        T sum{};
        vector<uint64_t> shape;

        Tensor(vector<uint64_t> shape) : ndim(uint8_t(shape.size())), shape(shape) {}

        T get(vector<uint64_t> &key);

        void set(std::vector<uint64_t> &key, T &value);

        bool isZero();

    };

}
#endif //LIBSPARSETENSOR_TENSOR_HPP
