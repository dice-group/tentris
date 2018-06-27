#include <gtest/gtest.h>


//TEST(TestSubscript, print_and_check_norming) {
//    using namespace ::sparsetensor::operations;
//    // data
//    vector<vector<label_t >> raw_op_sc{
//            {3, 2, 1},
//            {1, 2, 3, 2},
//            {0}
//    };

//    // print
//    std::cout << "raw_op_labels:\n\t" << raw_op_sc << std::endl;

//    vector<label_t> raw_res_sc{3, 0};
//    std::cout << "raw_res_labels:\n\t" << raw_res_sc << std::endl;

//    Subscript sc{raw_op_sc, raw_res_sc};
//    std::cout << sc << std::endl;

//    // check normed labels
//    auto all_labels = std::unordered_set<label_t>{0, 1, 2, 3};
//    ASSERT_TRUE(sc.getAllLabels() == all_labels);

//    vector<label_t> op_labels0{0, 1, 2};
//    ASSERT_TRUE(op_labels0 == sc.getOperandsLabels().at(0));

//    vector<label_t> op_labels1{2, 1, 0, 1};
//    ASSERT_TRUE(op_labels1 == sc.getOperandsLabels().at(1));

//    vector<label_t> op_labels2{3};
//    ASSERT_TRUE(op_labels2 == sc.getOperandsLabels().at(2));

//    vector<label_t> res_labels{0, 3};
//    ASSERT_TRUE(res_labels == sc.getResultLabels());
//}

//TEST(TestSubscript, optimize) {
//    using namespace ::sparsetensor::operations;
//    // data
//    vector<vector<label_t >> raw_op_sc{
//            {3, 2, 1},
//            {1, 2, 3, 2},
//            {0},
//            { 5,6,7}
//    };

//    vector<label_t> raw_res_sc{3, 0};

//    // print
//    std::cout << "raw_op_labels:\n\t" << raw_op_sc << std::endl;

//    std::cout << "raw_res_labels:\n\t" << raw_res_sc << std::endl;


//    Subscript opt_sc = Subscript{raw_op_sc, raw_res_sc}.bracketCrossproductFactors();

//    std::cout << opt_sc << std::endl;

//    for (auto entry : opt_sc.getSubSubscripts()) {
//        std::cout << entry.second << std::endl;
//    }

//}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
