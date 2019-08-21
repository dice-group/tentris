#ifndef HYPERTRIE_CARDINALITYESTIMATION_HPP
#define HYPERTRIE_CARDINALITYESTIMATION_HPP

#include <cmath>
#include "einsum/internal/Subscript.hpp"
#include "einsum/internal/Entry.hpp"

#define DEBUGCARD

namespace einsum::internal {


	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	struct CardinalityEstimation {
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;

		/**
		 *
		 * @param operands
		 * @param label_candidates
		 * @param sc
		 * @return
		 */
		static Label getMinCardLabel(const std::vector<const_BoolHypertrie_t> &operands,
		                             const std::shared_ptr<Subscript> &sc) {
			const tsl::hopscotch_set <Label> &operandsLabelSet = sc->getOperandsLabelSet();
			const tsl::hopscotch_set <Label> &lonely_non_result_labels = sc->getLonelyNonResultLabelSet();
			if (operandsLabelSet.size() == 1) {
				return *operandsLabelSet.begin();
			} else {

				Label min_label = *operandsLabelSet.begin();
				double min_cardinality = std::numeric_limits<double>::infinity();
				for (const Label label : operandsLabelSet) {
					if (lonely_non_result_labels.count(label))
						continue;
					const double label_cardinality = calcCard(operands, label, sc);
					if (label_cardinality < min_cardinality) {
						min_cardinality = label_cardinality;
						min_label = label;
					}
				}
				return min_label;
			}
		}

	protected:
		/**
		 * Calculates the cardinality of an Label in an Step.
		 * @tparam T type of the values hold by processed Tensors (Tensor).
		 * @param operands Operands for this Step.
		 * @param step current step
		 * @param label the label
		 * @return label's cardinality in current step.
		 */
		static double calcCard(const std::vector<const_BoolHypertrie_t> &operands, const Label label,
		                       const std::shared_ptr<Subscript> &sc) {
			// get operands that have the label
			const std::vector<LabelPos> &op_poss = sc->getPossOfOperandsWithLabel(label);
			std::vector<double> op_dim_cardinalities(op_poss.size(), 1.0);
			auto label_count = 0;
			auto min_dim_card = std::numeric_limits<size_t>::max();
			tsl::hopscotch_set <size_t> sizes{};

			const LabelPossInOperands &label_poss_in_operands = sc->getLabelPossInOperands(label);
			// iterate the operands that hold the label
			for (auto[i, op_pos] : enumerate(op_poss)) {
				const auto &operand = operands[op_pos];
				const auto op_dim_cards = operand.getCards(label_poss_in_operands[op_pos]);
				const auto min_op_dim_card = *std::min_element(op_dim_cards.cbegin(), op_dim_cards.cend());
				const auto max_op_dim_card = *std::max_element(op_dim_cards.cbegin(), op_dim_cards.cend());

				for (const auto &op_dim_card : op_dim_cards)
					sizes.insert(op_dim_card);

				label_count += op_dim_cards.size();
				// update minimal dimenension cardinality
				if (min_op_dim_card < min_dim_card)
					min_dim_card = min_op_dim_card;

				op_dim_cardinalities[i] = double(max_op_dim_card); //
			}

			std::size_t max_op_size = 0;
			std::vector<std::size_t> op_sizes{};
			for (auto op_pos : op_poss) {
				auto current_op_size = operands[op_pos].size();
				op_sizes.push_back(current_op_size);
				if (current_op_size > max_op_size)
					max_op_size = current_op_size;
			}

			auto const min_dim_card_d = double(min_dim_card);

			double card = std::accumulate(op_dim_cardinalities.cbegin(), op_dim_cardinalities.cend(), double(1),
			                              [&](double a, double b) {
				                              return a * min_dim_card_d / b;
			                              }) / sizes.size();
			return card;
		}
	};


}
#endif //HYPERTRIE_CARDINALITYESTIMATION_HPP
