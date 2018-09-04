#ifndef TNT_SLICE_HPP
#define TNT_SLICE_HPP

#include <variant>
#include <algorithm>

#include "tnt/util/All.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/operator/OperatorNode.hpp"


namespace {
    using namespace tnt::util::types;
    using namespace tnt::tensor::hypertrie;


}

namespace tnt::tensor::einsum::operators {

    template<typename RESULT_TYPE>
    class Slice : public OperatorNode<RESULT_TYPE> {
    public:
        enum SLICE_TYPE {
            SCALAR,
            HYPERTRIE
        };
        const SLICE_TYPE slice_type;
    private:
        const SliceKey_t _key;
        const BoolHyperTrie *_trie;
    public:
        Slice(const SliceKey_t &key, const BoolHyperTrie *trie) :
                _key{key}, _trie{trie},
                slice_type{(std::count(key.begin(), key.end(), std::nullopt) > 0) ? HYPERTRIE : SCALAR} {}

        /**
         * Must only be called if this->slice_type == SCALAR.
         * @return
         */
        bool getScalar() {
            return std::get<bool>(_trie->get(_key));
        }

        /**
         * Must only be called if this->slice_type == HYPERTRIE.
         * @return
         */
        BoolHyperTrie *getHyperTrie() {
            return std::get<BoolHyperTrie *>(_trie->get(_key));
        }

        yield_pull<RESULT_TYPE> get() const override {
            return yield_pull<RESULT_TYPE>(boost::bind(&Slice<RESULT_TYPE>::get, this, _1));
        }

    private:
        void get(yield_push<RESULT_TYPE> &yield) const;
    };

    template<>
    void Slice<INT_VALUES>::get(yield_push<INT_VALUES> &yield) const {
        // TODO: implement
    }

    template<>
    void Slice<BOOL_VALUES>::get(yield_push<BOOL_VALUES> &yield) const {
        // TODO: implement
    }


};
#endif //TNT_SLICE_HPP
