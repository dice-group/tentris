#include <gtest/gtest.h>

#include <memory>

#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/tensor/hypertrie/Join.hpp"


namespace {
    using Step = tnt::tensor::einsum::EinsumPlan::Step;
    using namespace tnt::util::types;
    using namespace tnt::tensor::hypertrie;
    using namespace tnt::tensor::einsum;
}


TEST(TestJoin, simple_call) {
    BoolHyperTrie tensor_0 = {{0},
                              {1},
                              {2}};

    BoolHyperTrie tensor_1{{2, 7},
                           {3, 8},
                           {4, 9}};

    BoolHyperTrie tensor_2{{2},
                           {6}};


    std::vector<raw_subscript> op_sc{{1},
                                     {1, 0},
                                     {1}};
    raw_subscript res_sc{0};

    std::shared_ptr<const Subscript> subscript = std::make_shared<Subscript>(op_sc, res_sc);
    const tnt::tensor::einsum::EinsumPlan plan{subscript};
    const Operands ops{&tensor_0, &tensor_1, &tensor_2};
    tnt::tensor::einsum::EinsumPlan::Step step = plan.getInitialStep(ops);
    ASSERT_EQ(step.label, 0);


    std::cout << step << std::endl;
    std::vector<size_t> key(1);
    Join join{key, ops, step};
    std::vector<Key_t> expected_keys = {{7},
                                        {8},
                                        {9}};
    for (const auto &[actual, expected_key] : zip(join, expected_keys)) {
        const auto &actual_key = actual.second;
        ASSERT_EQ(actual_key, expected_key);

        const auto &actual_operands = actual.first;
        auto expected_operands{ops};
        expected_operands.at(1) = ops.at(1)->getSlice({std::nullopt, expected_key[0]});
        ASSERT_EQ(actual_operands, expected_operands);

        const Step &step_2 = step.nextStep(expected_operands);
        std::cout << step_2 << std::endl;
        Join join_2{actual_key, expected_operands, step_2};

        if (actual_key[0] == 7) {
            Operands expected_operands_2{};

            for (const auto &[actual_operands2, actual_key2] : join_2) {
                ASSERT_EQ(actual_key2, expected_key); // TODO: correct or not?
                ASSERT_EQ(actual_operands2, expected_operands_2);
            }
        } else{
            ASSERT_EQ(join_2.begin(), join_2.end());
        }
    }
}

//TEST(TestJoin, simple_call2) {
//    BoolHyperTrie tensor_0{3};
//    tensor_0.set({4, 9, 11}, true);
//    tensor_0.set({5, 33, 54}, true);
//    tensor_0.set({2, 32, 51}, true);
//
//    BoolHyperTrie tensor_1{1};
//    tensor_1.set({3}, true);
//    tensor_1.set({4}, true);
//    tensor_1.set({5}, true);
//
//
//    std::vector<raw_subscript> op_sc{{0, 1, 2},
//                                     {0},
//    };
//    raw_subscript res_sc{0};
//
//    std::shared_ptr<const Subscript> subscript = std::make_shared<Subscript>(op_sc, res_sc);
//    const tnt::tensor::einsum::EinsumPlan plan{subscript};
//    const Operands ops{&tensor_0, &tensor_1};
//    tnt::tensor::einsum::EinsumPlan::Step step = plan.getInitialStep(ops);
//    std::cout << step << std::endl;
//    std::vector<size_t> key(1, 100000);
//    Join join{key, ops, step};
//    Join::iterator it = join.begin();
//    Join::iterator anEnd = join.end();
//    while (it != anEnd) {
//        std::cout << *it << std::endl;
//        ++it;
//    }
//}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
