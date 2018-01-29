//
// Created by me on 1/26/18.
//

#ifndef LIBSPARSETENSOR_JOINITERATOR_HPP
#define LIBSPARSETENSOR_JOINITERATOR_HPP

#include "HyperTrie.hpp"
#include <iterator>
#include <tuple>
#include <variant>
#include <vector>
#include <einsum/Subscript.hpp>
#include <einsum/EvalPlan.hpp>
#include <optional>


using std::tuple;
using std::set;
using std::optional;

/**
 * This class represents all sub-HyperTries that arise from matching one or multiple dimensions of one or multiple
 * HyperTries together with their respective key part.
 */
template<typename T>
class HyperTrieJoinIterator : public std::iterator<std::input_iterator_tag,
        tuple<key_part_t, vector<variant<HyperTrie<T> *, T>>>> {
public:
    HyperTrieJoinIterator(const vector<HyperTrie<T> *> &hyperTries, PlanStep *planStep, label_t label)
            : hyperTries(
            hyperTries), planStep(planStep), label(label) {

        const set<op_pos_t> &op_poss = planStep->operandsWithLabel(label);
        const std::optional<label_pos_t> &key_pos = planStep->labelPosInResult(label);

        vector<HyperTrieMatchKeyPosIterator> iterators{};

        op_pos_t min_card_op_pos{};
        size_t min_card = ::std::numeric_limits<size_t>::max();

        for (op_pos_t op_pos : op_poss) {
            const HyperTrie<T> *&hyperTrie = hyperTries[op_pos];
            const vector<label_pos_t> &label_poss = planStep->labelPossInOperand(op_pos, label);
            HyperTrieMatchKeyPosIterator iter{hyperTrie, label_poss};
            if (iter.getSize() < min_card) {
                min_card_op_pos = op_pos;
            }
        }

        // ....

    }

private:
    vector<HyperTrie<T> *> hyperTries;
    PlanStep *planStep;
    label_t label;
    HyperTrieMatchKeyPosIterator *iter;
    key_part_t key_part{};
public:

    HyperTrieJoinIterator &operator++() {


        for


        return *this;
    }

    HyperTrieJoinIterator operator++(int) {
        HyperTrieJoinIterator it_copy{*this};
        operator++();
        return it_copy;
    }

    bool operator==(const HyperTrieJoinIterator &rhs) const {
        // TODO: does this make sense?
        return key_part == rhs.key_part;
    }

    bool operator!=(const HyperTrieJoinIterator &rhs) const {
        // TODO: does this make sense?
        return key_part != rhs.key_part;
    }


};


#endif //LIBSPARSETENSOR_JOINITERATOR_HPP
