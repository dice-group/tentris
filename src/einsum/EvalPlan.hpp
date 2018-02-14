#ifndef SPARSETENSOR_EINSUM_EVALPLAN_HPP
#define SPARSETENSOR_EINSUM_EVALPLAN_HPP


#include "../hypertrie/PosCalc.hpp"
#include "Subscript.hpp"
#include <optional>
#include <variant>
#include <vector>
#include "../tensor/Tensor.hpp"
#include "../hypertrie/HyperTrie.hpp"
#include <parallel/numeric>
#include <cmath>
#include <ostream>

using std::optional;
using sparsetensor::hypertrie::HyperTrie;
using std::variant;
using std::vector;

namespace sparsetensor::einsum {

    class EvalPlan;

    /**
     * A PlanStep holds all relevant information for an step in sparsetensor::einsum::operators::Einsum and all
     * following steps. A step is the contraction of a single label.
     */
    class PlanStep {
        friend class EvalPlan;

        /**
         * Labels that could be processed in this step. Those labels must be adjacent to already processed labels in
         * the label dependency graph.
         * @see Subscript::calcLabelDependencyGraph()
         */
        unordered_set<label_t> label_candidates;
        /**
         * Labels that were already processed in previous steps.
         */
        unordered_set<label_t> processed_labels;
        /**
         * Operands that hold a specific label.
         */
        const unordered_map<label_t, vector<op_pos_t>> &operands_with_label;
        /**
         * Positions of a specific label in an specific operand.
         */
        unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operand;
        /**
         * Position of a specific label in the result.
         */
        const unordered_map<label_t, label_pos_t> &label_pos_in_result;
        /**
         * This PlanStep is part of that EvalPlan.
         */
        const EvalPlan *plan;
    public:
        /**
         * There are no labels left to be processed. This PlanStep MUST NOT be used.
         */
        bool all_done = true;

        PlanStep(const PlanStep &plan_step) :
                label_candidates(plan_step.label_candidates),
                processed_labels(plan_step.processed_labels),
                operands_with_label(plan_step.operands_with_label),
                label_pos_in_result(plan_step.label_pos_in_result),
                label_poss_in_operand(plan_step.label_poss_in_operand),
                plan(plan_step.plan),
                all_done(plan_step.all_done) {}

        PlanStep(const EvalPlan *plan);


        explicit PlanStep(unordered_set<label_t> label_candidates,
                          unordered_set<label_t> processed_labels,
                          map<op_pos_t, vector<label_t>> operands_labels,
                          const EvalPlan *plan);


        /**
         * Gives the positions of the operands that have the given label among their subscript labels.
         * @param label label to find the operands for
         * @return operands using the label
         */
        const vector<op_pos_t> &operandsWithLabel(const label_t &label) const {
            return operands_with_label.at(label);
        }

        /**
         * Position of a label in the result.
         * @return A map from a label to a label positions
         */
        const std::optional<label_pos_t> labelPosInResult(const label_t &label) const noexcept {
            auto res_pos_ = label_pos_in_result.find(label);
            if (res_pos_ != end(label_pos_in_result)) {
                return {res_pos_->second};
            } else {
                return std::nullopt;
            }
        }

        /**
         * Find the positions at which a given label occurs in a given operand.
         * @param op_pos Position of the operand.
         * @param label label that shall be found
         */
        const vector<label_pos_t> labelPossInOperand(const op_pos_t &op_pos, const label_t &label) const {
            return label_poss_in_operand.at({op_pos, label});
        }

    private:
        /**
         * Create a copy of operands_labels where #processed_labels are removed.
         * @param processed_labels Labels to be removed.
         * @return copy of operands_labels where #processed_labels are removed
         */
        map<op_pos_t, vector<label_t>> calcNextOperandsLabels(const unordered_set<label_t> &processed_labels);

    public:
        /**
         * Get the next PlanStep when using #label in this step.
         * @param label MUST be part of #label_candidates
         * @return the next PlanStep
         */
        PlanStep getNextStep(label_t label);

        friend ostream &operator<<(ostream &os, const PlanStep &step) {


            os << "<PlanStep: "
               << "label_candidates: " << step.label_candidates << ",\n processed_labels: " << step.processed_labels
               << ",\n operands_with_label: " << step.operands_with_label << ",\n label_pos_in_result: "
               << step.label_pos_in_result << ",\n label_poss_in_operand: " << step.label_poss_in_operand << ",\n all_done: "
               << step.all_done << ">";
            return os;
        }
    };

    /**
     * This Class plans the evaluation for an sparsetensor::einsum::operators::Einsum Operator. The plan is defined by a Subscript.
     */
    class EvalPlan {
        friend class PlanStep;

        /**
         * Subscript that is basis to this plan.
         */
        const Subscript &subscript;
        /**
         * Label dependency graph of this plan.
         * TODO: Currently just a copy of subscript.getLabelDependencyGraph().
         * When non_result_single_op_labels are introduced that will be different.
         */
        const UndirectedGraph<label_t> label_dependency_graph;
    public:
        /**
         * Constructor.
         * @param subscript Subscript that is basis to this plan. It is referenced.
         */
        explicit EvalPlan(const Subscript &subscript = Subscript{}) :
                subscript(subscript),
                label_dependency_graph(subscript.getLabelDependencyGraph()) {}


        /**
         * Calculates the first PlanStep.
         * @tparam T type of the values hold by processed Tensors (Tensor).
         * @param operands Operands that match the Subscript that was used to construct this plan.
         * @return a PlanStep and an Label (label_t). Check PlanStep.all_done before you use the the result.
         */
        template<typename T>
        tuple<PlanStep, label_t> firstStep(const vector<variant<HyperTrie<T> *, T>> &operands) {
            // todo: make temp_initial_step a pointer and cache it
            PlanStep temp_initial_step{this}; // todo: something is not right here
            unordered_set<label_t> label_candidates = subscript.getLabelDependencyGraph().getNodes();
            if (label_candidates.empty()) {
                return {temp_initial_step, {}};
            }
            label_t min_card_label = calcMinCardLabel<T>(operands, temp_initial_step, label_candidates);
            temp_initial_step.label_candidates = {min_card_label};
            PlanStep initial_step{{min_card_label}, {}, subscript.getOperandsLabels(), this};
            return {initial_step, min_card_label};
        }

        /**
         * Calculates the next PlanStep.
         * @tparam T type of the values hold by processed Tensors (Tensor).
         * @param operands Operands that match the Subscript that was used to construct this plan but reduced by all
         * dimensions that were selected in previous Steps (PlanStep).
         * @param last_step last PlanStep executed
         * @param last_label last Label used.
         * @return a PlanStep and an Label (label_t). Check PlanStep.all_done before you use the the result.
         */
        template<typename T>
        tuple<PlanStep, label_t> nextStep(const vector<variant<HyperTrie<T> *, T>> &operands, PlanStep &last_step,
                                          const label_t &last_label) {
            if (last_step.all_done) {
                throw "nextStep should not be called on 'last_step' with all_done = true";
            }
            PlanStep next_step = last_step.getNextStep(last_label);
            label_t min_card_label{};
            if(not next_step.all_done)
                min_card_label = calcMinCardLabel<T>(operands, next_step, next_step.label_candidates);
            return {next_step, min_card_label};
        }

        /**
         * Calculates the cardinality of an Label in an Step.
         * @tparam T type of the values hold by processed Tensors (Tensor).
         * @param operands Operands for this Step.
         * @param step current step
         * @param label the label
         * @return label's cardinality in current step.
         */
        template<typename T>
        float calcCard(const vector<variant<HyperTrie<T> *, T>> &operands, PlanStep &step, const label_t &label) {
            // get operands that have the label
            const vector<op_pos_t> &op_ids = step.operandsWithLabel(label);

            vector<float> dim_cardinalities(op_ids.size(), INFINITY);
            vector<float> operand_cardinalities(op_ids.size(), INFINITY);
            float min_dim_cardinality =  INFINITY;

            //iterate the operands that hold the label
            for (size_t i = 0; i < op_ids.size(); ++i) {
                const op_pos_t &op_id = op_ids.at(i);
                // get operand
                const HyperTrie<T> *operand = std::get<HyperTrie<T> *>(operands.at(i));
                // get cardinality of the operand in this dimension
                const vector<size_t> cards = operand->getCards(step.labelPossInOperand(op_id, label));

                // calc it's cardinality
                const size_t &dim_cardinality = *std::min_element(cards.cbegin(), cards.cend());
                // if it is zero the overall cardinality is zero
                if (dim_cardinality == 0)
                    return 0;

                if (dim_cardinality < min_dim_cardinality)
                    min_dim_cardinality = dim_cardinality;
                dim_cardinalities[i] = dim_cardinality;
                operand_cardinalities[i] = operand->leafcount;
            }

            // see: A. Swami and K. B. Schiefer, “On the estimation of join result sizes,” in International Conference on Extending Database Technology, 1994, pp. 287–300. (290-291)
            return min_dim_cardinality
                   * std::accumulate(operand_cardinalities.cbegin(), operand_cardinalities.cend(), 1,
                                     std::multiplies<size_t>())
                   / std::accumulate(dim_cardinalities.cbegin(), dim_cardinalities.cend(), 1,
                                     std::multiplies<size_t>())
                   // prefer smaller min_dim cardinality
                    + (1 - (1 /min_dim_cardinality));
        }


        /**
         * Calc the label out of label_candidates that has the minimum cardinality.
         * @tparam T type of the values hold by processed Tensors (Tensor).
         * @param operands Operands for this Step.
         * @param step current step
         * @param label_candidates labels that are candidates
         * @return the label with minimum cardinality
         */
        template<typename T>
        label_t calcMinCardLabel(const vector<variant<HyperTrie<T> *, T>> &operands, PlanStep &step,
                                 unordered_set<label_t> &label_candidates) {
            if (label_candidates.size() == 0) {
                throw "Label candidates must never be empty.";
            } else if (label_candidates.size() == 1) {
                return *label_candidates.begin();
            } else {
                label_t best_label{};
                float best_cardinality = INFINITY;

                // find the label with the lowest cardinality
                for (const label_t &label  : label_candidates) {
                    float cardinality = calcCard<T>(operands, step, label);
                    if (cardinality < best_cardinality) {
                        best_cardinality = cardinality;
                        best_label = label;
                    }
                    // nothing can be better than zero
                    if (cardinality == 0) {
                        return label;
                    }
                }
                return best_label;
            }
        }


    };

    PlanStep::PlanStep(unordered_set<label_t> label_candidates, unordered_set<label_t> processed_labels,
                       map<op_pos_t, vector<label_t>> operands_labels, const EvalPlan *plan)
            : label_candidates(label_candidates),
              processed_labels(processed_labels),
              operands_with_label(plan->subscript.getOperandsWithLabel()),
              label_pos_in_result(plan->subscript.getLabelPosInResult()),
              label_poss_in_operand(Subscript::calcLabelPossInOperand(operands_labels)),
              plan(plan),
              all_done(label_candidates.empty()) {}

    PlanStep::PlanStep(const EvalPlan *plan)
            : label_candidates(plan->subscript.getAllLabels()),
              processed_labels({}),
              operands_with_label(plan->subscript.getOperandsWithLabel()),
              label_pos_in_result(plan->subscript.getLabelPosInResult()),
              label_poss_in_operand(plan->subscript.getLabelPossInOperand()),
              plan(plan),
              all_done(plan->subscript.getAllLabels().empty()) {}

    map<op_pos_t, vector<label_t>> PlanStep::calcNextOperandsLabels(const unordered_set<label_t> &processed_labels) {
        map<op_pos_t, vector<label_t>> operands_labels{};
        for (const auto &
            [op_id, labels] : this->plan->subscript.getOperandsLabels()) {
            vector<label_t> reduced_labels{};

            for (const label_t &label : labels)
                if (not processed_labels.count(label))
                    reduced_labels.push_back(label);

            operands_labels.insert({op_id, reduced_labels});
        }
        return operands_labels;
    }

    PlanStep PlanStep::getNextStep(label_t label) {
        unordered_set<label_t> processed_labels{this->processed_labels};
        processed_labels.insert(label);

        unordered_set<label_t> label_candidates{this->label_candidates};
        label_candidates.erase(label);
        unordered_set<label_t> neighbors = plan->label_dependency_graph.neighbors(label);
        for (auto &&processed_label : processed_labels) {
            neighbors.erase(processed_label);
        }
        label_candidates.insert(neighbors.cbegin(), neighbors.cend());

        map<op_pos_t, vector<label_t>> operands_labels = calcNextOperandsLabels(processed_labels);

        return PlanStep{label_candidates, processed_labels, operands_labels, plan};
    }

}
#endif //SPARSETENSOR_EINSUM_EVALPLAN_HPP
