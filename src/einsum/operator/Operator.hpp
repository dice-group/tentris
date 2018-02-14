#ifndef SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP

#include "../Subscript.hpp"

using sparsetensor::einsum::Subscript;

namespace sparsetensor::einsum::operators {


    /**
     * Abstract superclass to Operators that process the einsum.
     * They are initialized by an subscript and can later be applied to any input tensors that satisfy the subscript.
     * @tparam T type of the values hold by processed Tensors.
     * @tparam IN_TENSOR type of the tensor that is accepted as inputs by the operator.
     * @tparam OUT_TENSOR type if the tensor that is output by the operator as result.
     */
    template<typename T, template<typename> class IN_TENSOR,
            template<typename> class OUT_TENSOR>
    class Operator {
    public:
        /**
         * Subscript that defines what the operator does.
         */
        const Subscript &subscript;

        /**
         * Constructor.
         * @param subscript Subscript that defines what the operator does.
         */
        explicit Operator(const Subscript &subscript) :
                subscript(subscript) {}

        /**
         * Takes in a vector of pointers to operand tensors and returns an result. The Memory for the result was
         * allocated and must be freed manually.
         * @param operands operands to be processed
         * @return result of processing
         */
        OUT_TENSOR<T> *getResult(const vector<IN_TENSOR<T> *> &operands);
    };

}


#endif //SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP
