//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_OPERATOR_HPP
#define LIBSPARSETENSOR_OPERATOR_HPP


#include "../Subscript.hpp"
#include "../../tensor/MapTensor.hpp"
#include "../../tensor/HyperTrieTensor.hpp"
#include <vector>

template<typename T>

class Operator {

public:

    static Operator buildOperatorGraph(Subscript &subscript, std::vector<HyperTrieTensor<T>> &vector);

    MapTensor<T> *getResult();
};


#endif //LIBSPARSETENSOR_OPERATOR_HPP
