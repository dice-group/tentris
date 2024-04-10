#ifndef TENTRIS_QUERY_HPP
#define TENTRIS_QUERY_HPP

#include "dice/rdf-tensor/HypertrieTrait.hpp"
#include "dice/rdf-tensor/metall_manager.hpp"
#include <dice/query.hpp>

namespace dice::rdf_tensor {
	using COUNTED_t = std::size_t;
	using Entry = dice::query::Entry<COUNTED_t, htt_t>;
	using DISTINCT_t = bool;
	using UncountedEntry = dice::query::Entry<DISTINCT_t, htt_t>;
	using Query = dice::query::Query<htt_t, allocator_type>;
}// namespace dice::rdf-tensor

#endif//TENTRIS_QUERY_HPP
