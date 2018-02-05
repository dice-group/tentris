#include <gtest/gtest.h>
#include "tensor/MapTensor.hpp"
#include "tensor/CrossProductTensor.hpp"
#include "tensor/Types.hpp"


TEST(CrossProductTensor, write_read_delete_read) {
    using namespace sparsetensor::tensor;
    //CrossProductTensor<int> tensor{shape_t{3, 3, 3}};
    // std::cout << tensor << std::endl;

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}