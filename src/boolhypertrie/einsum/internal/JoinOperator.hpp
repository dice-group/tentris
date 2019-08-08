#ifndef HYPERTRIE_JOINOPERATOR_HPP
#define HYPERTRIE_JOINOPERATOR_HPP

#include "einsum/internal/Operator.hpp"
#include "einsum/internal/CardinalityEstimation.hpp"

namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class JoinOperator {
		constexpr static const bool bool_value_type = std::is_same_v<value_type, bool>;
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;
		using Join_t = Join<key_part_type, map_type, set_type>;
		using Operator_t = Operator<value_type, key_part_type, map_type, set_type>;
		using CardinalityEstimation_t = CardinalityEstimation<key_part_type, map_type, set_type>;


		std::shared_ptr<Subscript> subscript;

		Join_t join;
		typename Join_t::iterator join_iter;
		bool is_result_label = false;
		LabelPossInOperands label_poss_in_ops;
		LabelPos label_pos_in_result;
		Label label = std::numeric_limits<Label>::max();
		key_part_type current_key_part;

		Operator_t sub_operator;
		typename Operator_t::iterator sub_operator_iter;

		bool ended_ = true;

	public:
		JoinOperator(std::shared_ptr<Subscript> subscript) : subscript(std::move(subscript)) {}


		static Entry<key_part_type, value_type> next(void *self_raw) {
			JoinOperator &self = *static_cast<JoinOperator *>(self_raw);
			if constexpr (bool_value_type) {
				if (self.subscript->all_result_done) {
					self.ended_ = true;
					return *self.sub_operator_iter;
				}
			}
			Entry<key_part_type, value_type> entry = *self.sub_operator_iter;
			if (self.is_result_label)
				entry.key[self.label_pos_in_result] = self.current_key_part;

			++self.sub_operator_iter;

			while (self.sub_operator_iter.ended()) {
				++self.join_iter;
				if (not self.join_iter) {
					self.ended_ = true;
					break;
				} else {
					std::vector<const_BoolHypertrie_t> next_operands;
					std::tie(next_operands, self.current_key_part) = *self.join_iter;
					self.sub_operator.load(std::move(next_operands));
					self.sub_operator_iter = self.sub_operator.begin();
				}
			}
			if constexpr (_debugeinsum_) fmt::print("[{}]->{} {}\n", fmt::join(entry.key, ","), entry.value, self.subscript);
			return entry;
		}

		static bool ended(void *self_raw) {
			return static_cast<JoinOperator *>(self_raw)->ended_;
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands) {
			static_cast<JoinOperator *>(self_raw)->load_impl(std::move(operands));
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<JoinOperator *>(self_raw)->subscript->hash();
		}

	private:
		inline void load_impl(std::vector<const_BoolHypertrie_t> operands) {
			if constexpr (_debugeinsum_) fmt::print("Join {}\n", subscript);
			ended_ = false;
			Label last_label = label;
			label = CardinalityEstimation_t::getMinCardLabel(operands, subscript);
			if (label != last_label) {
				label_poss_in_ops = subscript->getLabelPossInOperands(label);
				is_result_label = subscript->isResultLabel(label);
				if (is_result_label)
					label_pos_in_result = subscript->getLabelPosInResult(label);
			}
			std::shared_ptr<Subscript> next_subscript = subscript->removeLabel(label);
			// check if sub_operator was not yet initialized or if the next subscript is different
			if (sub_operator.type == Subscript::Type::None or sub_operator.hash() != next_subscript->hash()) {
				sub_operator = Operator_t::construct(next_subscript);
			}

			join = Join_t{operands, label_poss_in_ops};
			join_iter = join.begin();
			while (join_iter != join.end()) {
				std::vector<const_BoolHypertrie_t> next_operands;
				std::tie(next_operands, current_key_part) = *join_iter;
				sub_operator.load(std::move(next_operands));
				sub_operator_iter = sub_operator.begin();
				if (not sub_operator_iter.ended()) {
					return;
				} else {
					++join_iter;
					continue;
				}
			}
			ended_ = true;
		}

	};
}
#endif //HYPERTRIE_JOINOPERATOR_HPP
