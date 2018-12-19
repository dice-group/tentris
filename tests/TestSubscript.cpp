#include <gtest/gtest.h>


#include <tnt/tensor/hypertrie/BoolHyperTrie.hpp>
#include <tnt/tensor/einsum/operator/Einsum.hpp>
#include <tnt/util/FmtHelper.hpp>

namespace {
    using namespace tnt::util::types;
    using namespace tnt::tensor::einsum;
}


TEST(TestSubscript, check_simple_things) {
    std::vector<std::vector<label_t >> raw_op_sc{
            {3, 2, 1},
            {1, 2, 3, 2},
            {0}
    };
    std::vector<label_t> raw_res_sc{3, 0};
    fmt::print("raw_op_labels\n"
               "\t{}", raw_op_sc);
    fmt::print("raw_res_sc\n"
               "\t{}", raw_res_sc);

    Subscript sc{raw_op_sc, raw_res_sc};
    fmt::print("subscript\n"
               "\t{}",
               sc);
    // check all labels
    const std::set<label_t> all_labels{0, 1, 2, 3};
    ASSERT_EQ(all_labels, sc.getAllLabels());
    // check result labels
    const std::vector<label_t> result_labels{0, 1};
    ASSERT_EQ(result_labels, sc.getResultLabels());
    // check ops labels
    const std::vector<std::vector<label_t>> ops_labels{{0, 2, 3},
                                                       {3, 2, 0, 2},
                                                       {1}};
    for (const auto &[op_pos, op_labels]: enumerate(ops_labels)) {
        ASSERT_EQ(op_labels, sc.operandLabels(op_pos));
    }
    // there is no additional op
    ASSERT_THROW(sc.operandLabels(ops_labels.size()), std::exception);

    for (const auto &[pos, label] : enumerate(result_labels)) {
        ASSERT_EQ(sc.labelPosInResult(label), pos);
    }

    // check independent subsets
    std::vector<std::set<label_t>> independentLabelSubsets = sc.getIndependentLabelSubsets();
    ASSERT_EQ(independentLabelSubsets.size(), 2);
    size_t smaller_i;
    size_t larger_i;
    if (independentLabelSubsets[0].size() < independentLabelSubsets[1].size()) {
        smaller_i = 0;
        larger_i = 1;
    } else {
        smaller_i = 1;
        larger_i = 0;
    }
    std::set<label_t> small_ind_set = {label_t(1)};
    std::set<label_t> large_ind_set = {label_t(2), label_t(0), label_t(3)};
    ASSERT_EQ(independentLabelSubsets[smaller_i], small_ind_set);
    ASSERT_EQ(independentLabelSubsets[larger_i], large_ind_set);


}

TEST(TestSubscript, check_non_result_single_operand_labels) {
    std::vector<std::vector<label_t >> raw_op_sc{
            {3, 2, 1, 4, 4},
            {1, 2, 1, 3, 6, 2},
            {8, 8, 0, 8}
    };
    std::vector<label_t> raw_res_sc{3, 0};

    fmt::print("raw_op_labels\n"
               "\t{}\n", raw_op_sc);
    fmt::print("raw_res_sc\n"
               "\t{}\n", raw_res_sc);

    Subscript sc{raw_op_sc, raw_res_sc};
    fmt::print("subscript\n"
               "\t{}\n",
               sc);

    std::map<op_pos_t, std::vector<std::vector<label_pos_t>>> uniqueContr = sc.getUniqueNonResultContractions();
    for (const auto &[op_pos, contractions] : uniqueContr) {

        const std::vector<label_pos_t> &operand_labels = sc.operandLabels(op_pos);

        for (const std::vector<label_pos_t> &contraction : contractions) {
            label_pos_t first_label_pos = contraction.at(0);

            const label_t &label = operand_labels.at(first_label_pos);
            ASSERT_THROW(sc.labelPosInResult(label), std::out_of_range);


            for (const label_pos_t &label_pos : contraction) {
                ASSERT_EQ(label, operand_labels.at(label_pos));
            }

            for (const label_pos_t &label_pos : range(label_pos_t(operand_labels.size()))) {
                if (std::find(contraction.begin(), contraction.end(), label_pos) == contraction.end()) {
                    ASSERT_NE(operand_labels.at(label_pos), label);
                }
            }
        }
    }
    fmt::print("uniqueContr {}\n", uniqueContr);

    std::map<op_pos_t, std::vector<label_pos_t>> lonelyContr = sc.getLonleyNonResultContractions();
    for (const auto &[op_pos, contractions] : lonelyContr) {

        const std::vector<label_pos_t> &operand_labels = sc.operandLabels(op_pos);
        for (const label_pos_t &label_pos : contractions) {
            const label_t &label = operand_labels.at(label_pos);
            ASSERT_THROW(sc.labelPosInResult(label), std::out_of_range);
        }
    }
    fmt::print("lonelyContr {}\n", lonelyContr);

}

// TODO: make a real testcase out of it
TEST(TestSubscript, remove_label) {
    std::vector<std::vector<label_t >> raw_op_sc{
            {3, 0, 1},
            {1, 2, 3, 2},
            {0}
    };
    std::vector<label_t> raw_res_sc{0, 1, 2};

    fmt::print("raw_op_labels\n"
               "\t{}\n", raw_op_sc);
    fmt::print("raw_res_sc\n"
               "\t{}\n", raw_res_sc);

    Subscript sc{raw_op_sc, raw_res_sc};
    fmt::print("subscript\n"
               "\t{}\n",
               sc);

    const std::shared_ptr<const Subscript> sc_wo0 = sc.removeLabel(0);

    fmt::print("subscript\n"
               "\t{}\n",
               *sc_wo0);
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

