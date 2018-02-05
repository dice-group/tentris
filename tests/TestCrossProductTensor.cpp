#include <gtest/gtest.h>

#include "tensor/CrossProductTensor.hpp"
#include "tensor/Types.hpp"


TEST(CrossProductTensor, write_read_delete_read) {
    using namespace sparsetensor::tensor;
    MapTensor<int> mp{shape_t{3, 3, 3}};
    mp.set({1,2,2}, 3);

    Subscript sc{{{1, 2, 3}}, {1, 2, 3}};
    vector<MapTensor<int> *> vector1 = vector<MapTensor<int> *>{&mp};
    CrossProductTensor<int> tensor{vector1, sc};
    // std::cout << tensor << std::endl;
    bool empty = true;
//    for (const auto &non_zero : mp) {
//        empty = false;
//    }
    for(const auto& non_zero : tensor){
        empty = false;
    }
    ASSERT_TRUE(not empty);

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}