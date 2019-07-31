#ifndef HYPERTRIE_BOOLHYPERTRIE_HPP
#define HYPERTRIE_BOOLHYPERTRIE_HPP

#include "hypertrie/internal/BoolHypertrie.hpp"
#include "hypertrie/internal/RawBoolHypertrie.hpp"
#include "hypertrie/internal/Join.hpp"
#include "hypertrie/internal/HashJoin_impl.hpp"

#include <vector>
#include <map>
#include <itertools.hpp>

#include "hypertrie/boolhypertrie.hpp"
#include "hypertrie/internal/util/FrontSkipIterator.hpp"

#include "einsum/internal/Einsum.hpp"

namespace hypertrie {
	using Subscript =  ::einsum::internal::Subscript;
	using RawSubscript = ::einsum::internal::RawSubscript;

	template<typename key_part_type = unsigned long, template<typename, typename> class map_type = hypertrie::internal::container::tsl_sparse_map,
			template<typename> class set_type = hypertrie::internal::container::boost_flat_set>
	struct boolhypertrie {
		using pos_type = hypertrie::internal::pos_type;

		using BoolHypertrie = typename ::hypertrie::internal::interface::boolhypertrie<key_part_type, map_type, set_type>::BoolHypertrie;

		using const_BoolHypertrie = typename ::hypertrie::internal::interface::boolhypertrie<key_part_type, map_type, set_type>::const_BoolHypertrie;

		using Diagonal = typename ::hypertrie::internal::interface::boolhypertrie<key_part_type, map_type, set_type>::Diagonal;

		using Join = typename ::hypertrie::internal::interface::join<key_part_type, map_type, set_type>::Join;

		using Einsum = typename ::einsum::internal::Einsum<key_part_type, map_type, set_type>;


		template<pos_type depth>
		using RawBoolhypertrie = typename ::hypertrie::internal::interface::rawboolhypertrie<key_part_type, map_type, set_type>::template RawBoolHypertrie<depth>;


		template<typename Sink>
		static void
		einsum(std::shared_ptr<Subscript> subscript, std::vector<const_BoolHypertrie> operands, Sink result) {
			tsl::hopscotch_map<std::vector<key_part_type>, std::size_t, KeyHash> results{};
			for (const auto &operand : operands)
				if (operand.size() == 0)
					return;

			auto op = ::einsum::internal::Operator<key_part_type, map_type, set_type>::construct(subscript);
			op.load(operands);
			for (auto &&entry : op) {
				result.add(std::move(entry));
			}
		}

		template<typename value_type = size_t>
		using EinsumEntry = ::einsum::internal::Entry<key_part_type, value_type>;
		using EinsumUnsignedEntry = ::einsum::internal::UnsignedEntry<key_part_type>;

		static auto einsum2map(std::shared_ptr<Subscript> subscript, std::vector<const_BoolHypertrie> operands) {
			tsl::hopscotch_map<std::vector<key_part_type>, std::size_t, KeyHash> results{};
			for (const auto &operand : operands)
				if (operand.size() == 0)
					return results;

			auto op = ::einsum::internal::Operator<key_part_type, map_type, set_type>::construct(subscript);
			op.load(operands);
			for (auto &&entry : op) {
				results[entry.key] += entry.value;
			}
			return results;
		}

		using KeyHash = ::einsum::internal::KeyHash<key_part_type>;


	};

}

#endif //HYPERTRIE_BOOLHYPERTRIE_HPP
