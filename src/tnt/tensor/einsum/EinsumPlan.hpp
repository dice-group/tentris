#ifndef SPARSETENSOR_EINSUM_EINSUMPLAN
#define SPARSETENSOR_EINSUM_EINSUMPLAN

#include <vector>
#include <memory>
#include <exception>
#include <cmath>
#include <ostream>

#include "tnt/tensor/einsum/Subscript.hpp"
#include "tnt/util/All.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include <fmt/format.h>

namespace {
    using namespace tnt::util::types;
    using namespace tnt::tensor::hypertrie;

    //TODO: move to utils
    template<typename T>
    std::set<label_t> getSubset(const T &interable, const label_t &remove_) {
        std::set<label_t> sub_set;
        std::copy_if(interable.cbegin(), interable.cend(),
                     std::inserter(sub_set, sub_set.begin()),
                     [&](const label_t &l) { return remove_ != l; });
        return sub_set;
    }

    //TODO: move to utils
    template<typename T>
    std::set<T> setMinus(std::set<T> plusSet, std::set<T> minusSet1, std::set<T> minusSet2) {
        std::set<T> result{};
        for (const T &item : plusSet)
            if (not minusSet1.count(item) and not minusSet2.count(item))
                result.insert(item);
        return result;
    }

};

namespace tnt::tensor::einsum {
    class EinsumPlan {
    public:
        enum STRATEGY {
            RESULT_FIRST,
            ONLY_BY_CARDINALITY // TODO: implement it
        };

        class Step;

    private:

        const std::shared_ptr<const Subscript> _subscript;
        const std::vector<label_t> _result_labels;
        mutable Step *initial_step = nullptr;
    public:
        explicit EinsumPlan(const std::shared_ptr<const Subscript> subscript, STRATEGY = RESULT_FIRST) :
                _subscript(subscript),
                _result_labels{subscript->getResultLabels()} {}

        ~EinsumPlan() {
            if (initial_step != nullptr)
                delete initial_step;
        }

        Step &getInitialStep(const Operands &operands) const {
            if (initial_step == nullptr) {
                const std::set<label_t> result_label_set{_subscript->getResultLabels().cbegin(),
                                                         _subscript->getResultLabels().cend()};
                initial_step = new Step{_subscript, _subscript->getLabelPosInResult(), operands,
                                        setMinus(_subscript->getAllLabels(), _subscript->getUniqueNonResultLabels(),
                                                 _subscript->getLonelyNonResultLabels()), result_label_set};
            }
            return *initial_step;
        }


        const std::shared_ptr<const Subscript> getSubscript() const {
            return _subscript;
        }

        const std::vector<label_t> &getResultLabels() const {
            return _result_labels;
        }


        class Step {
        private:
            mutable std::map<label_t, Step> next_step_cache;
            const std::shared_ptr<const Subscript> _subscript;
            const std::map<label_t, label_pos_t> _result_label_poss;
        public:
            const label_t label;
        private:
            const std::set<label_t> _label_candidates;
            const std::set<label_t> _unused_result_labels;
        public:
            const bool all_done;
            const bool result_labels_done;
        private:
            std::vector<op_pos_t> _op_poss;
            std::optional<key_pos_t> _result_pos;
            std::map<op_pos_t, op_pos_t> _diagonal2result_pos;
            std::vector<std::vector<key_pos_t>> _joinee_key_part_poss; ///< the joining key part positions of each join operand.
            std::vector<op_pos_t> next_op_poss;
            std::vector<std::vector<label_pos_t>> _unique_labels;

        private:

            Step(const std::shared_ptr<const Subscript> subscript,
                 const std::map<label_t, label_pos_t> &result_label_poss,
                 const label_t &min_card_label,
                 const std::set<label_t> &label_candidates,
                 const std::set<label_t> &unused_result_labels) :
                    _subscript{subscript},
                    _result_label_poss{result_label_poss},
                    label{min_card_label},
                    _label_candidates{getSubset(label_candidates, label)},
                    _unused_result_labels{getSubset(unused_result_labels, label)},
                    all_done{not bool(label_candidates.size())},
                    result_labels_done{not bool(unused_result_labels.size())} {
                if (not all_done) {
                    _op_poss = _subscript->operandsWithLabel(label);
                    auto found = _result_label_poss.find(label);
                    if (found != _result_label_poss.end())
                        _result_pos = {found->second};
                    else
                        _result_pos = std::nullopt;


                    // the joining key part positions of each join operand.
                    _joinee_key_part_poss.reserve(_op_poss.size());
                    for (const op_pos_t &op_pos : _op_poss) {
                        _joinee_key_part_poss.emplace_back(_subscript->labelPossInOperand(op_pos, label));
                    }

                    // TODO: only if label was left
                    const std::shared_ptr<const Subscript> subsc = _subscript->removeLabel(label);
                    next_op_poss = subsc->getOriginalOpPoss();


                    for (op_pos_t i = 0, j = 0; i < _op_poss.size() and j < next_op_poss.size();) {
                        auto pos_of_join_in_operands = _op_poss.at(i);
                        auto pos_of_result_in_operands = next_op_poss.at(j);
                        if (pos_of_join_in_operands == pos_of_result_in_operands) {
                            _diagonal2result_pos[i] = j;
                            ++j;
                            ++i;
                        } else if (pos_of_join_in_operands < pos_of_result_in_operands) {
                            ++i;
                        } else {
                            ++j;
                        }
                    }
                } else {
                    for (const auto &op_pos: range(subscript->numberOfOperands())) {
                        const std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> &map = _subscript->getUniqueNonResultContractions();
                        if (const auto found = map.find(op_pos);
                                found != map.cend()) {
                            _unique_labels.push_back(found->second.at(0));
                        } else {
                            _unique_labels.push_back({});
                        }
                    }
                    for (const std::vector<std::vector<label_pos_t>> &unique :
                            values(_subscript->getUniqueNonResultContractions())) {
                        _unique_labels.push_back(unique[0]);
                    }
                }
            }


        public:

            Step(const std::shared_ptr<const Subscript> subscript,
                 const std::map<label_t, label_pos_t> &result_label_poss,
                 const Operands &operands, const std::set<label_t> &label_candidates,
                 const std::set<label_t> &unused_result_labels) :
                    Step(subscript, result_label_poss,
                         getMinCardLabel(operands, label_candidates, unused_result_labels, *subscript),
                         label_candidates, unused_result_labels) {}

            Step(const std::shared_ptr<const Subscript> subscript,
                 const std::map<label_t, label_pos_t> &result_label_poss,
                 const Operands &operands, const std::set<label_t> &unused_result_labels) :
                    Step(subscript, result_label_poss,
                         getMinCardLabel(operands, subscript->getAllLabels(), unused_result_labels, *subscript),
                         subscript->getAllLabels(), unused_result_labels) {}

            inline const std::map<label_t, label_pos_t> &getResultLabels() const {
                return _result_label_poss;
            }

            inline size_t getResultSize() const {
                return _result_label_poss.size();
            }

            Step &nextStep(const Operands &operands) const {
                if (all_done)
                    throw std::invalid_argument("Must not be called if all_done is true");
                label_t label_ = getMinCardLabel(operands, _label_candidates, _unused_result_labels,
                                                 *(_subscript->removeLabel(label)));

                auto found = next_step_cache.find(label_);
                if (found != next_step_cache.end()) {
                    return found->second;
                } else {
                    const auto &result = next_step_cache.emplace(label_, Step{_subscript->removeLabel(label),
                                                                              _result_label_poss, label_,
                                                                              _label_candidates,
                                                                              _unused_result_labels});
                    return result.first->second;
                }

            }

            inline const std::vector<op_pos_t> &getOperandPositions() const {
                return _op_poss;
            }

            inline const std::vector<std::vector<key_pos_t>> &getKeyPartPoss() const {
                return _joinee_key_part_poss;
            };

            inline const std::vector<op_pos_t> &getPosOfOperandsInResult() const {
                return next_op_poss;
            }

            /**
             * Returns a mapping from {0..n}->{0..r} where n is the number of operands with label and r is the number of
             * non-scalar hypertries that result from the operation described by this plan.
             * @return a map that like defined above
             */
            inline const std::map<op_pos_t, op_pos_t> &getDiagonal2ResultMapping() const {
                return _diagonal2result_pos;
            }

            /**
             * An optional position in the key that the key_part that fullfill this step must be written if it exists.
             * @return an optional position in the key_part.
             */
            inline const std::optional<key_pos_t> &getResulKeyPos() const {
                return _result_pos;
            }

            const std::vector<std::vector<label_pos_t>> &getUniqueNonResultContractions() const {
                return _unique_labels;
            }

        private:

            static label_t
            getMinCardLabel(const Operands &operands, const std::set<label_t> &label_candidates,
                            const std::set<label_t> &unused_result_labels, const Subscript &sc) {
                const std::set<label_t> &candidates_to_be_used = (not unused_result_labels.empty())
                                                                 ? unused_result_labels : label_candidates;
                unused_result_labels.size();
                if (candidates_to_be_used.size() == 1) {
                    return *candidates_to_be_used.begin();
                } else {

                    label_t min_label = *candidates_to_be_used.begin();
                    double min_cardinality = INFINITY;
                    for (const label_t &label: candidates_to_be_used) {
                        if (const double label_cardinality = calcCard(operands, label, sc); label_cardinality <
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
            static double calcCard(const Operands &operands, const label_t &label, const Subscript &sc) {
                // get operands that have the label
                const std::vector<op_pos_t> &op_poss = sc.operandsWithLabel(label);

                std::vector<double> dim_cardinalities(op_poss.size(), INFINITY);
                std::vector<double> operand_cardinalities(op_poss.size(), INFINITY);
                double min_dim_cardinality = INFINITY;

                //iterate the operands that hold the label
                for (const auto &[i, op_pos] : enumerate(op_poss)) {
                    // get operand
                    const BoolHyperTrie *operand = operands.at(op_pos);
                    // get cardinality of the operand in this dimension
                    const std::vector<size_t> cards = operand->getCards(sc.labelPossInOperand(op_pos, label));

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
                const int i1 = std::accumulate(dim_cardinalities.cbegin(), dim_cardinalities.cend(), 1,
                                               std::multiplies<size_t>());
                const int i2 = std::accumulate(operand_cardinalities.cbegin(), operand_cardinalities.cend(), 1,
                                               std::multiplies<size_t>());
                const double d = min_dim_cardinality
                                 * i2
                                 / i1
                                 //                 prefer smaller min_dim cardinality
                                 + (1 - (1 / min_dim_cardinality));
//                                 + (double(1) / double(op_poss.size()));
                return d;
            }


        public:

            friend struct ::fmt::formatter<Step>;
        };
    };
};

template<>
struct fmt::formatter<tnt::tensor::einsum::EinsumPlan::Step> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const tnt::tensor::einsum::EinsumPlan::Step &p, FormatContext &ctx) {
        return format_to(ctx.begin(), "asd{}", 1);
    }
};

#endif //SPARSETENSOR_EINSUM_EINSUMPLAN
