#include <gtest/gtest.h>
#include <tnt/tensor/hypertrie/PosCalc.hpp>

using namespace tnt::tensor::hypertrie;

TEST(TestPosCalc, every_different_instance_must_exist_only_once) {
    std::vector<bool> vec1 = std::vector<bool>(3);

    PosCalc const *inst000 = PosCalc::getInstance(vec1);
    PosCalc const *inst000x = PosCalc::getInstance(vec1);

    ASSERT_EQ(inst000, inst000x
    );

    PosCalc const *inst010 = inst000x->use(1);

    vec1[1] = true;
    PosCalc const *inst010x = PosCalc::getInstance(vec1);

    ASSERT_EQ(inst010, inst010x
    );
}

TEST(TestPosCalc, check_values) {
    std::vector<bool> vec1 = std::vector<bool>(3);

    PosCalc const *inst000 = PosCalc::getInstance(vec1);
    PosCalc const *inst100 = inst000->use(0);
    ASSERT_EQ(inst100->key_to_subkey_pos(1), 0);
    ASSERT_EQ(inst100->key_to_subkey_pos(2), 1);

    ASSERT_EQ(inst100->subkey_to_key_pos(0), 1);
    ASSERT_EQ(inst100->subkey_to_key_pos(1), 2);

    PosCalc const *inst010 = inst000->use(1);
    ASSERT_EQ(inst010->key_to_subkey_pos(0), 0);
    ASSERT_EQ(inst010->key_to_subkey_pos(2), 1);

    ASSERT_EQ(inst010->subkey_to_key_pos(0), 0);
    ASSERT_EQ(inst010->subkey_to_key_pos(1), 2);
}

