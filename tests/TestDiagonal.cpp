#include <gtest/gtest.h>
#include "hypertrie/Diagonal.hpp"
#include <optional>

TEST(HyperTrieDiagonal, simple_call) {
    using namespace sparsetensor::hypertrie;
    key_pos_t depth = 3;

    HyperTrie<int> trie{depth};
    tensor_0.set({0, 0, 0}, 1);
    tensor_0.set({3, 3, 3}, 3);
    tensor_0.set({5, 5, 5}, 5);
    tensor_0.set({1, 1, 2}, 7);
    tensor_0.set({5, 1, 2}, 11);
    tensor_0.set({5, 1, 128}, 13);
    
    Diagonal<int> diagonal{&trie, vector<label_pos_t>{0,1,2}};
    for (auto &&item :diagonal) {
        
    }


}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}