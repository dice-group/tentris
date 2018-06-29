#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include <vector>
#include <tuple>

#include "Einsum.hpp"
#include "../../../util/All.hpp"



namespace tnt::tensor::einsum::operators {

    /**
     * This is a cross product operator that takes in any number of operands and maps every of their dimensions into an
     * result dimension. The Mapping is defind by an sparsetensor::einsum::Subscript.
     * Example of an cross product: ij,kl->il <=> m,n->mn with m = (ij->i)  and n = (kl->l)
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename T>
    class CrossProduct {
        using Operands = tnt::tensor::hypertrie::Operands;
        template<typename V>
        using NDMap = tnt::util::container::NDMap<V>;
    protected:
        /**
         * This is a subscript where independently calculable parts are bracketed into sub-Subscripts (Subscript).
         */
        const Subscript &_subscript;
        /**
         * Every sub-Subscript (Subscript) is calculated by an Einsum Operator.
         */
        std::vector<Einsum<T>> predecessors{};
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

        class CrossProductResult {
            const std::vector<NDMap<T>> &_operands;
            const Subscript &_subscript;
            std::vector<std::tuple<size_t, size_t>> pos_mappings{};

            const CrossProductResult(const std::vector<NDMap<T>> &operands, const Subscript &subscript) :
                    _operands{operands}, _subscript{subscript} {

                // check if there is any input
                if (operands.size() == 0) {
                    return;
                }

                // check if any input is zero
                for (const NDMap<T> &op : operands) {
                    if (not op.size()) {
                        return;
                    }
                }

                const std::vector<label_t> &res_labels = _subscript.getResultLabels();
                for (const auto &[op_pos, op] : operands) {
                    std::vector<std::tuple<size_t, size_t>> op_to_res_pos{};
                    for (const auto &[label_pos_in_op, label] : enumerate(_subscript.operandLabels(op_pos))) {
                        if (const size_t label_pos_in_res = tnt::util::container::search(res_labels, label);
                                label_pos_in_res != tnt::util::container::NOT_FOUND)
                            op_to_res_pos.push_back({label_pos_in_op, label_pos_in_res});
                    }
                    pos_mappings.emplace_back(op_to_res_pos);
                }
            }


            class iterator {
                using op_c_iter_t = typename NDMap<T>::const_iterator;
                std::vector<op_c_iter_t> _begins;
                std::vector<op_c_iter_t> _ends;
                bool _ended{};
            public:
                iterator(const CrossProductResult &result, bool ended = false) : _ended{ended} {
                    for (const NDMap<T> &operand : result._operands) {
                        _begins.emplace_back(operand.cbegin());
                        _ends.emplace_back(operand.cend());
                    }
                }
            };

            iterator begin() {

                return {*this};
            }

            iterator end() {
                return {*this, true};
            }

        };
    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
