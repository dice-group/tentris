#ifndef LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
#define LIBSPARSETENSOR_HYPERTRIETENSOR_HPP


#include "Tensor.hpp"
#include "../hypertrie/HyperTrie.hpp"
#include "../util/ExtendContainerPrint.hpp"
#include "Types.hpp"

#include <cstdint>
#include <vector>
#include <map>

using std::vector;
using sparsetensor::hypertrie::HyperTrie;

namespace sparsetensor::einsum::operators {
    template<typename T>
    class Einsum;
}

namespace sparsetensor::tensor {

    template<typename T>
    class HyperTrieTensor : public Tensor<T, HyperTrieTensor> {
        friend class sparsetensor::einsum::operators::Einsum<T>;
        HyperTrie<T> *trie;

    public:

        explicit HyperTrieTensor<T>(const shape_t &shape) : Tensor<T, HyperTrieTensor>{shape},
                                                            trie(new HyperTrie<T>(uint8_t(shape.size()))) {}

        HyperTrieTensor<T>(const shape_t &shape, HyperTrie<T> *trie) : Tensor<T, HyperTrieTensor>{shape},
                                                                       trie(trie) {}

        virtual ~HyperTrieTensor() {
            delete trie;
        }

    public:


        virtual T get(const Key_t key) const override {
            if (trie->depth > 0) {
                const optional<variant<HyperTrie<T> *, T>> &value_ = trie->get(key);
                if (value_) {
                    return std::get<T>(*value_);
                } else {
                    return {};
                }
            } else {
                if (key != Key_t{})
                    throw "Scalars must be accessed with an empty key.";
                return trie->leafsum;
            }
        }

        virtual void set(const Key_t key, const T value) override {
            if (value != T{}) {
                if (trie->depth > 0) { // ndim >= 1 -> tensor
                    trie->set(key, value);
                } else { // ndmin == 1 -> scalar
                    if (key != Key_t{})
                        throw "Scalars must be accessed with an empty key.";
                    trie->leafcount = 1;
                    trie->leafsum = value;
                }
            } else {
                if (trie->depth != 0) { // ndim >= 1 -> tensor
                    trie->del(key);
                } else {
                    if (key != Key_t{})
                        throw "Scalars must be accessed with an empty key.";
                    trie->leafcount = 0;
                    trie->leafsum = value;
                }
            }
        }

        friend ostream &operator<<(ostream &out, HyperTrieTensor<T> &tensor) {
            out << "<Tensor: shape=" << tensor.shape << ", nnz=" << tensor.nnz << ", sum=" << tensor.sum << ">";
            return out;
        }
    };
}

#endif //LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
