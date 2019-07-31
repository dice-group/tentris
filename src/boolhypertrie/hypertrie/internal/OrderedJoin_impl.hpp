#ifndef HYPERTRIE_ORDEREDJOIN_IMPL_HPP
#define HYPERTRIE_ORDEREDJOIN_IMPL_HPP
#include "hypertrie/internal/util/CONSTANTS.hpp"

#include "hypertrie/internal/Join_impl.hpp"
#include "hypertrie/internal/BoolHypertrie.hpp"
#include "hypertrie/internal/util/PermutationSort.hpp"
#include "hypertrie/internal/util/CONSTANTS.hpp"
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>


namespace {
	using namespace iter;
}
namespace hypertrie::internal {
	template<typename key_part_type>
	class Join<key_part_type, hypertrie::internal::container::boost_flat_map, hypertrie::internal::container::boost_flat_set> {

		template<typename key, typename value>
		using map_type = container::tsl_sparse_map<key, value>;
		template<typename key>
		using set_type = container::boost_flat_set<key>;


		using Boolhypertrie = BoolHypertrieImpl<key_part_type, map_type, set_type>;
		using poss_type = std::vector<pos_type>;
		using Diagonal = typename Boolhypertrie::Diagonal;
		const std::vector<Boolhypertrie> &hypertries;
		const std::vector<std::vector<pos_type>> &positions;


	public:
		Join(const std::vector<Boolhypertrie> &hypertries,
		     const std::vector<std::vector<pos_type>> &positions)
				: hypertries(hypertries), positions(positions) {}

		class iterator {

		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::pair<const std::vector<Boolhypertrie>, key_part_type>;
			using difference_type = ptrdiff_t;
			using pointer = value_type *;
			using reference = value_type &;
		private:
			Join &join;
			std::vector<Boolhypertrie> outs;
			std::vector<std::vector<pos_type>> out_poss;
			std::vector<Diagonal> ops;
			std::vector<std::vector<pos_type>> op_poss;
			std::vector<pos_type> ops_in_out;
			std::vector<pos_type> pos_of_ops_in_out;

			key_part_type current_key_part;
			key_part_type end_key_part;
			bool ended = false;

			value_type value{outs, current_key_part};
		public:
			explicit iterator(Join &join, bool ended = false) : join{join} {
				for (const auto[pos, join_poss, hypertrie] : zip(range(std::size(join.hypertries)), join.positions,
				                                                 join.hypertries)) {
					if (size(join_poss) > 0) {
						ops.emplace_back(hypertrie.getDiagonal(join_poss));
						op_poss.emplace_back(pos);
					}

					if (size(join_poss) < hypertrie.depth()) {
						outs.emplace_back(hypertrie);
						out_poss.emplace_back(pos);
					}

				}
				auto min = std::numeric_limits<key_part_type>::min();
				if (not narrowRange(min)) {
					this->ended = true;
					return;
				}
				optimizeOperandOrder();
				ops[0].setLower(min); // TODO: go on here
				next();
			}

			inline void update() {
				for (const auto[out_pos, op_pos] : zip(pos_of_ops_in_out, ops_in_out)) {
					outs[out_pos] = ops[op_pos].getValue();
				}
			}

			inline void next() {
				// check if the end was reached
				static bool found;
				// _current_key_part is increased if containsAndUpdateLower returns false
				Diagonal &current_operand = ops.front();
				if (current_operand.empty()) {
					ended = true;
					return;
				}
				current_key_part = current_operand.getKeyPart();
				while (current_key_part < end_key_part) {
					found = true;
					// iterate all but the first Diagonal
					for (auto &operand: util::skip<1>(ops)) {

						if (not operand.containsAndIncrement(current_key_part)) {
							if (operand.empty()) {
								ended = true;
								return;
							}

							current_key_part = current_operand.setLower(operand.getKeyPart());
							found = false;
							break;
						}
					}

					if (found) {
						++current_operand;
						update();
						return;
					}

				}
				ended = true;
			}

			iterator &operator++() {
				if (not ended)
					next();
				return *this;
			}

			const iterator operator++(int) &{
				iterator next_it = *this;
				++(*this);
				return next_it;
			}

			inline bool operator==(const iterator &other) const {
				return this == &other or
				       (this->ended and other.ended) or
				       (not(this->ended or other.ended) and
				        (this->current_key_part == other.current_key_part));
			}

			inline bool operator!=(const iterator &other) const {
				return not operator==(other);
			}

			inline bool operator==(bool other) const { return other == *this; }

			inline bool operator!=(bool other) const { return other != *this; }

			reference operator*() const {
				return value;
			}

			explicit operator bool() const {
				return ended;
			}

		private:
			void optimizeOperandOrder() {
				// TODO: test it
				static const auto compare_func = [&](const Diagonal &x, const Diagonal &y) {
					return x.size() < y.size();
				};
				const auto permutation = util::sort_permutation::get<
						compare_func>(ops);
				util::sort_permutation::apply(ops, permutation);
				util::sort_permutation::apply(op_poss, permutation);

				// finds the stored positions from op_poss that are also in out_poss
				// it stores where they are found in out_poss to ops_in_out

				ops_in_out.clear();
				pos_of_ops_in_out.clear();

				auto out_poss_begin = out_poss.cbegin();
				auto out_poss_end = out_poss.cend();

				for (const auto[pos, op_pos] : enumerate(op_poss)) {
					auto found = std::find(out_poss_begin, out_poss_end, op_pos);
					if (found != out_poss_end) {
						pos_of_ops_in_out.emplace_back(std::distance(out_poss_begin, found));
						ops_in_out.emplace_back(pos);
					}
				}
			}

			bool narrowRange(key_part_type &min) {
				auto max = std::numeric_limits<key_part_type>::max();
				// get min and max
				for (auto i : range(2)) // do it twice
					for (const auto &diag :ops) {
						diag.narrowRange(min, max); // min and max are passed by reference and updated
						if (diag.empty())
							return false;
					}
				return true;
			}

			iterator begin() { return iterator(*this); }

			bool end() const { return false; }
		};

		iterator begin() { return iterator(*this); }

		bool end() const { return false; }


	};
}

#endif //HYPERTRIE_ORDEREDJOIN_IMPL_HPP


