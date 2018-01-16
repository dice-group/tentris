//
// Created by me on 1/16/18.
//

#ifndef LIBSPARSETENSOR_MAPTENSOR_HPP
#define LIBSPARSETENSOR_MAPTENSOR_HPP


#include "Tensor.hpp"

template<typename T>
class MapTensor : Tensor {
public:
    MapTensor(uint8_t ndim, const vector<uint64_t> &shape);

    T get(vector<uint64_t> key);

    void set(std::vector<uint64_t> key, T value);
};


#endif //LIBSPARSETENSOR_MAPTENSOR_HPP
