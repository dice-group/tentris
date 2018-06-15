
#include "einsum/Einsum.hpp"

using sparsetensor::tensor::shape_t;
using sparsetensor::operations::raw_subscript;
using sparsetensor::operations::einsum;


using std::vector;

int main() {
    shape_t shape{2, 2};

    HyperTrieTensor<int> tensor_0{shape};
    tensor_0.set({0, 0}, 1);
    tensor_0.set({0, 1}, 2);
    tensor_0.set({1, 0}, 3);
    tensor_0.set({1, 1}, 5);

    HyperTrieTensor<int> tensor_1{shape};
    tensor_1.set({0, 0}, 7);
    tensor_1.set({0, 1}, 11);
    tensor_1.set({1, 0}, 13);
    tensor_1.set({1, 1}, 17);

    vector<HyperTrieTensor<int> *> operands{&tensor_0, &tensor_1};

    vector<raw_subscript> op_sc{{0, 1},
                                {1, 2}};
    raw_subscript res_sc{0, 2};

    CrossProductTensor<int> *const result = einsum<int>(operands, op_sc, res_sc);
    for (const auto &non_zero : *result) {
        std::cout << non_zero << std::endl;
    }
}