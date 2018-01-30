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
 * This class the evaluation result of one label from einstein summation. It takes a planStep and an label as input and
 * returns the contracted operands and the updated key.
 * It is only iterable.
 */
template<typename T>
class HyperTrieJoin {
public:
    HyperTrieJoin(const vector<variant<HyperTrie<T> *, T>> &operands, const PlanStep &planStep, label_t label,
                  const vector<uint64_t> &result_key) {
        // positions in operands
        const vector<op_pos_t> &op_poss = planStep.operandsWithLabel(label);

        // collect data about operands
        op_pos_t min_card_op_pos = 0;
        size_t min_card = numeric_limits<size_t>::max();
        key_part_t min_key = numeric_limits<key_part_t>::min();
        key_part_t max_key = numeric_limits<key_part_t>::max();

        map<op_pos_t, HyperTrieDiagonal> hyper_trie_views{};

        for (op_pos_t op_pos : op_poss) {

            // gernerate view
            HyperTrieDiagonal view{std::get<HyperTrie<T> *>(operands[op_pos]),
                                   planStep.labelPossInOperand(op_pos, label)};
            hyper_trie_views[op_pos] = view;

            if (min_card > view.estimCard()) {
                min_card = view.estimCard();
                min_card_op_pos = op_pos;
            }
            min_key = (view.min() > min_key) ? view.min() : min_key;
            max_key = (view.max() < max_key) ? view.max() : max_key;
        }
        map<op_pos_t, HyperTrieDiagonal>::iterator min_card_op_ = hyper_trie_views.find(min_card_op_pos);
        hyper_trie_views.erase(min_card_op_);

        HyperTrieDiagonal min_card_op{std::get<HyperTrie<T> *>(operands[min_card_op_pos]),
                                      planStep.labelPossInOperand(min_card_op_pos, label)};

        HyperTrieMatchKeyPosIterator it_begin = min_card_op.lower_bound(min_key);
        HyperTrieMatchKeyPosIterator it_end = min_card_op.upper_bound(max_key);


        // position in result
        const std::optional<label_pos_t> &result_pos_ = planStep.labelPosInResult(label);
        bool has_result_pos = false;
        label_pos_t result_pos{};
        if (result_pos_) {
            has_result_pos = true;
            result_pos = *result_pos_;
        }

        hyperTrieJoinIterator = new HyperTrieJoinIterator(hyper_trie_views, it_begin, it_end, min_card_op_pos,
                                                          has_result_pos,
                                                          result_pos, operands, result_key);
    }

    ~HyperTrieJoin() = default {
        delete hyperTrieJoinIterator;
    }

    HyperTrieJoinIterator *hyperTrieJoinIterator;


    HyperTrieJoinIterator &begin() {
        return *hyperTrieJoinIterator;
    }

    HyperTrieJoinIterator &end();
};


template<typename T>
class HyperTrieJoinIterator
        : public iterator<input_iterator_tag, tuple<key_part_t, vector<variant<HyperTrie<T> *, T>>>> {
    HyperTrieJoinIterator() : it_begin(HyperTrieMatchKeyPosIterator::end()),
                              it_begin(HyperTrieMatchKeyPosIterator::end()),
                              operands({}),
                              result_key({}),
                              ended(true) {}

    inline static HyperTrieJoinIterator ended_instance{};
public:

    HyperTrieJoinIterator(
            const map<op_pos_t, HyperTrieDiagonal> &hyper_trie_views,
            const HyperTrieMatchKeyPosIterator &it_begin,
            const HyperTrieMatchKeyPosIterator &it_end,
            const op_pos_t it_ops_pos,
            bool in_result,
            label_pos_t result_pos,
            const vector<variant<HyperTrie<T> *, T>> &operands,
            const vector<uint64_t> &result_key)
            : hyper_trie_views(hyper_trie_views), it_begin(it_begin), it_end(it_end), it_ops_pos(it_ops_pos),
              in_result(in_result), result_pos(result_pos), operands(operands),
              result_key(result_key) {}


private:

    const map<op_pos_t, HyperTrieDiagonal> hyper_trie_views{};
    HyperTrieMatchKeyPosIterator it_begin;
    HyperTrieMatchKeyPosIterator it_end;
    const op_pos_t it_ops_pos{};

    key_part_t current_key_part{};

    const bool in_result{};
    const label_pos_t result_pos{};

    const vector<variant<HyperTrie<T> *, T>> operands;
    const vector<uint64_t> &result_key;

    bool ended = false;

    vector<variant<HyperTrie<T> *, T>> new_operands = vector<variant<HyperTrie<T> *, T>>(operands.size());
    vector<uint64_t> new_key = vector<uint64_t>(result_key.size());

public:

    HyperTrieJoinIterator &operator++() {
        bool match{};

        while (it_begin == it_end) {

            match = true;

            auto & [current_key_part, it_operand] = *it_begin;

            for (const auto &
                [op_pos, other_view] : hyper_trie_views) {

                const optional<variant<HyperTrie<T> *, T>> &other_operand = other_view.find(current_key_part);

                if (operand_result) {
                    new_operands[op_pos] = *operand_result;
                } else {
                    match = false;
                    break;
                }
            }

            if (match) {
                new_operands[it_ops_pos] = it_operand;
                if (in_result)
                    new_key[result_pos] = current_key_part;
                it_begin++;
                break;
            } else {
                it_begin++;
                continue;
            }
        }

        return *this;
    }

    HyperTrieJoinIterator operator++(int) {
        HyperTrieJoinIterator it_copy{*this};
        operator++();
        return it_copy;
    }

    tuple<vector<variant<HyperTrie<T> *, T>>, vector<uint64_t>> operator*() {
        return tuple<vector<variant<HyperTrie<T> *, T>>, vector<uint64_t>>{{operands},
                                                                           {new_key}};
    }

    bool operator==(const HyperTrieJoinIterator &rhs) const {
        if (rhs.end && end)
            return true;
        else
            return current_key_part == rhs.current_key_part;
    }

    bool operator!=(const HyperTrieJoinIterator &rhs) const {
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
