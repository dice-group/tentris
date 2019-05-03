#include <gtest/gtest.h>

#include <tentris/tensor/einsum/EinsumPlan.hpp>
#include <tentris/tensor/hypertrie/Join.hpp>
#include <tentris/util/FmtHelper.hpp>



namespace {
    using Step = tentris::tensor::einsum::EinsumPlan::Step;
    using namespace tentris::util::types;
    using namespace tentris::tensor::hypertrie;
    using namespace tentris::tensor::einsum;
}


TEST(TestJoin, DISABLED_full_sc_1_10_1___0) {
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
    const EinsumPlan plan{subscript};
    const Operands ops{&tensor_0, &tensor_1, &tensor_2};
    Step step = plan.getInitialStep(ops);
    ASSERT_EQ(step.label, 0);

    fmt::print("{}\n", step);
    std::vector<size_t> key(1);
    Join join{key, ops, step};
    std::vector<Key_t> expected_keys = {{7},
                                        {8},
                                        {9}};
    uint count = 0;
    for (const auto &[actual, expected_key] : zip(join, expected_keys)) {
        ++count;
        const auto &actual_key = actual.second;
        ASSERT_EQ(actual_key, expected_key);

        const auto &actual_operands = actual.first;
        auto expected_operands{ops};
        expected_operands.at(1) = ops.at(1)->getSlice({std::nullopt, expected_key[0]});
        ASSERT_EQ(actual_operands, expected_operands);

        const Step &step_2 = step.nextStep(expected_operands);
        fmt::print("{}\n", step_2);
        Join join_2{actual_key, expected_operands, step_2};

        if (actual_key[0] == 7) {
            Operands expected_operands_2{};

            for (const auto &[actual_operands2, actual_key2] : join_2) {
                ASSERT_EQ(actual_key2, expected_key);
                ASSERT_EQ(actual_operands2, expected_operands_2);
            }
        } else {
            ASSERT_EQ(join_2.begin(), join_2.end());
        }
    }
    ASSERT_EQ(count, 3);
}

TEST(TestJoin, part_sc_01_0___01) {
    std::vector<key_part_t> keys_00 = {7, 12, 14, 21, 26, 45, 47, 54, 59, 71, 72, 73, 74, 83, 85, 86, 87, 89, 94, 95,
                                       100, 106, 114, 119, 125, 134, 138, 139, 142, 145, 146, 152, 154, 157, 158, 159,
                                       160, 161, 162, 163, 164, 167, 168, 169, 170, 171, 172, 173, 175, 176, 233, 375,
                                       376, 377, 378, 382, 391, 397, 398, 399, 405, 406, 407, 409, 413, 416, 420, 425,
                                       442, 443, 451, 455, 466, 467, 497, 503, 506, 513, 516, 519, 522, 525, 529, 534,
                                       537, 541, 554, 560, 561, 562, 565, 568, 570, 571, 572, 573, 574, 576, 577, 578,
                                       579, 580, 581, 582, 585, 615, 616, 627, 638, 639, 675, 682, 684, 692, 693, 695,
                                       696, 699, 722, 723, 724, 735, 736, 739, 751, 755, 761, 762, 763, 764, 765, 772,
                                       776, 784, 786, 792, 810, 811, 812, 813, 814, 822, 824, 826, 830, 837, 842, 850,
                                       851, 863, 869, 884, 886, 888, 914, 991, 992, 995, 997, 999, 1000};
    std::vector<key_part_t> keys_10 = {41, 42, 78, 229, 269, 297, 311, 375, 377, 381, 382, 387, 391, 394, 397, 398, 399,
                                       400, 405, 406, 407, 409, 412, 413, 416, 418, 420, 425, 427, 431, 436, 442, 443,
                                       446, 447, 451, 457, 459, 463, 466, 467, 468, 472, 479, 480, 481, 486, 487, 488,
                                       490, 491, 492, 493, 494, 495, 497, 501, 503, 506, 513, 518, 519, 522, 525, 529,
                                       532, 534, 537, 541, 550, 554, 560, 562, 565, 568, 570, 571, 572, 573, 576, 577,
                                       578, 579, 580, 581, 582, 588, 615, 616, 638, 689, 692, 693, 708, 722, 731, 735,
                                       743, 755, 758, 759, 764, 771, 775, 780, 786, 792, 795, 810, 813, 814, 816, 822,
                                       836, 840, 845, 846, 850, 857, 861, 866, 888, 896, 901, 909, 930, 932, 955, 989};
    std::set<key_part_t> common_keys{};
    std::set_intersection(keys_00.begin(), keys_00.end(), keys_10.begin(), keys_10.end(),
                          std::inserter(common_keys, common_keys.begin()));
    fmt::print("keys_00 :{}\n", keys_00);
    fmt::print("keys_10 :{}\n", keys_10);
    fmt::print("common_keys :{}\n", common_keys);

    BoolHyperTrie tensor_0{2};
    for (const auto &key_part : keys_00) {
        tensor_0.set({key_part, 1}, true);
    }

    BoolHyperTrie tensor_1{1};
    for (const auto &key_part : keys_10) {
        tensor_1.set({key_part}, true);
    }


    std::vector<raw_subscript> op_sc{{0, 1},
                                     {0}};
    raw_subscript res_sc{0, 1};

    std::shared_ptr<const Subscript> subscript = std::make_shared<Subscript>(op_sc, res_sc);
    const EinsumPlan plan{subscript};
    const Operands ops{&tensor_0, &tensor_1};
    Step step = plan.getInitialStep(ops);
    ASSERT_EQ(step.label, 0);

    fmt::print("{}\n", step);
    std::vector<size_t> key(2);
    std::vector<Key_t> expected_keys;
    for (const auto &common_key : common_keys) {
        expected_keys.push_back({common_key, 0});
    }

    Join join{key, ops, step};
    uint count = 0;
    for (const auto &[actual, expected_key] : zip(join, expected_keys)) {
        ++count;
        const auto &actual_key = actual.second;
        ASSERT_EQ(actual_key, expected_key);
        fmt::print("{}\n", actual_key);

        const auto &actual_operands = actual.first;
        Operands expected_operands(1);
        expected_operands.at(0) = ops.at(0)->getSlice({expected_key[0], std::nullopt});
        ASSERT_EQ(actual_operands, expected_operands);

    }
    ASSERT_EQ(count, common_keys.size());
}

TEST(TestJoin, single_op_00__0) {
    std::vector<key_part_t> keys_00 = {7, 12, 14, 21, 26, 45, 47, 54, 59, 71, 72, 73, 74, 83, 85, 86, 87, 89, 94, 95,
                                       100, 106, 114, 119, 125, 134, 138, 139, 142, 145, 146, 152, 154, 157, 158, 159,
                                       160, 161, 162, 163, 164, 167, 168, 169, 170, 171, 172, 173, 175, 176, 233, 375,
                                       376, 377, 378, 382, 391, 397, 398, 399, 405, 406, 407, 409, 413, 416, 420, 425,
                                       442, 443, 451, 455, 466, 467, 497, 503, 506, 513, 516, 519, 522, 525, 529, 534,
                                       537, 541, 554, 560, 561, 562, 565, 568, 570, 571, 572, 573, 574, 576, 577, 578,
                                       579, 580, 581, 582, 585, 615, 616, 627, 638, 639, 675, 682, 684, 692, 693, 695,
                                       696, 699, 722, 723, 724, 735, 736, 739, 751, 755, 761, 762, 763, 764, 765, 772,
                                       776, 784, 786, 792, 810, 811, 812, 813, 814, 822, 824, 826, 830, 837, 842, 850,
                                       851, 863, 869, 884, 886, 888, 914, 991, 992, 995, 997, 999, 1000};
    std::vector<key_part_t> keys_01 = {7, 142, 145, 146, 152, 154, 157, 158, 159,
                                       160, 161, 162, 163, 164, 167, 168, 169, 170, 171, 172, 173, 175, 176, 233, 375,
                                       376, 377, 378, 382, 391, 397, 398, 399, 405, 406, 407, 409, 413, 416, 420, 425,
                                       442, 443, 451, 455, 466, 467, 639, 675, 682, 684, 692, 693, 695,
                                       696, 699, 722, 723, 724, 735, 736, 739, 751, 995, 997, 999};


    fmt::print("keys_00 :{}\n", keys_00);
    fmt::print("keys_01 :{}\n", keys_01);

    BoolHyperTrie tensor_0{2};
    for (const auto &key_part0 : keys_00) {
        for (const auto &key_part1: keys_01)
            tensor_0.set({key_part0, key_part1}, true);
    }

    std::vector<raw_subscript> op_sc{{0, 0}};
    raw_subscript res_sc{0};

    std::shared_ptr<const Subscript> subscript = std::make_shared<Subscript>(op_sc, res_sc);
    const EinsumPlan plan{subscript};
    const Operands ops{&tensor_0,};
    Step step = plan.getInitialStep(ops);
    ASSERT_EQ(step.label, 0);

    fmt::print("{}\n", step);
    std::vector<size_t> key(1);
    std::vector<Key_t> expected_keys;
    for (const auto &key_part : keys_01) {
        expected_keys.push_back({key_part});
    }

    Join join{key, ops, step};
    uint count = 0;
    for (const auto &[actual, expected_key] : zip(join, expected_keys)) {
        ++count;
        const auto &actual_key = actual.second;
        fmt::print("{}\n", actual_key);
        ASSERT_EQ(actual_key, expected_key);

        const auto &actual_operands = actual.first;
        Operands expected_operands(0);
        ASSERT_EQ(actual_operands, expected_operands);
    }
    ASSERT_EQ(count, keys_01.size());
}

TEST(TestJoin, single_op_01__0) {
    std::vector<key_part_t> keys_00 = {7, 142, 145, 146, 152, 154, 157, 158, 159,
                                       160, 161, 162, 163, 164, 167, 168, 169, 170, 171, 172, 173, 175, 176, 233, 375,
                                       376, 377, 378, 382, 391, 397, 398, 399, 405, 406, 407, 409, 413, 416, 420, 425,
                                       442, 443, 451, 455, 466, 467, 639, 675, 682, 684, 692, 693, 695,
                                       696, 699, 722, 723, 724, 735, 736, 739, 751, 995, 997, 999};
    std::vector<key_part_t> keys_01 = {7, 12, 14, 21, 26, 45, 47, 54, 59, 71, 72, 73, 74, 83, 85, 86, 87, 89, 94, 95,
                                       100, 106, 114, 119, 125, 134, 138, 139, 142, 145, 146, 152, 154, 157, 158, 159,
                                       160, 161, 162, 163, 164, 167, 168, 169, 170, 171, 172, 173, 175, 176, 233, 375,
                                       376, 377, 378, 382, 391, 397, 398, 399, 405, 406, 407, 409, 413, 416, 420, 425,
                                       442, 443, 451, 455, 466, 467, 497, 503, 506, 513, 516, 519, 522, 525, 529, 534,
                                       537, 541, 554, 560, 561, 562, 565, 568, 570, 571, 572, 573, 574, 576, 577, 578,
                                       579, 580, 581, 582, 585, 615, 616, 627, 638, 639, 675, 682, 684, 692, 693, 695,
                                       696, 699, 722, 723, 724, 735, 736, 739, 751, 755, 761, 762, 763, 764, 765, 772,
                                       776, 784, 786, 792, 810, 811, 812, 813, 814, 822, 824, 826, 830, 837, 842, 850,
                                       851, 863, 869, 884, 886, 888, 914, 991, 992, 995, 997, 999, 1000};

    fmt::print("keys_00 :{}\n", keys_00);
    fmt::print("keys_01 :{}\n", keys_01);

    BoolHyperTrie tensor_0{2};
    for (const auto &key_part0 : keys_00) {
        for (const auto &key_part1: keys_01)
            tensor_0.set({key_part0, key_part1}, true);
    }

    std::vector<raw_subscript> op_sc{{0, 1}};
    raw_subscript res_sc{0};

    std::shared_ptr<const Subscript> subscript = std::make_shared<Subscript>(op_sc, res_sc);
    const EinsumPlan plan{subscript};
    const Operands ops{&tensor_0,};
    Step step = plan.getInitialStep(ops);
    ASSERT_EQ(step.label, 0);

    fmt::print("{}\n", step);
    std::vector<size_t> key(1);
    std::vector<Key_t> expected_keys;
    for (const auto &key_part : keys_00) {
        expected_keys.push_back({key_part});
    }

    Join join{key, ops, step};
    uint count = 0;
    for (const auto &[actual, expected_key] : zip(join, expected_keys)) {
        ++count;
        const auto &actual_key = actual.second;
        fmt::print("{}\n", actual_key);
        ASSERT_EQ(actual_key, expected_key);

        const auto &actual_operands = actual.first;
        Operands expected_operands(1);
        expected_operands.at(0) = ops.at(0)->getSlice({expected_key[0], std::nullopt});
        ASSERT_EQ(actual_operands, expected_operands);
    }
    ASSERT_EQ(count, keys_00.size());
}

TEST(TestJoin, single_op_0__0) {
    std::vector<key_part_t> keys_00 = {7, 12, 14, 21, 26, 45, 47, 54, 59, 71, 72, 73, 74, 83, 85, 86, 87, 89, 94, 95,
                                       100, 106, 114, 119, 125, 134, 138, 139, 142, 145, 146, 152, 154, 157, 158, 159,
                                       160, 161, 162, 163, 164, 167, 168, 169, 170, 171, 172, 173, 175, 176, 233, 375,
                                       376, 377, 378, 382, 391, 397, 398, 399, 405, 406, 407, 409, 413, 416, 420, 425,
                                       442, 443, 451, 455, 466, 467, 497, 503, 506, 513, 516, 519, 522, 525, 529, 534,
                                       537, 541, 554, 560, 561, 562, 565, 568, 570, 571, 572, 573, 574, 576, 577, 578,
                                       579, 580, 581, 582, 585, 615, 616, 627, 638, 639, 675, 682, 684, 692, 693, 695,
                                       696, 699, 722, 723, 724, 735, 736, 739, 751, 755, 761, 762, 763, 764, 765, 772,
                                       776, 784, 786, 792, 810, 811, 812, 813, 814, 822, 824, 826, 830, 837, 842, 850,
                                       851, 863, 869, 884, 886, 888, 914, 991, 992, 995, 997, 999, 1000};

    fmt::print("keys_00 :{}\n", keys_00);

    BoolHyperTrie tensor_0{1};
    for (const auto &key_part0 : keys_00) {
            tensor_0.set({key_part0}, true);
    }

    std::vector<raw_subscript> op_sc{{0}};
    raw_subscript res_sc{0};

    std::shared_ptr<const Subscript> subscript = std::make_shared<Subscript>(op_sc, res_sc);
    const EinsumPlan plan{subscript};
    const Operands ops{&tensor_0,};
    Step step = plan.getInitialStep(ops);
    ASSERT_EQ(step.label, 0);

    fmt::print("{}\n", step);
    std::vector<size_t> key(1);
    std::vector<Key_t> expected_keys;
    for (const auto &key_part : keys_00) {
        expected_keys.push_back({key_part});
    }

    Join join{key, ops, step};
    uint count = 0;
    for (const auto &[actual, expected_key] : zip(join, expected_keys)) {
        ++count;
        const auto &actual_key = actual.second;
        fmt::print("{}\n", actual_key);
        ASSERT_EQ(actual_key, expected_key);

        const auto &actual_operands = actual.first;
        Operands expected_operands(0);
        ASSERT_EQ(actual_operands, expected_operands);
    }
    ASSERT_EQ(count, keys_00.size());
}

