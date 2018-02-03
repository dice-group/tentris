#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include "../../tensor/MapTensor.hpp"
#include "../../einsum/EvalPlan.hpp"
#include "../../hypertrie/Join.hpp"
#include "Operator.hpp"

#include <numeric>


using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::MapTensor;
using sparsetensor::hypertrie::Join;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class Einsum : public Operator<T> {

    public:
        Einsum(Subscript &subscript) : Operator<T>{subscript},
                                       plan(EvalPlan{subscript}),
                                       result(new MapTensor<T>(vector<uint64_t>{})) {}

    private:
        EvalPlan plan;

        MapTensor<T> *result; // TODO: use correct shape


    public:
        void rekEinsum(const vector<variant<Tensor<T> *, T>> &operands) {
            vector<uint64_t> result_key = vector<uint64_t>(this->result->ndim);
            const auto &[step, label] = plan.firstStep(operands);
            rekEinsum(operands, result_key, step, label);
        }


        void rekEinsum(const vector<variant<Tensor<T> *, T>> &operands, const vector<uint64_t> &result_key,
                       PlanStep &step, label_t &label) {
            if (not step.all_done) {
                Join<T> join{static_cast<const vector<variant<HyperTrie<T> *, T>> &>(operands), step, label, result_key};
                for (const auto &[next_operands, next_result_key] : join) {
                    const auto &[next_step, next_label] = plan.nextStep(operands, step, label);
                    rekEinsum(next_operands, next_result_key, next_step, next_label);
                }
            } else {
                result[result_key] += std::accumulate(operands.begin(), operands.end());
            }
        }


        Tensor<T> *getResult(const vector<variant<Tensor<T> *, T>> &tensors) override {
            rekEinsum(tensors);
            return result;
        }
    };


}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
