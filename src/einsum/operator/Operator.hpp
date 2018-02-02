//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_OPERATOR_HPP
#define LIBSPARSETENSOR_OPERATOR_HPP


#include "../Subscript.hpp"
#include "../../tensor/HyperTrieTensor.hpp"
#include <vector>

namespace sparsetensor::einsum::Operator {


    template<typename T>

    class Operator {
    public:
        Operator(Subscript &subscript) : subscript(subscript) {}

    public:

        Subscript subscript;

        static Operator buildOperatorGraph(Subscript &subscript, std::vector<HyperTrieTensor < T>>

        &vector);

        Tensor <T> *getResult(vector<HyperTrieTensor < T>>

        tensors);
    };
}


#endif //LIBSPARSETENSOR_OPERATOR_HPP
