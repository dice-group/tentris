//
// Created by me on 1/16/18.
//

#ifndef LIBSPARSETENSOR_TENSOR_HPP
#define LIBSPARSETENSOR_TENSOR_HPP


#include <cstdint>
#include <vector>
#include <map>
#include <iostream>
#include "../util.hpp"

using std::vector;
using std::ostream;

template<typename T>
class Tensor {
public:
    uint8_t ndim;
    uint64_t nnz{};
    T sum{};
    vector<uint64_t> shape;

    Tensor(uint8_t ndim, vector<uint64_t> shape) : ndim(ndim), shape(shape) {}

    T get(vector<uint64_t> &key);

    void set(std::vector<uint64_t> &key, T &value);

    ostream &operator<<(ostream &os) {
        os << "<Tensor: shape=" << this->shape << ", nnz=" << this->nnz << ", sum=" << this->sum << ">";
        return os;
    }

};

#endif //LIBSPARSETENSOR_TENSOR_HPP
