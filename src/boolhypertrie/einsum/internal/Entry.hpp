#ifndef HYPERTRIE_ENTRY_HPP
#define HYPERTRIE_ENTRY_HPP

#include <boost/container_hash/hash.hpp>
#include "hypertrie/internal/BoolHypertrie.hpp"
#include "hypertrie/internal/Join.hpp"

namespace einsum::internal {

	namespace {
		template<typename key_part_type, template<typename, typename> class map_type,
				template<typename> class set_type>
		using const_BoolHypertrie = typename ::hypertrie::internal::interface::boolhypertrie<key_part_type, map_type, set_type>::const_BoolHypertrie;
		template<typename key_part_type, template<typename, typename> class map_type,
				template<typename> class set_type>
		using Join = typename ::hypertrie::internal::interface::join<key_part_type, map_type, set_type>::HashJoin;
		template<typename key_part_type, template<typename, typename> class map_type,
				template<typename> class set_type>
		using Diagonal = typename ::hypertrie::internal::interface::boolhypertrie<key_part_type, map_type, set_type>::Diagonal;
	}

	template<typename key_part_type>
	using Key = std::vector<key_part_type>;

	template<typename T, typename =std::enable_if_t<(not std::is_same_v<std::decay_t<T>, bool>)>>
	struct KeyHash {
		std::size_t operator()(const ::einsum::internal::Key<T> &k) const {
			return boost::hash_range(k.begin(), k.end());
		}
	};

	template<typename key_part_type_, typename value_type_ = size_t, typename = std::enable_if_t<(std::is_integral_v<value_type_>)>>
	struct Entry {
		using key_part_type = key_part_type_;
		using key_type = Key<key_part_type>;
		using value_type = value_type_;
		value_type value;
		key_type key;
	};

}
#endif //HYPERTRIE_ENTRY_HPP

