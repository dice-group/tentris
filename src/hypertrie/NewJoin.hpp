#ifndef SPARSETENSOR_HYPERTRIE_JOIN
#define SPARSETENSOR_HYPERTRIE_JOIN

#include <vector>
#include <forward_list>
#include <variant>
#include <numeric>
#include <optional>
#include "BoolHyperTrie.hpp"
#include "Types.hpp"
#include "../einsum/Types.hpp"
#include "../einsum/EvalPlan.hpp"

namespace sparsetensor::hypertrie {
    using op_pos_t = sparsetensor::operations::op_pos_t;
    using sparsetensor::operations::OP_POS_MAX;

    template<typename T, typename Compare>
    std::vector<std::size_t> sortPermutation(
            const std::vector<T> &vec,
            const Compare &compare) {
        std::vector<std::size_t> p(vec.size());
        std::iota(p.begin(), p.end(), 0);
        std::sort(p.begin(), p.end(),
                  [&](size_t i, size_t j) { return compare(vec[i], vec[j]); });
        return p;
    }

    template<typename T>
    inline std::vector<size_t> invPermutation(const std::vector<T> &permutation) {
        std::vector<size_t> inv_permutation(permutation.size());
        for (size_t i = 0; i < permutation.size(); i++)
            inv_permutation[permutation[i] - 1] = i + 1;
        return inv_permutation;
    }


    /**
     * Applies a permutation inplace.
     * @tparam T
     * @param vec
     * @param p
     */
    template<typename T>
    void applyPermutation(std::vector<T> &vec, const std::vector<std::size_t> &p) {
        std::vector<bool> done(vec.size());
        for (std::size_t i = 0; i < vec.size(); ++i) {
            if (done[i]) {
                continue;
            }
            done[i] = true;
            std::size_t prev_j = i;
            std::size_t j = p[i];
            while (i != j) {
                std::swap(vec[prev_j], vec[j]);
                done[j] = true;
                prev_j = j;
                j = p[j];
            }
        }
    }

    using Operands =  typename std::vector<BoolHyperTrie *>;

    class NewJoin {
        using sparsetensor::operations::label_pos_t;

        key_part_t _min_keypart = KEY_PART_MAX;
        key_part_t _max_keypart = KEY_PART_MIN;

        std::vector<op_pos_t> _trie_poss;


        Operands _result; ///< Operands that are copied each time the iterator returns a result
        const Key_t &_key;

        std::vector<BoolHyperTrie::DiagonalView> diags{};

        // TODO: do that via template
        const std::optional<key_pos_t> &_result_key_pos;

        std::vector<op_pos_t> _joinee2result_pos;



    public:
//        static NewJoin create(const Key_t &key,
//                              const std::vector<BoolHyperTrie *> &operands,
//                              sparsetensor::operations::PlanStep *step) {
//            // TODO: implement step to fit this interface
//            const optional<operations::label_pos_t> &key_pos = step->labelPosInResult();
//            vector<op_pos_t> &op_ids = step->operandsWithLabel();
//            vector<vector<key_pos_t>> &key_poss
//            step->labelPossInOperands();
//        }

        /**
         * @brief NewJoin
         * @param key the current key that maybe gets updated.
         * @param operands the current operands
         * @param op_poss the positions of the joining BoolHyperTrie in operands
         * @param key_part_posss the the joining key positions of each join operand.
         * @param next_op_position the positions in operands of BoolHyperTrie that will be in the result
         * @param result_key_pos
         */
        NewJoin(const Key_t &key,
                const Operands &operands,
                const std::vector<op_pos_t> op_poss,
                const std::vector<std::vector<key_pos_t >> &key_part_posss,
                const std::vector<op_pos_t> next_op_position,
                const std::optional<key_pos_t> &result_key_pos) :
                _result(op_poss.size()), _key{key}, _result_key_pos{result_key_pos} {

            // write operands into _result
            for (size_t i; i < next_op_position.size(); ++i) {
                _result[i] = operands[next_op_position[i]];
            }

            // initialize diagonals with the operands and their positions to join on.
            auto &&key_part_poss = key_part_posss.cbegin();
            for (const op_pos_t &op_pos : op_poss) {
                diags.emplace_back(BoolHyperTrie::DiagonalView{operands[op_pos], *key_part_poss});
                ++key_part_poss;
            }

            // narrow the range of the diagonals
            const std::tuple<size_t, size_t> &min_max = BoolHyperTrie::DiagonalView::minimizeRange(diags);
            _min_keypart = std::get<0>(min_max);
            _max_keypart = std::get<1>(min_max);

            // calculate the position mapping from diagonals to result
            _joinee2result_pos = std::vector<op_pos_t>(next_op_position.size());
            for(size_t i = 0, j =0; i < op_poss.size(); ++i){
                auto pos_of_join_in_operands = op_poss[i];
                auto pos_of_result_in_operands = next_op_position[j];
                if(pos_of_join_in_operands == pos_of_result_in_operands){
                    _joinee2result_pos[i] = pos_of_join_in_operands;
                    ++j;
                } else{
                    _joinee2result_pos[i] = OP_POS_MAX;
                }
            }
        }


        class iterator {
            NewJoin &_join;
            std::vector<BoolHyperTrie::DiagonalView> &_diagonals;

            key_part_t _current_key_part;
            key_part_t _last_key_part;
            bool _ended{};
            std::vector<std::tuple<op_pos_t, op_pos_t>> _reorderedJoinee2result_pos{};
        public:

            iterator(NewJoin &join, bool ended = false) :
                    _join{join},
                    _diagonals{join.diags},
                    _current_key_part{(not ended) ? join._min_keypart : join._max_keypart + 1},
                    _last_key_part{join._max_keypart},
                    _ended{ended} {
                if (_current_key_part <= _last_key_part) {
                    // sort the diagonals by size
                    const std::vector<size_t> _sort_order = sortPermutation(_diagonals,
                                                  [](const BoolHyperTrie::DiagonalView &a,
                                                     const BoolHyperTrie::DiagonalView &b) {
                                                      return a.size() < b.size();
                                                  });
                    applyPermutation(_diagonals, _sort_order);

                    // get the inverse sort order
                    const std::vector<size_t> _inv_sort_order = invPermutation(_sort_order);

                    // calculate the mapping from the reordered Diagonals to the result from it
                    for(size_t posInReorderedDiagonals = 0; posInReorderedDiagonals < _inv_sort_order.size(); ++posInReorderedDiagonals ){
                        auto positionOfReorderedDiagonalInDiagonals= _inv_sort_order[posInReorderedDiagonals];
                        auto positionOfDiagonalInResult = join._joinee2result_pos[positionOfReorderedDiagonalInDiagonals];
                        if(positionOfDiagonalInResult != OP_POS_MAX){
                            _reorderedJoinee2result_pos.emplace_back(std::make_tuple(posInReorderedDiagonals, positionOfDiagonalInResult));
                        }
                    }

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
                continue_outer_loop:
                while (_current_key_part <= _last_key_part) {
                    for (size_t i = 1; i < _diagonals.size(); ++i) {
                        BoolHyperTrie::DiagonalView &diagonal = _diagonals[i];

                        if (not diagonal.containsAndUpdateLower(_current_key_part)) {
                            _current_key_part = _diagonals[0].first(diagonal.lower());
                            goto continue_outer_loop;
                        }
                    }
                    return;
                }
                _ended = true;
            }

            iterator &operator++() {
                _current_key_part = _diagonals[0].incrementMin();
                findNextMatch();
                return *this;
            }

            std::tuple<std::vector<BoolHyperTrie *>, vector<uint64_t>> operator*() {
                vector<BoolHyperTrie *> result = _join._result;

                for (auto &&[revJoinee_pos, result_pos] : _reorderedJoinee2result_pos) {
                    result[result_pos] = _diagonals[revJoinee_pos].minValue();
                }

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

        iterator begin() {
            return iterator{*this};
        }

        iterator end() {
            return iterator{*this, true};
        }
    };
}

#endif //SPARSETENSOR_HYPERTRIE_JOIN
