#ifndef TENTRIS_BOOLHYPERTRIE_HPP
#define TENTRIS_BOOLHYPERTRIE_HPP

#include <hypertrie/boolhypertrie.hpp>

namespace tentris::tensor {
	using ht = typename hypertrie::template boolhypertrie<>;

	using BoolHypertrie = ht::BoolHypertrie;
	using const_BoolHypertrie = ht::const_BoolHypertrie;
	template<typename result_type>
	using Einsum = ht::Einsum<result_type>;

	template<typename result_type>
	using EinsumEntry = ht::EinsumEntry<result_type>;
	using DISTINCT_t = bool;
	using COUNTED_t = std::size_t;
}

#endif //TENTRIS_BOOLHYPERTRIE_HPP
