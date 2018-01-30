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


template<typename T>
class HyperTrieJoin {
public:
    HyperTrieJoin(const vector<variant<HyperTrie<T> *, T>> &operands, PlanStep *planStep, label_t label,
                  const vector<uint64_t> &result_key) {
        // positions in operands
        const vector<op_pos_t> &op_poss = planStep->operandsWithLabel(label);

        // collect data about operands
        op_pos_t min_card_op_pos = 0;
        size_t min_card = numeric_limits<size_t>::max();
        key_part_t min_key = numeric_limits<key_part_t>::min();
        key_part_t max_key = numeric_limits<key_part_t>::max();

        map<op_pos_t, HyperTrieMatchKeyPosView> hyper_trie_views{};

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
        hyper_trie_views.erase(min_card_op_);

        HyperTrieMatchKeyPosView min_card_op{hyperTries[min_card_op_pos],
                                             planStep->labelPossInOperand(min_card_op_pos, label)};

        HyperTrieMatchKeyPosIterator it_begin = min_card_op.lower_bound(min_key);
        HyperTrieMatchKeyPosIterator it_end = min_card_op.second.upper_bound(max_key);


        // position in result
        const std::optional<label_pos_t> &result_pos_ = planStep->labelPosInResult(label);
        bool has_result_pos = false;
        label_pos_t result_pos{};
        if (result_pos_) {
            has_result_pos = true;
            result_pos = *result_pos_;
        }

        hyperTrieJoinIterator = new HyperTrieJoinIterator(hyper_trie_views, it_begin, it_end, has_result_pos,
                                                          result_pos, operands, result_key);
    }

    ~HyperTrieJoin() {
        delete hyperTrieJoinIterator;
    }

    HyperTrieJoinIterator *hyperTrieJoinIterator;


    HyperTrieJoinIterator &begin() {
        return *hyperTrieJoinIterator;
    }

    HyperTrieJoinIterator &end();
};


/**
 * This class represents all sub-HyperTries that arise from matching one or multiple dimensions of one or multiple
 * HyperTries together with their respective key part.
 */
template<typename T>
class HyperTrieJoinIterator
        : public iterator<input_iterator_tag, tuple<key_part_t, vector<variant<HyperTrie<T> *, T>>>> {
    HyperTrieJoinIterator() { ended = true; }

    inline static HyperTrieJoinIterator ended_instance{};
public:

    HyperTrieJoinIterator(
            const map<op_pos_t, HyperTrieMatchKeyPosView> &hyper_trie_views,
            const HyperTrieMatchKeyPosIterator &it_begin,
            const HyperTrieMatchKeyPosIterator &it_end,
            bool in_result,
            label_pos_t result_pos,
            const vector<variant<HyperTrie<T> *, T>> &operands,
            const vector<uint64_t> &result_key)
            : hyper_trie_views(hyper_trie_views), it_begin(it_begin), it_end(it_end),
              in_result(in_result), result_pos(result_pos), operands(operands),
              result_key(result_key) {}


private:

    const map<op_pos_t, HyperTrieMatchKeyPosView> hyper_trie_views{};
    HyperTrieMatchKeyPosIterator it_begin;
    HyperTrieMatchKeyPosIterator it_end;
    key_part_t current_key_part{};

    const bool in_result{};
    const label_pos_t result_pos{};

    const vector<variant<HyperTrie<T> *, T>> &operands;
    const vector<uint64_t> &result_key;

    bool ended = false;

    // results for all operands
    vector<variant<HyperTrie<T> *, T>> *first;
    // results binding, maybe vector?
    vector<uint64_t> *second;

public:

    HyperTrieJoinIterator &operator++() {
        bool match = false;

        do {
            if (it_begin == it_end) {
                break;
            }

            current_key_part = it_begin->first; // todo: that doesn't work totally so far
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
        if (rhs.end && end)
            return true;
        else
            return current_key_part == rhs.current_key_part;
    }

    bool operator!=(const HyperTrieJoinIterator &rhs) const {
        // TODO: does this make sense?
        if (rhs.end != end)
            return true;
        else
            return current_key_part != rhs.current_key_part;
    }


    static HyperTrieJoinIterator &end() {
        return ended_instance;
    }

};

template<typename T>
HyperTrieJoinIterator &HyperTrieJoin<T>::end() {
    return HyperTrieJoinIterator::end();
}


#endif //LIBSPARSETENSOR_JOINITERATOR_HPP
