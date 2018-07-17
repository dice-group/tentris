#include <gtest/gtest.h>


#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"

using namespace tnt::util::types;
using namespace tnt::tensor::einsum;

TEST(TestSubscript, print_and_check_norming) {
    std::vector<std::vector<label_t >> raw_op_sc{
            {3, 2, 1},
            {1, 2, 3, 2},
            {0}
    };
    std::vector<label_t> raw_res_sc{3, 0};

    std::cout << "raw_op_labels:\n\t" << raw_op_sc << std::endl;
    std::cout << "raw_res_labels:\n\t" << raw_res_sc << std::endl;

    Subscript sc{raw_op_sc, raw_res_sc};
    std::cout << sc << std::endl;
    // check all labels
    const std::set<label_t> all_labels{0, 1, 2, 3};
    ASSERT_EQ(all_labels, sc.getAllLabels());

    const std::vector<label_t> result_labels{0, 1};
    ASSERT_EQ(result_labels, sc.getResultLabels());

    const std::vector<std::vector<label_t>> ops_labels{{0, 2, 3},
                                                       {3, 2, 0, 2},
                                                       {1}};
    for (const auto &[op_pos, op_labels]: enumerate(ops_labels)) {
        ASSERT_EQ(op_labels, sc.operandLabels(op_pos));
    }


//    sc.getResultLabels()

//
//    vector<label_t> op_labels0{0, 1, 2};
//    ASSERT_TRUE(op_labels0 == sc.getOperandsLabels().at(0));
//
//    vector<label_t> op_labels1{2, 1, 0, 1};
//    ASSERT_TRUE(op_labels1 == sc.getOperandsLabels().at(1));
//
//    vector<label_t> op_labels2{3};
//    ASSERT_TRUE(op_labels2 == sc.getOperandsLabels().at(2));
//
//    vector<label_t> res_labels{0, 3};
//    ASSERT_TRUE(res_labels == sc.getResultLabels());
}

//TEST(TestSubscript, optimize) {
//    using namespace ::tnt::operations;
//    // data
//    vector<vector<label_t >> raw_op_sc{
//            {3, 2, 1},
//            {1, 2, 3, 2},
//            {0},
//            { 5,6,7}
//    };
//
//    vector<label_t> raw_res_sc{3, 0};
//
//    // print
//    std::cout << "raw_op_labels:\n\t" << raw_op_sc << std::endl;
//
//    std::cout << "raw_res_labels:\n\t" << raw_res_sc << std::endl;
//
//
//    Subscript opt_sc = Subscript{raw_op_sc, raw_res_sc}.bracketCrossproductFactors();
//
//    std::cout << opt_sc << std::endl;
//
//    for (auto entry : opt_sc.getSubSubscripts()) {
//        std::cout << entry.second << std::endl;
//    }
//
//}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
