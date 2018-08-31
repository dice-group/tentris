#ifndef TNT_OPERATORNODE_HPP
#define TNT_OPERATORNODE_HPP

#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"

namespace tnt::tensor::einsum::operators {

    enum OperatorType {
        SLICE,
        EINSUM,
        CROSSPRODUCT
    };

    template<typename RESULT_TYPE>
    class OperatorNode {
    public:
        OperatorType type;
        virtual yield_pull <RESULT_TYPE> get() const = 0;
    };
}


#endif //TNT_OPERATORNODE_HPP
