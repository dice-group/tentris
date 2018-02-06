#ifndef SPARSETENSOR_EINSUM_EVALPLAN_HPP
#define SPARSETENSOR_EINSUM_EVALPLAN_HPP


#include "../hypertrie/PosCalc.hpp"
#include "Subscript.hpp"
#include <optional>
#include <variant>
#include "Types.hpp"
#include "../tensor/Tensor.hpp"

using std::optional;
using sparsetensor::tensor::Tensor;
using std::variant;

namespace sparsetensor::einsum {

    class PlanStep;

    class EvalPlan {
        friend class PlanStep;

        const Subscript &subscript;
        const UndirectedGraph label_dependency_graph;
    public:
        EvalPlan(const Subscript &subscript) : subscript(subscript) {}

        template<typename T>
        tuple<PlanStep, label_t> nextStep(const vector<variant<HyperTrie<T> *, T>> &operands, const PlanStep &last_step,
                                          const label_t &last_label) {
            // TODO: implement
            return {};
        }

        template<typename T>
        tuple<PlanStep, label_t> firstStep(const vector<variant<HyperTrie<T> *, T>> &operands) {
            // TODO: implement
            return {};
        }


    };

    class PlanStep {
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
            for (const auto &[op_id, labels] : this->plan.subscript.getOperandsLabels()) {
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
