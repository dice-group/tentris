#ifndef TNT_SLICE_HPP
#define TNT_SLICE_HPP

#include <algorithm>
#include <variant>

#include "tnt/tensor/einsum/operator/OperatorNode.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/util/All.hpp"

namespace {
	using namespace tnt::util::types;
	using namespace tnt::tensor::hypertrie;

} // namespace

namespace tnt::tensor::einsum::operators {

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	class Slice : public OperatorNode<RESULT_TYPE> {
	public:
		enum SLICE_TYPE {
			SCALAR, HYPERTRIE
		};
		const SLICE_TYPE slice_type;

	private:
		const SliceKey_t _key;
		const BoolHyperTrie *_trie;

	public:
		Slice(const SliceKey_t &key, const BoolHyperTrie *trie)
				: OperatorNode<RESULT_TYPE>{0}, slice_type{hasSlices(key) ? HYPERTRIE : SCALAR}, _key{key},
				  _trie{trie} {}

		virtual ~Slice() = default;

		/**
		 * Must only be called if this->slice_type == SCALAR.
		 * @return
		 */
		bool getScalar() { return std::get<bool>(_trie->get(_key)); }

		/**
		 * Must only be called if this->slice_type == HYPERTRIE.
		 * @return
		 */
		BoolHyperTrie *getHyperTrie() { return std::get<BoolHyperTrie *>(_trie->get(_key)); }

		yield_pull <RESULT_TYPE> get() const override {
			return yield_pull<RESULT_TYPE>(boost::bind(&Slice<RESULT_TYPE>::get, this, _1));
		}

		const Result <RESULT_TYPE> &getFullResult() const override { throw "Not yet implemented."; }

		void clearCacheCanceled() const override {}

		void clearCacheDone() const override {}

	private:
		void get(yield_push <RESULT_TYPE> &yield) const;
	};

	template<>
	void Slice<counted_binding>::get([[maybe_unused]] yield_push <counted_binding> &yield) const {
		// TODO: implement
	}

	template<>
	void Slice<distinct_binding>::get([[maybe_unused]] yield_push <distinct_binding> &yield) const {
		// TODO: implement
	}

};         // namespace tnt::tensor::einsum::operators
#endif // TNT_SLICE_HPP
