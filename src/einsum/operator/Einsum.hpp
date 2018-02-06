#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include "../../tensor/MapTensor.hpp"
#include "../../einsum/EvalPlan.hpp"
#include "../../hypertrie/Join.hpp"

#include <numeric>
#include "../ShapeCalc.hpp"


using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::MapTensor;
using sparsetensor::hypertrie::Join;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class Einsum {
        EvalPlan plan;

        Subscript subscript;

        MapTensor<T> *result;
    public:
        Einsum(const Subscript &subscript) : plan(EvalPlan{subscript}), subscript(subscript) {

        }

        void rekEinsum(const vector<HyperTrieTensor<T> *> &operands) {
            vector<variant<HyperTrieTensor<T> *, T>> variant_operands{};
            for (const HyperTrieTensor<T> *&operand : operands) {
                if (operand->ndim == 0) {
                    variant_operands.push_back(variant<HyperTrieTensor<T> *, T>{operand->get({})});
                } else {
                    variant_operands.push_back(variant<HyperTrieTensor<T> *, T>{operand});
                }
            }

            Key_t result_key = Key_t(this->result->ndim);
            const auto &[step, label] = plan.firstStep(operands);

            rekEinsum(operands, result_key, step, label);
        }


        void rekEinsum(const vector<variant<HyperTrieTensor<T> *, T>> &operands, const Key_t &result_key,
                       PlanStep &step, label_t &label) {
            if (not step.all_done) {
                Join<T> join{static_cast<const vector<variant<HyperTrie<T> *, T>> &>(operands), step, label, result_key};
                // TODO: parallelize
                for (const auto &[next_operands, next_result_key] : join) {
                    const auto &[next_step, next_label] = plan.nextStep(operands, step, label);
                    rekEinsum(next_operands, next_result_key, next_step, next_label);
                }
            } else {
                result[result_key] += std::accumulate(operands.begin(), operands.end());
            }
        }


        MapTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) {
            const shape_t &result_shape = calcShape<T, HyperTrieTensor>(operands, subscript);
            result = new MapTensor<T>(result_shape);
            rekEinsum(operands);
            return result;
        }
    };


}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
