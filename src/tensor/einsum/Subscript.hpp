#ifndef SPARSETENSOR_EINSUM_SUBSCRIPT_HPP
#define SPARSETENSOR_EINSUM_SUBSCRIPT_HPP


#include <vector>
#include <tuple>
#include <map>
#include <numeric>
#include <algorithm>
#include <memory>

#include "../../util/All.hpp"
#include "../../util/UndirectedGraph.hpp"


//namespace tnt::tensor::einsum {
//    class Subscript;
//};
//
//::std::ostream &operator<<(::std::ostream &out, ::tnt::tensor::einsum::Subscript &subscript);

namespace tnt::tensor::einsum {
    using namespace tnt::util::types;


    /**
     * Representation of the subscript of a expression in einstein summation convention.
     * This provides also  brackets out all independently computable parts and resulting in a
     * cross product of the bracketed parts.
     */
    class Subscript {

        template<typename T>
        using UndirectedGraph = typename ::tnt::util::UndirectedGraph<T>;


        std::set<label_t> _all_labels;
        std::vector<std::vector<label_t>> _operands_labels;
        std::vector<label_t> _result_labels;
        std::vector<op_pos_t> _original_op_poss;
        std::vector<std::shared_ptr<Subscript>> _sub_subscripts;
        // TODO: remove?
        std::vector<std::set<label_t>> _distinct_operands_labels;
        std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> _label_poss_in_operands;
        std::map<label_t, label_pos_t> _label_pos_in_result;
        std::map<label_t, std::vector<op_pos_t>> _operands_with_label;
        std::set<label_t> _unique_non_result_labels;
        std::vector<op_pos_t> _operands_with_unique_non_result_labels;
        std::vector<op_pos_t> _operands_without_unique_non_result_labels;
        UndirectedGraph<label_t> _label_dependency_graph;
        std::vector<std::set<label_t>> _independent_label_subsets;
        std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> _unique_non_result_contractions;

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
            for (const auto &[op_pos, labels] : enumerate(_operands_labels))
                for (const label_t &label : labels)
                    _operands_with_label[label].push_back(op_pos_t(op_pos));

            _unique_non_result_labels = _calcNonResultSingleOperandLabels(_operands_with_label, _result_labels);

            _operands_with_unique_non_result_labels = std::vector<op_pos_t>{};
            _operands_without_unique_non_result_labels = std::vector<op_pos_t>{};
            for (const auto &[op_pos, labels] : enumerate(_distinct_operands_labels)) {
                std::vector<label_t> intersect;
                std::set_intersection(labels.cbegin(), labels.cend(), _unique_non_result_labels.cbegin(),
                                      _unique_non_result_labels.cend(), std::back_inserter(intersect));
                if (intersect.size())
                    _operands_with_unique_non_result_labels.push_back(op_pos_t(op_pos));
                else
                    _operands_without_unique_non_result_labels.push_back(op_pos_t(op_pos));
            }

            _label_dependency_graph = calcLabelDependencyGraph(_distinct_operands_labels);

            _independent_label_subsets = _label_dependency_graph.getConnectedComponents();

            _unique_non_result_contractions =
                    calcUniqueNonResultContractions(_operands_with_unique_non_result_labels, _distinct_operands_labels,
                                                    _unique_non_result_labels, _label_poss_in_operands);
        }

        Subscript() {};
    public:
        Subscript(std::vector<raw_subscript> operands, raw_subscript result) {
            std::tie(_operands_labels, _result_labels, _all_labels) = normalizeRawSubscripts(operands, result);

            _original_op_poss = std::vector<op_pos_t>{};
            _original_op_poss.reserve(numberOfOperands());
            std::iota(_original_op_poss.begin(), _original_op_poss.end(), 0);

            _sub_subscripts = {};

            // TODO: add execPlan and remove_labels_cache
//            self._exec_plan = None
//            self._remove_labels_cache: Dict[label_t, 'Subscript'] = dict()

            updateFields();
        }


        inline size_t numberOfOperands() const {
            return _operands_labels.size();
        }

        inline const UndirectedGraph<label_t> &getLabelDependencyGraph() const {
            return _label_dependency_graph;
        }

        inline const std::vector<std::set<label_t>> &getIndependentLabelSubsets() const {
            return _independent_label_subsets;
        }

        inline const std::set<label_t> &getAllLabels() const {
            return _all_labels;
        }

        inline const std::vector<label_t> &getResultLabels() const {
            return _result_labels;
        }

        inline const std::vector<op_pos_t> &getOriginalOpPoss() const {
            return _original_op_poss;
        }

        inline const std::vector<std::shared_ptr<Subscript>> &getSubSubscripts() const {
            return _sub_subscripts;
        }

        inline const std::vector<op_pos_t> &getOperandsWithUniqueNonResultLabels() const {
            return _operands_with_unique_non_result_labels;
        }

        inline const std::vector<op_pos_t> &getOperandsWithoutUniqueNonResultLabels() const {
            return _operands_without_unique_non_result_labels;
        }

        inline const std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> &getUniqueNonResultContractions() const {
            return _unique_non_result_contractions;
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

        inline const label_pos_t &labelPosInResult(const label_t &label) const {
            return _label_pos_in_result.at(label);
        }

        inline const std::vector<label_pos_t> labelPossInOperand(const op_pos_t &op_pos, const label_t &label) const {
            try {
                return _label_poss_in_operands.at(std::make_tuple(op_pos, label));
            } catch (...) {
                return {};
            }
        }

        inline const std::vector<label_pos_t> &operandLabels(const op_pos_t &op_pos) const {
            return _operands_labels.at(op_pos);
        }

        inline const std::set<label_pos_t> &distinctOperandLabels(const op_pos_t &op_pos) const {
            return _distinct_operands_labels.at(op_pos);
        }

        inline const std::vector<std::vector<label_pos_t>> &uniqueNonResultContractions(const op_pos_t &op_pos) const {
            return _unique_non_result_contractions.at(op_pos);
        }

        Subscript removeLabel(const label_t &label) const {
            if (not _all_labels.count(label))
                return *this;
            else {
                Subscript subscript{};

                subscript._all_labels = {};
                std::copy_if(_all_labels.begin(), _all_labels.end(),
                             std::inserter(subscript._all_labels, subscript._all_labels.begin()),
                             [&](const label_t &l) { return l != label; });

                subscript._operands_labels = {};
                subscript._original_op_poss = {};
                for (const auto &[op_pos, op_labels] : enumerate(_operands_labels)) {
                    std::vector<label_t> new_op_labels{};
                    std::copy_if(op_labels.begin(), op_labels.end(), std::back_inserter(new_op_labels),
                                 [&](const label_t &l) { return l != label; });
                    if (new_op_labels.size()) {
                        subscript._operands_labels.emplace_back(std::move(new_op_labels));
                        subscript._original_op_poss.emplace_back(op_pos);
                    }
                }

                subscript.updateFields();
                return subscript;
            }

        }

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

            // normalize the operands
            std::vector<std::vector<label_t >> norm_operand_subscripts{};
            norm_operand_subscripts.reserve(raw_operand_subscripts.size());
            for (const auto &[op_pos, op_labels] : enumerate(raw_operand_subscripts)) {
                norm_operand_subscripts[op_pos] = normalizeLabelVector(raw_to_norm_label, next_norm_label, op_labels);
            }

            // normalize the result
            std::vector<label_t> norm_result_subscript = normalizeLabelVector(raw_to_norm_label, next_norm_label,
                                                                              raw_result_subscript);

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

//    public:
//        friend ::std::ostream &operator<<(::std::ostream &out, ::tnt::operations::Subscript &subscript) {
//            out << "<Subscript: \n"
//                << "all_labels=" << subscript._all_labels
//                << ",\n\t"
//                << "operands_labels = \n\t\t" << subscript._operands_labels
//                << ",\n\t"
//                << "distinct_operands_labels = \n\t\t" << subscript._distinct_operands_labels
//                << ",\n\t"
//                << "result_labels = \n\t\t" << subscript._result_labels
//                << ",\n\t"
//                << "label_poss_in_operand = \n\t\t" << subscript._label_poss_in_operands
//                << ",\n\t"
//                << "label_pos_in_result = \n\t\t" << subscript._label_pos_in_result
//                << ",\n\t"
//                << "independent_label_subsets = \n\t\t" << subscript._independent_label_subsets
//                << "\n>";
//            return out;
//        }
    };
}

#endif //SPARSETENSOR_EINSUM_SUBSCRIPT_HPP

