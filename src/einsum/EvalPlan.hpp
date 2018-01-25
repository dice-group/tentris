#ifndef TEST_EVALPLAN_HPP
#define TEST_EVALPLAN_HPP


#include "Subscript.hpp"
#include "tensor/Tensor.hpp"

using Subscript::label_t;

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

};


#endif //TEST_EVALPLAN_HPP
