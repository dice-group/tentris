#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include <tensor/MapTensor.hpp>
#include <einsum/EvalPlan.hpp>
#include "Operator.hpp"
#include "CrossProduct.hpp"

using Subscript::label_t;
using Subscript::label_pos_t;
using Subscript::op_pos_t;

template<typename T>
class Einsum : public Operator<T> {
    friend class CrossProduct;

public:
    Einsum(Subscript &subscript) : Operator(subscript), plan(EvalPlan{subscript}),
                                   result(new MapTensor<T>(vector<uint64_t>{})) {}

private:
    EvalPlan plan{subscript};

    MapTensor<T> *result = new MapTensor<T>(vector<uint64_t>{}); // TODO: use correct shape


public:
    void rekEinsum(vector<Tensor<T>> operands);

    MapTensor<T> *getResult(vector<Tensor<T>> tensors) {
        rekEinsum(tensors);
        return nullptr;
    }

    void rekEinsum(vector<Tensor<T>> &operands, vector<uint64_t> &result_key, PlanStep &last_step,
                   label_t &last_label);

    void matchKeys(vector<Tensor<T>> &operands, vector<uint64_t> &key, PlanStep &step, label_t &label);
};

template<typename T>
void Einsum<T>::rekEinsum(vector<Tensor<T>> &operands) {
    vector<uint64_t> result_key = vector<uint64_t>(this->result->ndim);

}

template<typename T>
void Einsum<T>::rekEinsum(vector<Tensor<T>> &operands, vector<uint64_t> &result_key, PlanStep &last_step,
                          label_t &last_label) {
    PlanStep step;
    label_t label;
    std::tie(step, label) = plan.nextStep(operands, last_step, last_label);

    for (tuple<vector<Tensor<T>>, vector<uint64_t >> operands_and_key : matchKeys(operands, result_key, step, label)) {
        rekEinsum(std::get<0>(operands_and_key), std::get<1>(operands_and_key), step, label);
    }

}

template<typename T>
void Einsum<T>::matchKeys(vector<Tensor<T>> &operands, vector<uint64_t> &key, PlanStep &step, label_t &label) {

}


#endif //LIBSPARSETENSOR_EINSUM_HPP
