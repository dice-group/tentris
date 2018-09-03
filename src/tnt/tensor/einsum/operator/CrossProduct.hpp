#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include <vector>
#include <tuple>

#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/util/All.hpp"

#include "tnt/tensor/einsum/operator/OperatorNode.hpp"

namespace {
    using namespace tnt::tensor::hypertrie;
    using namespace tnt::util::types;
    using Join = tnt::tensor::hypertrie::Join;
}

namespace tnt::tensor::einsum::operators {
    /**
     * This is a cross product operator that takes in any number of operands and maps every of their dimensions into an
     * result dimension. The Mapping is defind by an sparsetensor::einsum::Subscript.
     * Example of an cross product: ij,kl->il <=> m,n->mn with m = (ij->i)  and n = (kl->l)
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename RESULT_TYPE>
    class CrossProduct : public OperatorNode<RESULT_TYPE> {
        using Operands = tnt::tensor::hypertrie::Operands;
    protected:
        /**
         * Every sub-Subscript (Subscript) is calculated by an Einsum Operator.
         */
        std::vector<std::shared_ptr<OperatorNode < RESULT_TYPE>>> predecessors{};
    public:
        /**
         * Constructor
         * @param subscript the passed Subscript must represent the dimension layout of the HyperTrieTenors (HyperTrieTenor)
         * that will be passed to getResult() . It will be bracketing out cross product factors will be done internally.
         */
        CrossProduct(const std::shared_ptr<const Subscript> subscript, const std::vector<SliceKey_t> &slice_keys,
                     const std::vector<BoolHyperTrie *> &tries)
                : OperatorNode<RESULT_TYPE>{} {
            this->type = OperatorType::CROSSPRODUCT;
            auto[sub_slice_keys, sub_tries] = extractSliceKeysAndTries(subscript, slice_keys, tries);
            for (const auto &[sub_subscript, sub_slice_key, sub_tries] : zip(subscript->getSubSubscripts(),
                                                                             sub_slice_keys, sub_tries)) {
                predecessors.emplace_back(std::shared_ptr<OperatorNode<RESULT_TYPE>>{
                        new Einsum<RESULT_TYPE>{sub_subscript, sub_slice_key, sub_tries}});
            }
        }

        std::tuple<std::vector<std::vector<SliceKey_t>>, std::vector<std::vector<BoolHyperTrie *>>>
        extractSliceKeysAndTries(const std::shared_ptr<const Subscript> subscript,
                                 const std::vector<SliceKey_t> &slice_keys,
                                 const std::vector<BoolHyperTrie *> &tries) {
            // TODO: implement
        }

        yield_pull <RESULT_TYPE> get() const override {
            // TODO: move the code from Home PC here
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
