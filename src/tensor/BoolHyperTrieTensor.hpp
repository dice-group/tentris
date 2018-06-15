#ifndef LIBSPARSETENSOR_BOOLHYPERTRIETENSOR_HPP
#define LIBSPARSETENSOR_BOOLHYPERTRIETENSOR_HPP


#include "Tensor.hpp"
#include "../hypertrie/BoolHyperTrie.hpp"
#include "../util/ExtendContainerPrint.hpp"
#include "Types.hpp"

#include <cstdint>
#include <vector>
#include <map>


namespace sparsetensor::operations::operators {
    template<typename T>
    class Einsum;
}

namespace sparsetensor::tensor {
    using sparsetensor::hypertrie::BoolHyperTrie;
    using std::vector;

    template<>
    class BoolHyperTrieTensor : public Tensor<bool, BoolHyperTrieTensor> {
        friend class sparsetensor::operations::operators::Einsum<bool>;

        BoolHyperTrie *trie;
        bool scalar{};

    public:

        explicit BoolHyperTrieTensor(const shape_t &shape) : Tensor<bool, BoolHyperTrieTensor>{shape},
                                                             trie{(shape.size() != 0) ? new BoolHyperTrie(
                                                                     uint8_t(shape.size())) : nullptr} {}

        BoolHyperTrieTensor(const shape_t &shape, BoolHyperTrie *trie) : Tensor<bool, BoolHyperTrieTensor>{shape},
                                                                         trie(trie) {}

        virtual ~BoolHyperTrieTensor() {
            delete trie;
        }

    public:


        virtual bool get(const Key_t key) const override {
            if (trie->depth() > 0) {
                try {
                    const variant<BoolHyperTrie *, bool> &value_ = trie->get(key);
                    return std::get<bool>(value_);
                } catch (...) {
                    return {};
                }
            } else {
                if (key != Key_t{})
                    throw "Scalars must be accessed with an empty key.";
                return scalar;
            }
        }

        virtual void set(const Key_t key, const bool value) override {
            if (trie->depth() > 0) { // ndim >= 1 -> tensor
                trie->set(key, value);
            } else {
                if (key != Key_t{})
                    throw "Scalars must be accessed with an empty key.";
                scalar = value;
            }
        }

        friend ostream &operator<<(ostream &out, BoolHyperTrieTensor &tensor) {
            out << "<Tensor: shape=" << tensor.shape << ", nnz=" << tensor.nnz << ", sum=" << tensor.sum << ">";
            return out;
        }
    };
}

#endif //LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
