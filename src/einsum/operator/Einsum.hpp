#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include <tensor/MapTensor.hpp>
#include <einsum/EvalPlan.hpp>
#include <hypertrie/HyperTrieJoinIterator.hpp>
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
    void rekEinsum(vector<Tensor<T>> &operands);

    MapTensor<T> *getResult(vector<Tensor<T>> tensors) {
        rekEinsum(tensors);
        return nullptr;
    }

    void rekEinsum(const vector<variant<Tensor<T> *, T>> &operands, const vector<uint64_t> &result_key,
                   PlanStep &last_step, label_t &last_label);

};

template<typename T>
void Einsum<T>::rekEinsum(vector<Tensor<T>> &operands) {
    vector<uint64_t> result_key = vector<uint64_t>(this->result->ndim);

}

template<typename T>
void Einsum<T>::rekEinsum(const vector<variant<Tensor<T> *, T>> &operands, const vector<uint64_t> &result_key,
                          PlanStep &last_step, label_t &last_label) {
    PlanStep step;
    label_t label;
    const auto &[step, label] = plan.nextStep(operands, last_step, last_label);

    for (const auto &[next_operands, next_result_key] : HyperTrieJoin{operands, step, label, result_key}) {
        rekEinsum(next_operands, next_result_key, step, label);
    }

}

#endif //LIBSPARSETENSOR_EINSUM_HPP
