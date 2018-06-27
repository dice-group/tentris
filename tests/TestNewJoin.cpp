#include <gtest/gtest.h>

#include "hypertrie/Join.hpp"

TEST(TestJoin, simple_call) {
    typeid(sparsetensor::hypertrie::Join).name();
    sparsetensor::hypertrie::Join join = sparsetensor::hypertrie::Join{{},
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