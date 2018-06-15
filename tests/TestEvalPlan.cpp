#include <gtest/gtest.h>

#include "einsum/EvalPlan.hpp"

using sparsetensor::tensor::shape_t;
using sparsetensor::hypertrie::HyperTrie;
using sparsetensor::operations::raw_subscript;


TEST(EvalPlan, simple_call) {
    using namespace sparsetensor::operations;

    HyperTrie<int> tensor_a{2};
    tensor_a.set({1, 0}, 7);
    tensor_a.set({2, 0}, 11);
    tensor_a.set({3, 0}, 5);

    vector<raw_subscript> op_sc{{0, 1}};
    raw_subscript res_sc{};

    Subscript subscript{op_sc, res_sc};

    EvalPlan plan{subscript};
    auto [next_step, next_label] = plan.firstStep(vector<variant<HyperTrie<int> *, int>>{&tensor_a});

    ASSERT_EQ(next_label, 1);

    std::cout << next_label << std::endl;

    HyperTrie<int> tensor_b{1};
    tensor_b.set({1}, 7);
    tensor_b.set({2}, 11);
    tensor_b.set({3}, 5);

    auto [last_step, last_label] = plan.nextStep(vector<variant<HyperTrie<int> *, int>>{&tensor_b}, next_step, next_label);

    ASSERT_EQ(last_label, 0);

    int scalar_c = 7 + 11 + 5;

    const auto &[alldone_step, alldone_label] = plan.nextStep(vector<variant<HyperTrie<int> *, int>>{scalar_c}, last_step, last_label);

    ASSERT_TRUE(alldone_step.all_done);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}