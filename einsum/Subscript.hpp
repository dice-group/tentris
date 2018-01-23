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

class Subscript {
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
                         const vector<label_t> &raw_labels) const;

    void init(vector<vector<label_t>> &raw_operand_subscripts, vector<label_t> &raw_result_subscript);

public:
    Subscript(vector<vector<uint8_t>> &raw_operand_subscripts, vector<uint8_t> &raw_result_subscript) {
        init(raw_operand_subscripts, raw_result_subscript);
    }

private:

    unordered_set<label_t> all_labels{};
    map<op_pos_t, vector<label_t>> operands_labels{};
    map<op_pos_t, set<label_t>> distinct_operands_labels{};
    vector<label_t> result_labels{};
    unordered_map<tuple<op_pos_t, label_t>, vector<label_pos_t>> label_poss_in_operand{};
    unordered_map<label_t, label_pos_t> label_pos_in_result{};
    unordered_map<label_t, set<op_pos_t>> operands_with_label{};
    UndirectedGraph<label_t> label_dependency_graph{};
    unordered_set<unordered_set<label_t>> independent_label_subsets{};


public:
    static Subscript
    optimized(vector<vector<uint8_t>> &raw_operand_subscripts, vector<uint8_t> &raw_result_subscript) {
        return {raw_operand_subscripts, raw_result_subscript};
    }
};


static vector<Subscript::label_t>
Subscript::normalizeLabelVector(unordered_map<Subscript::label_t, Subscript::label_t> &raw_to_norm_label,
                                Subscript::label_t &next_norm_label,
                                const vector<Subscript::label_t> &raw_labels) const {
    vector<label_t> norm_operand_labels(raw_labels.size());

    label_pos_t label_pos = 0;
    for (label_t raw_label : raw_labels) {
        // check if raw label is already mapped to a normed label
        auto norm_label_ = raw_to_norm_label.find(raw_label);
        label_t norm_label;
        if (norm_label_ == raw_to_norm_label.end()) {
            norm_label = norm_label_->second;
        } else {
            // otherwise map it
            norm_label = next_norm_label++;
            raw_to_norm_label[raw_label] = norm_label;
        }

        norm_operand_labels[label_pos] = norm_label;
    }
    return norm_operand_labels;
}

static tuple<vector<vector<Subscript::label_t >>, vector<Subscript::label_t>, uint8_t>
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
    uint8_t number_of_labels;
    tie(operand_subscripts, raw_result_subscript, number_of_labels) = normalizeRawSubscripts(raw_operand_subscripts,
                                                                                             raw_result_subscript);

    /// init fields

    // all_labels
    for (label_t label = 0; label < number_of_labels; ++label) {
        all_labels.insert(label);
    }

    // operands_labels
    // distinct_operands_labels
    for (op_pos_t op_pos = 0; op_pos < operand_subscripts.size(); ++op_pos) {
        operands_labels[op_pos] = operand_subscripts[op_pos];
        set<label_t> distict_labels(operand_subscripts.begin(), operand_subscripts.end());
        distinct_operands_labels[op_pos] = distict_labels;
    }

    // result_labels
    result_labels = result_subscript;

    // label_poss_in_operand
    for (op_pos_t op_id = 0; op_id < operands_labels.size(); ++op_id) {
        const vector<label_t> &operand_labels = operands_labels[op_id];
        for (label_t label : distinct_operands_labels[op_id]) {
            vector<label_pos_t> label_poss{};
            for (label_pos_t label_pos = 0; label_pos < operand_labels.size(); ++label_pos) {
                if (operand_labels[label_pos] == label) {
                    label_poss.push_back(label_pos);
                }
            }
            label_poss_in_operand[{op_id, label}] = label_poss;
        }
    }

    // label_pos_in_result
    for (label_pos_t label_pos = 0; label_pos < result_labels.size(); ++label_pos) {
        label_t label = result_labels[label_pos];
        label_pos_in_result[label] = label_pos;
    }

    // operands_with_label
    for (label_t label : all_labels) {
        set<op_pos_t> operand_poss{};
        for (op_pos_t op_id = 0; op_id < size(distinct_operands_labels); ++op_id) {
            if (distinct_operands_labels[op_id].count(label)) {
                operand_poss.insert(op_id);
            }
        }
        operands_with_label[label] = operand_poss;
    }

    // label_dependency_graph
    for (const auto &labels :distinct_operands_labels) {
        label_dependency_graph.addFullGraph(labels.second);
    }

    // independent_label_subsets
    independent_label_subsets = label_dependency_graph.getConnectedComponents();

}


#endif //LIBSPARSETENSOR_SUBSCRIPT_HPP
