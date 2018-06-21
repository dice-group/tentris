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

namespace sparsetensor::hypertrie {
    using op_pos_t = sparsetensor::operations::op_pos_t;

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

    template<typename COUNT_t>
    class Operands {
        std::vector<std::variant<BoolHyperTrie *, COUNT_t>> ops;
        std::vector<bool> is_trie;
    public:

        Operands(std::vector<std::variant<BoolHyperTrie *, COUNT_t>> operands) :
                ops{operands}, is_trie(operands.size()) {
            for (size_t i = 0; i < operands.size(); ++i) {
                if (std::holds_alternative<BoolHyperTrie *>(operands[i]))
                    is_trie[i] = true;
            }
        }

        inline size_t scalar_count() {
            return ops.size() - std::accumulate(is_trie.begin(), is_trie.end(), 0);
        }

        inline size_t trie_count() {
            return size_t(std::accumulate(is_trie.begin(), is_trie.end(), 0));
        }

        std::vector<op_pos_t> scalar_positions() {
            vector<op_pos_t> scalar_poss(ops.size() - std::accumulate(is_trie.begin(), is_trie.end(), 0));
            for (op_pos_t pos = 0, scalars_i = 0; pos < ops.size(); ++pos) {
                if (not is_trie[pos]) {
                    scalar_poss[scalars_i] = pos;
                    ++scalars_i;
                }
            }
            return scalar_poss;
        }

        std::vector<op_pos_t> trie_positions() {
            vector<op_pos_t> trie_poss(size_t(std::accumulate(is_trie.begin(), is_trie.end(), 0)));
            for (op_pos_t pos = 0, trie_i = 0; pos < ops.size(); ++pos) {
                if (is_trie[pos]) {
                    trie_poss[trie_i] = pos;
                    ++trie_i;
                }
            }
            return trie_poss;
        }

        std::vector<BoolHyperTrie *> tries() {
            const vector<op_pos_t> &trie_poss = trie_positions();
            const unsigned long trie_count = trie_poss.size();
            vector<BoolHyperTrie *> tries(trie_count);
            for (size_t i = 0; i < trie_count; ++i) {
                tries[i] = std::get<BoolHyperTrie *>(ops[trie_poss[i]]);
            }
            return tries;
        }

        inline BoolHyperTrie *trie_at(op_pos_t pos) const {
            return std::get<BoolHyperTrie *>(ops.at(pos));
        }

        inline const std::variant<BoolHyperTrie *, COUNT_t> &at(op_pos_t pos) const {
            return ops.at(pos);
        }

        inline const size_t &size() const {
            return is_trie.size();
        }


    };

    class NewJoin {

        key_part_t _min_keypart = KEY_PART_MAX;
        key_part_t _max_keypart = KEY_PART_MIN;

        std::vector<op_pos_t> _trie_poss;

        const std::vector<std::variant<BoolHyperTrie *, bool>> &_result;
        const Key_t &_key;

        std::vector<BoolHyperTrie::DiagonalView> diags{};

        // TODO: do that via template
        const std::optional<key_pos_t> &_result_pos;

        NewJoin(const Key_t &key,
                const std::vector<std::variant<BoolHyperTrie *, bool>> &operands,
                const std::vector<std::vector<key_pos_t >> &key_poss,
                const std::optional<key_pos_t> &result_pos) :
                _key{key}, _result{operands}, _result_pos{result_pos} {

            Operands<bool> ops_view{operands};

            if (ops_view.trie_count() > 0) {
                _trie_poss = ops_view.trie_positions();
                removeUnusedTriePoss(key_poss, _trie_poss);
                if (_trie_poss.size() > 0) {
                    diags.reserve(_trie_poss.size());
                    for (size_t i = 0; i < _trie_poss.size(); ++i) {
                        diags.emplace_back(
                                BoolHyperTrie::DiagonalView{ops_view.trie_at(_trie_poss[i]), key_poss[_trie_poss[i]]});
                    }
                    const std::tuple<size_t, size_t> &min_max = BoolHyperTrie::DiagonalView::minimizeRange(diags);
                    _min_keypart = std::get<0>(min_max);
                    _max_keypart = std::get<1>(min_max);
                };
            }
        }

        // TODO: maybe we already know which positions are used in advance?
        void removeUnusedTriePoss(const vector<vector<key_pos_t>> &dims, vector<op_pos_t> &trie_poss) const {
            // remove positions of iterators that are not involved in the join.
            vector<op_pos_t>::iterator trie_poss_it = trie_poss.begin();
            vector<op_pos_t>::iterator trie_poss_end = trie_poss.begin();
            for (size_t pos = 0; (pos < dims.size()) and (trie_poss_it != trie_poss_end); ++pos) {
                if (*trie_poss_it == pos) {
                    if (dims[pos].size() == 0) {
                        trie_poss_it = trie_poss.erase(trie_poss_it);
                    } else {
                        ++trie_poss_it;
                    }
                }
            }
        }

        class iterator {
            const NewJoin &_join;
            key_part_t _current_key_part;
            key_part_t _last_key_part;
            bool _ended{};
            std::vector<size_t> _sort_order;
            BoolHyperTrie::DiagonalView *_min_diag;
        public:

            iterator(NewJoin &join, bool ended = false) :
                    _join{join},
                    _ended{ended},
                    _current_key_part{(not ended) ? join._min_keypart : join._max_keypart + 1},
                    _last_key_part{join._max_keypart} {
                if (_current_key_part <= _last_key_part) {
                    _sort_order = sortPermutation(join.diags,
                                                  [](const BoolHyperTrie::DiagonalView &a,
                                                     const BoolHyperTrie::DiagonalView &b) {
                                                      return a.size() < b.size();
                                                  });
                    _min_diag = &_join.diags[_sort_order[0]];
                    _current_key_part = _min_diag->min();
                    findNextMatch();
                } else {
                    _sort_order = {};
                    _min_diag = nullptr;
                };
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
                    for (size_t i = 1; i < _sort_order.size(); ++i) {
                        BoolHyperTrie::DiagonalView &diag = _join.diags[_sort_order[i]];

                        if (not diag.containsAndUpdateLower(_current_key_part)) {
                            _current_key_part = _min_diag->min(diag.lower());
                            goto continue_outer_loop;
                        }
                    }
                    return;
                }
            }

            iterator &operator++() {
                BoolHyperTrie::DiagonalView &min_diag = _join.diags[_sort_order[0]];
                if (_current_key_part <= _last_key_part) {
                    _current_key_part = _min_diag->incrementMin();
                    findNextMatch();
                }
                return *this;
            }

            std::tuple<std::vector<std::variant<BoolHyperTrie *, bool>>, vector<uint64_t>> operator*() {
                vector<variant<BoolHyperTrie *, bool>> result = _join._result;

                for (size_t i = 0; i < _join._trie_poss.size(); ++i) {
                    op_pos_t &pos = _join._trie_poss[i];

                    result[pos] = _join.diags[i].minValue();
                }

                Key_t key = _join._key;
                if (_join._result_pos)
                    key[*_join._result_pos] = _current_key_part;

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
