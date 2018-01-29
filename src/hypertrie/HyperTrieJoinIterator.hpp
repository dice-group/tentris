//
// Created by me on 1/26/18.
//

#ifndef LIBSPARSETENSOR_JOINITERATOR_HPP
#define LIBSPARSETENSOR_JOINITERATOR_HPP

#include "HyperTrie.hpp"
#include "HyperTrieMatchKeyPosIterator.hpp"
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
using std::numeric_limits;
using std::iterator;
using std::input_iterator_tag;

/**
 * This class represents all sub-HyperTries that arise from matching one or multiple dimensions of one or multiple
 * HyperTries together with their respective key part.
 */
template<typename T>
class HyperTrieJoinIterator
        : public iterator<input_iterator_tag, tuple<key_part_t, vector<variant<HyperTrie<T> *, T>>>> {
public:
    HyperTrieJoinIterator(const vector<HyperTrie<T> *> &hyperTries, PlanStep *planStep, label_t label)
            : planStep(planStep),
              label(label) {

        // positions in operands
        const vector<op_pos_t> &op_poss = planStep->operandsWithLabel(label);
        // position in result
        const std::optional<label_pos_t> &result_pos = planStep->labelPosInResult(label);

        // collect data about operands
        op_pos_t min_card_op_pos = 0;
        size_t min_card = numeric_limits<size_t>::max();
        key_part_t min_key = numeric_limits<key_part_t>::min();
        key_part_t max_key = numeric_limits<key_part_t>::max();


        // views of operands where the relevant positions can be set to the same value

        for (op_pos_t op_pos : op_poss) {

            // gernerate view
            HyperTrieMatchKeyPosView view{hyperTries[op_pos], planStep->labelPossInOperand(op_pos, label)};
            hyper_trie_views[op_pos] = view;

            if (min_card > view.estimCard()) {
                min_card = view.estimCard();
                min_card_op_pos = op_pos;
            }
            min_key = (view.min() > min_key) ? view.min() : min_key;
            max_key = (view.max() < max_key) ? view.max() : max_key;
        }
        map<op_pos_t, HyperTrieMatchKeyPosView>::iterator min_card_op_ = hyper_trie_views.find(min_card_op_pos);

        it_begin = min_card_op_->second.lower_bound(min_key);
        it_end = min_card_op_->second.upper_bound(max_key);

        hyper_trie_views.erase(min_card_op_); // TODO: is it a problem that I erase it here?

    }

private:
    PlanStep *planStep;
    label_t label;

    key_part_t current_key_part{};

    map<op_pos_t, HyperTrieMatchKeyPosView> hyper_trie_views{};
    HyperTrieMatchKeyPosIterator it_begin;
    HyperTrieMatchKeyPosIterator it_end;

    // results for all operands
    vector<variant<HyperTrie<T> *, T>> *first;
    // results binding, maybe vector?
    label_t *second;

public:

    HyperTrieJoinIterator &operator++() {
        bool match = false;

        do {
            if (it_begin == it_end) {
                break;
            }

            current_key_part = it_begin->first;
            for (const auto &other_view_ :hyper_trie_views) {
                HyperTrieMatchKeyPosView other_view = other_view_->second;
                if (not other_view.find(current_key_part)) { // safe the results of those finds to a vector
                    goto no_match;
                }
            }
            break;

            no_match:
            it_begin++;
        } while (true);

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
