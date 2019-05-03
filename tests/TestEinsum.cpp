#include <gtest/gtest.h>

#include <tentris/tensor/hypertrie/BoolHyperTrie.hpp>
#include <tentris/tensor/einsum/operator/Einsum.hpp>

using namespace tentris::util::types;
using namespace tentris::tensor::hypertrie;
using namespace tentris::tensor::einsum::operators;
using namespace tentris::tensor::einsum;

//TODO: reenable

TEST(TestEinsum, DISABLED_tupid) {
    const Subscript &subscript = Subscript{};
    const std::vector<SliceKey_t> &vector1 = std::vector<SliceKey_t>{};
    const std::vector<BoolHyperTrie *> &vector2 = std::vector<BoolHyperTrie *>{};
    Einsum<std::tuple<Key_t, size_t>> asd{subscript, vector1, vector2};
}


TEST(TestEinsum, DISABLED_simple_call) {

    BoolHyperTrie tensor_0{2};
    tensor_0.set({0, 0}, true);
    tensor_0.set({0, 1}, true);
    tensor_0.set({1, 0}, true);
    tensor_0.set({1, 1}, true);

    BoolHyperTrie tensor_1{2};
    tensor_1.set({0, 0}, true);
    tensor_1.set({0, 1}, true);
    tensor_1.set({1, 0}, true);
    tensor_1.set({1, 1}, true);

    std::vector<BoolHyperTrie *> operands{&tensor_0, &tensor_1};

    std::vector<raw_subscript> op_sc{{0, 1},
                                     {1, 2}};
    raw_subscript res_sc{0, 2};

    const Subscript &subscript = Subscript{op_sc, res_sc};
    std::cout << subscript << std::endl;
    Einsum<int> einsum_op{subscript};

    auto result_generator = einsum_op.get();

    std::cout << "results:" << std::endl;
    std::set<Key_t> actual_keys;
    for (const auto &[key, value] : result_generator) {
        ASSERT_EQ(value, 2);
        actual_keys.insert(key);
        std::cout << key << " : " << value << std::endl;
    }
    // check that there are no duplicated keys
    std::set<Key_t> expected_keys{
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1}
    };
    ASSERT_EQ(actual_keys, expected_keys);

}

TEST(TestEinsum, DISABLED_simple_call2) {

    BoolHyperTrie tensor_0{2};
//    tensor_0.set({0, 0}, false);
    tensor_0.set({0, 1}, true);
    tensor_0.set({1, 0}, true);
    tensor_0.set({1, 1}, true);


    std::vector<BoolHyperTrie *> operands{&tensor_0};

    std::vector<raw_subscript> op_sc{{0, 0},
    };
    raw_subscript res_sc{0};

    const Subscript &subscript = Subscript{op_sc, res_sc};
    std::cout << subscript << std::endl;
    Einsum<int> einsum_op{subscript};

    auto result_generator = einsum_op.get();

    std::cout << "results:" << std::endl;
    std::set<Key_t> actual_keys;
    for (const auto &[key, value] : result_generator) {
        actual_keys.insert(key);
        std::cout << key << " : " << value << std::endl;
    }

//    ASSERT_EQ(result.count({0}), 0);
//    ASSERT_EQ(result.count({1}), 1);
    // check that there are no duplicated keys
//    std::set<Key_t > expected_keys{
//            {0,0}, {0,1}, {1,0}, {1,1}
//    };
//    ASSERT_EQ(actual_keys, expected_keys);

}
