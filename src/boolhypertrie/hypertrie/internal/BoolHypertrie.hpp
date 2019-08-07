#ifndef HYPERTRIE_BOOLHYPERTRIE_INTERFACE_HPP
#define HYPERTRIE_BOOLHYPERTRIE_INTERFACE_HPP

#include "hypertrie/internal/util/CONSTANTS.hpp"

#include "hypertrie/internal/container/AllContainer.hpp"

#include "hypertrie/internal/BoolHypertrie_impl.hpp"
#include "hypertrie/internal/BoolHypertrie_Hash_Diagonal_impl.hpp"

namespace hypertrie::internal::interface {
	template<typename key_part_type = unsigned long, template<typename, typename> class map_type = hypertrie::internal::container::tsl_sparse_map,
			template<typename> class set_type = hypertrie::internal::container::boost_flat_set>
	struct boolhypertrie {
		using BoolHypertrie = hypertrie::internal::BoolHypertrie<key_part_type, map_type, set_type>;
		using const_BoolHypertrie = hypertrie::internal::const_BoolHypertrie<key_part_type, map_type, set_type>;
		using HashDiagonal = hypertrie::internal::HashDiagonal<key_part_type, map_type, set_type>;
		using OrderedDiagonal = hypertrie::internal::OrderedDiagonal<key_part_type, map_type, set_type>;
	};
}

#endif //HYPERTRIE_BOOLHYPERTRIE_INTERFACE_HPP
