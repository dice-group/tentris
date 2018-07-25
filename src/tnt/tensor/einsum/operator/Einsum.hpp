#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include <algorithm>
#include <numeric>
#include <vector>
#include <boost/coroutine2/all.hpp>
#include <boost/bind.hpp>

#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/tensor/hypertrie/Join.hpp"
#include "tnt/tensor/einsum/operator/Slice.hpp"
#include "tnt/util/container/NDMap.hpp"
#include "tnt/util/All.hpp"


namespace tnt::tensor::einsum::operators {
    namespace {
        template<typename RESULT_TYPE>
        using yield_push = typename boost::coroutines2::coroutine<RESULT_TYPE>::push_type;
        template<typename RESULT_TYPE>
        using yield_pull = typename boost::coroutines2::coroutine<RESULT_TYPE>::pull_type;
        using BoolHyperTrie = tnt::tensor::hypertrie::BoolHyperTrie;
        using SliceKey_t = tnt::util::types::SliceKey_t;
        template<typename T>
        using NDMap = tnt::util::container::NDMap<T>;
        using Join = tnt::tensor::hypertrie::Join;
        using Operands = tnt::tensor::hypertrie::Operands;
    }


    /**
     * This is an basic Einstein-Summation Operator that can perform any Einstein Summation Convenction Operation. In most cases this
     * operator should only be used as sub operator of a CrossProduct as it is not very effective if an cross product is involved.
     * @see CrossProduct
     */
    template<typename RESULT_TYPE>
    class BaseEinsum {
    protected:
        /**
         * The evaluation plan for this->subscript.
         */
        const EinsumPlan _plan;
        mutable std::vector<Slice> _predecessors;
        mutable Operands _operands;
        mutable bool operands_generated = false;
        mutable bool may_have_results = false;

    public:
        /**
         * Constructor.
         * @param subscript its subscript.
         * @param slice_keys the key to its operators
         * @param trie the tries that shall be sliced.
         */
        BaseEinsum(const Subscript &subscript, std::vector<SliceKey_t> slice_keys, std::vector<BoolHyperTrie *> tries)
                : _plan{subscript} {
            for (const auto &[slice_key, trie] : zip(slice_keys, tries)) {
                _predecessors.push_back({slice_key, trie});
            }
        }

    protected:
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
            yield_pull<RESULT_TYPE> results(&rekEinsum, operands, result_key, step);
            for (const RESULT_TYPE &result : results) {
                yield(result);
            }
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
        rekEinsum(yield_push<RESULT_TYPE> &yield, const Operands &operands, const Key_t &result_key, const EinsumPlan::Step &step);

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

    public:
        void get(yield_push<RESULT_TYPE> yield) const {
            if (not operands_generated) {
                operands_generated = true;

                for (Slice &slice : _predecessors) {
                    switch (slice.type) {
                        case Slice::SCALAR: {
                            if (not slice.getScalar())
                                return;
                            break;
                        }

                        case Slice::HYPERTRIE: {
                            BoolHyperTrie *trie = slice.getHyperTrie();
                            if (trie == nullptr)
                                return;
                            _operands.push_back(trie);
                            break;
                        }
                    }
                }
                may_have_results = true;
            }

            if (may_have_results) {
                yield_pull<RESULT_TYPE> results(&calcEinsum, _operands, _plan);
                for (const RESULT_TYPE &result : results) {
                    yield(result);
                }
            }
        }

        template<typename COUNT_TYPE>
        static COUNT_TYPE contract(const Operands &operands, const EinsumPlan::Step &step){}

        class iterator {
        public:
            const bool _ended;
            const std::unique_ptr<yield_pull<RESULT_TYPE>> _results;
            RESULT_TYPE result;
            const bool _last_element{false};
        public:
            explicit iterator(const BaseEinsum &einsum, bool ended = false) :
                    _ended{ended},
                    _results{(not ended) ? std::unique_ptr<yield_pull<RESULT_TYPE>>{new yield_pull<RESULT_TYPE>(boost::bind(&get, &einsum))}
                                         : std::unique_ptr<yield_pull<RESULT_TYPE>>{}} {
                if (not _ended)
                    _ended = not(*_results);
            }

            const RESULT_TYPE &operator*() {
                return result;
            }

            iterator &operator++() {
                if (_last_element)
                    _ended = true;
                if (not _ended) {
                    result = _results->get();
                    _last_element = not(*_results);
                }
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

    template<typename RESULT_TYPE>
    class Einsum : public BaseEinsum<RESULT_TYPE>{
    };

    template<>
    class Einsum<std::tuple<Key_t, size_t>>: public BaseEinsum<std::tuple<Key_t, size_t>>{
        using RESULT_TYPE = std::tuple<Key_t, size_t>;
    public:
        using BaseEinsum<RESULT_TYPE>::get;
        using BaseEinsum<RESULT_TYPE>::BaseEinsum;
        using BaseEinsum<RESULT_TYPE>::iterator;
        using BaseEinsum<RESULT_TYPE>::begin;
        using BaseEinsum<RESULT_TYPE>::end;
        using BaseEinsum<RESULT_TYPE>::calcEinsum;
        using BaseEinsum<RESULT_TYPE>::rekEinsum;

        static size_t contract(const Operands &operands, const EinsumPlan::Step &step) {
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
            return std::accumulate(results.begin(), results.end(), size_t(0));
        }

        static void rekEinsum(
                yield_push<RESULT_TYPE> &yield,
                const tnt::tensor::hypertrie::Operands &operands,
                const Key_t &result_key,
                const EinsumPlan::Step &step);


    };

    void Einsum<std::tuple<Key_t, size_t>>::rekEinsum(
            yield_push<std::tuple<Key_t, size_t>> &yield,
            const tnt::tensor::hypertrie::Operands &operands,
            const Key_t &result_key,
            const EinsumPlan::Step &step) {
        using RESULT_TYPE = std::tuple<Key_t, size_t>;
        std::cout << step << std::endl;
        // there are steps left
        if (not step.all_done) {
            // calculate next operands and result_key from current operands, step, label and resultKey
            Join join{result_key, operands, step};

            for (const auto&[next_operands, next_result_key] : join) {
                const EinsumPlan::Step &next_step = step.nextStep(next_operands);
                // start next recursive step.
                yield_pull<RESULT_TYPE> results{&rekEinsum, operands, result_key, step};
//                yield_pull<RESULT_TYPE> results{&rekEinsum, next_operands, next_result_key, next_step};
//                    for (const RESULT_TYPE &result : results) {
//                        yield(result);
//                    }
            }
        } else { // there are no steps left
            if (not operands.empty()) { // there are lonely and/or unique labels left.
                yield({result_key, contract(operands, step)});
            } else { // no labels left
                yield({result_key, 1});
            }
        }
    }

}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
