#ifndef LIBSPARSETENSOR_TENSOR_HPP
#define LIBSPARSETENSOR_TENSOR_HPP


#include "Types.hpp"
#include <cstdint>
#include <vector>
#include <map>
#include <ostream>
#include <tuple>

using std::vector;
using std::tuple;

namespace sparsetensor::tensor {
    template<typename T>
    class Tensor {

    public:
        uint8_t ndim{};
        uint64_t nnz{};
        T sum{};
        shape_t shape;

        // todo: Copy-constructor

        explicit Tensor(const shape_t &shape = shape_t{}) : ndim(uint8_t(shape.size())),
                                                            shape(shape) {}

    protected:
        void setShape(const shape_t &shape) {
            ndim = uint8_t(shape.size());
            this->shape.clear();
            this->shape = shape;
        }

    public:
        virtual T get(const Key_t &key) =0;

        virtual void set(const Key_t &key, const T &value) =0;

        bool isZero() {
            return nnz == 0;
        }

        class Iterator {
        public:
            virtual Iterator &operator++() {};

            virtual Iterator operator++(int) {};

            virtual tuple<Key_t, T> operator*() {};

            virtual bool operator==(const Iterator &rhs) const {};

            virtual bool operator!=(const Iterator &rhs) const {};
        };

        virtual Iterator begin() = 0;

        virtual Iterator end() = 0;
    };


}
#endif //LIBSPARSETENSOR_TENSOR_HPP
