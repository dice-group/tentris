#ifndef HYPERTRIE_CARTESIANOPERATOR_HPP
#define HYPERTRIE_CARTESIANOPERATOR_HPP

#include "einsum/internal/Operator.hpp"

namespace einsum::internal {

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class CartesianOperator {
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;
		using Operator_t = Operator<key_part_type, map_type, set_type>;

		using SubResult = tsl::hopscotch_map<Key < key_part_type>, size_t, KeyHash <key_part_type >>;


		class FullCarthesianResult;

		std::shared_ptr<Subscript> subscript; // set in construct
		std::vector<Operator_t> sub_operators; // set in construct

		std::size_t iterated_pos; // set in load_impl
		typename Operator_t::iterator iterated_sub_operator_iterator; // set in load_impl // updated in next
		UnsignedEntry <key_part_type> iterated_sub_operator_entry; // set in load_impl // updated in next
		OriginalResultPoss iterated_sub_operator_result_mapping; // set in load_impl
		FullCarthesianResult calculated_operands; // set in load_impl // updated in next
		bool ended_ = true; // set in load_impl // updated in load_impl, next

	public:
		CartesianOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {
			// generate sub-operators
			for (const auto &sub_subscript : this->subscript->getCartesianSubscript().getSubSubscripts())
				sub_operators.push_back(Operator_t::construct(sub_subscript));
		}


		static UnsignedEntry <key_part_type> next(void *self_raw) {
			auto &self = *static_cast<CartesianOperator *>(self_raw);
			// get the accumulated entry from the from pre-calculated carth_operands
			auto entry = *self.calculated_operands;
			// add also the iterated carth_operand to that entry
			writeToEntry(self.iterated_sub_operator_result_mapping, entry, self.iterated_sub_operator_entry);

			//// increment
			// inc the precalculated carth_operands
			++self.calculated_operands;

			if (self.calculated_operands.ended()) {
				// if they are at the end, it means the iterated carth_operand must be incremented
				++self.iterated_sub_operator_iterator;
				// if it is not ended, cache its new result and restart the calculated_operands
				if (not self.iterated_sub_operator_iterator.ended()) {
					self.iterated_sub_operator_entry = *self.iterated_sub_operator_iterator;
					self.calculated_operands.restart();
				} else { // if it is ended, set to ended_
					self.ended_ = true;
				}
			}

			return entry;
		}

		static bool ended(void *self_raw) {
			return static_cast<CartesianOperator *>(self_raw)->ended_;
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<CartesianOperator *>(self_raw)->subscript->hash();
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands) {
			static_cast<CartesianOperator *>(self_raw)->load_impl(std::move(operands));
		}


	private:

		std::vector<const_BoolHypertrie_t>
		extractOperands(CartesianOperandPos cart_op_pos, const std::vector<const_BoolHypertrie_t> &operands) {

			std::vector<const_BoolHypertrie_t> sub_operands;

			for (const auto &original_op_pos : subscript->getCartesianSubscript().getOriginalOperandPoss(cart_op_pos))
				sub_operands.emplace_back(operands[original_op_pos]);

			return sub_operands;
		}

		inline void load_impl(std::vector<const_BoolHypertrie_t> operands) {
			ended_ = false;
			iterated_pos = 0; // todo: we can do better here
			iterated_sub_operator_result_mapping = {
					subscript->getCartesianSubscript().getOriginalResultPoss()[iterated_pos]};

			std::vector<SubResult> sub_results{};
			// load the non iterated sub_operators

			for (auto cart_op_pos: iter::range(sub_operators.size())) {
				if (cart_op_pos == iterated_pos)
					continue;
				auto &cart_op = sub_operators[cart_op_pos];
				cart_op.load(extractOperands(cart_op_pos, operands));
				if (cart_op.begin().ended()) {
					ended_ = true;
					return;
				}
			}
			// calculate results of non-iterated sub_operators
			// TODO: parallelize
			for (auto cart_op_pos: iter::range(sub_operators.size())) {
				SubResult sub_result{};
				if (cart_op_pos == iterated_pos)
					continue;
				auto &cart_op = sub_operators[cart_op_pos];
				for (auto entry : cart_op)
					sub_result[entry.key] += entry.value;
				if (sub_result.empty()) {
					ended_ = true;
					return;
				}
				sub_results.emplace_back(std::move(sub_result));
			}
			calculated_operands = FullCarthesianResult(std::move(sub_results), subscript->getCartesianSubscript());

			// init iterator for the subscript part that is iterated as results are written out.
			Operator_t &iterated_sub_operator = sub_operators[iterated_pos];
			iterated_sub_operator.load(extractOperands(iterated_pos, operands));
			iterated_sub_operator_iterator = iterated_sub_operator.begin();

			if (iterated_sub_operator_iterator.ended()) {
				ended_ = true;
				return;
			}
			iterated_sub_operator_entry = *iterated_sub_operator_iterator;
		}


		static void writeToEntry(const OriginalResultPoss &original_result_poss, UnsignedEntry <key_part_type> &sink,
		                         const UnsignedEntry <key_part_type> &source, const std::size_t last_value = 1) {
			for (auto i : iter::range(original_result_poss.size()))
				sink.key[original_result_poss[i]] = source.key[i];
			sink.value = sink.value * source.value / last_value;
		}


		class FullCarthesianResult {
			std::vector<SubResult> sub_results;
			typename std::vector<typename SubResult::const_iterator> iters; // set in constructor
			typename std::vector<typename SubResult::const_iterator> ends;
			UnsignedEntry <key_part_type> current_entry;
			std::vector<OriginalResultPoss> result_mapping;
			bool ended_ = false;

		public:
			FullCarthesianResult() = default;

			FullCarthesianResult(std::vector<SubResult> sub_results,
			                     const CartesianSubSubscripts &cartSubSubscript) :
					sub_results{sub_results}, iters(sub_results.size()), ends(sub_results.size()),
					current_entry{} {
				const auto &original_result_poss = cartSubSubscript.getOriginalResultPoss();
				result_mapping = {original_result_poss.begin() + 1, // TODO: we need to change that when we do better
				                  original_result_poss.end()};
				current_entry.key.resize(cartSubSubscript.getSubscript()->resultLabelCount(), {});
				restart();
			}

			const UnsignedEntry <key_part_type> &operator*() {
				return current_entry;
			}

			void operator++() {
				for (auto i : iter::range(sub_results.size())) {
					auto last_value = iters[i]->second;
					++iters[i];
					if (iters[i] != ends[i]) {
						writeToEntry(result_mapping[i], current_entry, {iters[i]->second, iters[i]->first}, last_value);
						return;
					} else {
						iters[i] = sub_results[i].cbegin();
						writeToEntry(result_mapping[i], current_entry, {iters[i]->second, iters[i]->first}, last_value);
					}
				}
				ended_ = true;
			}

			bool ended() {
				return ended_;
			}

			void restart() {
				current_entry.value = 1;
				ended_ = false;
				for (auto i: range(sub_results.size())) {
					const auto &sub_result = sub_results[i];
					iters[i] = sub_result.cbegin();
					ends[i] = sub_result.cend();
					writeToEntry(result_mapping[i], current_entry, {iters[i]->second, iters[i]->first});
				}
			}
		};

	};
}
#endif //HYPERTRIE_CARTESIANOPERATOR_HPP
