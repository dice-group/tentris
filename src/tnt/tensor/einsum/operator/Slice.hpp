#ifndef TNT_SLICE_HPP
#define TNT_SLICE_HPP

#include <variant>
#include <algorithm>
//#include "tnt/store/RDF/Term.hpp"
//#include "tnt/store/SPARQL/Variable.hpp"
#include "tnt/util/All.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"

namespace tnt::tensor::einsum::operators {
    class Slice {
    public:
        enum TYPE {
            SCALAR,
            HYPERTRIE
        };
    private:
        using SliceKey_t = tnt::util::types::SliceKey_t;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;

        const SliceKey_t _key;
        const BoolHyperTrie *_trie;
    public:
        const TYPE type;

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
        BoolHyperTrie * getHyperTrie() {
            return std::get<BoolHyperTrie *>(_trie->get(_key));
        }
    };


};
#endif //TNT_SLICE_HPP
