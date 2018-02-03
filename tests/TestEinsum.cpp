#include <gtest/gtest.h>

#include "einsum/Einsum.hpp"


TEST(TestEinsum, simple_call) {
    //einsum(vector<HyperTrieTensor<int>>{}, vector<raw_subscript>{}, raw_subscript{});
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}