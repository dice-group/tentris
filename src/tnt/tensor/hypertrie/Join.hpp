#ifndef SPARSETENSOR_HYPERTRIE_JOIN
#define SPARSETENSOR_HYPERTRIE_JOIN

#include <vector>
#include <forward_list>
#include <variant>
#include <numeric>
#include <optional>
#include <ostream>

#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/util/container/Sort.hpp"
#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/util/All.hpp"

namespace {
    using namespace tnt::util::types;
    using EinsumPlan = tnt::tensor::einsum::EinsumPlan;
}

namespace tnt::tensor::hypertrie {

    /**
     * Joins two or more tensors and returns the non-scalar results of the return via the iterator
     */
    class Join {
        const Key_t &_key; ///< a key that is copied each time the iterator returns a result

        key_part_t _min_keypart = KEY_PART_MAX; ///< a lower bound to the key parts that are candidates for this join

        key_part_t _max_keypart = KEY_PART_MIN; ///< a upper bound to the key parts that are candidates for this join

        // TODO: do that via template
        const std::optional<key_pos_t> &_result_key_pos; ///< an optional position in the result key where to write the binding to

        const std::map<op_pos_t, op_pos_t> &_diagonal2result_pos; ///< an position mapping from _diagonals to _results

        Operands _result; ///< Operands that are copied each time the iterator returns a result

        std::vector<BoolHyperTrie::DiagonalView> _diagonals{}; ///< diagonals that are used in the join


    public:

        Join(const Key_t &key, const Operands &operands, const EinsumPlan::Step &step) :
                _key{key},
                _result_key_pos{step.getResulKeyPos()},
                _diagonal2result_pos{step.getDiagonal2ResultMapping()} {

            // write operands into _result
            for (const op_pos_t &pos : step.getPosOfOperandsInResult()) {
                _result.push_back(operands.at(pos));
            }

            // initialize diagonals with the operands and their positions to join on.
            for (const auto &[op_pos, key_part_poss] : zip(step.getOperandPositions(), step.getKeyPartPoss())) {
                _diagonals.emplace_back(BoolHyperTrie::DiagonalView{operands[op_pos], key_part_poss});
            }

            // narrow the range of the diagonals
            std::tie(_min_keypart, _max_keypart) = BoolHyperTrie::DiagonalView::minimizeRange(_diagonals);
        }

        friend std::ostream &operator<<(std::ostream &os, const Join &join1) {
            os << "_key: " << join1._key << " _min_keypart: " << join1._min_keypart << " _max_keypart: "
               << join1._max_keypart << " _result_key_pos: " << join1._result_key_pos << " _diagonal2result_pos: "
               << join1._diagonal2result_pos;
            os << " _diagonals: \n";
            for (const auto &diag :join1._diagonals)
                os << "   " << diag << "\n";
            return os;
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

            std::map<op_pos_t, op_pos_t> _reorderedDiagonals2result_pos{};  ///< Mapping from reordered diagonal to result positions.
        public:

            iterator(Join &join, bool ended = false) :
                    _join{join},
                    _diagonals{join._diagonals},
                    _current_key_part{(not ended) ? join._min_keypart : join._max_keypart + 1},
                    _last_key_part{join._max_keypart},
                    _ended{ended} {
                if (not _ended) {
                    if (_current_key_part <= _last_key_part) {
                        // sort the diagonals by size
                        const std::vector<size_t> _sort_order = tnt::util::container::sortPermutation(
                                _diagonals, [](const BoolHyperTrie::DiagonalView &a,
                                               const BoolHyperTrie::DiagonalView &b) {
                                    return a.size() <
                                           b.size();
                                });

                        ::tnt::util::container::applyPermutation(_diagonals, _sort_order);
                        const std::vector<size_t> &inv = ::tnt::util::container::invPermutation(_sort_order);

                        // calculate the mapping from the reordered Diagonals to the result from it
                        // (that means just apply the permutation to the image of the map)
                        for (const auto &[diagonal_pos, result_pos] : _join._diagonal2result_pos) {
                            _reorderedDiagonals2result_pos[inv[diagonal_pos]] = result_pos;
                        }
                        // find the first result
                        _current_key_part = _diagonals[0].first();
                        findNextMatch();
                    } else {
                        _ended = true;
                    }
                }
            }

            friend std::ostream &operator<<(std::ostream &os, const iterator &iterator) {
                os << "_join: " << iterator._join << " _current_key_part: " << iterator._current_key_part
                   << " _last_key_part: " << iterator._last_key_part << " _ended: " << iterator._ended;
                return os;
            }

            /**
             * When this function is called _current_key_part
             * MUST be either a valid key for _min_diag OR (_current_key_part >_last_key_part).
             * After running this function _current_key_part is greater or equal to the value it had before such that
             * it is a valid key for all DiagonalView s in _join.diags .
             */
            inline void findNextMatch() {
                // check if the end was reached
                bool found;
                // _current_key_part is increased if containsAndUpdateLower returns false
                while (_current_key_part <= _last_key_part) {
                    found = true;
                    // iterate all but the first diagonal
                    for (size_t i = 1; i < _diagonals.size(); ++i) {
                        BoolHyperTrie::DiagonalView &diagonal = _diagonals[i];
                        // check if the diagonal contains the current key
                        if (not diagonal.containsAndUpdateLower(_current_key_part)) {
                            if (diagonal.size() == 0) {
                                _ended = true;
                                return;
                            }
                            // if not, update the current key by searching the next
                            // greaterEqual key to the new lower bound of this diagonal from the first diagonal
                            _current_key_part = _diagonals[0].first(diagonal.lower());
                            // and start over
                            found = false;
                            break;
                        }
                    }
                    if (found)
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
                for (auto &[revJoinee_pos, result_pos] : _reorderedDiagonals2result_pos) {
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
                return (_ended and rhs._ended);
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
