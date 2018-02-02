#ifndef LIBSPARSETENSOR_MAPTENSOR_HPP
#define LIBSPARSETENSOR_MAPTENSOR_HPP


#include "Tensor.hpp"
#include "util/ExtendContainerPrint.hpp"

namespace sparsetensor::tensor {
    template<typename T>
    class MapTensor;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, sparsetensor::tensor::MapTensor<T> &tensor);

namespace sparsetensor::tensor {


    template<typename T>
    class MapTensor : public Tensor<T> {
        friend ::std::ostream &::operator<<<>(::std::ostream &out, ::sparsetensor::tensor::MapTensor<T> &tensor);

        using Tensor<T>::Tensor;
        std::map<std::vector<uint64_t>, T> entries{};

    public:


        T get(vector<uint64_t> &key) {
            auto entry_ = this->entries.find(key);

            if (entry_ != this->entries.end()) {
                return entry_->second;
            } else {
                return {};
            }
        }

        void set(std::vector<uint64_t> &key, T &value) {
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

        friend std::ostream &operator<<<>(std::ostream &out, MapTensor<T> &tensor);

    };

}

template<typename T>
std::ostream &operator<<(std::ostream &out, sparsetensor::tensor::MapTensor<T> &tensor) {
    out << "<Tensor: shape=" << tensor.shape << ", nnz=" << tensor.nnz << ", sum=" << tensor.sum << ">";
    return out;
}

#endif //LIBSPARSETENSOR_MAPTENSOR_HPP
