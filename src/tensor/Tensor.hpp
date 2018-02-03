#ifndef LIBSPARSETENSOR_TENSOR_HPP
#define LIBSPARSETENSOR_TENSOR_HPP


#include <cstdint>
#include <vector>
#include <map>
#include <ostream>

using std::vector;

namespace sparsetensor::tensor {
    template<typename T>
    class Tensor {
    public:
        uint8_t ndim{};
        uint64_t nnz{};
        T sum{};
        vector<uint64_t> shape;

        explicit Tensor(const vector<uint64_t> &shape) : ndim(uint8_t(shape.size())), shape(shape) {}

        virtual T get(vector<uint64_t> &key) =0;

        virtual void set(std::vector<uint64_t> &key, T &value) =0;

        bool isZero() {
            if (nnz == 0) {
                return true;
            }
        }
    };

}
#endif //LIBSPARSETENSOR_TENSOR_HPP
