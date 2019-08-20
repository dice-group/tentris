#ifndef HYPERTRIE_CARTESIANOPERATOR_HPP
#define HYPERTRIE_CARTESIANOPERATOR_HPP

#include "einsum/internal/Operator.hpp"
#include <tsl/sparse_map.h>

namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class CartesianOperator {
		constexpr static const bool bool_value_type = std::is_same_v<value_type, bool>;
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;
		using Operator_t = Operator<value_type, key_part_type, map_type, set_type>;

		using SubResult = tsl::sparse_map<Key < key_part_type>, size_t, absl::Hash<Key < key_part_type> >>;


		class FullCartesianResult;

		std::shared_ptr<Subscript> subscript; // set in construct
		std::vector<Operator_t> sub_operators; // set in construct
		std::vector<Entry < key_part_type, value_type>> sub_entries;
		Entry <key_part_type, value_type> *entry;

		std::size_t iterated_pos; // set in load_impl
		OriginalResultPoss iterated_sub_operator_result_mapping; // set in load_impl
		FullCartesianResult calculated_operands; // set in load_impl // updated in next
		bool ended_ = true; // set in load_impl // updated in load_impl, next

	public:
		CartesianOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {
			// generate sub-operators
			const std::vector<std::shared_ptr<Subscript>> &sub_subscripts = this->subscript->getCartesianSubscript().getSubSubscripts();
			sub_operators.reserve(sub_subscripts.size());
			sub_entries.reserve(sub_subscripts.size());
			for (const auto &sub_subscript : sub_subscripts) {
				sub_operators.push_back(Operator_t::construct(sub_subscript));
				using EntryKey = typename Entry<key_part_type, value_type>::key_type;
				sub_entries.push_back({value_type(0), EntryKey(sub_subscript->resultLabelCount(), std::numeric_limits<key_part_type>::max())});
			}

		}


		static void next(void *self_raw) {
			auto &self = *static_cast<CartesianOperator *>(self_raw);
			// get the accumulated entry from the from pre-calculated carth_operands
			if constexpr (bool_value_type) {
				if (self.subscript->all_result_done) {
					self.ended_ = true;
					return;
				}
			}
			++self.calculated_operands;

			if (self.calculated_operands.ended()) {
				auto &intereated_sub_op = self.sub_operators[self.iterated_pos];
				++intereated_sub_op;
				if (not intereated_sub_op.ended()) {
					self.calculated_operands.restart();

				} else { // if it is ended, set to ended_
					self.ended_ = true;
					return;
				}
			}
			const auto &iterated_sub_entry = self.sub_entries[self.iterated_pos];
			updateEntryKey(self.iterated_sub_operator_result_mapping, *self.entry, iterated_sub_entry.key);
			assert(iterated_sub_entry.value);
			assert(self.entry->value);
			if constexpr (not bool_value_type)
				self.entry->value *= iterated_sub_entry.value;
			if constexpr (_debugeinsum_)
				fmt::print("[{}]->{} {}\n", fmt::join(self.entry->key, ","), self.entry->value, self.subscript);
		}

		static bool ended(void *self_raw) {
			auto &self = *static_cast<CartesianOperator *>(self_raw);
			return self.ended_;
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<CartesianOperator *>(self_raw)->subscript->hash();
		}

		static void
		load(void *self_raw, std::vector<const_BoolHypertrie_t> operands, Entry <key_part_type, value_type> &entry) {
			static_cast<CartesianOperator *>(self_raw)->load_impl(std::move(operands), entry);
		}


	private:

		std::vector<const_BoolHypertrie_t>
		extractOperands(CartesianOperandPos cart_op_pos, const std::vector<const_BoolHypertrie_t> &operands) {

			std::vector<const_BoolHypertrie_t> sub_operands;

			for (const auto &original_op_pos : subscript->getCartesianSubscript().getOriginalOperandPoss(cart_op_pos))
				sub_operands.emplace_back(operands[original_op_pos]);

			return sub_operands;
		}

		inline void load_impl(std::vector<const_BoolHypertrie_t> operands, Entry <key_part_type, value_type> &entry) {
			if constexpr(_debugeinsum_) fmt::print("Cartesian {}\n", subscript);
			this->entry = &entry;
			ended_ = false;
			iterated_pos = 0; // todo: we can do better here
			iterated_sub_operator_result_mapping = {
					subscript->getCartesianSubscript().getOriginalResultPoss()[iterated_pos]};

			std::vector<SubResult> sub_results{};
			// load the non iterated sub_operators

			if constexpr(_debugeinsum_) fmt::print("Cartesian sub start {}\n", subscript);
			for (auto cart_op_pos: iter::range(sub_operators.size())) {
				auto &cart_op = sub_operators[cart_op_pos];
				cart_op.load(extractOperands(cart_op_pos, operands), sub_entries[cart_op_pos]);
				if (cart_op.ended()) {
					ended_ = true;
					return;
				}
			}
			if constexpr(_debugeinsum_) fmt::print("Cartesian sub gen {}\n", subscript);
			// calculate results of non-iterated sub_operators
			// TODO: parallelize
			for (auto cart_op_pos: iter::range(sub_operators.size())) {
				if (cart_op_pos == iterated_pos)
					continue;
				SubResult sub_result{};
				auto &cart_op = sub_operators[cart_op_pos];
				if constexpr (bool_value_type) {
					if (subscript->all_result_done) {
						if (cart_op.ended()) {
							ended_ = true;
							return;
						}
						auto &sub_entry = sub_entries[cart_op_pos];
						assert(sub_entry.value);
						sub_result[sub_entry.key] = sub_entry.value;
						sub_results.emplace_back(std::move(sub_result));
						continue;
					}
				}
				auto &sub_entry = sub_entries[cart_op_pos];
				while (not cart_op.ended()) {
					assert(sub_entry.value);
					sub_result[sub_entry.key] += sub_entry.value;
					++cart_op;
				}
				if (sub_result.empty()) {
					ended_ = true;
					return;
				}
				sub_results.emplace_back(std::move(sub_result));
			}
			calculated_operands = FullCartesianResult(std::move(sub_results), subscript->getCartesianSubscript(),
			                                          *this->entry);
			if constexpr(_debugeinsum_) fmt::print("Cartesian main start {}\n", subscript);

			// init iterator for the subscript part that is iterated as results are written out.
			Operator_t &iterated_sub_operator = sub_operators[iterated_pos];
			if (iterated_sub_operator.ended()) {
				ended_ = true;
				return;
			}
			updateEntryKey(iterated_sub_operator_result_mapping, *this->entry, sub_entries[iterated_pos].key);
			this->entry->value *= sub_entries[iterated_pos].value;
		}


		static void
		updateEntryKey(const OriginalResultPoss &original_result_poss, Entry <key_part_type, value_type> &sink,
		               const typename Entry<key_part_type, value_type>::key_type &source_key) {
			for (auto i : iter::range(original_result_poss.size()))
				sink.key[original_result_poss[i]] = source_key[i];
		}


		class FullCartesianResult {
			std::vector<SubResult> sub_results;
			typename std::vector<typename SubResult::const_iterator> iters; // set in constructor
			typename std::vector<typename SubResult::const_iterator> ends;
			Entry <key_part_type, value_type> *entry;
			value_type value;
			std::vector<OriginalResultPoss> result_mapping;
			bool ended_ = false;

		public:
			FullCartesianResult() = default;

			FullCartesianResult(std::vector<SubResult> sub_results,
			                    const CartesianSubSubscripts &cartSubSubscript,
			                    Entry <key_part_type, value_type> &entry)
					:
					sub_results{std::move(sub_results)}, iters(this->sub_results.size()),
					ends(this->sub_results.size()),
					entry{&entry} {
				const auto &original_result_poss = cartSubSubscript.getOriginalResultPoss();
				result_mapping = {original_result_poss.begin() + 1, // TODO: we need to change that when we do better
				                  original_result_poss.end()};
				value = value_type(1);
				for (auto i: range(this->sub_results.size())) {
					const auto &sub_result = this->sub_results[i];
					iters[i] = sub_result.cbegin();
					ends[i] = sub_result.cend();
					updateEntryKey(result_mapping[i], *this->entry, iters[i]->first);
					assert(value);
					assert(iters[i]->second);
					if constexpr (not bool_value_type)
						value *= iters[i]->second;
				}
				this->entry->value = value;
				restart();
			}

			inline void operator++() {
				next();
			}

			void next() {
				for (auto i : iter::range(sub_results.size())) {
					[[maybe_unused]] auto last_value = iters[i]->second;
					++iters[i];
					if (iters[i] != ends[i]) {
						updateEntryKey(result_mapping[i], *this->entry, iters[i]->first);
						if constexpr (not bool_value_type) // all entries are true anyways
							value = (value * iters[i]->second) / last_value;
						this->entry->value = value;
						return;
					} else {
						iters[i] = sub_results[i].cbegin();
						updateEntryKey(result_mapping[i], *this->entry, iters[i]->first);
						if constexpr (not bool_value_type) // all entries are true anyways
							value = (value * iters[i]->second) / last_value;
					}
				}
				assert(value);
				this->entry->value = value;
				ended_ = true;
			}

			bool ended() {
				return ended_;
			}

			void restart() {
				ended_ = false;
				for (auto i: range(sub_results.size())) {
					const auto &sub_result = sub_results[i];
					assert(iters[i] == sub_result.cbegin());
					assert(ends[i] == sub_result.cend());
				}
			}
		};

	};
}
#endif //HYPERTRIE_CARTESIANOPERATOR_HPP

