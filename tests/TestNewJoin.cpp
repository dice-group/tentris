#include <gtest/gtest.h>

#include "hypertrie/NewJoin.hpp"

#include <typeinfo>

TEST(TestJoin, simple_call) {
    typeid(sparsetensor::hypertrie::NewJoin).name();
    sparsetensor::hypertrie::NewJoin join = sparsetensor::hypertrie::NewJoin{{}, {}, {}, {}, {} , {}};
    join.begin();


}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}