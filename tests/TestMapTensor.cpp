#include <gtest/gtest.h>

#include "tensor/MapTensor.hpp"


TEST(TestMapTensor, write_read_delete_read) {
    using namespace sparsetensor::tensor;
    MapTensor<int> tensor{vector<uint64_t>{3, 3, 3}};
    std::cout << tensor << std::endl;

    vector<uint64_t> coord{1, 1, 1};
    int value{4};
    tensor.set(coord, value);
    std::cout << tensor << std::endl;

    int retrieved_value = tensor.get(coord);
    ASSERT_EQ(retrieved_value, value);

    int zero{};
    tensor.set(coord, zero);
    std::cout << tensor << std::endl;

    retrieved_value = tensor.get(coord);
    ASSERT_EQ(retrieved_value, zero);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}