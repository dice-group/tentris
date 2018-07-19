#include <gtest/gtest.h>

#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/tensor/einsum/Subscript.hpp"

using namespace tnt::util::types;
using namespace tnt::tensor::hypertrie;
using namespace tnt::tensor::einsum::operators;
using namespace tnt::tensor::einsum;
template<typename T>
using NDMap = tnt::util::container::NDMap<T>;


TEST(TestEinsum, simple_call) {

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
    
    NDMap<int> result = einsum_op.getResult(operands);

    std::cout << "results:" << std::endl;
    for (const auto &non_zero : result) {
        std::cout << non_zero << std::endl;
    }

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}