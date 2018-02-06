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

    template<typename T, template <typename >class TensorImpl>
    class Iterator;

    template<typename T, template <typename >class TensorImpl>
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
            this->ndim = uint8_t(shape.size());
            this->shape.clear();
            this->shape = shape;
        }

    public:
        virtual T get(const Key_t key) const =0;

        virtual void set(const Key_t key, const T value) =0;

        bool isZero() {
            return nnz == 0;
        }

        Iterator<T, TensorImpl> begin();

        Iterator<T, TensorImpl> end();
    };

    template<typename T, template <typename >class TensorImpl>
    class Iterator {
        static_assert(std::is_base_of<Tensor<T, TensorImpl>, TensorImpl<T>>::value, "TensorImpl not derived from Tensor");
    public:

        tuple<Key_t, T> operator*();

        Iterator<T, TensorImpl> &operator++();

        bool operator==(const Iterator<T, TensorImpl> &rhs) const;

        bool operator!=(const Iterator<T, TensorImpl> &rhs) const;
    };


}
#endif //LIBSPARSETENSOR_TENSOR_HPP
