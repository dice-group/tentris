#include <gtest/gtest.h>

#include "tensor/CrossProductTensor.hpp"


TEST(CrossProductTensor, write_read_delete_read) {
    using namespace sparsetensor::tensor;

    MapTensor<int> input_tensor_1_2{shape_t{3, 3, 3}};
    input_tensor_1_2.set({1, 2, 2}, 3);
    input_tensor_1_2.set({2, 1, 0}, 1);

    MapTensor<int> input_scalar{shape_t{}};
    input_scalar.set({}, 5);

    Subscript sc{{{1, 2, 3}, {4, 5, 6}, {}},

                 {1, 2, 3, 4, 5, 6}};


    vector<MapTensor<int> *> input_tensors = {&input_tensor_1_2, &input_tensor_1_2, &input_scalar};
    CrossProductTensor<int> tensor{input_tensors, sc};
    bool empty = true;
    for (const auto &non_zero : tensor) {
        std::cout << non_zero << std::endl;
    }
}

TEST(CrossProductTensor, scalar_multiplication) {
    using namespace sparsetensor::tensor;

    MapTensor<int> input_scalar_0{shape_t{}};
    input_scalar_0.set({}, 5);

    MapTensor<int> input_scalar_1{shape_t{}};
    input_scalar_1.set({}, 5);

    MapTensor<int> input_scalar_2{shape_t{}};
    input_scalar_2.set({}, 5);

    Subscript sc{{{}, {}, {}},{}};

    vector<MapTensor<int> *> input_tensors = {&input_scalar_0, &input_scalar_1, &input_scalar_2};

    CrossProductTensor<int> product{input_tensors, sc};
    const auto &[key,value] = *(product.begin());

    std::cout << key << " : " << value << std::endl;

    ASSERT_EQ(key, Key_t{});
    ASSERT_EQ(value, 5*5*5);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}