#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include <parallel/algorithm>
#include <parallel/numeric>
#include <vector>

#include "../../tensor/MapTensor.hpp"
#include "../../einsum/EvalPlan.hpp"
#include "../../hypertrie/Join.hpp"
#include "../ShapeCalc.hpp"


using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::MapTensor;
using sparsetensor::hypertrie::Join;


namespace sparsetensor::operations::operators {

    /**
     * This is an basic Einstein-Summation Operator that can perform any Einstein Summation Convenction Operation. In most cases this
     * operator should only be used as sub operator of a CrossProduct as it is not very effective if an cross product is involved.
     * @see CrossProduct
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename T>
    class Einsum : public Operator<T, HyperTrieTensor, MapTensor> {
        /**
         * The evaluation plan for this->subscript.
         */
        EvalPlan plan;
        /**
         * This is where the result is written to.
         */
        MapTensor<T> *result;

    public:
        /**
         * Basic Constructor.
         * @param subscript Subscript that defines what the operator does.
         */
        Einsum(const Subscript &subscript) : Operator<T, HyperTrieTensor, MapTensor>{subscript},
                                             plan(EvalPlan{subscript}) {}

        /**
         * Prepares the arguments for the recursive calculation of the results.
         * @param operands vector of tensors
         */
        void rekEinsum(const vector<HyperTrieTensor<T> *> &operands) {
            // unpacks HyperTrieTensors to HyperTries or value types T
            vector<variant<HyperTrie<T> *, T>> hypertrie_operands{};
            for (HyperTrieTensor<T> *operand : operands) {
                if (operand->ndim == 0) {
                    hypertrie_operands.push_back({operand->trie->leafsum});
                } else {
                    hypertrie_operands.push_back({operand->trie});
                }
            }
            // initialize emtpy result key
            Key_t result_key = Key_t(this->result->ndim, 50);
            // plan first step
            auto[step, label] = plan.firstStep(hypertrie_operands);
            // start recursion
            rekEinsum(hypertrie_operands, result_key, step, label);
        }

        /**
         * Acutal recursion to calculate the result
         * @param operands vector of current operands
         * @param result_key current result key. Is filled step by step. When all positions are filled this key represents
         * the key of an result value.
         * @param step current step. This holds also data to plan the next step.
         * @param label the current label that is to be processed in this recursive step
         */
        void rekEinsum(const vector<variant<HyperTrie<T> *, T>> &operands, const Key_t &result_key,
                       PlanStep &step, const label_t &label) {
            // there are steps left
            if (not step.all_done) {
                // calculate next operands and result_key from current operands, step, label and resultKey
                Join<T> join{operands, step, label, result_key};

                for (auto[next_operands, next_result_key] : join) {
                    auto[next_step, next_label] = plan.nextStep<T>(operands, step, label);
                    // start next recursive step.
                    rekEinsum(next_operands, next_result_key, next_step, next_label);
                }
            } else { // there are no steps left
                T result_value = 1;
                for (const auto &operand : operands) {
                    result_value *= std::get<T>(operand);
                }
                result->set(result_key, result->get(result_key) + result_value);
            }
        }

        MapTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) {
            const shape_t &result_shape = calcResultShape<T, HyperTrieTensor>(operands, this->subscript);
            result = new MapTensor<T>(result_shape);
            rekEinsum(operands);
            return result;
        }
    };


}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
