#include <gtest/gtest.h>

#include "tnt/tensor/einsum/EinsumPlan.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/hypertrie/Join.hpp"

using namespace tnt::util::types;
using namespace tnt::tensor::hypertrie;
using namespace tnt::tensor::einsum;


TEST(TestJoin, simple_call) {
    BoolHyperTrie tensor_0{1};
    tensor_0.set({0}, true);
    tensor_0.set({1}, true);
    tensor_0.set({2}, true);

    BoolHyperTrie tensor_1{2};
    tensor_1.set({2,7}, true);
    tensor_1.set({3,8}, true);
    tensor_1.set({4,9}, true);

    BoolHyperTrie tensor_2{1};
    tensor_2.set({2}, true);
    tensor_2.set({6}, true);



    std::vector <raw_subscript> op_sc{{1},
                                 {1,0},
                                 {1}};
    raw_subscript res_sc{0};

    Subscript subscript{op_sc, res_sc};
    const tnt::tensor::einsum::EinsumPlan plan{subscript};
    const Operands ops{&tensor_0, &tensor_1, &tensor_2};
    tnt::tensor::einsum::EinsumPlan::Step step = plan.getInitialStep(ops);
    std::cout << step << std::endl;
    std::vector<size_t> key(1, 100000);
    Join join{key, ops, step};
    Join::iterator it = join.begin();
    Join::iterator anEnd = join.end();
    while(it != anEnd){
        std::cout << *it << std::endl;
        ++it;
    }

    






}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
