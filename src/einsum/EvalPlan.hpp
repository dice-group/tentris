#ifndef TEST_EVALPLAN_HPP
#define TEST_EVALPLAN_HPP


#include <hypertrie/PosCalc.hpp>
#include "Subscript.hpp"
#include "tensor/Tensor.hpp"
#include <optional>

using std::optional;

using Subscript::label_t;
using Subscript::label_pos_t;
using Subscript::op_pos_t;

class PlanStep;


class EvalPlan {
public:
    EvalPlan(const Subscript &subscript) : subscript(subscript) {}

private:

    Subscript subscript;

public:
    template<typename T>
    tuple<PlanStep, label_t> nextStep(vector<Tensor<T>> &operands, PlanStep &last_step, label_t &last_label) {
        // TODO: implement
        return nullptr;
    }


};

class PlanStep {

    unordered_map<label_t, set<op_pos_t>> *operands_with_label;
    unordered_map<label_t, label_pos_t> *label_pos_in_result;
    unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> *label_poss_in_operand;
public:
    /**
     * Gives the positions of the operands that have the given label among their subscript labels.
     * @param label label to find the operands for
     * @return operands using the label
     */
    const set<op_pos_t> &operandsWithLabel(const label_t &label) const {
        return operands_with_label->at(label);
    }

    /**
     * Position of a label in the result.
     * @return A map from a label to a label positions
     */
    const std::optional<label_pos_t> labelPosInResult(const label_t &label) const noexcept {
        auto res_pos_ = label_pos_in_result->find(label);
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
        return label_poss_in_operand->at({op_pos, label});
    }
};


#endif //TEST_EVALPLAN_HPP
