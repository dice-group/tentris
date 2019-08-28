#ifndef TENTRIS_BOOLHYPERTRIE_HPP
#define TENTRIS_BOOLHYPERTRIE_HPP

#include <hypertrie/boolhypertrie.hpp>
#include "tentris/store/RDF/TermStore.hpp"

namespace tentris::tensor {
	using key_part_type = store::rdf::TermStore::ptr_type;
	using ht = typename hypertrie::template boolhypertrie<key_part_type, hypertrie::internal::container::tsl_sparse_map,
			hypertrie::internal::container::tsl_sparse_set>;

	using SliceKey = ht::const_BoolHypertrie::SliceKey;
	using Key = ht::const_BoolHypertrie::Key;

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
