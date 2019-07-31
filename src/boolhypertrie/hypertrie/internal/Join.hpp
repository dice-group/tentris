#ifndef HYPERTRIE_JOIN_HPP
#define HYPERTRIE_JOIN_HPP

#include "hypertrie/internal/util/CONSTANTS.hpp"
#include "hypertrie/internal/Join_impl.hpp"
#include "hypertrie/internal/HashJoin_impl.hpp"

namespace hypertrie::internal::interface {
	template<typename key_part_type = unsigned long, template<typename, typename> class map_type = hypertrie::internal::container::tsl_sparse_map,
			template<typename> class set_type = hypertrie::internal::container::boost_flat_set>
	struct join {
		using Join = hypertrie::internal::Join<key_part_type, map_type, set_type>;
	};
}

#endif //HYPERTRIE_JOIN_HPP
