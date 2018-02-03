#ifndef LIBSPARSETENSOR_EINSUM_HPP
#define LIBSPARSETENSOR_EINSUM_HPP


#include "../../tensor/MapTensor.hpp"
#include "../../einsum/EvalPlan.hpp"
#include "../../hypertrie/Join.hpp"
#include "Operator.hpp"
#include "CrossProduct.hpp"

#include <numeric>


using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::MapTensor;
using sparsetensor::hypertrie::Join;


namespace sparsetensor::einsum::Operator {

    template<typename T>
    class Einsum : public Operator::Operator<T> {
        friend class CrossProduct;

    public:
        Einsum(Subscript &subscript) : Operator::Operator<T>{subscript}, plan(EvalPlan{subscript}),
                                       result(new MapTensor<T>(vector<uint64_t>{})) {}

    private:
        EvalPlan plan{subscript};

        MapTensor<T> *result = new MapTensor<T>(vector<uint64_t>{}); // TODO: use correct shape


    public:
        void rekEinsum(vector<Tensor<T>> &operands);

        MapTensor<T> *getResult(vector<Tensor<T>>

                                tensors) {
            rekEinsum(tensors);
            return nullptr;
        }

        void rekEinsum(const vector<variant<Tensor<T> *, T>> &operands,
                       const vector<uint64_t> &result_key,
                       PlanStep
                       &last_step,
                       label_t &last_label
        );

    };

    template<typename T>
    void Einsum<T>::rekEinsum(vector<Tensor<T>> &operands) {
        vector<uint64_t> result_key = vector<uint64_t>(this->result->ndim);

    }

    template<typename T>
    void Einsum<T>::rekEinsum(const vector<variant<Tensor<T> *, T>> &operands,
                              const vector<uint64_t> &result_key,
                              PlanStep
                              &last_step,
                              label_t &last_label) {
        const auto &
        [step, label] = plan.nextStep(operands, last_step, last_label);

        if (not step.all_done) {
            Join<T> join{operands, step, label, result_key};
            for (const auto &
                [next_operands, next_result_key] : join) {
                rekEinsum(next_operands, next_result_key, step, label);
            }
        } else {
            result[result_key] = std::accumulate(operands.begin(), operands.end());
        }
    }
}
#endif //LIBSPARSETENSOR_EINSUM_HPP
