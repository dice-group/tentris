#ifndef TENTRIS_BOOLHYPERTRIE_HPP
#define TENTRIS_BOOLHYPERTRIE_HPP

#include <Dice/hypertrie/hypertrie.hpp>
#include "tentris/store/RDF/TermStore.hpp"

namespace tentris::tensor {
	using key_part_type = store::rdf::TermStore::ptr_type;
	using tr = hypertrie::Hypertrie_t<key_part_type,
			bool,
			hypertrie::internal::container::tsl_sparse_map,
			hypertrie::internal::container::tsl_sparse_set,
			false>;

	using SliceKey = typename tr::SliceKey;
	using Key = typename tr::Key ;

	using BoolHypertrie = hypertrie::Hypertrie<tr>;
	using const_BoolHypertrie = hypertrie::const_Hypertrie<tr>;
	template<typename result_type>
	using Einsum = hypertrie::Einsum<result_type, tr>;

	template<typename result_type>
	using EinsumEntry = hypertrie::EinsumEntry<result_type, tr>;
	using DISTINCT_t = bool;
	using COUNTED_t = std::size_t;
}

#endif //TENTRIS_BOOLHYPERTRIE_HPP
