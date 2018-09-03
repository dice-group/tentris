#ifndef SPARSETENSOR_EINSUM_SUBSCRIPT_HPP
#define SPARSETENSOR_EINSUM_SUBSCRIPT_HPP


#include <vector>
#include <tuple>
#include <map>
#include <numeric>
#include <algorithm>
#include <memory>
#include <ostream>

#include "tnt/util/All.hpp"
#include "tnt/util/UndirectedGraph.hpp"

namespace {
    using namespace tnt::util::types;
}

namespace tnt::tensor::einsum {
    /**
     * Representation of the subscript of a expression in einstein summation convention.
     * This provides also  bracketing of independently computable parts and resulting in a
     * cross product of the bracketed parts.
     */
    class Subscript {

        template<typename T>
        using UndirectedGraph = typename ::tnt::util::UndirectedGraph<T>;
        mutable std::map<label_t, std::shared_ptr<Subscript>> _cache_for_remove_label{};


        std::set<label_t> _all_labels;
    public:
        std::vector<std::vector<label_t>> _operands_labels;
    private:
        std::vector<label_t> _result_labels;
        std::vector<op_pos_t> _original_op_poss;
        std::vector<std::shared_ptr<Subscript>> _sub_subscripts;
        std::vector<std::set<label_t>> _distinct_operands_labels;
        std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> _label_poss_in_operands;
        std::map<label_t, label_pos_t> _label_pos_in_result;
        std::map<label_t, std::vector<op_pos_t>> _operands_with_label;
        std::set<label_t> _lonely_non_result_labels;
        std::vector<op_pos_t> _operands_with_lonely_non_result_labels;
        std::vector<op_pos_t> _operands_without_lonely_non_result_labels;
        std::map<op_pos_t, std::vector<label_pos_t>> _lonely_non_result_contractions_by_op;
        std::set<label_t> _unique_non_result_labels;
        std::vector<op_pos_t> _operands_with_unique_non_result_labels;
        std::vector<op_pos_t> _operands_without_unique_non_result_labels;
        std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> _unique_non_result_contractions_by_op;
        UndirectedGraph<label_t> _label_dependency_graph;
        std::vector<std::set<label_t>> _independent_label_subsets;

        /**
         * This function must be called if _operands_labels, _result_labels or _all_labels where changed. The user is
         * responsible to keep _operands_labels, _result_labels and _all_labels valid.
         */
        void updateFields() {
            _distinct_operands_labels = {};
            _distinct_operands_labels.reserve(numberOfOperands());
            for (const std::vector<label_t> &operands : _operands_labels)
                _distinct_operands_labels.emplace_back(std::set<label_t>(operands.begin(), operands.end()));


            _label_poss_in_operands = _calc_label_poss_in_operands(_operands_labels);

            _label_pos_in_result = {};
            for (const auto &[pos, label] : enumerate(_result_labels))
                _label_pos_in_result[pos] = label;

            _operands_with_label = {};
            for (const auto &[op_pos, labels] : enumerate(_distinct_operands_labels))
                for (const label_t &label : labels)
                    _operands_with_label[label].push_back(op_pos_t(op_pos));

            std::tie(_lonely_non_result_labels, _operands_with_lonely_non_result_labels,
                     _operands_without_lonely_non_result_labels, _lonely_non_result_contractions_by_op,
                     _unique_non_result_labels, _operands_with_unique_non_result_labels,
                     _operands_without_unique_non_result_labels,
                     _unique_non_result_contractions_by_op) = _calcUniqueAndLonelyNonResultLabels(
                    _operands_with_label, _label_poss_in_operands, _label_pos_in_result, numberOfOperands());

            _label_dependency_graph = calcLabelDependencyGraph(_distinct_operands_labels);

            _independent_label_subsets = _label_dependency_graph.getConnectedComponents();
        }

    public:
        Subscript() = default;

        Subscript(const Subscript &subscript) = default;

        /**
         * Generate a subscript from operands' labels and result's labels.
         * @param operands a vector that contains for every operands a raw_subscript, i.e. a vector of labels
         * @param result a raw_subscript, i.e. a vector of labels for the result
         */
        Subscript(std::vector<raw_subscript> operands, raw_subscript result) {
            std::tie(_operands_labels, _result_labels, _all_labels) = normalizeRawSubscripts(operands, result);

            _original_op_poss = std::vector<op_pos_t>(numberOfOperands());
            std::iota(_original_op_poss.begin(), _original_op_poss.end(), 0);

            _sub_subscripts = {};


            updateFields();
        }


        /**
         * Returns the number of operands that this subscript has.
         * @return number of operands
         */
        inline size_t numberOfOperands() const {
            return _operands_labels.size();
        }

        /**
         * Returns an "label dependency graph". It has labels as vertices. Two labels are connected by an edge iff they
         * are present at at least one common operand.
         * @return label dependency graph
         */
        inline const UndirectedGraph<label_t> &getLabelDependencyGraph() const {
            return _label_dependency_graph;
        }

        /**
         * Sets of labels that share no common operand between the sets.
         * @return independent label sets
         */
        inline const std::vector<std::set<label_t>> &getIndependentLabelSubsets() const {
            return _independent_label_subsets;
        }

        /**
         * A set of all labels used at the operands. (All labels used at the result must also occur at an operand.)
         * @return  all labels
         */
        inline const std::set<label_t> &getAllLabels() const {
            return _all_labels;
        }

        /**
         * a vector of reuslt labels.
         * @return
         */
        inline const std::vector<label_t> &getResultLabels() const {
            return _result_labels;
        }

        /**
         * A vector of the operands positions in the root subscript that this subscript was derived from.
         * @return by default this returns a sequence 0 ... number_of_ops -1 if this is the root subscript. Otherwise
         * any sequence that is a possibly shorter poermutation of that squence.
         */
        inline const std::vector<op_pos_t> &getOriginalOpPoss() const {
            return _original_op_poss;
        }

        /**
         * Get a vector of sub-subscripts. They are only available for subscripts that were created by calling
         * optimized() on another subscript. They are the subscripts that must be applied to the operands before this
         * subscript may be applied to their those resulting operands.
         * @return vector of sub subscripts.
         */
        inline const std::vector<std::shared_ptr<Subscript>> &getSubSubscripts() const noexcept {
            return _sub_subscripts;
        }

        const std::set<label_t> &getUniqueNonResultLabels() const {
            return _unique_non_result_labels;
        }

        const std::set<label_t> &getLonelyNonResultLabels() const {
            return _lonely_non_result_labels;
        }

        /**
         * A vector with positions of operands that have unique labels that don't occur in result.
         *
         * A unique label is a  label that is only present at one operand but there it must be present at least twice. If it is present only
         * once it is an lonely label.
         * @return set of operand positions
         */
        inline const std::vector<op_pos_t> &getOperandsWithUniqueNonResultLabels() const {
            return _operands_with_unique_non_result_labels;
        }

        /**
         * A vector with positions of operands that do NOT have unique labels that don't occur in result.
         *
         * A unique label is a label that is only present at one operand but there it must be present at least twice. If it is present only
         * once it is an lonely label.
         * @return set of operand positions
         */
        inline const std::vector<op_pos_t> &getOperandsWithoutUniqueNonResultLabels() const {
            return _operands_without_unique_non_result_labels;
        }

        /**
         * A vector with positions of operands that have lonely labels that don't occur in result.
         *
         * A lonely label is a label that is only present at one operand and there only once. If it is present multiple times it is an
         * unique label.
         * @return set of operand positions
         */
        inline const std::vector<op_pos_t> &getOperandsWithLonelyNonResultLabels() const {
            return _operands_with_lonely_non_result_labels;
        }

        /**
        * A vector with positions of operands that do NOT have lonely labels that don't occur in result.
        *
        * A lonely label is a label that is only present at one operand and there only once. If it is present multiple times it is an
        * unique label.
        * @return set of operand positions
        */
        inline const std::vector<op_pos_t> &getOperandsWithoutLonelyNonResultLabels() const {
            return _operands_without_lonely_non_result_labels;
        }

        /**
         * A map that gives for a position of an operand all diagonals on unique labels that don't occur in result.
         *
         * A unique label is a label that is only present at one operand but there it must be present at least twice.
         * If it is present only once it is an lonely label.
         *
         * The diagonals are encoded as a vectors of key_part_pos_t that are part of a diagonal. Every operand can have
         * multiple of such diagonals.
         *
         * Only operand positions that are in getOperandsWithUniqueNonResultLabels() are certain to be contained in the
         * set.
         * @return
         */
        inline const std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> &getUniqueNonResultContractions() const {
            return _unique_non_result_contractions_by_op;
        }


        /**
         * A map that gives for a position of an operand all diagonals on lonely labels that don't occur in result.
         *
         * A lonely label is a label that is only present at one operand and there only once. If it is present multiple
         * times it is an unique label.
         *
         * The diagonals are encoded by their position in the operand. All positions of lonely non operand labels are
         * stored in a vector by the operands position.
         *
         * Only operand positions that are in getOperandsWithLonelyNonResultLabels() are certain to be contained in the
         * set.
         * @return
         */
        inline const std::map<op_pos_t, std::vector<label_pos_t>> &getLonleyNonResultContractions() const {
            return _lonely_non_result_contractions_by_op;
        }

        /**
         * Get all positions of operands that use a given label.
         * @param label the label
         * @return set of operand positions
         * @throws out_of_range if label is not used in this subscript
         */
        inline const std::vector<op_pos_t> &operandsWithLabel(const label_t &label) const {
            return _operands_with_label.at(label);
        }

        /**
         * The position of an label in the result.
         * @param label  the label
         * @return position in the result
         * @throws out_of_range if label is not used in the result
         */
        inline const label_pos_t &labelPosInResult(const label_t &label) const {
            return _label_pos_in_result.at(label);
        }

        inline const std::map<label_t, label_pos_t> &getLabelPosInResult() const {
            return _label_pos_in_result;
        }

        /**
         * Get the positions of an label in an operand.
         * @param op_pos the position of the operand
         * @param label the label
         * @return a vector of label's positions in the operand
         */
        inline const std::vector<label_pos_t>
        labelPossInOperand(const op_pos_t &op_pos, const label_t &label) const noexcept {
            try {
                return _label_poss_in_operands.at(std::make_tuple(op_pos, label));
            } catch (...) {
                return {};
            }
        }

        /**
         * Get the labels of an Operand.
         * @param op_pos position of the operand
         * @return a vector of labels
         * @throws out_of_range if the operand position is out of range
         */
        inline const std::vector<label_pos_t> &operandLabels(const op_pos_t &op_pos) const {
            return _operands_labels.at(op_pos);
        }

        /**
         * Get the distinct labels of an Operand.
         * @param op_pos position of the operand
         * @return a set of labels
         * @throws out_of_range if the operand position is out of range
         */
        inline const std::set<label_pos_t> &distinctOperandLabels(const op_pos_t &op_pos) const {
            return _distinct_operands_labels.at(op_pos);
        }

        /**
         * Generates the subscript that arises from removing a label from all operands and the result. Operands that
         * result in scalars (no labels left) are removed. The original op position is saved in getOriginalOpPoss(). If
         * this is called repeatedly on the result the original op position refer always to the first subscript.
         * @param label the label to be removed.
         * @return a subscript without the given label.
         */
        const std::shared_ptr<const Subscript> removeLabel(const label_t &label) const {
            auto found = _cache_for_remove_label.find(label);
            if (found != _cache_for_remove_label.end())
                return found->second;
            else {
                if (not _all_labels.count(label))
                    throw std::invalid_argument("only labels that are present in an subscript may be removed.");
                else {
                    std::shared_ptr<Subscript> subscript = _cache_for_remove_label[label];
                    if (not subscript){
                        subscript = std::shared_ptr<Subscript>{new Subscript()};
                        _cache_for_remove_label[label] = subscript;
                    }

                    // remove the label from _all_labels
                    subscript->_all_labels = {};
                    std::copy_if(_all_labels.begin(), _all_labels.end(),
                                 std::inserter(subscript->_all_labels, subscript->_all_labels.begin()),
                                 [&](const label_t &l) { return l != label; });

                    // remove the label from _result_labels
                    subscript->_result_labels = {};
                    std::copy_if(_result_labels.begin(), _result_labels.end(),
                                 std::back_inserter(subscript->_result_labels),
                                 [&](const label_t &l) { return l != label; });

                    // remove the label from _operands_labels
                    subscript->_operands_labels = {};
                    subscript->_original_op_poss = {};
                    for (const auto &[op_pos, op_labels] : enumerate(_operands_labels)) {
                        std::vector<label_t> new_op_labels{};
                        std::copy_if(op_labels.begin(), op_labels.end(), std::back_inserter(new_op_labels),
                                     [&](const label_t &l) { return l != label; });
                        if (new_op_labels.size()) {
                            subscript->_operands_labels.emplace_back(std::move(new_op_labels));
                            subscript->_original_op_poss.emplace_back(op_pos);
                        }
                    }

                    subscript->updateFields();
                    return subscript;
                }
            }

        }

        /**
         * Returns an optimized subscript. It is equal to this if it is already optimal. Otherwise the subscript has is
         * bracketed into cross products. The original operands are then used in the new operands. The subscripts for the
         * new operands may be retrieved from the new Subscript by getSubSubscripts().
         * @return optimized Subscript or this
         */
        Subscript optimized() const {
            std::vector<std::shared_ptr<Subscript>> sub_subscripts{};
            std::vector<std::vector<label_t>> new_operands_labels{};
            std::vector<std::shared_ptr<Subscript>> new_sub_subscripts{};

            for (const std::set<label_t> &label_subset : _independent_label_subsets) {

                auto sub_subscript = std::make_shared<Subscript>(extractSubscript(label_subset));
                new_operands_labels.emplace_back(sub_subscript->getResultLabels());
                new_sub_subscripts.push_back(sub_subscript);
            }
            if (new_sub_subscripts.size() >= 1) {
                return {*this};
            } else {
                Subscript opt_subscript{new_operands_labels, _result_labels};
                opt_subscript._sub_subscripts = new_sub_subscripts;
                return opt_subscript;
            }
        }

        /**
         * Returns an subscript that uses only labels form this that are present in label_subset. If the labels of an
         * operand result to be empty by doing so it is removed. The original op pos of the resulting Subscript refer to
         * to the original op pos as used in this subscript.
         * @param label_subset a set of labels that are allowed in the new subscript
         * @return a SubSubscript
         */
        Subscript extractSubscript(const std::set<label_t> &label_subset) const {
            std::vector<std::vector<label_t>> operands_labels;
            std::vector<op_pos_t> original_op_poss;
            for (const auto &[parent_op_pos, parent_op_labels] : enumerate(_operands_labels)) {
                std::vector<label_t> op_labels{};
                for (const label_t &label : parent_op_labels) {
                    if (label_subset.count(label))
                        op_labels.emplace_back(label);
                }
                if (op_labels.size()) {
                    operands_labels.push_back(op_labels);
                    original_op_poss.push_back(op_pos_t(parent_op_pos));
                }
            }
            std::vector<label_t> result_labels{};
            for (const label_t &label : _result_labels)
                if (label_subset.count(label))
                    result_labels.emplace_back(label);

            Subscript extracted_subscript{operands_labels, result_labels};
            extracted_subscript._original_op_poss = original_op_poss;
            return extracted_subscript;
        }

    private:
        static std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>>
        _calc_label_poss_in_operands(std::vector<std::vector<label_t>> operands_labels) {
            std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> label_poss_in_operands{};

            for (const auto &[op_id, labels] : enumerate(operands_labels))
                for (const auto &[label_pos, label] : enumerate(labels))
                    label_poss_in_operands[{op_id, label}].push_back(label_pos_t(label_pos));
            return label_poss_in_operands;
        }

        static std::map<op_pos_t, std::vector<std::vector<label_pos_t>>>
        calcUniqueNonResultContractions(std::vector<op_pos_t> operands_with_unique_non_result_labels,
                                        std::vector<std::set<label_t>> distinct_operands_labels,
                                        std::set<label_t> unique_non_result_labels,
                                        std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> label_poss_in_operands) {

            std::map<op_pos_t, std::vector<std::vector<label_pos_t >>> unique_non_result_contractions;

            for (const op_pos_t &op_pos : operands_with_unique_non_result_labels) {

                std::vector<std::vector<label_pos_t>> contractions{};

                for (const label_t &label : distinct_operands_labels[op_pos])
                    if (unique_non_result_labels.count(label)) {
                        const std::vector<label_pos_t> &contraction = label_poss_in_operands[std::make_tuple(op_pos,
                                                                                                             label)];
                        if (contraction.size())
                            contractions.emplace_back(contraction);
                    }

                if (contractions.size())
                    unique_non_result_contractions.emplace(op_pos, contractions);
            }

            return unique_non_result_contractions;
        }

        static auto
        _calcUniqueAndLonelyNonResultLabels(const std::map<label_t, std::vector<op_pos_t>> &operands_with_label,
                                            const std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> &label_poss_in_operands,
                                            const std::map<label_t, label_pos_t> &label_pos_in_result,
                                            const size_t &numberOfOprands)
        -> std::tuple<std::set<label_t>, std::vector<op_pos_t>, std::vector<op_pos_t>, std::map<op_pos_t, std::vector<label_pos_t>>,
                std::set<label_t>, std::vector<op_pos_t>, std::vector<op_pos_t>, std::map<op_pos_t, std::vector<std::vector<label_pos_t>>>> {
            std::set<label_t> lonely_non_result_labels;
            std::vector<op_pos_t> operands_with_lonely_non_result_labels;
            std::vector<op_pos_t> operands_without_lonely_non_result_labels;
            std::map<op_pos_t, std::vector<label_pos_t>> lonely_non_result_contractions_by_op;
            std::set<label_t> unique_non_result_labels;
            std::vector<op_pos_t> operands_with_unique_non_result_labels;
            std::vector<op_pos_t> operands_without_unique_non_result_labels;
            std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> unique_non_result_contractions_by_op;


            // go by label thru the operands
            for (const auto &[label, ops_with_label] : operands_with_label) {
                // check if the label is only at one operand an dif it is not at the result
                if (ops_with_label.size() == 1 and not label_pos_in_result.count(label)) {
                    const op_pos_t &op_pos = ops_with_label[0];
                    const std::vector<label_pos_t> &label_poss_in_op = label_poss_in_operands.at({op_pos, label});
                    // check if the label is only once at the operand it is an lonely label ...
                    if (label_poss_in_op.size() == 1) {
                        lonely_non_result_labels.insert(label);
                        std::vector<label_pos_t> &contractions = lonely_non_result_contractions_by_op[op_pos];
                        const label_pos_t &label_position = label_poss_in_op[0];
                        contractions.push_back(label_position);
                    } else { // label_poss_in_op.size() > 1
                        // ... otherwise it is just unique
                        unique_non_result_labels.insert(label);
                        std::vector<std::vector<label_pos_t>> &contractions = unique_non_result_contractions_by_op[op_pos];
                        contractions.push_back(label_poss_in_op);
                    }
                }
            }
            // go thru all op_pos
            for (const op_pos_t &op_pos : range(op_pos_t(numberOfOprands))) {
                // write out if a op has non result lonely labels or not
                if (lonely_non_result_contractions_by_op.count(op_pos))
                    operands_with_lonely_non_result_labels.push_back(op_pos);
                else
                    operands_without_lonely_non_result_labels.push_back(op_pos);

                // write out if a op has non result unique labels or not
                if (unique_non_result_contractions_by_op.count(op_pos))
                    operands_with_unique_non_result_labels.push_back(op_pos);
                else
                    operands_without_unique_non_result_labels.push_back(op_pos);
            }

            return {lonely_non_result_labels, operands_with_lonely_non_result_labels,
                    operands_without_lonely_non_result_labels, lonely_non_result_contractions_by_op,
                    unique_non_result_labels, operands_with_unique_non_result_labels,
                    operands_without_unique_non_result_labels, unique_non_result_contractions_by_op};
        }

        static std::set<label_t>
        _calcNonResultSingleOperandLabels(const std::map<label_t, std::vector<op_pos_t>> &operands_with_label,
                                          const std::vector<label_t> &result_labels) {
            std::set<label_t> non_result_single_operand{};

            for (const auto &[label, ops_with_label] : operands_with_label)
                if ((ops_with_label.size() == 1) and
                    (not std::count(result_labels.cbegin(), result_labels.cend(), label)))
                    non_result_single_operand.insert(label);

            return non_result_single_operand;
        }


        static UndirectedGraph<label_t>
        calcLabelDependencyGraph(const std::vector<std::set<label_t>> &distinct_operands_labels) {
            UndirectedGraph<label_t> label_dependency_graph{};
            for (const std::set<label_t> &labels : distinct_operands_labels) {
                label_dependency_graph.addCompleteGraph(labels);
            }
            return label_dependency_graph;
        }

        static std::tuple<std::vector<std::vector<label_t >>, std::vector<label_t>, std::set<label_t>>
        normalizeRawSubscripts(
                const std::vector<std::vector<label_t>> &raw_operand_subscripts,
                const std::vector<label_t> &raw_result_subscript) {
            // map storing the mapping from old to new labels
            std::map<label_t, label_t> raw_to_norm_label{};
            // next new label to be used
            label_t next_norm_label = 0;

            // normalize the result
            std::vector<label_t> norm_result_subscript = normalizeLabelVector(raw_to_norm_label, next_norm_label,
                                                                              raw_result_subscript);

            // normalize the operands
            std::vector<std::vector<label_t >> norm_operand_subscripts{};
            norm_operand_subscripts.reserve(raw_operand_subscripts.size());
            for (const auto &op_labels : raw_operand_subscripts) {
                norm_operand_subscripts.push_back(normalizeLabelVector(raw_to_norm_label, next_norm_label, op_labels));
            }

            // put all new labels that were used into a set
            std::set<label_t> labels{};
            for (label_t label : range(next_norm_label))
                labels.emplace(label);

            return {norm_operand_subscripts, norm_result_subscript, labels};
        }

        static std::vector<label_t> normalizeLabelVector(
                std::map<label_t, label_t> &raw_to_norm_label, label_t &next_norm_label,
                const std::vector<label_t> &raw_labels) {
            std::vector<label_t> norm_operand_labels(raw_labels.size());

            label_pos_t label_pos = 0;
            for (label_t raw_label : raw_labels) {
                // check if raw label is already mapped to a normed label
                auto norm_label_ = raw_to_norm_label.find(raw_label);
                label_t norm_label;
                if (norm_label_ != end(raw_to_norm_label)) {
                    norm_label = norm_label_->second;
                } else {
                    // otherwise map it
                    norm_label = next_norm_label++;
                    raw_to_norm_label[raw_label] = norm_label;
                }

                norm_operand_labels[label_pos] = norm_label;
                label_pos++;
            }
            return norm_operand_labels;
        }

    public:
        friend std::ostream &operator<<(std::ostream &os, const Subscript &subscript) {
            os << "Subscript: _operands_labels: " << subscript._operands_labels
               << " _result_labels: " << subscript._result_labels;
            return os;
        }

    };
}

#endif //SPARSETENSOR_EINSUM_SUBSCRIPT_HPP

