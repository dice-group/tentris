//
// Created by me on 1/9/18.
//

#ifndef LIBSPARSETENSOR_HYPERTRIE_HPP
#define LIBSPARSETENSOR_HYPERTRIE_HPP


#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include "boost/variant.hpp"

class HyperTrie {
public:
    HyperTrie(uint8_t depth);

private:
    uint8_t depth;
//    uint8_t total_depth;
//    std::vector<std::map<uint64_t, boost::variant<uint64_t, HyperTrie *> *>> edges_by_pos;
//    HyperTrie *head_node;
//    // previous_pos
//    // min_card_pos
//    int64_t leafsum;
//    int64_t leafcount;
//    int64_t *head_counts;

};


#endif //LIBSPARSETENSOR_HYPERTRIE_HPP
