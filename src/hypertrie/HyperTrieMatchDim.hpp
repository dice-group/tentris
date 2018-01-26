//
// Created by me on 1/26/18.
//

#ifndef TEST_HYPERTRIEMATCHDIM_HPP
#define TEST_HYPERTRIEMATCHDIM_HPP

#include "HyperTrie.hpp"
#include <iterator>
#include <tuple>
#include <variant>
#include <vector>
#include <einsum/Subscript.hpp>

using std::tuple;

using HyperTrie::key_pos_t;
using HyperTrie::key_part_t;

/**
 * This class represents all sub-HyperTries that arise from matching one or multiple dimensions of one or multiple
 * HyperTries together with their respective key part.
 */
template<typename T>
class HyperTrieMatchDim {
public:
    HyperTrieMatchDim(const vector<HyperTrie *> &HyperTries, const vector<vector<key_pos_t>> &positions) : HyperTries(
            HyperTries), positions(positions) {}

private:

    vector<HyperTrie *> HyperTries{};
    vector<vector<key_pos_t>> positions{};
};

template<typename T>
class HyperTrieMatchDimIterator : public std::iterator<std::input_iterator_tag,
        tuple<key_part_t, vector<variant<HyperTrie *, T>>>> {
    HyperTrieMatchDim *hyperTrieMatchDim;
    key_part_t key_part{};
public:
    HyperTrieMatchDimIterator(HyperTrieMatchDim *hyperTrieMatchDim) : hyperTrieMatchDim(hyperTrieMatchDim) {}

    HyperTrieMatchDimIterator(HyperTrieMatchDimIterator it) : hyperTrieMatchDim(it.hyperTrieMatchDim) {}

    HyperTrieMatchDimIterator &operator++() {
        // TODO: do the actual thing
        return *this;
    }

    HyperTrieMatchDimIterator operator++(int) {
        HyperTrieMatchDimIterator it_copy{*this};
        operator++();
        return it_copy;
    }

    bool operator==(const HyperTrieMatchDimIterator &rhs) const {
        // TODO: does this make sense?
        return key_part == rhs.key_part;
    }

    bool operator!=(const HyperTrieMatchDimIterator &rhs) const {
        // TODO: does this make sense?
        return key_part != rhs.key_part;
    }


};


#endif //TEST_HYPERTRIEMATCHDIM_HPP
