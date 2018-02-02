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
        op_pos_t number_of_native_operands{};

    public:
        /**
         * Parses the subscript. Labels may or may not be optimized.
         * @param [in] raw_operand_subscripts vector of vectors of the operand's labels.
         * @param [in] raw_result_subscript vector of the result's.
         */
        Subscript(vector<vector<uint8_t>> &raw_operand_subscripts, vector<uint8_t> &raw_result_subscript) {
            init(raw_operand_subscripts, raw_result_subscript);
        }

    private:
        /**
         * Empty constructor. The fields are initialized but not filled with data.
         */
        Subscript() {}

        /**
         * Parses the subscript and writes the result to this' fields. Labels may or may not be optimized.
         * @param [in] raw_operand_subscripts vector of vectors of the operand's labels.
         * @param [in] raw_result_subscript vector of the result's.
         */
        void init(vector<vector<label_t>> &raw_operand_subscripts, vector<label_t> &raw_result_subscript);

    public:
        /**
         * This provides brackets out all independently computable parts and resulting in a cross product of the bracketed parts.
         * @return this instance
         */
        Subscript optimize();

        /**
         * A set of all labels used in this subscript. Labels from bracketed expressions are not included.
         * @return unordered_set of labels.
         */
        const unordered_set<label_t> &getAllLabels() const {
            return all_labels;
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
         * Number of used native operands (not operands that were created by bracketing out independent label subsets).
         * @return number of used native operands.
         */
        op_pos_t getNumberOfNativeOperands() const {
            return number_of_native_operands;
        }

    private:
        /**
         * This function normalizes operand and result labels. Normalized means that the labels are in order of
            first appeareance named 0,1,...
         * @param raw_operand_subscripts vector of vectors of the operand's labels
         * @param raw_result_subscript vector of the result's labels
         * @return tuple of normalized operand labels, normalized result labels and the total number of different labels
         */
        static tuple<vector<vector<label_t >>, vector<label_t>, label_t>
        normalizeRawSubscripts(vector<vector<label_t >> &raw_operand_subscripts, vector<label_t> &raw_result_subscript);

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
         * Calculates the label_poss_in_operand field using:
         * - operands_labels
         * - distinct_operands_labels
         * @param sc Subscript to be updated.
         */
        static void calcLabelPossInOperand(Subscript &sc);

        /**
         * Calculates the label_pos_in_result field using:
         * - result_labels
         * @param sc Subscript to be updated.
         */
        static void calcLabelPosInResult(Subscript &sc);

        /**
         * Calculates the operands_with_label field using:
         * - all_labels
         * - distinct_operands_labels
         * @param sc Subscript to be updated.
         */
        static void calcOperandsWithLabel(Subscript &sc);

        /**
         * Calculates the label_dependency_graph field using:
         * - distinct_operands_labels
         * @param sc Subscript to be updated.
         */
        static void calcLabelDependencyGraph(Subscript &sc);

        /**
         * Calculates the independent_label_subsets field using:
         * - label_dependency_graph
         * @param sc Subscript to be updated.
         */
        static void calcIndependentLabelSubsets(Subscript &sc);
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
    Subscript::normalizeRawSubscripts(vector<vector<label_t>> &raw_operand_subscripts,
                                      vector<label_t> &raw_result_subscript) {
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


    void Subscript::init
            (vector<vector<label_t>> &raw_operand_subscripts,
             vector<label_t> &raw_result_subscript) {

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
        // distinct_operands_labels
        for (op_pos_t op_pos = 0; op_pos < operand_subscripts.size(); ++op_pos) {
            vector<label_t> &reference = operand_subscripts[op_pos];
            operands_labels[op_pos] = reference;
            set<label_t> distict_labels(reference.begin(), reference.end());
            distinct_operands_labels[op_pos] = distict_labels;
        }

        // next_operand_pos
        next_operand_pos = operands_labels.size();
        // number_of_native_operands
        number_of_native_operands = operands_labels.size();

        // result_labels
        result_labels = result_subscript;

        // label_poss_in_operand
        calcLabelPossInOperand(*this);

        // label_pos_in_result
        calcLabelPosInResult(*this);

        // operands_with_label
        calcOperandsWithLabel(*this);

        // label_dependency_graph
        calcLabelDependencyGraph(*this);

        // independent_label_subsets
        calcIndependentLabelSubsets(*this);


    }

    Subscript Subscript::optimize() {
        // if (size((*this).independent_label_subsets) > 1)
        for (const unordered_set<label_t> &label_subset : (*this).independent_label_subsets) {

            Subscript sub_sc{};

            /// all_labels
            sub_sc.all_labels = unordered_set<label_t>{label_subset};

            /// operands_labels
            /// distinct_operands_labels
            for (op_pos_t op_pos = 0; op_pos < (*this).operands_labels.size(); ++op_pos) { // iterate all operands
                vector<label_t> &operand = (*this).operands_labels[op_pos];

                // write out all labels from label_subset
                vector<label_t> sub_operand_labels{};
                set<label_t> sub_distinct_operand_labels{};
                for (auto label_ = begin(operand); label_ != end(operand);) {
                    if (label_subset.count(*label_)) {
                        sub_operand_labels.push_back(*label_);
                        sub_distinct_operand_labels.insert(*label_);
                        // and delete them from original subscript
                        label_ = operand.erase(label_);
                    } else {
                        ++label_;
                    }
                }
                // if there were labels add them to operands_labels
                if (not sub_distinct_operand_labels.empty()) {
                    sub_sc.operands_labels[op_pos] = sub_operand_labels;
                    sub_sc.distinct_operands_labels[op_pos] = sub_distinct_operand_labels;
                }
            }
            // remove operands that got empty from operands_labels
            for (auto it = begin((*this).operands_labels); it != end((*this).operands_labels);) {
                if (it->second.empty()) {
                    it = (*this).operands_labels.erase(it);
                } else {
                    ++it;
                }
            }
            // remove operands that got empty from distinct_operands_labels
            for (auto it = begin((*this).distinct_operands_labels); it != end((*this).distinct_operands_labels);) {
                if (it->second.empty()) {
                    it = (*this).distinct_operands_labels.erase(it);
                } else {
                    ++it;
                }
            }

            /// number_of_native_operands
            sub_sc.number_of_native_operands = sub_sc.operands_labels.size();

            /// sub_sc result_labels
            for (label_t label : (*this).result_labels) {
                if (label_subset.count(label)) {
                    sub_sc.result_labels.push_back(label);
                }
            }

            /// add new operand to sc
            (*this).operands_labels[(*this).next_operand_pos] = sub_sc.result_labels;
            (*this).distinct_operands_labels[(*this).next_operand_pos] = set<label_t>(begin(sub_sc.result_labels),
                                                                                      end(sub_sc.result_labels));


            /// next_operand_pos
            (*this).next_operand_pos++;
            sub_sc.next_operand_pos = (*this).next_operand_pos;


            /// label_poss_in_operand
            calcLabelPossInOperand(*this);
            calcLabelPossInOperand(sub_sc);

            /// label_pos_in_result
            calcLabelPosInResult(sub_sc);

            /// operands_with_label
            calcOperandsWithLabel(*this);
            calcOperandsWithLabel(sub_sc);

            /// label_dependency_graph
            calcLabelDependencyGraph(*this);
            calcLabelDependencyGraph(sub_sc);

            /// independent_label_subsets
            calcIndependentLabelSubsets(*this);
            calcIndependentLabelSubsets(sub_sc);

            (*this).sub_subscripts.insert_or_assign((*this).next_operand_pos - 1, sub_sc);
        }
        return (*this);
    }

    void Subscript::calcLabelPossInOperand(Subscript &sc) {
        sc.label_poss_in_operand.clear();

        for (op_pos_t op_id = 0; op_id < sc.operands_labels.size(); ++op_id) {
            const vector<label_t> &operand_labels = sc.operands_labels[op_id];
            for (label_t label : sc.distinct_operands_labels[op_id]) {
                vector<label_pos_t> label_poss{};
                for (label_pos_t label_pos = 0; label_pos < operand_labels.size(); ++label_pos) {
                    if (operand_labels[label_pos] == label) {
                        label_poss.push_back(label_pos);
                    }
                }
                sc.label_poss_in_operand[{op_id, label}] = label_poss;
            }
        }
    }


    void Subscript::calcOperandsWithLabel(Subscript &sc) {
        sc.operands_with_label.clear();
        for (label_t label : sc.all_labels) {
            vector<op_pos_t> operand_poss{};
            for (op_pos_t op_id = 0; op_id < size(sc.distinct_operands_labels); ++op_id) {
                if (sc.distinct_operands_labels[op_id].count(label)) {
                    operand_poss.push_back(op_id);
                }
            }
            sc.operands_with_label[label] = operand_poss;
        }
    }


    void Subscript::calcLabelPosInResult(Subscript &sc) {
        for (label_pos_t label_pos = 0; label_pos < sc.result_labels.size(); ++label_pos) {
            label_t label = sc.result_labels[label_pos];
            sc.label_pos_in_result[label] = label_pos;
        }
    }


    void Subscript::calcLabelDependencyGraph(Subscript &sc) {
        sc.label_dependency_graph.clear();
        for (const auto &labels :sc.distinct_operands_labels) {
            sc.label_dependency_graph.addCompleteGraph(labels.second);
        }
    }


    void Subscript::calcIndependentLabelSubsets(Subscript &sc) {
        sc.independent_label_subsets.clear();
        sc.independent_label_subsets = sc.label_dependency_graph.getConnectedComponents();
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
