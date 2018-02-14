#ifndef SPARSETENSOR_EINSUM_SUBSCRIPT_HPP
#define SPARSETENSOR_EINSUM_SUBSCRIPT_HPP


#include "../util/All.hpp"
#include "util/UndirectedGraph.hpp"
#include "Types.hpp"
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <parallel/numeric>
#include <parallel/algorithm>
#include <iterator>

using std::unordered_set;
using std::set;
using std::unordered_map;
using std::map;
using std::vector;
using std::tuple;

using ::sparsetensor::einsum::util::UndirectedGraph;

namespace sparsetensor::einsum {
    class Subscript;
}

std::ostream &operator<<(std::ostream &out, sparsetensor::einsum::Subscript &subscript);

namespace sparsetensor::einsum {
    /**
     * Representation of the subscript of a expression in einstein summation convention.
     * This provides also Subscript::optimize() brackets out all independently computable parts and resulting in a
     * cross product of the bracketed parts.
     */
    class Subscript {
        friend ::std::ostream &::operator<<(::std::ostream &out, ::sparsetensor::einsum::Subscript &tensor);

    private:
        unordered_set<label_t> all_labels{};
        vector<op_pos_t> op_poss{};
        vector<op_pos_t> original_op_poss{};
        map<op_pos_t, vector<label_t>> operands_labels{};
        map<op_pos_t, set<label_t>> distinct_operands_labels{};
        vector<label_t> result_labels{};
        unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operand{};
        unordered_map<label_t, label_pos_t> label_pos_in_result{};
        unordered_map<label_t, vector<op_pos_t>> operands_with_label{};
        UndirectedGraph<label_t> label_dependency_graph{};
        unordered_set<unordered_set<label_t>> independent_label_subsets{};
        map<op_pos_t, Subscript> sub_subscripts{};
        op_pos_t next_operand_pos{};
        label_t next_label{};

    public:
        /**
         * Parses the subscript. Labels may or may not be optimized.
         * @param [in] raw_operand_subscripts vector of vectors of the operand's labels.
         * @param [in] raw_result_subscript vector of the result's.
         */
        Subscript(const vector<vector<uint8_t>> raw_operand_subscripts, const vector<uint8_t> raw_result_subscript) {
            init(raw_operand_subscripts, raw_result_subscript);
        }

        /**
         * Empty constructor. The fields are initialized but not filled with data.
         */
        Subscript() {}

        Subscript(const Subscript &subscript) = default;

    private:

        /**
         * Parses the subscript and writes the result to this' fields. Labels may or may not be optimized.
         * @param [in] raw_operand_subscripts vector of vectors of the operand's labels.
         * @param [in] raw_result_subscript vector of the result's.
         */
        void init(const vector<vector<label_t>> &raw_operand_subscripts, const vector<label_t> &raw_result_subscript);

    public:
        /**
         * This provides brackets out all independently computable parts and resulting in a cross product of the bracketed parts.
         * @return this instance
         */
        Subscript bracketCrossproductFactors();

        /**
         * A set of all labels used in this subscript. Labels from bracketed expressions are not included.
         * @return unordered_set of labels.
         */
        const unordered_set<label_t> &getAllLabels() const {
            return all_labels;
        }

        /**
         * A vector of all operands' positions.
         * @return vector of op_pos_t.
         */
        const vector<op_pos_t> &getOperandPoss() const {
            return op_poss;
        }

        /**
         * A vector of all original operands' positions.
         * @return vector of op_pos_t.
         */
        const vector<op_pos_t> &getOriginalOperandPoss() const {
            return original_op_poss;
        }

        /**
         * Stores the labels of the operands.
         * @return Map from operand position to a vector holding it's labels in order.
         */
        const map<op_pos_t, vector<label_t>> &getOperandsLabels() const {
            return operands_labels;
        }

        /**
         * Stores the disctinct labels of the operands.
         * @return Map from operand position to a set holding it's labels.
         */
        const map<op_pos_t, set<label_t>> &getDistinctOperandsLabels() const {
            return distinct_operands_labels;
        }

        /**
         * The result labels.
         * @return A vector holding the result labels in order.
         */
        const vector<label_t> &getResultLabels() const {
            return result_labels;
        }

        /**
         * Stores a mapping from operand position and label to the positions where that label is stored in this operand.
         * @return A map from (operand position, label) to a vector of label positions
         */
        const unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> &getLabelPossInOperand() const {
            return label_poss_in_operand;
        }

        /**
         * Stores a mapping from label to its position in the result.
         * @return A map from a label to a label positions
         */
        const unordered_map<label_t, label_pos_t> &getLabelPosInResult() const {
            return label_pos_in_result;
        }

        /**
         * Stores which label is in what operands.
         * @return A map from a label to a set of operand positions.
         */
        const unordered_map<label_t, vector<op_pos_t>> &getOperandsWithLabel() const {
            return operands_with_label;
        }

        /**
         * A label dependency graph is defined like:
         * - It has a node for every label.
         * - The "dependency" of the labels are represented by the edges. A edge is between two nodes if the corresponding labels occur together in at least one operand subscript.
         * The reason why this is interesting is that unconnected sub-graphs (independent node sets) are sets of labels that can be evaluated independent of the others. The full result can later be calculated by a simple cross-product.
         * @return
         */
        const UndirectedGraph<label_t> &getLabelDependencyGraph() const {
            return label_dependency_graph;
        }

        /**
         * These sets of labels that are independently of each other computable.
         * @return sets of mutually independent label set
         */
        const unordered_set<unordered_set<label_t>> &getIndependentLabelSubsets() const {
            return independent_label_subsets;
        }

        /**
         * A map holding all Subscript s that were bracketed out by Subscript::optimize() .
         * @return A map from operand position to sub Subscript.
         */
        const map<op_pos_t, Subscript> &getSubSubscripts() const {
            return sub_subscripts;
        }

        /**
         * Next free position where a new operand could be.
         * Note: adding additional operands is currently not directly supported and happens only internally when Subscript::optimize() is called.
         * @return next free operand position.
         */
        op_pos_t getNextOperandPos() const {
            return next_operand_pos;
        }

        /**
         * Next unused label.
         * Note: Changing or adding labels is currently not supported.
         * @return next unsed Label or total number of  used labels.
         */
        label_t getNextLabel() const {
            return next_label;
        }

        /**
         * Calculates the label_poss_in_operand field using:
         * - operands_labels
         * - distinct_operands_labels
         * @param sc Subscript to be updated.
         */
        static unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>>
        calcLabelPossInOperand(const map<op_pos_t, vector<label_t>> &operands_labels);

        /**
         * Calculates the label_pos_in_result field using:
         * - result_labels
         * @param sc Subscript to be updated.
         */
        static unordered_map<label_t, label_pos_t> calcLabelPosInResult(const vector<label_t> &result_labels);

        /**
         * Calculates the operands_with_label field using:
         * - all_labels
         * - distinct_operands_labels
         * @param sc Subscript to be updated.
         */
        static unordered_map<label_t, vector<op_pos_t>> calcOperandsWithLabel(const unordered_set<label_t> &all_labels,
                                                                              const map<op_pos_t, vector<label_t>> &operands_labels);

    private:
        /**
         * This function normalizes operand and result labels. Normalized means that the labels are in order of
            first appeareance named 0,1,...
         * @param raw_operand_subscripts vector of vectors of the operand's labels
         * @param raw_result_subscript vector of the result's labels
         * @return tuple of normalized operand labels, normalized result labels and the total number of different labels
         */
        static tuple<vector<vector<label_t >>, vector<label_t>, label_t>
        normalizeRawSubscripts(const vector<vector<label_t >> &raw_operand_subscripts,
                               const vector<label_t> &raw_result_subscript);

        /**
         * norms a vector of label_t
         * @param raw_to_norm_label maps raw label_t to normed label_t, changes are written back
         * @param next_norm_label  the next normed label_t to be used, changes are written back
         * @param raw_labels the vector of raw labels to be normed
         * @return a vector of normed label_t
         */
        static vector<label_t>
        normalizeLabelVector(unordered_map<label_t, label_t> &raw_to_norm_label, label_t &next_norm_label,
                             const vector<label_t> &raw_labels);


        /**
         * Calculates the label_dependency_graph field using:
         * - distinct_operands_labels
         * @param sc Subscript to be updated.
         */
        static UndirectedGraph<label_t>
        calcLabelDependencyGraph(const map<op_pos_t, set<label_t>> &distinct_operands_labels);

        /**
         * Calculates the independent_label_subsets field using:
         * - label_dependency_graph
         * @param sc Subscript to be updated.
         */
        static unordered_set<unordered_set<label_t>>
        calcIndependentLabelSubsets(const UndirectedGraph<label_t> &independent_label_subsets);

        static map<op_pos_t, set<label_t>>
        calcDistinctOperandsLabels(const map<op_pos_t, vector<label_t>> &operands_labels);

        static vector<op_pos_t> calcOperandsPoss(const map<op_pos_t, vector<label_t>> &operands_labels);

        void calcDependentFields() {
            /// calc fields
            this->distinct_operands_labels = calcDistinctOperandsLabels(this->operands_labels);

            this->label_poss_in_operand = calcLabelPossInOperand(this->operands_labels);

            this->label_pos_in_result = calcLabelPosInResult(this->result_labels);

            this->operands_with_label = calcOperandsWithLabel(this->all_labels, this->operands_labels);

            this->op_poss = calcOperandsPoss(this->operands_labels);

            this->label_dependency_graph = calcLabelDependencyGraph(this->distinct_operands_labels);

            this->independent_label_subsets = calcIndependentLabelSubsets(this->label_dependency_graph);
        }
    };

/*
 * Implementations start here
 */


    vector<label_t>
    Subscript::normalizeLabelVector(unordered_map<label_t, label_t> &raw_to_norm_label,
                                    label_t &next_norm_label,
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

    tuple<vector<vector<label_t >>, vector<label_t>, label_t>
    Subscript::normalizeRawSubscripts(const vector<vector<label_t>> &raw_operand_subscripts,
                                      const vector<label_t> &raw_result_subscript) {
        unordered_map<label_t, label_t> raw_to_norm_label{};
        label_t next_norm_label = 0;

        vector<vector<label_t >> norm_operand_subscripts(raw_operand_subscripts.size());

        op_pos_t op_pos = 0;
        for (const vector<label_t> &op_labels : raw_operand_subscripts) {
            vector<label_t> norm_operand_labels = normalizeLabelVector(raw_to_norm_label, next_norm_label, op_labels);
            norm_operand_subscripts[op_pos++] = norm_operand_labels;
        }

        vector<label_t> norm_result_subscript = normalizeLabelVector(raw_to_norm_label, next_norm_label,
                                                                     raw_result_subscript);


        return {norm_operand_subscripts, norm_result_subscript, next_norm_label};
    }


    void Subscript::init(const vector<vector<label_t>> &raw_operand_subscripts,
                         const vector<label_t> &raw_result_subscript) {

        vector<vector<label_t>> operand_subscripts;
        vector<label_t> result_subscript;
        // next_label_pos
        tie(operand_subscripts, result_subscript, next_label) = normalizeRawSubscripts(raw_operand_subscripts,
                                                                                       raw_result_subscript);
        /// init fields

        // all_labels
        for (label_t label = 0; label < next_label; ++label) {
            all_labels.insert(label);
        }

        // operands_labels
        for (op_pos_t op_pos = 0; op_pos < operand_subscripts.size(); ++op_pos) {
            vector<label_t> &labels = operand_subscripts[op_pos];
            operands_labels[op_pos] = labels;
        }


        this->distinct_operands_labels = calcDistinctOperandsLabels(this->operands_labels);

        // next_operand_pos
        next_operand_pos = operands_labels.size();

        // result_labels
        result_labels = result_subscript;

        // label_poss_in_operand
        this->label_poss_in_operand = calcLabelPossInOperand(this->operands_labels);

        // label_pos_in_result
        this->label_pos_in_result = calcLabelPosInResult(this->result_labels);

        // operands_with_label
        this->operands_with_label = calcOperandsWithLabel(this->all_labels, this->operands_labels);

        this->op_poss = calcOperandsPoss(this->operands_labels);

        // label_dependency_graph
        this->label_dependency_graph = calcLabelDependencyGraph(this->distinct_operands_labels);

        // independent_label_subsets
        this->independent_label_subsets = calcIndependentLabelSubsets(this->label_dependency_graph);

        for (auto && [op_pos, labels] :this->operands_labels) {
            op_poss.push_back(op_pos);
        }
    }

    Subscript Subscript::bracketCrossproductFactors() {
        op_pos_t sub_sc_pos = 0;
        for (const unordered_set<label_t> &label_subset : this->independent_label_subsets) {

            Subscript sub_sc{};

            /// init all_labels
            sub_sc.all_labels = unordered_set<label_t>{label_subset};

            /// init operands_labels
            /// and original op_poss
            for (auto & [parent_op_pos, parent_labels] : this->operands_labels) { // iterate all operands

                // write out all labels from label_subset
                vector<label_t> operand_labels{};
                for (auto label_ = begin(parent_labels); label_ != end(parent_labels);) {
                    if (label_subset.count(*label_)) {
                        operand_labels.push_back(*label_);
                        // and delete them from original subscript
                        label_ = parent_labels.erase(label_);
                    } else {
                        ++label_;
                    }
                }
                // if there were labels add them to operands_labels
                if (not operand_labels.empty()) {
                    sub_sc.operands_labels[sub_sc.next_operand_pos] = operand_labels;
                    sub_sc.original_op_poss.push_back(parent_op_pos);
                    sub_sc.next_operand_pos++;
                }
            }

            /// init result_labels
            for (const label_t &label : this->result_labels) {
                if (label_subset.count(label)) {
                    sub_sc.result_labels.push_back(label);
                }
            }

            sub_sc.calcDependentFields();

            this->sub_subscripts.insert_or_assign(sub_sc_pos++, sub_sc);

            // remove operands that got empty from operands_labels
            for (auto it = begin(this->operands_labels); it != end(this->operands_labels);) {
                if (it->second.empty()) {
                    it = this->operands_labels.erase(it);
                } else {
                    ++it;
                }
            }
        }

        /// add new operands to sc
        for(auto &[drop, sub_sub_sc]: this->sub_subscripts){
            this->operands_labels[this->next_operand_pos++] = sub_sub_sc.result_labels;
        }

        this->all_labels = unordered_set<label_t >(this->result_labels.begin(), this->result_labels.end());

        vector<op_pos_t> old_op_pos{};
        for (const auto &[op_pos, labels]:operands_labels) {
            old_op_pos.push_back(op_pos);
        }
        this->next_operand_pos = 0;
        for (const op_pos_t old_op_pos: old_op_pos) {
            {
                auto node_handle = operands_labels.extract(old_op_pos);
                node_handle.key() = this->next_operand_pos;
                operands_labels.insert(std::move(node_handle));
            }
            ++(this->next_operand_pos);
        }

        /// calc fields
        calcDependentFields();
        return (*this);
    }

    unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>>
    Subscript::calcLabelPossInOperand(const map<op_pos_t, vector<label_t>> &operands_labels) {

        unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operands{};

        for (const auto &
            [op_id, labels] : operands_labels) {
            for (size_t label_pos = 0; label_pos < labels.size(); ++label_pos) {
                const label_t label = labels[label_pos];

                label_poss_in_operands[{op_id, label}].push_back(label_pos);
            }
        }
        return label_poss_in_operands;
    }


    unordered_map<label_t, vector<op_pos_t>> Subscript::calcOperandsWithLabel(
            const unordered_set<label_t> &all_labels,
            const map<op_pos_t, vector<label_t>> &operands_labels) {
        unordered_map<label_t, vector<op_pos_t>> operands_with_label{};

        for (const auto &
            [op_id, op_labels] : operands_labels) {
            for (label_t label : all_labels) {

                // if op labels contains current label
                if (std::find(op_labels.cbegin(), op_labels.cend(), label) != op_labels.cend()) {

                    vector<op_pos_t> &operands = operands_with_label[label];
                    operands.push_back(op_id);
                }
            }
        }
        return operands_with_label;
    }

    unordered_map<label_t, label_pos_t> Subscript::calcLabelPosInResult(const vector<label_t> &result_labels) {
        unordered_map<label_t, label_pos_t> label_pos_in_result{};
        for (label_pos_t label_pos = 0; label_pos < result_labels.size(); ++label_pos) {
            label_t label = result_labels.at(label_pos);
            label_pos_in_result[label] = label_pos;
        }
        return label_pos_in_result;
    }


    UndirectedGraph<label_t>
    Subscript::calcLabelDependencyGraph(const map<op_pos_t, set<label_t>> &distinct_operands_labels) {
        UndirectedGraph<label_t> label_dependency_graph{};
        for (const auto &
            [op_id, labels] :distinct_operands_labels) {
            label_dependency_graph.addCompleteGraph(labels);
        }
        return label_dependency_graph;
    }


    unordered_set<unordered_set<label_t>>
    Subscript::calcIndependentLabelSubsets(const UndirectedGraph<label_t> &label_dependency_graph) {
        return label_dependency_graph.getConnectedComponents();
    }

    map<op_pos_t, set<label_t>>
    Subscript::calcDistinctOperandsLabels(const map<op_pos_t, vector<label_t>> &operands_labels) {
        // operands_labels
        // distinct_operands_labels
        map<op_pos_t, set<label_t>> distinct_operands_labels{};
        for (const auto &
            [op_pos, op_labels]:operands_labels) {
            set<label_t> distict_labels(op_labels.begin(), op_labels.end());
            distinct_operands_labels[op_pos] = distict_labels;
        }
        return distinct_operands_labels;
    }

    vector<op_pos_t> Subscript::calcOperandsPoss(const map<op_pos_t, vector<label_t>> &operands_labels) {
        vector<op_pos_t> op_poss{};
        for (op_pos_t op_pos = 0; op_pos < operands_labels.size(); ++op_pos)
            op_poss.push_back(op_pos);
        return op_poss;
    }
}

std::ostream &operator<<(std::ostream &out, sparsetensor::einsum::Subscript &subscript) {
    out << "<Subscript: \n"
        << "all_labels=" << subscript.all_labels
        << ",\n\t"
        << "operands_labels = \n\t\t" << subscript.operands_labels
        << ",\n\t"
        << "distinct_operands_labels = \n\t\t" << subscript.distinct_operands_labels
        << ",\n\t"
        << "result_labels = \n\t\t" << subscript.result_labels
        << ",\n\t"
        << "label_poss_in_operand = \n\t\t" << subscript.label_poss_in_operand
        << ",\n\t"
        << "label_pos_in_result = \n\t\t" << subscript.label_pos_in_result
        << ",\n\t"
        << "operands_labels = \n\t\t" << subscript.operands_labels
        << ",\n\t"
        << "independent_label_subsets = \n\t\t" << subscript.independent_label_subsets
        << "\n>";
    return out;
}

#endif //SPARSETENSOR_EINSUM_SUBSCRIPT_HPP

