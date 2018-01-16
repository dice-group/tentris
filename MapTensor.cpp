//
// Created by me on 1/16/18.
//

#include "MapTensor.hpp"

MapTensor::MapTensor(uint8_t ndim, const vector<uint64_t> &shape) : Tensor(ndim, shape) {}

template<typename T>
T MapTensor::get(vector<uint64_t> key) {
    auto entry_ = this->entries.find(key);
    if (entry_ != this->entries.end()) {
        return *entry_;
    } else {
        return {};
    }
}

template<typename T>
void MapTensor::set(std::vector<uint64_t> key, T value) {
    if (value != T{}) {
        this->entries[key] = value;
    } else {
        this->entries.erase(key);
    }
}