#ifndef SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP


#include <vector>
#include <tuple>

#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/util/All.hpp"


namespace tnt::tensor::einsum::operators {
    namespace {
        template<typename V>
        using NDMap = std::set<Key_t, V>;
    };

    template<typename T>
    class CrossProductResult;

    /**
     * This is a cross product operator that takes in any number of operands and maps every of their dimensions into an
     * result dimension. The Mapping is defind by an sparsetensor::einsum::Subscript.
     * Example of an cross product: ij,kl->il <=> m,n->mn with m = (ij->i)  and n = (kl->l)
     * @tparam T type of the values hold by processed Tensors (Tensor).
     */
    template<typename T>
    // TODO: reeanble
    class CrossProduct {
        using Operands = tnt::tensor::hypertrie::Operands;
    protected:
        /**
         * This is a subscript where independently calculable parts are bracketed into sub-Subscripts (Subscript).
         */
        const Subscript &_subscript;
        /**
         * Every sub-Subscript (Subscript) is calculated by an Einsum Operator.
         */
        std::vector<tnt::tensor::einsum::operators::Einsum<T>> predecessors{};
    public:
        /**
         * Constructor
         * @param subscript the passed Subscript must represent the dimension layout of the HyperTrieTenors (HyperTrieTenor)
         * that will be passed to getResult() . It will be bracketing out cross product factors will be done internally.
         */
        explicit CrossProduct(const Subscript &subscript) : _subscript{subscript} {
            for (const std::shared_ptr<Subscript> &sub_subscript : subscript.getSubSubscripts()) {
                predecessors.push_back(Einsum<T>{*sub_subscript});
            }
        }

        CrossProductResult<T> getResult(const Operands &operands) {
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
                    throw "no result";
                }
            }
            return CrossProductResult<T>{predecessor_results, _subscript};
        }
    };


    template<typename T>
    class CrossProductResult {
        using Operands = tnt::tensor::hypertrie::Operands;
        const std::vector<NDMap<T>> &_operands;
        std::vector<std::vector<std::tuple<size_t, size_t>>> pos_mappings{};
        size_t _size{};
        size_t _width{};
    public:
        CrossProductResult(const std::vector<NDMap<T>> &operands, const Subscript &subscript) :
                _operands{operands} {

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

            // calculate result mapping
            const std::vector<label_t> &res_labels = subscript.getResultLabels();
            for (const auto &op_pos : range(op_pos_t(operands.size()))) {
                std::vector<std::tuple<size_t, size_t>> op_to_res_pos{};
                for (const auto &[label_pos_in_op, label] : enumerate(subscript.operandLabels(op_pos)))
                    if (const size_t label_pos_in_res = tnt::util::container::search(res_labels, label);
                            label_pos_in_res != tnt::util::container::NOT_FOUND)
                        op_to_res_pos.push_back({label_pos_in_op, label_pos_in_res});

                pos_mappings.emplace_back(op_to_res_pos);
            }

            // calculate number of generated entries
            _size = 1;
            for (const NDMap<T> &operand : _operands) {
                _size *= operand.size();
            }

            //
            _width = res_labels.size();
        }


        class iterator {
            using op_c_iter_t = typename NDMap<T>::const_iterator;
            const CrossProductResult &_result;
            std::vector<op_c_iter_t> _begins;
            std::vector<op_c_iter_t> _ends;
            size_t _id{};
            std::tuple<Key_t, T> result_binding_with_count;

        public:
            explicit iterator(const CrossProductResult &result, bool ended = false) : _result{result} {
                if (ended)
                    _id = _result._size;
                else
                    for (const NDMap<T> &operand : result._operands) {
                        _begins.emplace_back(operand.cbegin());
                        _ends.emplace_back(operand.cend());
                    }
                result_binding_with_count = {Key_t(_result._width), {}};
            }

            std::tuple<Key_t, T> &operator*() {
                // reset the value
                Key_t &key = std::get<0>(result_binding_with_count);
                T &value = std::get<1>(result_binding_with_count);
                value = 1;

                // iterate inputs
                for (const auto &[entry, pos_mapping] : zip(_begins, _result.pos_mappings)) {
                    const auto &[binding, count] = *entry;

                    // set the value
                    value *= count;
                    // set key at right positions
                    for (const auto &[pos_in_op, pos_in_res] : pos_mapping)
                        key[pos_in_res] = binding.at(pos_in_op);
                }

                return result_binding_with_count;
            }

            iterator &operator++() {
                ++_id;
                for (auto &&[i, iter_and_end] : enumerate(zip(_begins, _ends))) {
                    auto &&[iter, end_] = iter_and_end;
                    ++iter;
                    if (iter == end_) {
                        iter = _result._operands[i].cbegin();
                    } else {
                        break;
                    }
                }
                return *this;
            }

            bool operator==(const iterator &rhs) const {
                return rhs._id == _id;
            }

            bool operator!=(const iterator &rhs) const {
                return rhs._id != _id;
            }
        };

        iterator begin() {

            return iterator{*this};
        }

        iterator end() {
            return iterator{*this, true};
        }

    };
}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
