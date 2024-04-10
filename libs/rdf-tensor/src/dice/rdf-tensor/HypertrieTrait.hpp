#ifndef TENTRIS_HYPERTRIETRAIT_HPP
#define TENTRIS_HYPERTRIETRAIT_HPP

#include "dice/rdf-tensor/NodeWrapper.hpp"

#include <dice/hypertrie.hpp>
#include <dice/sparse-map/sparse_map.hpp>
#include <dice/sparse-map/sparse_set.hpp>

namespace dice::rdf_tensor {
	using key_part_type = NodeWrapper;
	template<typename Key, typename T, typename Allocator>
	using map_type = dice::sparse_map::sparse_map<Key,
									 T,
									 dice::hash::DiceHashMartinus<Key>,
									 std::equal_to<Key>,
									 typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<Key, T>>,
									 dice::sparse_map::sh::power_of_two_growth_policy<2>,
									 dice::sparse_map::sh::exception_safety::basic,
									 dice::sparse_map::sh::sparsity::high>;

	template<typename Key, typename Allocator>
	using set_type = dice::sparse_map::sparse_set<
			Key,
			dice::hash::DiceHashMartinus<Key>,
			std::equal_to<Key>,
			typename std::allocator_traits<Allocator>::template rebind_alloc<Key>,
			dice::sparse_map::sh::power_of_two_growth_policy<2>,
			dice::sparse_map::sh::exception_safety::basic,
			dice::sparse_map::sh::sparsity::high>;

	using htt_t = dice::hypertrie::Hypertrie_trait<key_part_type,
												   bool,
												   map_type,
												   set_type,
												   63>;

	using SliceKey = dice::hypertrie::SliceKey<htt_t>;
	using Key = dice::hypertrie::Key<htt_t>;
	using NonZeroEntry = dice::hypertrie::NonZeroEntry<htt_t>;
}// namespace dice::rdf-tensor
#endif//TENTRIS_HYPERTRIETRAIT_HPP
