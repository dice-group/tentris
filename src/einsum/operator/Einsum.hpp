#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include <algorithm>
#include <numeric>
#include <vector>
#include <einsum/Einsum.hpp>

#include "../../hypertrie/BoolHyperTrie.hpp"
#include "../../einsum/EinsumPlan.hpp"
#include "../../hypertrie/NewJoin.hpp"
#include "../../container/NDMap.hpp"

namespace sparsetensor::operations::operators {
    using BoolHypertrie = sparsetensor::hypertrie::BoolHyperTrie;
    using Join = sparsetensor::hypertrie::NewJoin;
    using Key_t = sparsetensor::hypertrie::Key_t;
    template<typename T>
    using NDMap = sparsetensor::container::NDMap<T> ;

    /**
     * This is an basic Einstein-Summation Operator that can perform any Einstein Summation Convenction Operation. In most cases this
     * operator should only be used as sub operator of a CrossProduct as it is not very effective if an cross product is involved.
     * @see CrossProduct
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename OUT_COUNT_T>
    class Einsum : public Operator<BoolHyperTrie, NDMap<OUT_COUNT_T>> {
        /**
         * The evaluation plan for this->subscript.
         */
        EinsumPlan plan;
        /**
         * This is where the result is written to.
         */
        NDMap<OUT_COUNT_T> *result;

    public:
        /**
         * Basic Constructor.
         * @param subscript Subscript that defines what the operator does.
         */
        Einsum(const Subscript &subscript) : Operator<BoolHyertrieTensor, MapTensor<OUT_COUNT_T>>{subscript},
                                             plan{subscript} {}

        /**
         * Prepares the arguments for the recursive calculation of the results.
         * @param operands vector of tensors
         */
        void rekEinsum(const vector<BoolHyertrieTensor *> &operands) {
            // unpacks HyperTrieTensors to HyperTries or value types T
            // initialize emtpy result key
            // plan first step
            const EinsumPlan::EinsumStep &step = plan.getInitialStep();
            const EinsumPlan::EinsumStep::Action &action = step.getAction(operands);
            Key_t result_key = Key_t(step.getResultSize());
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

        MapTensor<T> *getResult(const vector<HyperTrieTensor < T> *

        > &operands) {
            const shape_t &result_shape = calcResultShape<T, HyperTrieTensor>(operands, this->subscript);
            result = new MapTensor<T>(result_shape);
            rekEinsum(operands);
            return result;
        }
    };


}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
