//
// Created by me on 1/16/18.
//

#include "MapTensor.hpp"


template<typename T>
T MapTensor<T>::get(vector<uint64_t> &key) {
    auto entry_ = this->entries.find(key);

    if (entry_ != this->entries.end()) {
        return *entry_;
    } else {
        return {};
    }
}

template<typename T>
void MapTensor<T>::set(std::vector<uint64_t> &key, T &value) {
    auto old_value_ = this->entries.find(key);
    // old value exists
    if (old_value_ != this->entries.end()) { // new value is not zero
        // udate entry
        this->sum += value - old_value_->second;
        if (value != T{}) {
            old_value_->second = value;
        } else { // new value is zero
            // delete entry
            this->entries.erase(key);
            this->nnz -= 1;
        }
    } else { // old doesn't exists
        if (value != T{}) { // new value is not zero
            // add new entry
            this->entries[key] = value;
            this->sum += value;
            this->nnz += 1;
        }
    }
}

