#ifndef SPARSETENSOR_EINSUM_OPERATOR_RESULT
#define SPARSETENSOR_EINSUM_OPERATOR_RESULT

#include <algorithm>
#include <numeric>
#include <vector>
#include <memory>

#include "Einsum.hpp"
#include "CrossProduct.hpp"
#include "../../hypertrie/BoolHyperTrie.hpp"
#include "../../einsum/EinsumPlan.hpp"
#include "../../hypertrie/Join.hpp"
#include "../../container/NDMap.hpp"
#include "../../util/All.hpp"

namespace sparsetensor::operations::operators {

    template<typename T>
    class Result {
        using namespace sparsetensor::util::types;
        using BoolHyperTrie = sparsetensor::hypertrie::BoolHyperTrie;
        using Join = sparsetensor::hypertrie::Join;
        template<typename V>
        using NDMap = sparsetensor::container::NDMap<V>;
        using NewJoin = sparsetensor::hypertrie::Join;
        using Operands = sparsetensor::hypertrie::Operands;
    public:

        const Subscript &_subscript;

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
