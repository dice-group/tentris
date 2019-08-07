#ifndef HYPERTRIE_JOIN_IMPL_HPP
#define HYPERTRIE_JOIN_IMPL_HPP

#include "hypertrie/internal/util/CONSTANTS.hpp"

namespace hypertrie::internal {


	template<typename key_part_type,
			template<typename, typename> class map_type,
			template<typename> class set_type>
	class HashJoin;

	template<typename key_part_type,
			template<typename, typename> class map_type,
			template<typename> class set_type>
	class OrderedJoin;
}
#endif //HYPERTRIE_JOIN_IMPL_HPP
