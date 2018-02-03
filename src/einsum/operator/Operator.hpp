#ifndef SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP


#include "../Subscript.hpp"
#include "../../tensor/HyperTrieTensor.hpp"
#include <vector>

using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;

namespace sparsetensor::einsum::operators {


    template<typename T>
    class Operator {
    public:
        explicit Operator(const Subscript &subscript) : subscript(subscript) {}

    public:

        Subscript subscript;

        static Operator<T> buildOperatorGraph(Subscript &subscript, std::vector<HyperTrieTensor<T>> &vector) {
            return nullptr;
        }

        virtual Tensor<T> *getResult(const vector<variant<Tensor<T> *, T>> &tensors){
            throw "getResult() must be implemented by inheriting Class.";
        };

    };
}


#endif //SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP
