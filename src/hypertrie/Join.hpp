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

    private:
        const vector<variant<HyperTrie<T> *, T>> &operands;
        const Key_t &result_key;
        vector<Diagonal<T>> diagonals{};
        vector<op_pos_t> join_op_pos{};
        op_pos_t min_card_op_pos = 0;
        Diagonal<T> *min_card_diagonal;
        key_part_t min_key_part = KEY_PART_MIN;
        key_part_t max_key_part = KEY_PART_MAX;
        bool has_result_pos = false;
        label_pos_t result_pos{};

    public:

        /**
         * initializes this->diagonals with all diagonals and writes the position on the vector of the
         * operand with minimal cardinality to min_card_op_pos
         * @param [in] planStep
         * @param [in] label
         * @param [in] operands
         * @param [in,out] min_key_part returns the largest lower_bound of all diagonals
         * @param [in,out] max_key_part returns the smallest upper_bound of all diagonals
         */
        void initDiagonals(const PlanStep &planStep,
                           const label_t &label,
                           const vector<variant<HyperTrie<T> *, T>> &operands,
                           key_part_t &min_key_part,
                           key_part_t &max_key_part) {

            size_t min_card = SIZE_MAX;

            for(const  op_pos_t &op_pos: planStep.operandsWithLabel(label)){

                // gernerate diagonal
                Diagonal<T> diagonal{std::get<HyperTrie<T> *>(operands.at(op_pos)),
                                     planStep.labelPossInOperand(op_pos, label)};

                if (min_card > diagonal.estimCard()) {
                    min_card = diagonal.estimCard();
                    min_card_op_pos = op_pos;
                }
                min_key_part = (diagonal.min() > min_key_part) ? diagonal.min() : min_key_part;
                max_key_part = (diagonal.max() < max_key_part) ? diagonal.max() : max_key_part;

                diagonals.push_back(diagonal);
                join_op_pos.push_back(op_pos);
            }
        }

        Join(const vector<variant<HyperTrie<T> *, T>> &operands,
             const PlanStep &planStep,
             const label_t &label,
             const Key_t &result_key)
                : result_key(result_key),
                  operands(operands){


            // get non min_card diagonals
            initDiagonals(planStep, label, operands, min_key_part, max_key_part);

            // get min_card diagonal
            min_card_diagonal = new Diagonal<T>{std::get<HyperTrie<T> *>(operands[min_card_op_pos]),
                                                planStep.labelPossInOperand(min_card_op_pos, label)};
            min_card_diagonal->setLowerBound(min_key_part);
            min_card_diagonal->setUpperBound(max_key_part);

            // position in result
            const optional<label_pos_t> &result_pos_ = planStep.labelPosInResult(label);
            has_result_pos = bool(result_pos_);
            label_pos_t result_pos{};
            if (has_result_pos = bool(result_pos_))
                result_pos = *result_pos_;
        }

        ~Join() {
            delete min_card_diagonal;
        }


        Iterator begin() {
            return Iterator{*this};
        }

        Iterator end() {
            return Iterator{*this, true};
        }

        class Iterator {
            friend class Join<T>;

            /**
             * Diagonals of the hypertries that are joined except for the one with the smallest estimated cardinality.
             */
            const vector<Diagonal<T>> &diagonals;

            const op_pos_t &min_card_op_pos;

            const vector<op_pos_t> &join_op_pos;
            /**
             * Iterator of the diagonal with the smallest estimated cardinality.
             */
            typename Diagonal<T>::Iterator min_card_diagonal_iter;

            const op_pos_t it_ops_pos{};
            /**
             * The keypart that is currently in use
             */
            key_part_t current_key_part;

            const bool &in_result;
            const label_pos_t &result_pos;

            bool ended = false;

            const Key_t &result_key;

            vector<variant<HyperTrie<T> *, T>> joined_operands;
            Key_t joined_key;

        public:

            Iterator(const Join<T> &join,
                     const bool ended = false) :
                    min_card_diagonal_iter(join.min_card_diagonal->begin()),
                    diagonals(join.diagonals),
                    min_card_op_pos(join.min_card_op_pos),
                    join_op_pos(join.join_op_pos),
                    current_key_part(join.min_key_part),
                    in_result(join.has_result_pos),
                    result_pos(join.result_pos),
                    result_key(join.result_key),
                    joined_key(join.result_key),
                    joined_operands(join.operands){
                this->ended = ended;
                if (not ended && current_key_part > join.min_card_diagonal->max()) { // todo: remove
                    throw "something is fishy.";
                }
                if (ended) {
                    current_key_part = std::max(join.min_card_diagonal->max() + 1, KEY_PART_MAX);
                }
                operator++();
            }


            Iterator &operator++() {
                if (not ended) {
                    while (not min_card_diagonal_iter.hasEnded()) {

                        bool match = true;

                        const auto & [current_key_part, min_card_op] = *min_card_diagonal_iter;

                        auto diagonal_ = diagonals.begin();
                        for(const op_pos_t &op_pos : join_op_pos){
                            if(op_pos != min_card_op_pos){
                                optional<variant<HyperTrie<T> *, T>> op = (*diagonal_).find(current_key_part);

                                if (op) {
                                    joined_operands.at(op_pos) = *op; // todo: is this the right op_pos
                                } else {
                                    ++min_card_diagonal_iter;
                                    goto continue_while;
                                }
                            }
                            ++diagonal_;
                        }

                        joined_operands.at(it_ops_pos) = min_card_op;
                        this->current_key_part = current_key_part;
                        if (in_result) {
                            joined_key.at(result_pos) = current_key_part;
                        }
                        ++min_card_diagonal_iter;
                        return *this;

                        continue_while:;
                    }
                    ended = true;
                }
                return *this;
            }

            Iterator operator++(int) {
                operator++();
                return *this;
            }

            tuple<vector<variant<HyperTrie<T> *, T>>, vector<uint64_t>> operator*() {
                return {{joined_operands},
                        {joined_key}};
            }

            bool operator==(const Iterator &rhs) const {
                // if both ended they are equal
                if (rhs.ended && ended)
                    return true;
                    // if rhs ended and lhs's key_pos is greater then rhs's
                else if (rhs.ended && rhs.current_key_part <= current_key_part)
                    return true;
                    // the same the other way around
                else if (ended && current_key_part <= rhs.current_key_part)
                    return true;
                    // both key_parts are equal
                else
                    return current_key_part == rhs.current_key_part;
            }

            bool operator!=(const Iterator &rhs) const {
                return not this->operator==(rhs);
            }
        };
    };
}


#endif //SPARSETENSOR_HYPERTRIE_JOIN_HPP
