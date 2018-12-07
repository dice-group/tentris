#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include <algorithm>
#include <numeric>
#include <vector>
#include <memory>

#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/tensor/hypertrie/Join.hpp"
#include "tnt/tensor/einsum/operator/Slice.hpp"
#include "tnt/util/All.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/tensor/einsum/operator/OperatorNode.hpp"

namespace {
    using namespace tnt::tensor::hypertrie;
    using namespace tnt::util::types;
    using Join = tnt::tensor::hypertrie::Join;
}

namespace tnt::tensor::einsum::operators {
    /**
     * This is an basic Einstein-Summation Operator that can perform any Einstein Summation Convenction Operation. In most cases this
     * operator should only be used as sub operator of a CrossProduct as it is not very effective if an cross product is involved.
     * @see CrossProduct
     */
    template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
    class Einsum : public OperatorNode<RESULT_TYPE> {
    protected:
        /**
         * The evaluation plan for this->subscript.
         */
        mutable EinsumPlan _plan;
        mutable std::vector<Slice<RESULT_TYPE>> _predecessors{};
        mutable Operands _operands{};
        mutable bool _operands_generated = false;
        mutable bool _may_have_results = false;

    public:

        /**
         * Constructor.
         * @param subscript its subscript.
         * @param slice_keys the key to its operators
         * @param trie the tries that shall be sliced.
         */
        Einsum(const std::shared_ptr<const Subscript> subscript, const std::vector<SliceKey_t> &slice_keys,
               const std::vector<BoolHyperTrie *> &tries)
                : OperatorNode<RESULT_TYPE>{}, _plan{subscript} {
            this->type = OperatorType::EINSUM;
            for (const auto &[slice_key, trie] : zip(slice_keys, tries)) {
                _predecessors.push_back({slice_key, trie});
            }
        }

        Einsum(Einsum &&) = default;

        /**
         * Prepares the arguments for the recursive calculation of the results.
         * @param operands vector of tensors
         */
        static void calcEinsum(yield_push<RESULT_TYPE> &yield, const Operands &operands, const EinsumPlan &plan) {
            // unpacks HyperTrieTensors to HyperTries or value types T
            // initialize emtpy result key
            // plan first step
            EinsumPlan::Step step = plan.getInitialStep(operands);

            Key_t result_key = Key_t(step.getResultSize(), KEY_PART_MAX);
            // start recursion
            rekEinsum(yield, operands, result_key, step);
        }

        /**
         * Acutal recursion to calculate the result
         * @param operands vector of current operands
         * @param result_key current result key. Is filled step by step. When all positions are filled this key represents
         * the key of an result value.
         * @param step current step. This holds also data to plan the next step.
         * @param label the current label that is to be processed in this recursive step
         */
        static void
        rekEinsum(yield_push<RESULT_TYPE> &yield, const Operands &operands, const Key_t &result_key,
                  const EinsumPlan::Step &step);

        static typename RESULT_TYPE::count_t contract(const Operands &operands, const EinsumPlan::Step &step);

    private:
        /**
         * Extracts the operands needed for the given Einsum Operator.
         * @param operands all operands that are input to this CrossProduct
         * @param einsum the Einsum Operator that the operands shall be extracted
         * @return the operands relevant for the given Einsum Operator
         */
        Operands extractRelevantOperands(const Operands &all_operands) const {
            Operands operands{};
            for (const op_pos_t &op_pos : _plan.getSubscript()->getOriginalOpPoss()) {
                operands.push_back(all_operands.at(op_pos));
            }
            return operands;
        }

    public:
        yield_pull<RESULT_TYPE> get() const override{
            return yield_pull<RESULT_TYPE>(boost::bind(&Einsum<RESULT_TYPE>::get, this, _1));
        }

    private:
        void get(yield_push<RESULT_TYPE> &yield) const {
            if (not _operands_generated) {
                _operands_generated = true;

                for (Slice<RESULT_TYPE> &slice : _predecessors) {
                    switch (slice.slice_type) {
                        case Slice<RESULT_TYPE>::SCALAR: {
                            if (not slice.getScalar())
                                return;
                            break;
                        }

                        case Slice<RESULT_TYPE>::HYPERTRIE: {
                            BoolHyperTrie *trie = slice.getHyperTrie();
                            if (trie == nullptr)
                                return;
                            _operands.push_back(trie);
                            break;
                        }
                    }
                }
                _may_have_results = true;
            }

            if (_may_have_results) {
                calcEinsum(yield, _operands, _plan);
            }
        }

    public:
        class iterator {
            bool _ended;
            const std::unique_ptr<yield_pull<RESULT_TYPE>> _results;
        public:
            explicit iterator(const Einsum &einsum, bool ended = false) :
                    _ended{ended},
                    _results{(not ended) ? std::unique_ptr<yield_pull<RESULT_TYPE>>{
                            new yield_pull<RESULT_TYPE>(boost::bind(&get, &einsum))}
                                         : std::unique_ptr<yield_pull<RESULT_TYPE>>{}} {
                if (not _ended)
                    _ended = not(*_results);
            }

            const RESULT_TYPE &operator*() {
                return _results.get();
            }

            iterator &operator++() {
                _results();
                _ended = not(*_results);
                return *this;
            }

            bool operator==(const iterator &rhs) const {
                return _ended == rhs._ended;
            }

            bool operator!=(const iterator &rhs) const {
                return !(rhs == *this);
            }

        };

        iterator begin() {
            return iterator{*this};
        };

        iterator end() {
            return iterator{*this, true};
        };
    };

    template<>
    typename counted_binding::count_t Einsum<counted_binding>::contract(const Operands &operands, const EinsumPlan::Step &step) {
        const std::vector<std::vector<label_pos_t>> &unique_contractions = step.getUniqueNonResultContractions();
        std::vector<size_t> results(operands.size());
        for (const auto &[op_pos, op_and_contr] : enumerate(zip(operands, unique_contractions))) {
            const auto &[op, unique_contraction] = op_and_contr;

            if (not unique_contraction.empty() and op->depth() == 3)
                for (const BoolHyperTrie *hyperTrie : BoolHyperTrie::DiagonalView{op, unique_contraction})
                    results[op_pos] += hyperTrie->size();
            else
                results[op_pos] += op->size();
        }
        return std::accumulate(results.begin(), results.end(), size_t(1), std::multiplies<size_t>());
    }

    template<>
    void Einsum<counted_binding>::rekEinsum(
            yield_push<counted_binding> &yield,
            const Operands &operands,
            const Key_t &result_key,
            const EinsumPlan::Step &step) {
        // there are steps left
        if (not step.all_done) {
            // calculate next operands and result_key from current operands, step, label and resultKey
            Join join{result_key, operands, step};

            for (const auto&[next_operands, next_result_key] : join) {
                const EinsumPlan::Step &next_step = step.nextStep(next_operands);
                // start next recursive step.
                rekEinsum(yield, next_operands, next_result_key, next_step);
            }
        } else { // there are no steps left
            if (not operands.empty()) { // there are lonely and/or unique labels left.
                const size_t i = contract(operands, step);
                if (i > 0)
                    yield({result_key, i});
            } else { // no labels left
                yield({result_key, 1});
            }
        }
    };

    template<>
    bool Einsum<distinct_binding >::contract(const Operands &operands, const EinsumPlan::Step &step) {
        const std::vector<std::vector<label_pos_t>> &unique_contractions = step.getUniqueNonResultContractions();
        std::vector<bool> results(operands.size(), false);
        for (const auto &[op_pos, op_and_contr] : enumerate(zip(operands, unique_contractions))) {
            const auto &[op, unique_contraction] = op_and_contr;

            if (not unique_contraction.empty() and op->depth() == 3)
                for ([[maybe_unused]]const BoolHyperTrie *hyperTrie :
                        BoolHyperTrie::DiagonalView{op, unique_contraction}) {
                    results[op_pos] = true;
                    break;
                }
            else
                results[op_pos] = true;
        }
        return std::accumulate(results.begin(), results.end(), true, std::logical_and<bool>());
    }

    void rekEinsumBoolNonResult(
            yield_push<distinct_binding> &yield,
            const Operands &operands,
            const Key_t &result_key,
            const EinsumPlan::Step &step) {
        // there are steps left
        if (not step.all_done) {
            // calculate next operands and result_key from current operands, step, label and resultKey
            Join join{result_key, operands, step};
            for (const auto&[next_operands, next_result_key] : join) {
                const EinsumPlan::Step &next_step = step.nextStep(next_operands);
                // start next recursive step.
                rekEinsumBoolNonResult(yield, next_operands, next_result_key, next_step);
                break;
            }
        } else { // there are no steps left
            if (not operands.empty()) {
                if (Einsum<distinct_binding>::contract(operands, step)) {
                    // there are lonely and/or unique labels left.
                    yield(result_key);
                }
            } else { // no labels left
                yield(result_key);
            }
        }
    };

    template<>
    void Einsum<distinct_binding>::rekEinsum(
            yield_push<distinct_binding> &yield,
            const Operands &operands,
            const Key_t &result_key,
            const EinsumPlan::Step &step) {
        // there are steps left
        if (step.result_labels_done) {
            rekEinsumBoolNonResult(yield, operands, result_key, step);
        } else if (not step.all_done) {
            // calculate next operands and result_key from current operands, step, label and resultKey
            Join join{result_key, operands, step};
            for (const auto&[next_operands, next_result_key] : join) {
                const EinsumPlan::Step &next_step = step.nextStep(next_operands);
                // start next recursive step.
                rekEinsum(yield, next_operands, next_result_key, next_step);
            }
        } else { // there are no steps left
            if (not operands.empty()) {
                if (contract(operands, step)) { // there are lonely and/or unique labels left.
                    yield(result_key);
                }
            } else { // no labels left
                yield(result_key);
            }
        }
    };
}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
