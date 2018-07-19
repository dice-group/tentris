#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include <algorithm>
#include <numeric>
#include <vector>

#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/tensor/hypertrie/Join.hpp"
#include "tnt/util/container/NDMap.hpp"
#include "tnt/util/All.hpp"


namespace tnt::tensor::einsum::operators {


    /**
     * This is an basic Einstein-Summation Operator that can perform any Einstein Summation Convenction Operation. In most cases this
     * operator should only be used as sub operator of a CrossProduct as it is not very effective if an cross product is involved.
     * @see CrossProduct
     */
    template<typename OUT_COUNT_T>
    class Einsum {

        using BoolHyperTrie = tnt::tensor::hypertrie::BoolHyperTrie;

        template<typename T>
        using NDMap = tnt::util::container::NDMap<T>;
        using Join = tnt::tensor::hypertrie::Join;
        using Operands = tnt::tensor::hypertrie::Operands;
        /**
         * The evaluation plan for this->subscript.
         */
        EinsumPlan _plan;

    public:
        /**
         * Basic Constructor.
         * @param subscript Subscript that defines what the operator does.
         */
        explicit Einsum(const Subscript &subscript) : _plan{subscript} {}


        NDMap<OUT_COUNT_T> getResult(const Operands &operands) {
            return calcEinsum(extractRelevantOperands(operands));
        }

    private:
        /**
         * Prepares the arguments for the recursive calculation of the results.
         * @param operands vector of tensors
         */
        NDMap<OUT_COUNT_T> calcEinsum(const Operands &operands) {
            // unpacks HyperTrieTensors to HyperTries or value types T
            // initialize emtpy result key
            // plan first step
            EinsumPlan::Step step = _plan.getInitialStep(operands);
            NDMap<OUT_COUNT_T> result{};
            Key_t result_key = Key_t(step.getResultSize(), KEY_PART_MAX);
            // start recursion
            rekEinsum(operands, result_key, step, result);
            return result;
        }

        /**
         * Acutal recursion to calculate the result
         * @param operands vector of current operands
         * @param result_key current result key. Is filled step by step. When all positions are filled this key represents
         * the key of an result value.
         * @param step current step. This holds also data to plan the next step.
         * @param label the current label that is to be processed in this recursive step
         */
        void
        rekEinsum(const Operands &operands, const Key_t &result_key, const EinsumPlan::Step &step,
                  NDMap<OUT_COUNT_T> &result) {
            std::cout << step << std::endl;
            // there are steps left
            if (not step.all_done) {
                // calculate next operands and result_key from current operands, step, label and resultKey
                Join join{result_key, operands, step};

                for (auto[next_operands, next_result_key] : join) {
                    EinsumPlan::Step next_step = step.nextStep(operands);
                    // start next recursive step.
                    rekEinsum(next_operands, next_result_key, next_step, result);
                }
            } else { // there are no steps left
                result[result_key] += OUT_COUNT_T(1);
            }
        }

        /**
             * Extracts the operands needed for the given Einsum Operator.
             * @param operands all operands that are input to this CrossProduct
             * @param einsum the Einsum Operator that the operands shall be extracted
             * @return the operands relevant for the given Einsum Operator
             */
        Operands extractRelevantOperands(const Operands &all_operands) const {
            Operands operands{};
            for (const op_pos_t &op_pos : _plan.getSubscript().getOriginalOpPoss()) {
                operands.push_back(all_operands.at(op_pos));
            }
            return operands;
        }
    };


}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
