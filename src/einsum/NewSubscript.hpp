#ifndef SPARSETENSOR_EINSUM_SUBSCRIPT_HPP
#define SPARSETENSOR_EINSUM_SUBSCRIPT_HPP


#include <vector>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <parallel/numeric>
#include <parallel/algorithm>
#include "Types.hpp"
#include "../util/All.hpp"
#include "util/UndirectedGraph.hpp"


// std::ostream &operator<<(std::ostream &out, sparsetensor::operations::Subscript &subscript);

namespace sparsetensor::operations {

    using ::sparsetensor::operations::util::UndirectedGraph;

    /**
     * Representation of the subscript of a expression in einstein summation convention.
     * This provides also  brackets out all independently computable parts and resulting in a
     * cross product of the bracketed parts.
     */
    class NewSubscript {


        std::unordered_set<label_t> _all_labels;
        std::vector<std::vector<label_t>> _operands_labels;
        std::vector<label_t> _result_labels;
        std::vector<op_pos_t> _original_op_poss;
        std::vector<NewSubscript *> _sub_subscripts;
        label_t _max_label;
        std::vector<std::set<label_t>> _distinct_operands_labels;
        std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> _label_poss_in_operands;
        std::map<label_t, label_pos_t> _label_pos_in_result;
        std::map<label_t, std::set<op_pos_t>> _operands_with_label;
        std::set<label_t> _unique_non_result_labels;
        std::vector<op_pos_t> _operands_with_unique_non_result_labels;
        std::vector<op_pos_t> _operands_without_unique_non_result_labels;
        UndirectedGraph<label_t> _label_dependency_graph;
        std::vector<std::set<label_t>> _independent_label_subsets;
        std::map<op_pos_t, vector<vector<label_pos_t>>> _unique_non_result_contractions;

    public:

        NewSubscript(std::vector<raw_subscript> operands, raw_subscript result) {
            std::tie(_operands_labels, _result_labels, _all_labels) = normalizeRawSubscripts(operands, result);

            // fill original pos with 0 ... (_operands_labels.size() - 1)
            _original_op_poss = std::vector<op_pos_t>{};
            _original_op_poss.reserve(numberOfOperands());
            std::iota(_original_op_poss.begin(), _original_op_poss.end(), 0);

//            self._exec_plan = None
//            self._remove_labels_cache: Dict[label_t, 'Subscript'] = dict()

            updateFields();
        }

        static std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>>
        _calc_label_poss_in_operands(std::vector<std::vector<label_t>> operands_labels) {
            std::map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operands{};

            for (const auto &[op_id, labels] : enumerate(operands_labels))
                for (const auto &[label_pos, label] : enumerate(labels))
                    label_poss_in_operands[{op_id, label}].push_back(label_pos_t(label_pos));
            return label_poss_in_operands;
        }

        void updateFields() {
            _max_label = (_all_labels.size() > 0) ?
                         *std::max_element(_all_labels.begin(), _all_labels.end()) : NO_LABEL;

            _distinct_operands_labels = {};
            _distinct_operands_labels.reserve(numberOfOperands());
            std::for_each(_operands_labels.begin(), _operands_labels.end(),
                          [&](const std::vector<label_t> &operands) {
                              _distinct_operands_labels.emplace_back(
                                      std::set<label_t>(operands.begin(), operands.end()));
                          });

            _label_poss_in_operands = _calc_label_poss_in_operands(_operands_labels);

            _label_pos_in_result = {};
            for (const auto &[pos, label] : enumerate(_result_labels)) {
                _label_pos_in_result[pos] = label;
            }

            _operands_with_label = {};
            for (const auto &[op_pos, labels] : enumerate(_operands_labels))
                for (const label_t &label : labels)
                    _operands_with_label[label].insert(op_pos_t(op_pos));

            _unique_non_result_labels = _calcNonResultSingleOperandLabels(_operands_with_label, _result_labels);

            _operands_with_unique_non_result_labels = std::vector<op_pos_t>{};
            _operands_without_unique_non_result_labels = std::vector<op_pos_t>{};
            for (const auto &[op_pos, labels] : enumerate(_distinct_operands_labels)) {
                std::vector<label_t> intersect;
                std::set_intersection(labels.cbegin(), labels.cend(), _unique_non_result_labels.cbegin(),
                                      _unique_non_result_labels.cend(), std::back_inserter(intersect));
                if (intersect.size() > 0)
                    _operands_with_unique_non_result_labels.push_back(op_pos_t(op_pos));
                else
                    _operands_without_unique_non_result_labels.push_back(op_pos_t(op_pos));
            }

            _label_dependency_graph = calcLabelDependencyGraph(_distinct_operands_labels);

            _independent_label_subsets = _label_dependency_graph.getConnectedComponents();

            _unique_non_result_contractions =
                    calcUniqueNonResultContractions(_operands_with_unique_non_result_labels, _operands_labels,
                                                    _unique_non_result_labels, _label_poss_in_operands);


        }

        static std::map<op_pos_t, vector<vector<label_pos_t>>>
        calcUniqueNonResultContractions(std::vector<op_pos_t> operands_with_unique_non_result_labels,
                                        std::vector<std::vector<label_t>> operands_labels,
                                        std::set<label_t> unique_non_result_labels,
                                        std::map<std::tuple<op_pos_t, label_t>, std::vector<label_pos_t>> label_poss_in_operands) {

            std::map<op_pos_t, std::vector<std::vector<label_pos_t >>> unique_non_result_contractions;

            for (const op_pos_t &op_pos : operands_with_unique_non_result_labels) {

                std::vector<std::vector<label_pos_t>> contractions{};

                for (const label_t &label : std::set<label_t>(operands_labels[op_pos].cbegin(),
                                                              operands_labels[op_pos].cend()))
                    if (unique_non_result_labels.count(label)) {
                        const vector<label_pos_t> &contraction = label_poss_in_operands[std::make_tuple(op_pos, label)];
                        if (contraction.size())
                            contractions.emplace_back(contraction);
                    }

                if (contractions.size())
                    unique_non_result_contractions.emplace(op_pos, contractions);
            }

            return unique_non_result_contractions;
        }

        static std::set<label_t>
        _calcNonResultSingleOperandLabels(const std::map<label_t, std::set<op_pos_t>> &operands_with_label,
                                          const std::vector<label_t> &result_labels) {
            std::set<label_t> non_result_single_operand{};

            for (const auto &[label, ops_with_label] : operands_with_label)
                if ((ops_with_label.size() == 1) and
                    (not std::count(result_labels.cbegin(), result_labels.cend(), label)))
                    non_result_single_operand.insert(label);

            return non_result_single_operand;
        }

        UndirectedGraph<label_t> calcLabelDependencyGraph(const std::vector<set<label_t>> &distinct_operands_labels) {
            UndirectedGraph<label_t> label_dependency_graph{};
            for (const set<label_t> &labels:distinct_operands_labels) {
                label_dependency_graph.addCompleteGraph(labels);
            }
            return label_dependency_graph;
        }


        tuple<vector<vector<label_t >>, vector<label_t>, std::unordered_set<label_t>> normalizeRawSubscripts(
                const vector<vector<label_t>> &raw_operand_subscripts, const vector<label_t> &raw_result_subscript) {
            // map storing the mapping from old to new labels
            unordered_map<label_t, label_t> raw_to_norm_label{};
            // next new label to be used
            label_t next_norm_label = 0;

            // init the output for the operands
            vector<vector<label_t >> norm_operand_subscripts(raw_operand_subscripts.size());

            // normalize the operands
            op_pos_t op_pos = 0;
            for (const vector<label_t> &op_labels : raw_operand_subscripts) {
                vector<label_t> norm_operand_labels = normalizeLabelVector(raw_to_norm_label, next_norm_label,
                                                                           op_labels);
                norm_operand_subscripts[op_pos++] = norm_operand_labels;
            }

            // normalize the result
            vector<label_t> norm_result_subscript = normalizeLabelVector(raw_to_norm_label, next_norm_label,
                                                                         raw_result_subscript);

            // put all new labels that were used into a set
            std::unordered_set<label_t> labels{};
            for (label_t label = 0; label < next_norm_label; ++label)
                labels.emplace(label);

            return std::make_tuple(norm_operand_subscripts, norm_result_subscript, labels);
        }


        vector<label_t> normalizeLabelVector(
                unordered_map<label_t, label_t> &raw_to_norm_label, label_t &next_norm_label,
                const vector<label_t> &raw_labels) {
            vector<label_t> norm_operand_labels(raw_labels.size());

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

        inline size_t numberOfOperands() {
            return _operands_labels.size();
        }

        /**
         * Stores a mapping from operand position and label to the positions where that label is stored in this operand.
         * @return A map from (operand position, label) to a vector of label positions
         */
        inline const vector<label_pos_t> &labelPossInOperand(const op_pos_t &op_pos, const label_t &label) {
            return _label_poss_in_operands[std::make_tuple(op_pos, label)];
        }

        friend ::std::ostream &operator<<(::std::ostream &out, ::sparsetensor::operations::NewSubscript &subscript) {
            out << "<Subscript: \n"
                << "all_labels=" << subscript._all_labels
                << ",\n\t"
                << "operands_labels = \n\t\t" << subscript._operands_labels
                << ",\n\t"
                << "distinct_operands_labels = \n\t\t" << subscript._distinct_operands_labels
                << ",\n\t"
                << "result_labels = \n\t\t" << subscript._result_labels
                << ",\n\t"
                << "label_poss_in_operand = \n\t\t" << subscript._label_poss_in_operands
                << ",\n\t"
                << "label_pos_in_result = \n\t\t" << subscript._label_pos_in_result
                << ",\n\t"
                << "independent_label_subsets = \n\t\t" << subscript._independent_label_subsets
                << "\n>";
            return out;
        }
    };
}

//::std::ostream &operator<<(::std::ostream &out, ::sparsetensor::operations::NewSubscript &subscript) {
//    out << "<Subscript: \n"
//        << "all_labels=" << subscript._all_labels
//        << ",\n\t"
//        << "operands_labels = \n\t\t" << subscript._operands_labels
//        << ",\n\t"
//        << "distinct_operands_labels = \n\t\t" << subscript._distinct_operands_labels
//        << ",\n\t"
//        << "result_labels = \n\t\t" << subscript._result_labels
//        << ",\n\t"
//        << "label_poss_in_operand = \n\t\t" << subscript._label_poss_in_operands
//        << ",\n\t"
//        << "label_pos_in_result = \n\t\t" << subscript._label_pos_in_result
//        << ",\n\t"
//        << "independent_label_subsets = \n\t\t" << subscript._independent_label_subsets
//        << "\n>";
//    return out;
//}


#endif //SPARSETENSOR_EINSUM_SUBSCRIPT_HPP

