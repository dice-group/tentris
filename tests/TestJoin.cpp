#include <gtest/gtest.h>

#include "hypertrie/Join.hpp"

using sparsetensor::tensor::shape_t;
using sparsetensor::einsum::raw_subscript;
using sparsetensor::einsum::Subscript;
using sparsetensor::einsum::EvalPlan;


TEST(TestJoin, simple_call) {
    using namespace sparsetensor::hypertrie;
    key_pos_t depth = 2;

    HyperTrie<int> tensor_0{depth};
    tensor_0.set({0, 0}, 1);
    tensor_0.set({0, 1}, 2);
    tensor_0.set({1, 0}, 3);
    tensor_0.set({1, 1}, 5);

    HyperTrie<int> tensor_1{depth};
    tensor_1.set({0, 0}, 7);
    tensor_1.set({0, 1}, 11);
    tensor_1.set({1, 0}, 13);
    tensor_1.set({1, 1}, 17);

    vector<variant<HyperTrie<int> *, int >> operands{&tensor_0, &tensor_1};

    vector<raw_subscript> op_sc{{0, 1},
                                {1, 2}};
    raw_subscript res_sc{0, 2};

    Subscript subscript{op_sc, res_sc};

    EvalPlan plan{subscript};
    auto[step, label] = plan.firstStep(operands);

    Key_t key(depth);

    Join join{operands, step, label, key};

    bool not_yet_fully_implemented = false;
    ASSERT_TRUE(not_yet_fully_implemented);


}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}