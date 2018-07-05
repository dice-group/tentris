#include <gtest/gtest.h>

#include "tnt/tensor/hypertrie/Join.hpp"

using namespace tnt::tensor::hypertrie;

TEST(TestJoin, simple_call) {
    typeid(Join).name();
    Join join{{},
              {},
              {},
              {},
              {},
              {}};
    join.begin();


}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}