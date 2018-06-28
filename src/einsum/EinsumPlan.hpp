#ifndef SPARSETENSOR_EINSUM_EINSUMPLAN
#define SPARSETENSOR_EINSUM_EINSUMPLAN

#include <vector>
#include <memory>
#include <exception>

#include "Subscript.hpp"
#include "Types.hpp"
#include "../util/All.hpp"
#include "../hypertrie/BoolHyperTrie.hpp"

namespace sparsetensor::operations {
    class EinsumPlan {
        using BoolHyperTrie = sparsetensor::hypertrie::BoolHyperTrie;
        using Operands =  typename std::vector<BoolHyperTrie *>;
        using key_pos_t = sparsetensor::tensor::key_pos_t;
        const Subscript &_subscript;
        const std::vector<label_t> &_result_labels;
    public:
        class Step;

        explicit EinsumPlan(const Subscript &subscript) : _subscript(subscript),
                                                          _result_labels{subscript.getResultLabels()} {}

        Step getInitialStep(const Operands &operands) const {
            return Step{_subscript, _subscript.getResultLabels(), operands};
        }

        inline const Subscript &getSubscript() const {
            return _subscript;
        }

        const std::vector<label_t> &getResultLabels() const {
            return _result_labels;
        }


        class Step {
            const Subscript &_subscript;
            const std::vector<label_t> &_result_labels;
        public:
            const label_t label;
        private:
            const std::set<label_t> _label_candidates;
        public:
            const bool all_done;
        private:
            Step(const Subscript &subscript, const std::vector<label_t> &result_labels,
                 const std::set<label_t> &label_candidates, const Operands &operands) :
                    _subscript(subscript),
                    _result_labels(result_labels),
                    label{getMinCardLabel(operands)},
                    _label_candidates{getSubset(label_candidates, label)},
                    all_done{bool(_label_candidates.size())} {}

            template<typename T>
            static std::set<label_t> getSubset(const T &interable, const label_t &remove_) {
                std::set<label_t> sub_set;
                std::copy_if(interable.cbegin(), interable.cend(),
                             std::inserter(sub_set, sub_set.begin()),
                             [&](const label_t &l) { return remove_ != l; });
                return sub_set;
            }

        public:

            Step(const Subscript &subscript, const std::vector<label_t> &result_labels, const Operands &operands) :
                    Step(subscript, result_labels, subscript.getAllLabels(), operands) {}

            inline const std::vector<label_t> &getResultLabels() const {
                return _result_labels;
            }

            inline size_t getResultSize() const {
                return _result_labels.size();
            }

            Step nextStep(const Operands &operands) const {
                if (all_done)
                    throw "Must not be called if all_done is true";
                return {_subscript, _result_labels, _label_candidates, operands};
            }

            inline const std::vector<op_pos_t> &getOperandPositions() const {
                return _subscript.operandsWithLabel(label);
            }

            std::vector<std::vector<key_pos_t>> getKeyPartPoss() const {
                std::vector<std::vector<key_pos_t>> key_part_poss{};
                key_part_poss.reserve(getOperandPositions().size());
                for (const op_pos_t &op_pos : getOperandPositions()) {
                    key_part_poss.emplace_back(_subscript.labelPossInOperand(op_pos, label));
                }
                return key_part_poss;
            };

            std::vector<op_pos_t> getPosOfOperandsInResult() const {
                const std::vector<op_pos_t> &result_ops = _subscript.removeLabel(label).getOriginalOpPoss();
                return result_ops;
            }

            std::optional<key_pos_t> getResulKeyPos() const {
                try {
                    return _subscript.labelPosInResult(label);
                } catch (...) {
                    return std::nullopt;
                }
            }


        private:

            label_t getMinCardLabel(const Operands &operands) {
                if (_label_candidates.size() == 1) {
                    return *_label_candidates.begin();
                } else {

                    label_t min_label = *_label_candidates.begin();
                    double min_cardinality = INFINITY;
                    for (const label_t &label: _label_candidates) {
                        if (const double label_cardinality = calcCard(operands, label); label_cardinality <
                                                                                        min_cardinality) {
                            min_cardinality = label_cardinality;
                            min_label = label;
                        }
                    }
                    return min_label;
                }
            }

            /**
         * Calculates the cardinality of an Label in an Step.
         * @tparam T type of the values hold by processed Tensors (Tensor).
         * @param operands Operands for this Step.
         * @param step current step
         * @param label the label
         * @return label's cardinality in current step.
         */
            double calcCard(const Operands &operands, const label_t &label) const {
                // get operands that have the label
                const std::vector<op_pos_t> &op_poss = _subscript.operandsWithLabel(label);

                std::vector<double> dim_cardinalities(op_poss.size(), INFINITY);
                std::vector<double> operand_cardinalities(op_poss.size(), INFINITY);
                double min_dim_cardinality = INFINITY;

                //iterate the operands that hold the label
                for (const auto &[i, op_pos] : enumerate(op_poss)) {
                    // get operand
                    const BoolHyperTrie *operand = operands.at(op_pos);
                    // get cardinality of the operand in this dimension
                    const std::vector<size_t> cards = operand->getCards(_subscript.labelPossInOperand(op_pos, label));

                    // calc it's cardinality
                    const size_t &dim_cardinality = *std::min_element(cards.cbegin(), cards.cend());
                    // if it is zero the overall cardinality is zero
                    if (dim_cardinality == 0)
                        return 0;

                    if (dim_cardinality < min_dim_cardinality)
                        min_dim_cardinality = dim_cardinality;
                    dim_cardinalities[i] = dim_cardinality;
                    operand_cardinalities[i] = operand->size();
                }

                // see: A. Swami and K. B. Schiefer, “On the estimation of join result sizes,” in International Conference on Extending Database Technology, 1994, pp. 287–300. (290-291)
                return min_dim_cardinality
                       * std::accumulate(operand_cardinalities.cbegin(), operand_cardinalities.cend(), 1,
                                         std::multiplies<size_t>())
                       / std::accumulate(dim_cardinalities.cbegin(), dim_cardinalities.cend(), 1,
                                         std::multiplies<size_t>())
                       // prefer smaller min_dim cardinality
                       + (1 - (1 / min_dim_cardinality));
            }

        };
    };
};


#endif //SPARSETENSOR_EINSUM_EINSUMPLAN
