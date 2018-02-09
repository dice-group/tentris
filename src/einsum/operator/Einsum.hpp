#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include "../../tensor/MapTensor.hpp"
#include "../../einsum/EvalPlan.hpp"
#include "../../hypertrie/Join.hpp"

#include <numeric>
#include "../ShapeCalc.hpp"
#include <algorithm>


using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::MapTensor;
using sparsetensor::hypertrie::Join;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class Einsum {
        EvalPlan plan;

        MapTensor<T> *result;
    public:
        Subscript subscript;

        Einsum() : plan({}),
                   subscript({}),
                   result(nullptr) {}

        Einsum(const Subscript &subscript) : plan(EvalPlan{subscript}), subscript(subscript) {

        }

        void rekEinsum(const vector<HyperTrieTensor<T> *> &operands) {
            vector<variant<HyperTrie<T> *, T>> hypertrie_operands{};
            for (HyperTrieTensor<T> *operand : operands) {
                if (operand->ndim == 0) {
                    hypertrie_operands.push_back({operand->trie->leafsum});
                } else {
                    hypertrie_operands.push_back({operand->trie});
                }
            }

            Key_t result_key = Key_t(this->result->ndim);
            auto [step, label] = plan.firstStep(hypertrie_operands);

            rekEinsum(hypertrie_operands, result_key, step, label);
        }


        void rekEinsum(const vector<variant<HyperTrie<T> *, T>> &operands, const Key_t &result_key,
                       PlanStep &step, const label_t &label) {
            if (not step.all_done) {
                Join<T> join{operands, step, label, result_key};
                // TODO: parallelize
                for (const auto &[next_operands, next_result_key] : join) {
                    auto [next_step, next_label] = plan.nextStep<T>(operands, step, label);
                    rekEinsum(next_operands, next_result_key, next_step, next_label);
                }
            } else {
                T result_value = 1;
                for (auto &&operand : operands) {
                    result_value *= std::get<T>(operand);
                }
                result->set(result_key, result->get(result_key) + result_value);
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
