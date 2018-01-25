#ifndef LIBSPARSETENSOR_SUBSCRIPT_HPP
#define LIBSPARSETENSOR_SUBSCRIPT_HPP


#include <vector>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "util/UndirectedGraph.hpp"

using std::unordered_set;
using std::set;
using std::unordered_map;
using std::map;
using std::vector;
using std::tuple;

class Subscript;

std::ostream &operator<<(std::ostream &out, Subscript &subscript);

class Subscript {
    friend std::ostream &operator<<(std::ostream &out, Subscript &tensor);

public:
    typedef uint8_t label_t;
    typedef uint8_t op_pos_t;
    typedef uint8_t label_pos_t;
private:
    /**
     * This function normalizes operand and result labels. Normalized means that the labels are in order of
        first appeareance named 0,1,...
     * @param raw_operand_subscripts vector of vectors of the operand's labels
     * @param raw_result_subscript vector of the result's labels
     * @return tuple of normalized operand labels, normalized result labels and the total number of different labels
     */
    static tuple<vector<vector<label_t >>, vector<label_t>, uint8_t>
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

    void init(vector<vector<label_t>> &raw_operand_subscripts, vector<label_t> &raw_result_subscript);

public:

    Subscript(vector<vector<uint8_t>> &raw_operand_subscripts, vector<uint8_t> &raw_result_subscript) {
        init(raw_operand_subscripts, raw_result_subscript);
    }

private:
    Subscript() {}

    unordered_set<label_t> all_labels{};
    map<op_pos_t, vector<label_t>> operands_labels{};
    map<op_pos_t, set<label_t>> distinct_operands_labels{};
    vector<label_t> result_labels{};
    unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operand{};
    unordered_map<label_t, label_pos_t> label_pos_in_result{};
    unordered_map<label_t, set<op_pos_t>> operands_with_label{};
    UndirectedGraph<label_t> label_dependency_graph{};
    unordered_set<unordered_set<label_t>> independent_label_subsets{};
    map<op_pos_t, Subscript> sub_subscripts{};
    op_pos_t next_operand_pos{};
    label_pos_t next_label_pos{};
    op_pos_t number_of_native_operands{};


public:
    static Subscript
    optimized(vector<vector<uint8_t>> &raw_operand_subscripts, vector<uint8_t> &raw_result_subscript) {
        Subscript sc{raw_operand_subscripts, raw_result_subscript};
        // if (size(sc.independent_label_subsets) > 1)
        for (const unordered_set<label_t> &label_subset : sc.independent_label_subsets) {

            Subscript sub_sc{};

            /// all_labels
            sub_sc.all_labels = unordered_set<label_t> {label_subset};

            /// operands_labels
            /// distinct_operands_labels
            for (op_pos_t op_pos = 0; op_pos < sc.operands_labels.size(); ++op_pos) { // iterate all operands
                vector<label_t> &operand = sc.operands_labels[op_pos];

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
            for (auto it = begin(sc.operands_labels); it != end(sc.operands_labels);) {
                if (it->second.empty()) {
                    it = sc.operands_labels.erase(it);
                } else {
                    ++it;
                }
            }
            // remove operands that got empty from distinct_operands_labels
            for (auto it = begin(sc.distinct_operands_labels); it != end(sc.distinct_operands_labels);) {
                if (it->second.empty()) {
                    it = sc.distinct_operands_labels.erase(it);
                } else {
                    ++it;
                }
            }

            /// number_of_native_operands
            sub_sc.number_of_native_operands = sub_sc.operands_labels.size();

            /// sub_sc result_labels
            for (label_t label : sc.result_labels) {
                if (label_subset.count(label)) {
                    sub_sc.result_labels.push_back(label);
                }
            }

            /// add new operand to sc
            sc.operands_labels[sc.next_operand_pos] = sub_sc.result_labels;
            sc.distinct_operands_labels[sc.next_operand_pos] = set<label_t>(begin(sub_sc.result_labels),
                                                                            end(sub_sc.result_labels));


            /// next_operand_pos
            sc.next_operand_pos++;
            sub_sc.next_operand_pos = sc.next_operand_pos;


            /// label_poss_in_operand
            calc_label_poss_in_operand(sc);
            calc_label_poss_in_operand(sub_sc);

            /// label_pos_in_result
            calc_label_pos_in_result(sub_sc);

            /// operands_with_label
            calc_operands_with_label(sc);
            calc_operands_with_label(sub_sc);

            /// label_dependency_graph
            calc_label_dependency_graph(sc);
            calc_label_dependency_graph(sub_sc);

            /// independent_label_subsets
            calc_independent_label_subsets(sc);
            calc_independent_label_subsets(sub_sc);

            sc.sub_subscripts.insert_or_assign(sc.next_operand_pos - 1, sub_sc);
        }
        return sc;
    }

    /**
     * Getters
     *
     */

    const unordered_set<label_t> &getAll_labels() const {
        return all_labels;
    }

    const map<op_pos_t, vector<label_t>> &getOperands_labels() const {
        return operands_labels;
    }

    const map<op_pos_t, set<label_t>> &getDistinct_operands_labels() const {
        return distinct_operands_labels;
    }

    const vector<label_t> &getResult_labels() const {
        return result_labels;
    }

    const unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> &getLabel_poss_in_operand() const {
        return label_poss_in_operand;
    }

    const unordered_map<label_t, label_pos_t> &getLabel_pos_in_result() const {
        return label_pos_in_result;
    }

    const unordered_map<label_t, set<op_pos_t>> &getOperands_with_label() const {
        return operands_with_label;
    }

    const UndirectedGraph<label_t> &getLabel_dependency_graph() const {
        return label_dependency_graph;
    }

    const unordered_set<unordered_set<label_t>> &getIndependent_label_subsets() const {
        return independent_label_subsets;
    }

    const map<op_pos_t, Subscript> &getSub_subscripts() const {
        return sub_subscripts;
    }

    op_pos_t getNext_operand_pos() const {
        return next_operand_pos;
    }

    label_pos_t getNext_label_pos() const {
        return next_label_pos;
    }

    op_pos_t getNumber_of_native_operands() const {
        return number_of_native_operands;
    }

    /**
     * Calculates the label_poss_in_operand field using:
     * - operands_labels
     * - distinct_operands_labels
     * @param sc Subscript to be updated.
     */
    static void calc_label_poss_in_operand(Subscript &sc);

    static void calc_label_pos_in_result(Subscript &sc);

    static void calc_operands_with_label(Subscript &sc);

    static void calc_label_dependency_graph(Subscript &sc);

    static void calc_independent_label_subsets(Subscript &sc);

};

/**
 * Implementations start here
 */


vector<Subscript::label_t>
Subscript::normalizeLabelVector(unordered_map<Subscript::label_t, Subscript::label_t> &raw_to_norm_label,
                                Subscript::label_t &next_norm_label,
                                const vector<Subscript::label_t> &raw_labels) {
    vector<label_t> norm_operand_labels(raw_labels.size());

    label_pos_t label_pos = 0;
    for (label_t raw_label : raw_labels) {
        // check if raw label is already mapped to a normed label
        auto norm_label_ = raw_to_norm_label.find(raw_label);
        label_t norm_label;
        if (norm_label_ != raw_to_norm_label.end()) {
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

tuple<vector<vector<Subscript::label_t >>, vector<Subscript::label_t>, uint8_t>
Subscript::normalizeRawSubscripts(vector<vector<Subscript::label_t>> &raw_operand_subscripts,
                                  vector<Subscript::label_t> &raw_result_subscript) {
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
        (vector<vector<Subscript::label_t>> &raw_operand_subscripts, vector<Subscript::label_t> &raw_result_subscript) {

    vector<vector<label_t>> operand_subscripts;
    vector<label_t> result_subscript;
    // next_label_pos
    tie(operand_subscripts, result_subscript, next_label_pos) = normalizeRawSubscripts(raw_operand_subscripts,
                                                                                       raw_result_subscript);
    /// init fields

    // all_labels
    for (label_t label = 0; label < next_label_pos; ++label) {
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
    calc_label_poss_in_operand(*this);

    // label_pos_in_result
    calc_label_pos_in_result(*this);

    // operands_with_label
    calc_operands_with_label(*this);

    // label_dependency_graph
    calc_label_dependency_graph(*this);

    // independent_label_subsets
    calc_independent_label_subsets(*this);


}

void Subscript::calc_operands_with_label(Subscript &sc) {
    sc.operands_with_label.clear();
    for (label_t label : sc.all_labels) {
        set<op_pos_t> operand_poss{};
        for (op_pos_t op_id = 0; op_id < size(sc.distinct_operands_labels); ++op_id) {
            if (sc.distinct_operands_labels[op_id].count(label)) {
                operand_poss.insert(op_id);
            }
        }
        sc.operands_with_label[label] = operand_poss;
    }
}

void Subscript::calc_label_pos_in_result(Subscript &sc) {
    for (label_pos_t label_pos = 0; label_pos < sc.result_labels.size(); ++label_pos) {
        label_t label = sc.result_labels[label_pos];
        sc.label_pos_in_result[label] = label_pos;
    }
}

void Subscript::calc_label_poss_in_operand(Subscript &sc) {
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

std::ostream &operator<<(std::ostream &out, Subscript &subscript) {
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

void Subscript::calc_label_dependency_graph(Subscript &sc) {
    sc.label_dependency_graph.clear();
    for (const auto &labels :sc.distinct_operands_labels) {
        sc.label_dependency_graph.addCompleteGraph(labels.second);
    }
}

void Subscript::calc_independent_label_subsets(Subscript &sc) {
    sc.independent_label_subsets.clear();
    sc.independent_label_subsets = sc.label_dependency_graph.getConnectedComponents();
}

#endif //LIBSPARSETENSOR_SUBSCRIPT_HPP
