#ifndef TNT_SLICE_HPP
#define TNT_SLICE_HPP

#include <variant>
#include <algorithm>

#include "tnt/util/All.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"

namespace {
    using namespace tnt::util::types;
    using namespace tnt::tensor::hypertrie;


}

namespace tnt::tensor::einsum::operators {
    class Slice {
    public:
        enum TYPE {
            SCALAR,
            HYPERTRIE
        };
        const TYPE type;
    private:
        const SliceKey_t _key;
        const BoolHyperTrie *_trie;
    public:
        Slice(const SliceKey_t &key, const BoolHyperTrie *trie) :
                _key{key}, _trie{trie},
                type{(std::count(key.begin(), key.end(), std::nullopt) > 0) ? HYPERTRIE : SCALAR} {}

        /**
         * Must only be called if this->type == SCALAR.
         * @return
         */
        bool getScalar() {
            return std::get<bool>(_trie->get(_key));
        }

        /**
         * Must only be called if this->type == HYPERTRIE.
         * @return
         */
        BoolHyperTrie *getHyperTrie() {
            return std::get<BoolHyperTrie *>(_trie->get(_key));
        }
    };


};
#endif //TNT_SLICE_HPP
