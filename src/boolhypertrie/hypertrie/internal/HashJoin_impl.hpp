#ifndef HYPERTRIE_HASHJOIN_IMPL_HPP
#define HYPERTRIE_HASHJOIN_IMPL_HPP

#include <utility>

#include "hypertrie/internal/BoolHypertrie.hpp"
#include "hypertrie/internal/util/PermutationSort.hpp"
#include "hypertrie/internal/util/CONSTANTS.hpp"
#include "hypertrie/internal/container/TslMap.hpp"
#include "hypertrie/internal/container/BoostFlatSet.hpp"
#include "hypertrie/internal/util/CONSTANTS.hpp"
#include "hypertrie/internal/Join_impl.hpp"


namespace {
	using namespace iter;

}

namespace hypertrie::internal {

	template<typename key_part_type,
			template<typename, typename> class map_type,
			template<typename> class set_type>
	class HashJoin {

	public:
		using const_BoolHypertrie = typename interface::boolhypertrie<key_part_type, map_type, set_type>::const_BoolHypertrie;
		using Diagonal = typename interface::boolhypertrie<key_part_type, map_type, set_type>::HashDiagonal;
		using poss_type = std::vector<pos_type>;

	private:
		std::vector<const_BoolHypertrie> hypertries;
		std::vector<poss_type> positions;

	public:

		HashJoin() = default;

		HashJoin(HashJoin &) = default;

		HashJoin(const HashJoin &) = default;


		HashJoin(std::vector<const_BoolHypertrie> hypertries, std::vector<poss_type> positions)
				: hypertries(std::move(hypertries)), positions(std::move(positions)) {}

		class iterator {

		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::pair<std::vector<const_BoolHypertrie>, key_part_type>;
			using difference_type = ptrdiff_t;
			using pointer = value_type *;
			using reference = value_type &;
		private:
			poss_type pos_in_out{};
			std::vector<pos_type> result_depths{};
			std::vector<Diagonal> ops{};
			std::vector<void*> raw_outs{};

			bool ended = false;

			value_type value{};
		public:
			iterator() = default;

			iterator(const HashJoin &join) {
				auto max_op_count = join.hypertries.size();
				pos_in_out.reserve(max_op_count);
				result_depths.reserve(max_op_count);
				ops.reserve(max_op_count);

				pos_type out_pos = 0;
				for (const auto &[pos, join_poss, hypertrie] : zip(range(join.hypertries.size()), join.positions,
				                                                   join.hypertries)) {
					if (size(join_poss) > 0) {
						ops.emplace_back(Diagonal{hypertrie, join_poss});
						auto result_depth = result_depths.emplace_back(hypertrie.depth() - size(join_poss));
						if (result_depth) {
							pos_in_out.push_back(out_pos++);
							value.first.push_back(hypertrie); // only a place holder, is to be replaced in next()
						} else {
							pos_in_out.push_back(std::numeric_limits<pos_type>::max());
						}
					} else {
						assert(hypertrie.depth() != 0); // TODO: currently not possible
						value.first.push_back(hypertrie); // this stays unchanged during the itration
						++out_pos;
					}
				}
				optimizeOperandOrder();
				raw_outs.resize(ops.size());
				ops.front().init();
				next();
			}

			inline void next() {
				// check if the end was reached
				static bool found;
				// _current_key_part is increased if containsAndUpdateLower returns false
				Diagonal &smallest_operand = ops.front();

				while (not smallest_operand.empty()) {

					value.second = smallest_operand.currentKeyPart();

					found = true;
					// iterate all but the first Diagonal
					for (const auto &[op_pos, operand]: iter::enumerate(util::skip<1>(ops), 1)) {
							if (not operand.contains(value.second)) {
								found = false;
								break;
							}
					}
					if (found) {
						for (const auto &[op_pos, raw_op_ptr]: iter::enumerate(raw_outs)) {
							if (const auto &result_depth = result_depths[op_pos]; result_depth)
								value.first[pos_in_out[op_pos]] = const_BoolHypertrie::instance(result_depth, ops[op_pos].currentValue());
						}
						++smallest_operand;
						return;
					}
					++smallest_operand;
				}
				ended = true;
			}

			iterator &operator++() {
				if (not ended)
					next();
				return *this;
			}

			inline operator bool() const {
				return not ended;
			}

			value_type operator*() const {
				return value;
			}

		private:
			void optimizeOperandOrder() {
				const auto permutation = util::sort_permutation::get<Diagonal>(ops);
				util::sort_permutation::apply(ops, permutation);
				util::sort_permutation::apply(pos_in_out, permutation);
				util::sort_permutation::apply(result_depths, permutation);
			}

		};

		iterator begin() const { return iterator(*this); }

		bool end() const { return false; }


	};
}

#endif //HYPERTRIE_HASHJOIN_IMPL_HPP



