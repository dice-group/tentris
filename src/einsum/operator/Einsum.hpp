#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include <tensor/MapTensor.hpp>
#include "Operator.hpp"
#include "CrossProduct.hpp"

template<typename T>
class Einsum : public Operator<T> {
    friend class CrossProduct;

    explicit Einsum(Subscript &subscript) : Operator(subscript) {}


public:
    MapTensor<T> *getResult(vector<Tensor<T>> tensors);
};


#endif //LIBSPARSETENSOR_EINSUM_HPP
