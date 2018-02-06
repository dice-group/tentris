#ifndef SPARSETENSOR_EINSUM_EVALPLAN_HPP
#define SPARSETENSOR_EINSUM_EVALPLAN_HPP


#include "../hypertrie/PosCalc.hpp"
#include "Subscript.hpp"
#include <optional>
#include <variant>
#include <vector>
#include "../tensor/Tensor.hpp"
#include "../hypertrie/HyperTrie.hpp"
#include <algorithm>

using std::optional;
using sparsetensor::tensor::Tensor;
using std::variant;
using std::vector;

namespace sparsetensor::einsum {

    class PlanStep;

    class EvalPlan {
        friend class PlanStep;

        const Subscript &subscript;
        const UndirectedGraph label_dependency_graph;
        PlanStep initial_step;
    public:
        EvalPlan(const Subscript &subscript) :
                subscript(subscript),
                label_dependency_graph(subscript.getLabelDependencyGraph()),
                initial_step({}, {}, subscript.getOperandsLabels(), *this) {}

        template<typename T>
        tuple<PlanStep, label_t> nextStep(const vector<variant<HyperTrie<T> *, T>> &operands, const PlanStep &last_step,
                                          const label_t &last_label) {
            // TODO: implement
            return {};
        }

        template<typename T>
        tuple<PlanStep &, label_t> firstStep(const vector<variant<HyperTrie<T> *, T>> &operands) {

            return {};
        }

        template<typename T>
        uint64_t calcCard(const vector<variant<HyperTrie<T> *, T>> &operands, PlanStep step, const label_t &label) {
            const std::vector<op_pos_t> &op_ids = step.operandsWithLabel(label);
            vector<size_t> dim_cardinalities(op_ids.size(), SIZE_MAX);
            vector<size_t> operand_cardinalities(op_ids.size(), SIZE_MAX);

            size_t min_dim_cardinality = SIZE_MAX;
            for (int i = 0; i < op_ids.size(); ++i) {
                const op_pos_t &op_id = op_ids.at(i);
                const HyperTrie<T> *&operand = std::get<HyperTrie<T> *>(operands.at(i));
                const vector<size_t> cards = operand->getCards(step.labelPossInOperand(op_id, label));

                const size_t &dim_cardinality = *std::min_element(cards.cbegin(), cards.cend());
                if (dim_cardinality == 0)
                    return 0;
                if (dim_cardinality < min_dim_cardinality)
                    min_dim_cardinality = dim_cardinality;
                dim_cardinalities[i] = dim_cardinality;
                operand_cardinalities[i] = operand->leafcount;
            }

            return min_dim_cardinality
                   * std::accumulate(operand_cardinalities.cbegin(), operand_cardinalities.cend(), 1,
                                     std::multiplies<size_t>())
                   / std::accumulate(dim_cardinalities.cbegin(), dim_cardinalities.cend(), 1,
                                     std::multiplies<size_t>());
        }


        template<typename T>
        label_t calcBestLabel(const vector<variant<HyperTrie<T> *, T>> &operands, PlanStep step,
                              unordered_set<label_t> label_candidates) {
            if (label_candidates.size() == 0) {
                throw "Label candidates must never be empty.";
            } else if (label_candidates.size() == 1) {
                return *label_candidates.begin();
            } else {
                label_t best_label{};
                uint64_t best_cardinality = UINT64_MAX;

                for (const label_t &label  : label_candidates) {
                    uint64_t cardinality = calcCard(operands, step, label);
                    if (cardinality < best_cardinality) {
                        best_cardinality = cardinality;
                        best_label = label;
                    }
                    if (cardinality == 0) {
                        return label;
                    }
                }
                return best_label;
            }
        }


    };

    class PlanStep {
        friend class EvalPlan;

        unordered_set<label_t> label_candidates;
        unordered_set<label_t> processed_labels;
        unordered_map<label_t, vector<op_pos_t>> operands_with_label;
        const unordered_map<label_t, label_pos_t> &label_pos_in_result;
        unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operand;
        const EvalPlan &plan;
        bool all_done = true;

        PlanStep(unordered_set<label_t> label_candidates,
                 unordered_set<label_t> processed_labels,
                 map<op_pos_t, vector<label_t>> operands_labels,
                 const EvalPlan &plan)
                : label_candidates(label_candidates),
                  operands_with_label(Subscript::calcOperandsWithLabel(label_candidates, operands_labels)),
                  label_pos_in_result(plan.subscript.getLabelPosInResult()),
                  label_poss_in_operand(Subscript::calcLabelPossInOperand(operands_labels)),
                  plan(plan),
                  all_done(label_candidates.empty()) {}

    public:


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
        map<op_pos_t, vector<label_t>> calcNextOperandsLabels(const unordered_set<label_t> &processed_labels) {
            map<op_pos_t, vector<label_t>> operands_labels{};
            for (const auto &
                [op_id, labels] : this->plan.subscript.getOperandsLabels()) {
                vector<label_t> reduced_labels{};

                for (const label_t &label : labels)
                    if (not processed_labels.count(label))
                        reduced_labels.push_back(label);

                operands_labels.insert(op_id, reduced_labels);
            }
            return operands_labels;
        }

    public:
        PlanStep getNextStep(label_t label) {
            unordered_set<label_t> processed_labels{this->processed_labels};
            processed_labels.insert(label);

            unordered_set<label_t> label_candidates{this->label_candidates};
            label_candidates.erase(label);
            unordered_set<label_t> neighbors = plan.label_dependency_graph.neighbors(label);
            neighbors.erase(processed_labels.cbegin(), processed_labels.cend());
            label_candidates.insert(neighbors.cbegin(), neighbors.cend());

            map<op_pos_t, vector<label_t>> operands_labels = calcNextOperandsLabels(processed_labels);

            return PlanStep(label_candidates, processed_labels, operands_labels, plan);
        }
    };

}
#endif //SPARSETENSOR_EINSUM_EVALPLAN_HPP
