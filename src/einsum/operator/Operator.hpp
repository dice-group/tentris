#ifndef SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP

#include "../Subscript.hpp"

using sparsetensor::operations::Subscript;

namespace sparsetensor::operations::operators {


    /**
     * Abstract superclass to Operators that process the einsum.
     * They are initialized by an subscript and can later be applied to any input tensors that satisfy the subscript.
     * @tparam T type of the values hold by processed Tensors (Tensor).
     * @tparam IN_TENSOR type of the tensor that is accepted as inputs by the operator.
     * @tparam OUT_TENSOR type if the tensor that is output by the operator as result.
     */
    template<typename IN_TYPE,
            typename OUT_TYPE>
    class Operator {
    public:

        template<typename IN_TYPE_>
        void setInputOperators(const vector<Operator<IN_TYPE_, IN_TYPE>*> input_operators);
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
        OUT_TYPE *getResult();
    };

}


#endif //SPARSETENSOR_EINSUM_OPERATOR_OPERATOR_HPP
