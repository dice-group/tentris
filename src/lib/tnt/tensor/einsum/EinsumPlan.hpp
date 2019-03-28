#ifndef SPARSETENSOR_EINSUM_EINSUMPLAN
#define SPARSETENSOR_EINSUM_EINSUMPLAN

#include <cmath>
#include <exception>
#include <memory>
#include <ostream>
#include <vector>

#include "tnt/tensor/einsum/Subscript.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/util/All.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

namespace {
	using namespace tnt::util::types;
	using namespace tnt::tensor::hypertrie;

	// TODO: move to utils
	template<typename T>
	std::set<label_t> getSubset(const T &interable, const label_t &remove_) {
		std::set<label_t> sub_set;
		std::copy_if(interable.cbegin(), interable.cend(), std::inserter(sub_set, sub_set.begin()),
		             [&](const label_t &l) { return remove_ != l; });
		return sub_set;
	}

	// TODO: move to utils
	template<typename T>
	std::set<T> setMinus(std::set<T> plusSet, std::set<T> minusSet1, std::set<T> minusSet2) {
		std::set<T> result{};
		for (const T &item : plusSet)
			if (not minusSet1.count(item) and not minusSet2.count(item))
				result.insert(item);
		return result;
	}

}; // namespace

namespace tnt::tensor::einsum {
	/**
	 * An EinsumPlan models the order in which the labels of an Einstein summation expression are resolved.
	 */
	class EinsumPlan {
	public:
		class Step;

	private:
		/**
		 * The subscript that is base to this Plan.
		 */
		const std::shared_ptr<const Subscript> subscript;
		/**
		 * The set of result labels.
		 */
		const std::vector<label_t> result_labels;
		/**
		 * The first step of this plans. All further Steps are within that step.
		 */
		mutable Step *initial_step = nullptr;

	public:
		explicit EinsumPlan(const std::shared_ptr<const Subscript> subscript)
				: subscript(subscript), result_labels{subscript->getResultLabels()} {}

		~EinsumPlan() {
			delete initial_step;
		}

		Step &getInitialStep(const Operands &operands) const {
			if (initial_step == nullptr) {
				initial_step = Step::getInitialStep(*this, operands);
			}
			return *initial_step;
		}

		const std::shared_ptr<const Subscript> getSubscript() const { return subscript; }

		const std::vector<label_t> &getResultLabels() const { return result_labels; }

		/**
		 * This class models a step in the evaluation of a Einstein summation expression. Each step represents one label
		 * that is resolved.
		 */
		class Step {
		private:
			mutable std::map<label_t, Step> next_step_cache;
			const std::shared_ptr<const Subscript> subscript;
			const std::map<label_t, label_pos_t> result_label_poss;

		public:
			const label_t label;

		private:
			const std::set<label_t> label_candidates;
			const std::set<label_t> unused_result_labels;

		public:
			const bool all_done;
			const bool result_labels_done;

		private:
			std::vector<op_pos_t> op_poss;
			std::optional<key_pos_t> result_pos;
			std::map<op_pos_t, op_pos_t> _diagonal2result_pos;
			std::vector<std::vector<key_pos_t>> joinee_key_part_poss; ///< the joining key part positions of each join operand.
			std::vector<op_pos_t> next_op_poss;
			std::vector<std::vector<label_pos_t>> unique_label_poss;

		private:
			/**
			 * Constructor for a step. All other Constructors must delegate to this one.
			 * @param subscript the subscript for this step
			 * @param result_label_poss the position of result labels
			 * @param min_card_label the label which this step provides as best label
			 * @param label_candidates the labels that are canditates to be calculated next. This may include min_card_label
			 * @param unused_result_labels the result labels that have not been used yet
			 */
			Step(const std::shared_ptr<const Subscript> subscript,
			     const std::map<label_t, label_pos_t> &result_label_poss,
			     const label_t &min_card_label,
			     const std::set<label_t> &label_candidates,
			     const std::set<label_t> &unused_result_labels)
					: subscript{subscript},
					  result_label_poss{result_label_poss},
					  label{min_card_label},
					  label_candidates{getSubset(label_candidates, label)},
					  unused_result_labels{getSubset(unused_result_labels, label)},
					  all_done{label_candidates.empty()},
					  result_labels_done{unused_result_labels.empty()} {
				if (not all_done) {
					op_poss = subscript->operandsWithLabel(label);
					auto found = result_label_poss.find(label);
					if (found != result_label_poss.end())
						result_pos = {found->second};
					else
						result_pos = std::nullopt;

					// the joining key part positions of each join operand.
					joinee_key_part_poss.reserve(op_poss.size());
					for (const op_pos_t &op_pos : op_poss) {
						joinee_key_part_poss.emplace_back(subscript->labelPossInOperand(op_pos, label));
					}

					const auto subsc = subscript->removeLabel(label);
					next_op_poss = subsc->getOriginalOpPoss();

					for (op_pos_t i = 0, j = 0; i < op_poss.size() and j < next_op_poss.size();) {
						auto pos_of_join_in_operands = op_poss.at(i);
						auto pos_of_result_in_operands = next_op_poss.at(j);
						if (pos_of_join_in_operands == pos_of_result_in_operands) {
							_diagonal2result_pos[i] = j;
							++j;
							++i;
						} else if (pos_of_join_in_operands < pos_of_result_in_operands) {
							++i;
						} else {
							++j;
						}
					}
				} else {
					// copy the sparse map of unique non-result contractions into a dense vector.
					const auto &u_contr = subscript->getUniqueNonResultContractions();

					for (const op_pos_t op_pos : range(op_pos_t(subscript->numberOfOperands()))) {
						if (const auto found = u_contr.find(op_pos); found != u_contr.cend()) {
							unique_label_poss.push_back(found->second.at(0));
						} else {
							unique_label_poss.emplace_back(std::vector<label_pos_t>{});
						}
					}
				}
			}

			/**
			 * This constructor is used to construct non-initial steps. It is called only by internal functions.
			 * @param subscript the subscript for this step
			 * @param result_label_poss the position of result labels
			 * @param operands the operands neccessary to calculate which label is calculated next best.
			 * @param label_candidates the labels that are canditates to be calculated next
			 * @param unused_result_labels the result labels that have not been used yet
			 */
			Step(const std::shared_ptr<const Subscript> subscript,
			     const std::map<label_t, label_pos_t> &result_label_poss,
			     const Operands &operands, const std::set<label_t> &label_candidates,
			     const std::set<label_t> &unused_result_labels)
					: Step(subscript, result_label_poss, getMinCardLabel(operands, label_candidates, *subscript),
					       label_candidates, unused_result_labels) {}

		public:
			/**
			 * Get the initial steps for a plan and a given operands.
			 * @param plan the plan
			 * @param operands the operands
			 * @return the first step to process the operands with the plan.
			 */
			static Step *getInitialStep(const EinsumPlan &plan, const Operands &operands) {
				const auto subscript = plan.subscript;

				const auto result_labels = subscript->getResultLabels();
				const std::set<label_t> result_label_set{result_labels.cbegin(), result_labels.cend()};

				const auto label_candidates = setMinus(subscript->getAllLabels(), subscript->getUniqueNonResultLabels(),
				                                       subscript->getLonelyNonResultLabels());

				const auto min_card_label = getMinCardLabel(operands, label_candidates, *subscript);

				return new Step{subscript, subscript->getLabelPosInResult(), min_card_label, label_candidates,
				                result_label_set};
			}

			/**
			 * Assuming you have an approriate operands list that shall be evaluated according to the subscript of this
			 * Step. When using this.label to resolve this step, nextStep() returns the Step for the resulting operands.
			 * @param operands the operands which are associated with this Step
			 * @return the Step for operands that result from resolving the label proposed by this Step
			 */
			Step &nextStep(const Operands &operands) const {
				if (all_done)
					throw std::invalid_argument("Must not be called if all_done is true");
				label_t next_label = getMinCardLabel(operands, label_candidates, *(subscript->removeLabel(label)));

				if (auto found = next_step_cache.find(next_label); found != next_step_cache.end()) {
					return found->second;
				} else {
					const auto &result =
							next_step_cache.emplace(next_label, Step{subscript->removeLabel(label), result_label_poss,
							                                         next_label, label_candidates,
							                                         unused_result_labels});
					return result.first->second;
				}
			}

			const std::map<label_t, label_pos_t> &getResultLabels() const { return result_label_poss; }

			size_t getResultSize() const { return result_label_poss.size(); }

			const std::vector<op_pos_t> &getOperandPositions() const { return op_poss; }

			const std::vector<std::vector<key_pos_t>> &getKeyPartPoss() const { return joinee_key_part_poss; }

			const std::vector<op_pos_t> &getPosOfOperandsInResult() const { return next_op_poss; }

			/**
             * Returns a mapping from {0..n}->{0..r} where n is the number of
             * operands with label and r is the number of non-scalar hypertries
             * that result from the operation described by this plan.
			 * @return a map that like defined above
			 */
			const std::map<op_pos_t, op_pos_t> &
			getDiagonal2ResultMapping() const { return _diagonal2result_pos; }

			/**
             * An optional position in the key that the key_part that fullfill this
             * step must be written if it exists.
			 * @return an optional position in the key_part.
			 */
			const std::optional<key_pos_t> &getResulKeyPos() const { return result_pos; }

			const std::vector<std::vector<label_pos_t>> &
			getUniqueNonResultContractions() const { return unique_label_poss; }

		private:
			/**
			 *
			 * @param operands
			 * @param label_candidates
			 * @param sc
			 * @return
			 */
			static label_t getMinCardLabel(const Operands &operands,
			                               const std::set<label_t> &label_candidates,
			                               const Subscript &sc) {
				if (label_candidates.size() == 1) {
					return *label_candidates.begin();
				} else {

					label_t min_label = *label_candidates.begin();
					double min_cardinality = std::numeric_limits<double>::infinity();
					for (const label_t &label : label_candidates) {
						if (const double label_cardinality = calcCard(operands, label, sc);
								label_cardinality < min_cardinality) {
							min_cardinality = label_cardinality;
							min_label = label;
						}
					}
					return min_label;
				}
			}

			/**
			 * Calculates the cardinality of an Label in an Step.
			 * @tparam T type of the values hold by processed Tensors (Tensor).
			 * @param operands Operands for this Step.
			 * @param step current step
			 * @param label the label
			 * @return label's cardinality in current step.
			 */
			static double calcCard(const Operands &operands, const label_t &label, const Subscript &sc) {
				// get operands that have the label
				const std::vector<op_pos_t> &op_poss = sc.operandsWithLabel(label);
				std::vector<double> op_dim_cardinalities(op_poss.size(), 1.0);
				auto label_count = 0;
				auto min_dim_card = std::numeric_limits<size_t>::max();

				// iterate the operands that hold the label
				for (auto[i, op_pos] : enumerate(op_poss)) {
					const auto &operand = operands[op_pos];
					const auto op_dim_cards = operand->getCards(sc.labelPossInOperand(op_pos, label));
					const auto min_op_dim_card = *std::min_element(op_dim_cards.cbegin(), op_dim_cards.cend());
					const auto max_op_dim_card = *std::max_element(op_dim_cards.cbegin(), op_dim_cards.cend());

					label_count += op_dim_cards.size();
					// update minimal dimenension cardinality
					if (min_op_dim_card < min_dim_card)
						min_dim_card = min_op_dim_card;

					op_dim_cardinalities[i] = double(max_op_dim_card); // / op_dim_cards.size();
				}

				auto const min_dim_card_d = double(min_dim_card);
				const double card =
						std::accumulate(op_dim_cardinalities.cbegin(), op_dim_cardinalities.cend(), double(1),
										[&](double a, double b) {
											return a * min_dim_card_d / b;
										});
				return card;
			}

		public:
			friend struct ::fmt::formatter<tnt::tensor::einsum::EinsumPlan::Step>;
		};
	};
}; // namespace tnt::tensor::einsum

template<>
struct fmt::formatter<tnt::tensor::einsum::EinsumPlan::Step> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tnt::tensor::einsum::EinsumPlan::Step &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
		                 "<Step> \n"
		                 "{}\n"
		                 " label: {}\n"
		                 " label_candidates: {}\n"
		                 " all_done: {}\n"
		                 " op_poss: {}\n"
		                 " result_pos: {}\n"
		                 " next_op_poss: {}\n",
		                 *p.subscript, p.label, p.label_candidates, p.all_done, p.op_poss, p.result_pos,
		                 p.next_op_poss);
	}
};

#endif // SPARSETENSOR_EINSUM_EINSUMPLAN
