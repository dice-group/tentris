#ifndef LIBSPARSETENSOR_MAPTENSOR_HPP
#define LIBSPARSETENSOR_MAPTENSOR_HPP


#include <ostream>
#include "Tensor.hpp"
#include "../util/ExtendContainerPrint.hpp"

namespace sparsetensor::tensor {

    template<typename T>
    class MapTensor : public Tensor<T> {
        using Tensor<T>::Tensor;
        std::map<std::vector<uint64_t>, T> entries{};

    public:


        inline T get(vector<uint64_t> &key) override {
            auto entry_ = this->entries.find(key);

            if (entry_ != this->entries.end()) {
                return entry_->second;
            } else {
                return {};
            }
        }

        inline void set(std::vector<uint64_t> &key, T &value) override {
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

        friend ostream &operator<<(ostream &out, MapTensor<T> &tensor) {
            out << "<Tensor: shape=" << tensor.shape << ", nnz=" << tensor.nnz << ", sum=" << tensor.sum << ">";
            return out;
        }
    };
};


#endif //LIBSPARSETENSOR_MAPTENSOR_HPP
