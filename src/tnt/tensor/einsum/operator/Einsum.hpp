#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP

#include <algorithm>
#include <memory>
#include <numeric>
#include <vector>

#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/tensor/Result.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/tensor/einsum/operator/OperatorNode.hpp"
#include "tnt/tensor/einsum/operator/Slice.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/hypertrie/Join.hpp"
#include "tnt/util/All.hpp"
#include "tnt/util/LogHelper.hpp"

namespace {
	using namespace tnt::tensor::hypertrie;
	using namespace tnt::util::types;
	using Join = tnt::tensor::hypertrie::Join;
	using namespace ::tnt::logging;
} // namespace

namespace tnt::tensor::einsum::operators {
	/**
	 * This is an basic Einstein-Summation Operator that can perform any Einstein Summation Convention Operation. In
	 * most cases this operator should only be used as sub operator of a CrossProduct as it is not very effective if an
	 * cross product is involved.
	 * @see CrossProduct
	 */
	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	class Einsum : public OperatorNode<RESULT_TYPE> {
	public:
		using Slice_t = Slice<RESULT_TYPE>;
		using yield_pull_t = yield_pull<RESULT_TYPE>;
		using yield_push_t = yield_push<RESULT_TYPE>;

	protected:
		/**
		 * The evaluation plan for this->subscript.
		 */
		mutable EinsumPlan plan;
		mutable std::vector<Slice_t> predecessors{};
		mutable Operands operands{};
		mutable bool operands_generated = false;
		mutable bool may_have_results = false;
		mutable bool result_calculated = false;
		mutable Result<RESULT_TYPE> result{};

	public:
		/**
		 * Constructor.
		 * @param subscript its subscript.
		 * @param slice_keys the key to its operators
		 * @param trie the tries that shall be sliced.
		 */
		Einsum(const std::shared_ptr<const Subscript> subscript, const std::vector<SliceKey_t> &slice_keys,
		       const std::vector<BoolHyperTrie *> &tries)
				: OperatorNode<RESULT_TYPE>{}, plan{subscript} {
			this->type = OperatorType::EINSUM;
			for (const auto[slice_key, trie] : zip(slice_keys, tries)) {
				predecessors.push_back({slice_key, trie});
			}
		}

		Einsum(Einsum &&) = default;

		virtual ~Einsum() = default;

		yield_pull_t get() const override { return yield_pull_t(boost::bind(&Einsum<RESULT_TYPE>::get, this, _1)); }

		const Result<RESULT_TYPE> &getFullResult() const override {
			calcResult();
			return result;
		}

	private:

		void calcResult() const {
			// generate operands only once
			if (not operands_generated) {
				operands_generated = true;

				for (Slice_t &slice : predecessors) {
					switch (slice.slice_type) {
						case Slice_t::SCALAR: {
							if (not slice.getScalar())
								return;
							break;
						}

						case Slice_t::HYPERTRIE: {
							BoolHyperTrie *trie = slice.getHyperTrie();
							if (trie == nullptr)
								return;
							operands.push_back(trie);
							break;
						}
					}
				}
				may_have_results = true;
			}

			if (may_have_results and not result_calculated) {
				calcEinsum(result, operands, plan);
				result_calculated = true;
			}
		}

		void get(yield_push_t &yield) const {

			calcResult();

			for (const auto &binding : result) {
				yield(binding);
			}
		}

		/**
		 * Prepares the arguments for the recursive calculation of the results.
		 * @param operands vector of tensors
		 */
		static void calcEinsum(Result<RESULT_TYPE> &result, const Operands &operands, const EinsumPlan &plan) {
			// unpacks HyperTrieTensors to HyperTries or value types T
			// initialize emtpy result key
			// plan first step
			EinsumPlan::Step step = plan.getInitialStep(operands);

			Key_t result_key = Key_t(step.getResultSize(), KEY_PART_MAX);
			// start recursion
			rekEinsum(result, operands, result_key, step);
		}

		/**
		 * Acutal recursion to calculate the result
		 * @param operands vector of current operands
		 * @param result_key current result key. Is filled step by step. When all positions are filled this key represents
		 * the key of an result value.
		 * @param step current step. This holds also data to plan the next step.
		 * @param label the current label that is to be processed in this recursive step
		 */
		static void rekEinsum(Result<RESULT_TYPE> &, const Operands &operands, const Key_t &result_key,
		                      const EinsumPlan::Step &step);

		static typename RESULT_TYPE::count_t contract(const Operands &operands, const EinsumPlan::Step &step);


	public:
		typename RESULT_TYPE::collection_t::iterator begin() {
			calcResult();
			return result.begin();
		}

		typename RESULT_TYPE::collection_t::iterator end() {
			calcResult();
			return result.end();
		}

		typename RESULT_TYPE::collection_t::const_iterator cbegin() const {
			calcResult();
			return result.cbegin();
		}

		typename RESULT_TYPE::collection_t::const_iterator cend() const {
			calcResult();
			return result.cend();
		}

		friend bool
		rekEinsumBoolNonResult(const Operands &operands, const Key_t &result_key, const EinsumPlan::Step &step);
	};

	template<>
	typename counted_binding::count_t
	Einsum<counted_binding>::contract(const Operands &operands, const EinsumPlan::Step &step) {

		std::vector<counted_binding::count_t> results(operands.size(), 0);
		for (const auto[op_pos, op_and_contr] : enumerate(zip(operands, step.getUniqueNonResultContractions()))) {
			const auto &[op, unique_contraction] = op_and_contr;

			if (unique_contraction.empty() or op->depth() != 3) {
				results[op_pos] += op->size();
			} else {
				for (const BoolHyperTrie *hyperTrie : BoolHyperTrie::DiagonalView{op, unique_contraction})
					results[op_pos] += hyperTrie->size();
			}
		}
		return std::accumulate(results.begin(), results.end(), counted_binding::count_t(1), std::multiplies<>());
	}

	template<>
	distinct_binding::count_t
	Einsum<distinct_binding>::contract(const Operands &operands, const EinsumPlan::Step &step) {
		std::vector<distinct_binding::count_t> results(operands.size(), false);
		// A unique label is a label that is only present at one operand but there it must be present at least twice. If
		// it is present only once it is an lonely label.
		for (const auto[op_pos, op_and_contr] : enumerate(zip(operands, step.getUniqueNonResultContractions()))) {
			const auto &[op, unique_contraction] = op_and_contr;
			if (unique_contraction.empty() or op->depth() != 3) {
				results[op_pos] = true;
			} else { // not unique_contraction.empty() and op->depth() == 3
				// it could be that there is one unique label and one lonely label.
				// TODO: compute an estimation if it is better to first process the unique label oder the lonely label
				BoolHyperTrie::DiagonalView diag{op, unique_contraction};
				if (diag.begin() != diag.end())
					results[op_pos] = true;
			}
		}
		return std::accumulate(results.begin(), results.end(), true, std::logical_and<>());
	}

	template<>
	void Einsum<counted_binding>::rekEinsum(Result<counted_binding> &result, const Operands &operands,
	                                        const Key_t &result_key, const EinsumPlan::Step &step) {
		// there are steps left
		if (not step.all_done) {
			// calculate next operands and result_key from current operands, step, label and resultKey
			Join join{result_key, operands, step};

			for (const auto &[next_operands, next_result_key] : join) {
				const EinsumPlan::Step &next_step = step.nextStep(next_operands);
				// start next recursive step.
				rekEinsum(result, next_operands, next_result_key, next_step);
			}
		} else {                                 // there are no steps left
			if (not operands.empty()) { // there are lonely and/or unique labels left.
				const size_t i = contract(operands, step);
				if (i > 0)
					result.insert({result_key, i});
			} else { // no labels left
				result.insert({result_key, 1});
			}
		}
	};

	/**
	 * Assume a distinct/bool query/einsum operation. If all result labels were processed it is sufficient to check if
	 * any valid combination for the remaining labels exists.
	 * @param operands
	 * @param result_key
	 * @param step
	 * @return if a valid combination of label bindings was found return
	 */

	inline bool
	rekEinsumBoolNonResult(const Operands &operands, const Key_t &result_key, const EinsumPlan::Step &step) {
		// there are steps left
		if (not step.all_done) {
			// calculate next operands and result_key from current operands, step, label and resultKey
			Join join{result_key, operands, step};
			for (const auto[next_operands, next_result_key] : join) {
				const EinsumPlan::Step &next_step = step.nextStep(next_operands);
				tnt::logging::log(fmt::format("{}\n", next_step));
				// start next recursive step.
				bool exists = rekEinsumBoolNonResult(next_operands, next_result_key, next_step);
				if (exists) // if a valid combination of label bindings was found return
					return true;
			}
			// no valid combination of label bindings was found for this step
			return false;

		} else {                            // there are no steps left
			if (operands.empty()) { // there are no further labels left. all operands are already reduced to scalars.
				return true;
			} else { // no labels left
				return Einsum<distinct_binding>::contract(operands, step);
			}
		}
	};

	template<>
	void Einsum<distinct_binding>::rekEinsum(Result<distinct_binding> &result, const Operands &operands,
	                                         const Key_t &result_key, const EinsumPlan::Step &step) {
		logTrace("step: {}"_format(step));
		// there are steps with operand labels left
		if (not step.result_labels_done) {
			// calculate next operands and result_key from current operands, step, label and resultKey
			Join join{result_key, operands, step};
			for (const auto &[next_operands, next_result_key] : join) {
				const EinsumPlan::Step &next_step = step.nextStep(next_operands);
				// start next recursive step.
				rekEinsum(result, next_operands, next_result_key, next_step);
			}
		} else if (step.all_done) {
			if (operands.empty()) { // there are no further labels left. all operands are already reduced to scalars.
				result.insert(result_key);
			} else { // no labels left
				if (Einsum<distinct_binding>::contract(operands, step)) {
					result.insert(result_key);
				}
			}
		} else if (not result.contains(result_key) and rekEinsumBoolNonResult(operands, result_key, step)) {
			// there are no steps with operand labels left
			// check if  a valid combination of non-result labels for the current result candidate exists
			result.insert(result_key);
		}

	};

} // namespace tnt::tensor::einsum::operators
#endif // SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
