#ifndef LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
#define LIBSPARSETENSOR_HYPERTRIETENSOR_HPP


#include "Tensor.hpp"
#include "../hypertrie/HyperTrie.hpp"
#include "../util.hpp"

#include <cstdint>
#include <vector>
#include <map>

using std::vector;

template<typename T>
class HyperTrieTensor;

template<typename T>
std::ostream &operator<<(std::ostream &out, HyperTrieTensor<T> &tensor);

template<typename T>
class HyperTrieTensor : public Tensor<T> {
    HyperTrieTensor(const vector<uint64_t> &shape, HyperTrie<T> *trie) : Tensor<T>(shape), trie(trie) {}

public:

    HyperTrieTensor(const vector<uint64_t> &shape) : Tensor<T>(shape), trie(new HyperTrie<T>(uint8_t(shape.size()))) {}

private:
    HyperTrie<T> *trie;
public:


    T get(vector<uint64_t> &key) {
        const optional<variant<HyperTrie<T> *, T>> &value_ = trie->get(key);
        if (value_) {
            return std::get<T>(*value_);
        } else {
            return {};
        }
    }

    void set(std::vector<uint64_t> &key, T &value) {
        if (value != T{}) {
            trie->set(key, value);
        } else { // new value is zero
            trie->del(key);
        }
        this->nnz = trie->leafcount;
        this->sum = trie->leafsum;
    }

};

template<typename T>
std::ostream &operator<<(std::ostream &out, HyperTrieTensor<T> &tensor) {
    out << "<Tensor: shape=" << tensor.shape << ", nnz=" << tensor.nnz << ", sum=" << tensor.sum << ">";
    return out;
}


#endif //LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
