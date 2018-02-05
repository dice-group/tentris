#ifndef LIBSPARSETENSOR_MAPTENSOR_HPP
#define LIBSPARSETENSOR_MAPTENSOR_HPP


#include <ostream>
#include "Tensor.hpp"
#include "../util/ExtendContainerPrint.hpp"
#include "Types.hpp"

using std::map;

namespace sparsetensor::tensor {

    template<typename T>
    class MapTensor;

    template<typename T>
    class Iterator<T, MapTensor>;

    template<typename T>
    class MapTensor : public Tensor<T, MapTensor> {
        using Tensor<T, MapTensor>::Tensor;

        friend class Iterator<T, MapTensor>;

        map<Key_t, T> entries{};

    public:
        inline T get(const Key_t &key) override {
            auto entry_ = this->entries.find(key);

            if (entry_ != this->entries.end()) {
                return entry_->second;
            } else {
                return {};
            }
        }

        inline void set(const Key_t &key, const T &value) override {
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
            out << "<MapTensor: shape=" << tensor.shape << ", ndmin=" << tensor.ndim << ", nnz=" << tensor.nnz
                << ", sum=" << tensor.sum << ">";
            return out;
        }


        Iterator<T, MapTensor> begin() {
            return Iterator<T, MapTensor>{*this};
        }

        Iterator<T, MapTensor> end() {
            return Iterator<T, MapTensor>{*this, true};
        }
    };

    template<typename T>
    class Iterator<T, MapTensor> {
        typename map<Key_t, T>::const_iterator entries_iter;
        typename map<Key_t, T>::const_iterator entries_iter_end;
        Key_t key{};
        T value{};

    public:
        explicit Iterator(const MapTensor<T> &map_tensor, bool is_end = false) :
                entries_iter((not is_end) ? map_tensor.entries.cbegin() : map_tensor.entries.cend()),
                entries_iter_end(map_tensor.entries.cend()) {}

        Iterator(const Iterator &other) : entries_iter(other.entries_iter),
                                          entries_iter_end(other.entries_iter_end) {}

        Iterator &operator++() {
            if (entries_iter != entries_iter_end)
                ++entries_iter;
            return *this;
        }

        Iterator operator++(int i) {
            operator++();
            return *this;
        }

        tuple<Key_t, T> operator*() {
            const auto &
            [key, value] = *entries_iter;
            return {key, value};
        }

        bool operator!=(const Iterator &rhs) const {
            return rhs.entries_iter != this->entries_iter;
        }
    };


};


#endif //LIBSPARSETENSOR_MAPTENSOR_HPP
