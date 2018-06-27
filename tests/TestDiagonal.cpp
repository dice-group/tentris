#include <gtest/gtest.h>
#include "hypertrie/Diagonal.hpp"
#include "util/All.hpp"
#include <optional>
#include <iostream>
#include <variant>

TEST(HyperTrieDiagonal, iterate_values
) {
using namespace sparsetensor::hypertrie;
key_pos_t depth = 3;

HyperTrie<int> trie{depth};
trie.set({
0, 0, 0}, 23);
trie.set({
3, 3, 3}, 37);
trie.set({
5, 5, 5}, 57);
trie.set({
1, 1, 2}, 7);
trie.set({
5, 1, 2}, 11);
trie.set({
5, 1, 128}, 13);

Diagonal<int> diagonal{&trie, vector < label_pos_t > {0, 1, 2}};
for (const auto &[key, val] :diagonal) {
std::cout << key << " : " <<
std::get<int>(val)
<<
std::endl;
}
}

TEST(HyperTrieDiagonal, iterate_subtries
) {
using namespace sparsetensor::hypertrie;
key_pos_t depth = 3;

HyperTrie<int> trie{depth};
trie.set({
0, 0, 0}, 23);
trie.set({
3, 3, 3}, 37);
trie.set({
5, 5, 5}, 57);
trie.set({
1, 1, 2}, 7);
trie.set({
5, 1, 2}, 11);
trie.set({
5, 1, 128}, 13);

Diagonal<int> diagonal{&trie, vector < label_pos_t > {0, 1}};
for (const auto &[key, val] :diagonal) {
std::cout << key << " : " << *std::get<HyperTrie < int> *>(val) <<
std::endl;
}
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
