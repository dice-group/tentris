#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include <vector>
#include <tuple>

#include "Einsum.hpp"


namespace sparsetensor::operations::operators {

    /**
     * This is a cross product operator that takes in any number of operands and maps every of their dimensions into an
     * result dimension. The Mapping is defind by an sparsetensor::einsum::Subscript.
     * Example of an cross product: ij,kl->il <=> m,n->mn with m = (ij->i)  and n = (kl->l)
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename T>
    class CrossProduct {
        using Operands = sparsetensor::hypertrie::Operands;
        template<typename V>
        using NDMap = sparsetensor::container::NDMap<V>;
    protected:
        /**
         * This is a subscript where independently calculable parts are bracketed into sub-Subscripts (Subscript).
         */
        const Subscript &_subscript;
        /**
         * Every sub-Subscript (Subscript) is calculated by an Einsum Operator.
         */
        std::vector<Einsum < T>> predecessors{};
    public:
        /**
         * Constructor
         * @param subscript the passed Subscript must represent the dimension layout of the HyperTrieTenors (HyperTrieTenor)
         * that will be passed to getResult() . It will be bracketing out cross product factors will be done internally.
         */
        explicit CrossProduct(const Subscript &subscript) : _subscript{subscript} {
            for (const auto &[op_id, sub_subscript] : subscript.getSubSubscripts()) {
                predecessors.push_back({sub_subscript});
            }
        }


        NDMap<T> getResult(const Operands &operands) {
            std::vector<NDMap<T>> predecessor_results{};
            predecessor_results.reserve(predecessors.size());

            // TODO: make parallel
            // get results from all predecessors
            for (size_t i : range(predecessors.size())) {
                // get the result
                predecessor_results.emplace_back(predecessors.at(i).getResult(operands));

                // if one of the results is 0 the cross product will be zero. So no more calculation is needed.
                if (not predecessor_results.at(i).size()) {
                    // TODO: when parallel -> cancel all other threads.
                    return {};
                }
            }
            return {};
            // return new CrossProductTensor<T>(predecessor_results, bracketed_subscript);
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
