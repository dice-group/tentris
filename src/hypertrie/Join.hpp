#ifndef SPARSETENSOR_HYPERTRIE_JOIN_HPP
#define SPARSETENSOR_HYPERTRIE_JOIN_HPP

#include "HyperTrie.hpp"
#include "Diagonal.hpp"
#include "../einsum/EvalPlan.hpp"
#include "../tensor/Types.hpp"
#include <iterator>
#include <tuple>
#include <variant>
#include <vector>
#include <optional>


using std::tuple;
using std::set;
using std::optional;
using std::numeric_limits;
using std::input_iterator_tag;
using ::sparsetensor::einsum::label_t;
using ::sparsetensor::einsum::op_pos_t;
using ::sparsetensor::einsum::PlanStep;
using sparsetensor::einsum::label_pos_t;
using sparsetensor::tensor::key_pos_t;
using sparsetensor::tensor::key_part_t;


namespace sparsetensor::hypertrie {
/**
 * This class the evaluation result of one label from einstein summation. It takes a planStep and an label as input and
 * returns the contracted operands and the updated key.
 * It is only iterable.
 */
    template<typename T>
    class Join {

    public:
        class Iterator;

        Join(const vector<variant<HyperTrie<T> *, T>> &operands, const PlanStep &planStep, label_t label,
             const vector<uint64_t> &result_key) {
            // positions in operands
            const vector<op_pos_t> &op_poss = planStep.operandsWithLabel(label);

            // collect data about operands
            op_pos_t min_card_op_pos = 0;
            size_t min_card = numeric_limits<size_t>::max();
            key_part_t min_key = KEY_PART_MIN;
            key_part_t max_key = KEY_PART_MAX;

            map<op_pos_t, Diagonal<T>> hyper_trie_views{};

            for (op_pos_t op_pos : op_poss) {

                // gernerate view
                Diagonal view{std::get<HyperTrie<T> *>(operands[op_pos]),
                              planStep.labelPossInOperand(op_pos, label)};
                hyper_trie_views[op_pos] = view;

                if (min_card > view.estimCard()) {
                    min_card = view.estimCard();
                    min_card_op_pos = op_pos;
                }
                min_key = (view.min() > min_key) ? view.min() : min_key;
                max_key = (view.max() < max_key) ? view.max() : max_key;
            }
            typename map<op_pos_t, Diagonal<T>>::iterator min_card_op_ = hyper_trie_views.find(min_card_op_pos);
            hyper_trie_views.erase(min_card_op_);

            Diagonal min_card_op{std::get<HyperTrie<T> *>(operands[min_card_op_pos]),
                                 planStep.labelPossInOperand(min_card_op_pos, label)};
            min_card_op.setLowerBound(min_key);
            min_card_op.setUpperBound(max_key);
            auto & [it_begin, it_end] = min_card_op.getIterator();


            // position in result
            const optional<label_pos_t> &result_pos_ = planStep.labelPosInResult(label);
            bool has_result_pos = false;
            label_pos_t result_pos{};
            if (result_pos_) {
                has_result_pos = true;
                result_pos = *result_pos_;
            }

            iter = new Iterator(hyper_trie_views, it_begin, it_end, min_card_op_pos,
                                has_result_pos,
                                result_pos, operands, result_key);
            iter_end = new Iterator(iter->it_end);
        }

        ~Join() {
            delete iter;
        }

        Iterator *iter;
        Iterator *iter_end;


        Iterator &begin() {
            return *iter;
        }

        Iterator &end() {
            return *iter_end;
        }

        class Iterator {
            using diag_it_t = typename Diagonal<T>::Iterator;

            Iterator(diag_it_t it_end) : it_begin(it_end),
                                         it_end(it_end),
                                         operands({}),
                                         result_key({}),
                                         ended(true) {}

            inline static Iterator ended_instance{};
        public:

            Iterator(
                    const map<op_pos_t, Diagonal<T>> &hyper_trie_views,
                    const diag_it_t &it_begin,
                    const diag_it_t &it_end,
                    const op_pos_t it_ops_pos,
                    bool in_result,
                    label_pos_t result_pos,
                    const vector<variant<HyperTrie<T> *, T>> &operands,
                    const vector<uint64_t> &result_key)
                    : hyper_trie_views(hyper_trie_views), it_begin(it_begin), it_end(it_end), it_ops_pos(it_ops_pos),
                      in_result(in_result), result_pos(result_pos), operands(operands),
                      result_key(result_key) {}


        private:

            const map<op_pos_t, Diagonal<T>> hyper_trie_views{};
            diag_it_t it_begin;
            diag_it_t it_end;
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

            Iterator &operator++() {
                bool match{};

                while (it_begin == it_end) {

                    match = true;

                    auto &[current_key_part, it_operand] = *it_begin;

                    for (const auto &[op_pos, other_view] : hyper_trie_views) {

                        const optional<variant<HyperTrie<T> *, T>> &other_operand = other_view.find(current_key_part);

                        if (other_operand) {
                            new_operands[op_pos] = *other_operand;
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

            Iterator operator++(int) {
                Iterator it_copy{*this};
                operator++();
                return it_copy;
            }

            tuple<vector<variant<HyperTrie<T> *, T>>, vector<uint64_t>> operator*() {
                return tuple<vector<variant<HyperTrie<T> *, T>>, vector<uint64_t>>{{operands},
                                                                                   {new_key}};
            }

            bool operator==(const Iterator &rhs) const {
                if (rhs.ended && ended)
                    return true;
                else
                    return current_key_part == rhs.current_key_part;
            }

            bool operator!=(const Iterator &rhs) const {
                if (rhs.ended != ended)
                    return true;
                else
                    return current_key_part != rhs.current_key_part;
            }

            Iterator end() {
                return {it_end};
            }
        };
    };
}


#endif //SPARSETENSOR_HYPERTRIE_JOIN_HPP
