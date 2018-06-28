#ifndef SPARSETENSOR_HYPERTRIE_JOIN
#define SPARSETENSOR_HYPERTRIE_JOIN

#include <vector>
#include <forward_list>
#include <variant>
#include <numeric>
#include <optional>

#include "BoolHyperTrie.hpp"
#include "../../util/container/Sort.hpp"
#include "../einsum/EinsumPlan.hpp"
#include "../../util/All.hpp"

namespace tnt::hypertrie {
    using Operands =  typename std::vector<BoolHyperTrie *>;


    /**
     * Joins two or more tensors and returns the non-scalar results of the return via the iterator
     */
    class Join {
        using key_pos_t = tnt::util::types::key_pos_t;
        using op_pos_t = tnt::operations::op_pos_t;
        using label_pos_t = tnt::operations::label_pos_t;
        using key_part_t =  tnt::util::types::key_part_t;
        using Key_t =  tnt::util::types::Key_t;
        using EinsumPlan = tnt::operations::EinsumPlan;

        key_part_t _min_keypart = KEY_PART_MAX; ///< a lower bound to the key parts that are candidates for this join

        key_part_t _max_keypart = KEY_PART_MIN; ///< a upper bound to the key parts that are candidates for this join

        Operands _result; ///< Operands that are copied each time the iterator returns a result

        const Key_t &_key; ///< a key that is copied each time the iterator returns a result

        std::vector<BoolHyperTrie::DiagonalView> _diagonals{}; ///< diagonals that are used in the join

        // TODO: do that via template
        const std::optional<key_pos_t> &_result_key_pos; ///< an optional position in the result key where to write the binding to

        std::vector<op_pos_t> _diagonal2result_pos; ///< an position mapping from _diagonals to _results


    public:

        Join(const Key_t &key, const Operands &operands, const EinsumPlan::Step &step) :
                Join{key, operands, step.getOperandPositions(), step.getKeyPartPoss(), step.getPosOfOperandsInResult(),
                     step.getResulKeyPos()} {}

        /**
         *
         * @param key the current key that maybe gets updated.
         * @param operands the current operands
         * @param op_poss the positions of the joining BoolHyperTrie in operands
         * @param key_part_posss the joining key part positions of each join operand.
         * @param next_op_position the positions in operands that will be in the result
         * @param result_key_pos
         */
        Join(const Key_t &key,
             const Operands &operands,
             const std::vector<op_pos_t> op_poss,
             const std::vector<std::vector<key_pos_t >> &key_part_posss,
             const std::vector<op_pos_t> next_op_position,
             const std::optional<key_pos_t> &result_key_pos) :
                _result(op_poss.size()), _key{key}, _result_key_pos{result_key_pos} {

            // write operands into _result
            for (size_t i = 0; i < next_op_position.size(); ++i) {
                _result[i] = operands[next_op_position[i]];
            }

            // initialize diagonals with the operands and their positions to join on.
            auto &&key_part_poss = key_part_posss.cbegin();
            for (const op_pos_t &op_pos : op_poss) {
                _diagonals.emplace_back(BoolHyperTrie::DiagonalView{operands[op_pos], *key_part_poss});
                ++key_part_poss;
            }

            // narrow the range of the diagonals
            const std::tuple<size_t, size_t> &min_max = BoolHyperTrie::DiagonalView::minimizeRange(_diagonals);
            _min_keypart = std::get<0>(min_max);
            _max_keypart = std::get<1>(min_max);

            // calculate the position mapping from diagonals to result
            // TODO: move that to PlanStep
            _diagonal2result_pos = std::vector<op_pos_t>(next_op_position.size());
            for (size_t i = 0, j = 0; i < op_poss.size(); ++i) {
                auto pos_of_join_in_operands = op_poss[i];
                auto pos_of_result_in_operands = next_op_position[j];
                if (pos_of_join_in_operands == pos_of_result_in_operands) {
                    _diagonal2result_pos[i] = pos_of_join_in_operands;
                    ++j;
                } else {
                    _diagonal2result_pos[i] = OP_POS_MAX;
                }
            }
        }


        /**
         *
         * @brief An Iterator for NewJoins that returns its results.
         */
        class iterator {
            Join &_join; ///< the join that is iterated.

            std::vector<BoolHyperTrie::DiagonalView> &_diagonals; ///< the diagonals of the join i.e. its inputs.

            key_part_t _current_key_part; ///< the key part of the current result

            key_part_t _last_key_part; ///< the key part that is the last candidate to produce a result.

            bool _ended{};  ///< if the end was reached.

            std::vector<std::tuple<op_pos_t, op_pos_t>> _reorderedDiagonals2result_pos{};  ///< Mapping from reordered diagonal to result positions.
        public:

            iterator(Join &join, bool ended = false) :
                    _join{join},
                    _diagonals{join._diagonals},
                    _current_key_part{(not ended) ? join._min_keypart : join._max_keypart + 1},
                    _last_key_part{join._max_keypart},
                    _ended{ended} {
                if ((not ended) and (_current_key_part <= _last_key_part)) {
                    // sort the diagonals by size
                    const std::vector<size_t> _sort_order = tnt::sorting::sortPermutation(
                            _diagonals, [](const BoolHyperTrie::DiagonalView &a,
                                           const BoolHyperTrie::DiagonalView &b) {
                                return a.size() <
                                       b.size();
                            });

                    ::tnt::sorting::applyPermutation(_diagonals, _sort_order);

                    // get the inverse sort order
                    const std::vector<size_t> _inv_sort_order = ::tnt::sorting::invPermutation(_sort_order);

                    // calculate the mapping from the reordered Diagonals to the result from it
                    for (size_t posInReorderedDiagonals = 0;
                         posInReorderedDiagonals < _inv_sort_order.size(); ++posInReorderedDiagonals) {
                        auto positionOfReorderedDiagonalInDiagonals = _inv_sort_order[posInReorderedDiagonals];
                        auto positionOfDiagonalInResult = join._diagonal2result_pos[positionOfReorderedDiagonalInDiagonals];
                        if (positionOfDiagonalInResult != OP_POS_MAX) {
                            _reorderedDiagonals2result_pos.emplace_back(
                                    std::make_tuple(posInReorderedDiagonals, positionOfDiagonalInResult));
                        }
                    }
                    // find the first result
                    _current_key_part = _diagonals[0].first();
                    findNextMatch();
                }
            }

            /**
             * When this function is called _current_key_part
             * MUST be either a valid key for _min_diag OR (_current_key_part >_last_key_part).
             * After running this function _current_key_part is greater or equal to the value it had before such that
             * it is a valid key for all DiagonalView s in _join.diags .
             */
            inline void findNextMatch() {
                // check if the end was reached
                continue_outer_loop:
                while (_current_key_part <= _last_key_part) {
                    // iterate all but the first diagonal
                    for (size_t i = 1; i < _diagonals.size(); ++i) {
                        BoolHyperTrie::DiagonalView &diagonal = _diagonals[i];
                        // check if the diagonal contains the current key
                        if (not diagonal.containsAndUpdateLower(_current_key_part)) {
                            // if not, update the current key by searching the next
                            // greaterEqual key to the new lower bound of this diagonal from the first diagonal
                            _current_key_part = _diagonals[0].first(diagonal.lower());
                            // and start over
                            goto continue_outer_loop;
                        }
                    }
                    return;
                }
                // the end was reached
                _ended = true;
            }

            iterator &operator++() {
                _current_key_part = _diagonals[0].incrementMin();
                findNextMatch();
                return *this;
            }

            std::tuple<std::vector<BoolHyperTrie *>, std::vector<uint64_t>> operator*() {
                // build the result
                std::vector<BoolHyperTrie *> result = _join._result;
                for (auto &&[revJoinee_pos, result_pos] : _reorderedDiagonals2result_pos) {
                    result[result_pos] = _diagonals[revJoinee_pos].minValue();
                }

                // set the entry in the key
                Key_t key = _join._key;
                if (_join._result_key_pos)
                    key[*_join._result_key_pos] = _current_key_part;

                return {{result},
                        {key}};
            }


            inline bool operator==(const iterator &rhs) const {
                // careful, it doesn't check if it is tested against another iterator for the same Join.
                return ((rhs._ended and _ended) or
                        (rhs._current_key_part == _current_key_part) or
                        (_current_key_part > _last_key_part and rhs._current_key_part > rhs._last_key_part));
            }

            inline bool operator!=(const iterator &rhs) const {
                return not this->operator==(rhs);
            }

        };

        /**
         * Iterates the result of the Join. MUST NOT be called twice!
         * @return an iterator
         */
        iterator begin() {
            return iterator{*this};
        }

        /**
         * End iterator. MAY be called multiple times.
         * @return the end iterator.
         */
        iterator end() {
            return iterator{*this, true};
        }
    };
}

#endif //SPARSETENSOR_HYPERTRIE_JOIN
