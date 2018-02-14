#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include <vector>
#include <tuple>
#include "Operator.hpp"
#include "Einsum.hpp"
#include "../../tensor/CrossProductTensor.hpp"

using std::vector;
using std::tuple;

using sparsetensor::tensor::CrossProductTensor;
using sparsetensor::tensor::MapTensor;


namespace sparsetensor::einsum::operators {

    /**
     * This is a cross product operator that takes in any number of operands and maps every of their dimensions into an
     * result dimension. The Mapping is defind by an sparsetensor::einsum::Subscript.
     * Example of an cross product: ij,kl->il <=> m,n->mn with m = (ij->i)  and n = (kl->l)
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename T>
    class CrossProduct : public Operator<T, HyperTrieTensor, CrossProductTensor> {
    protected:
        /**
         * This is a subscript where independently calculable parts are bracketed into sub-Subscripts (Subscript).
         */
        const Subscript bracketed_subscript;
        /**
         * Every sub-Subscript (Subscript) is calculated by an Einsum Operator.
         */
        vector<Einsum<T>> predecessors{};
    public:
        /**
         * Constructor
         * @param subscript the passed Subscript must represent the dimension layout of the HyperTrieTenors (HyperTrieTenor)
         * that will be passed to getResult() . It will be bracketing out cross product factors will be done internally.
         */
        explicit CrossProduct(const Subscript &subscript) :
                Operator<T, HyperTrieTensor, CrossProductTensor>{subscript},
                bracketed_subscript(Subscript{subscript}.bracketCrossproductFactors()) {
            // initialize predecessor Einsums with sub-Subscripts
            for (const auto &
            [op_id, sub_subscript] : bracketed_subscript.getSubSubscripts()) {
                predecessors.push_back({sub_subscript});
            }
        }

        /**
         * Extracts the operands needed for the given Einsum Operator.
         * @param operands all operands that are input to this CrossProduct
         * @param einsum the Einsum Operator that the operands shall be extracted
         * @return the operands relevant for the given Einsum Operator
         */
        vector<HyperTrieTensor<T> *> extractPredecessorOperands(
                const vector<HyperTrieTensor<T> *> &operands,
                const Einsum<T> &einsum) const {
            vector<HyperTrieTensor<T> *> einsum_operands{};
            for (const op_pos_t &op_pos : einsum.subscript.getOriginalOperandPoss()) {
                einsum_operands.push_back(operands.at(op_pos));
            }
            return einsum_operands;
        }

        CrossProductTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) {
            vector<MapTensor<T> *> predecessor_results(predecessors.size());
            // TODO: make parallel
            // get results from all predecessors
            for (int i = 0; i < size(predecessors); ++i) {

                // get the result
                Einsum<T> &predecessor = predecessors.at(i);
                vector<HyperTrieTensor<T> *> einsum_operands = extractPredecessorOperands(operands, predecessor);
                predecessor_results.at(i) = predecessor.getResult(einsum_operands);

                // if one of the results is 0 the cross product will be zero. So no more calculation is needed.
                if (predecessor_results.at(i)->nnz == 0) {
                    // TODO: when parallel -> cancel all other threads.
                    shape_t result_shape = calcResultShape<T, HyperTrieTensor>(operands, this->subscript);
                    return new CrossProductTensor<T>(result_shape);
                }
            }
            return new CrossProductTensor<T>(predecessor_results, bracketed_subscript);
        }
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
