#include <gtest/gtest.h>

#include "tensor/hypertrie/Join.hpp"

TEST(TestJoin, simple_call) {
    typeid(tnt::hypertrie::Join).name();
    tnt::hypertrie::Join join = tnt::hypertrie::Join{{},
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