#ifndef SPARSETENSOR_EINSUM_OPERATOR_RESULT
#define SPARSETENSOR_EINSUM_OPERATOR_RESULT

#include <algorithm>
#include <numeric>
#include <vector>
#include <memory>

#include "../../hypertrie/BoolHyperTrie.hpp"
#include "../../hypertrie/Join.hpp"
#include "Einsum.hpp"
#include "CrossProduct.hpp"
#include "../EinsumPlan.hpp"
#include "../../../util/container/NDMap.hpp"
#include "../../../util/All.hpp"

namespace tnt::tensor::einsum::operators {

    template<typename T>
    class Result {
        using BoolHyperTrie = tnt::tensor::hypertrie::BoolHyperTrie;
        using Join = tnt::tensor::hypertrie::Join;
        template<typename V>
        using NDMap = tnt::util::container::NDMap<V>;
        using NewJoin = tnt::tensor::hypertrie::Join;
        using Operands = tnt::tensor::hypertrie::Operands;
    public:

        const Subscript _subscript;

        /**
         * Basic Constructor.
         * @param subscript Subscript that defines what the operator does.
         */
        Result(const Subscript &subscript) : _subscript{subscript.optimized()} {}


        const NDMap<T> &getResult(const Operands &operands) {
            const std::vector<std::shared_ptr<Subscript>> &sub_subscripts = _subscript.getSubSubscripts();
            if (sub_subscripts.size()) {
                return CrossProduct<T>{_subscript}.getResult(operands);
            } else {
                return Einsum<T>{_subscript}.getResult(operands);
            }
        }
    };
};

#endif //SPARSETENSOR_EINSUM_OPERATOR_RESULT
