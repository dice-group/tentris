#ifndef TENTRIS_BOOLHYPERTRIE_HPP
#define TENTRIS_BOOLHYPERTRIE_HPP

#include <hypertrie/boolhypertrie.hpp>

namespace tentris::tensor {
	using ht = typename hypertrie::template boolhypertrie<>;

	using BoolHypertrie = ht::BoolHypertrie;
	using const_BoolHypertrie = ht::const_BoolHypertrie;
	using Einsum = ht::Einsum;
}

#endif //TENTRIS_BOOLHYPERTRIE_HPP
