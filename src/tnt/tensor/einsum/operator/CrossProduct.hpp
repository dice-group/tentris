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
    public:
        typedef std::vector<std::tuple<size_t, size_t>> op2result_pos_t;
    protected:
        /**
         * Every sub-Subscript (Subscript) is calculated by an Einsum Operator.
         */
        std::vector<std::shared_ptr<OperatorNode < RESULT_TYPE>>> predecessors{};
        mutable std::vector<yield_pull < RESULT_TYPE>> predecessor_generators{};
        const std::shared_ptr<const Subscript> subscript;
    public:
        /**
         * Constructor
         * @param subscript the passed Subscript must represent the dimension layout of the HyperTrieTenors (HyperTrieTenor)
         * that will be passed to getResult() . It will be bracketing out cross product factors will be done internally.
         */
        CrossProduct(const std::shared_ptr<const Subscript> subscript, const std::vector<SliceKey_t> &slice_keys,
                     const std::vector<BoolHyperTrie *> &tries)
                : OperatorNode<RESULT_TYPE>{}, subscript{subscript} {
            this->type = OperatorType::CROSSPRODUCT;
            auto[sub_slice_keys, sub_tries] = extractSliceKeysAndTries(subscript, slice_keys, tries);
            for (const auto &[sub_subscript, sub_slice_key, sub_tries] : zip(subscript->getSubSubscripts(),
                                                                             sub_slice_keys, sub_tries)) {
                // TODO: use Slices directly for subsubscripts with empty result labels
                predecessors.emplace_back(std::shared_ptr<OperatorNode<RESULT_TYPE>>{
                        new Einsum<RESULT_TYPE>{sub_subscript, sub_slice_key, sub_tries}});
            }
        }

        /**
         * This uses the given subscripts (and its sub-subscripts) to split the given vector of slice_keys into the
         * single vectors of slice_keys for every sub-subscript. The same is done with the given vector of tries. The
         * tries are those tries the keys are applied to later. <br/>
         * note for better understanding: if only one graph is used all entries in tries are the same. acutally, multiple
         * graphs are not supported so far.
         * @param subscript a subscript which is optimized and has a cross product.
         * @param slice_keys the slice keys to the unoptimized subscript
         * @param tries the tries for the unoptimized subscript
         * @return a tuple of two vectors: <br/>
         * - vector of the vector of slice_keys for each operand of this CrossProduct
         * - vector of the vector of tries for each operand of this CrossProduct
         */
        std::tuple<std::vector<std::vector<SliceKey_t>>, std::vector<std::vector<BoolHyperTrie *>>>
        extractSliceKeysAndTries(const std::shared_ptr<const Subscript> subscript,
                                 const std::vector<SliceKey_t> &slice_keys,
                                 const std::vector<BoolHyperTrie *> &tries) {
            const std::vector<std::shared_ptr<Subscript>> &subsubscripts = subscript->getSubSubscripts();

            std::vector<std::vector<SliceKey_t>> sub_slices_keys(subsubscripts.size());
            std::vector<std::vector<BoolHyperTrie *>> sub_triess(subsubscripts.size());

            for (const auto &[sub_op_pos, subsubscript] : enumerate(subsubscripts)) {
                std::vector<SliceKey_t> &sub_slice_keys = sub_slices_keys.at(sub_op_pos);
                std::vector<BoolHyperTrie *> &sub_tries = sub_triess.at(sub_op_pos);

                const std::vector<op_pos_t> &original_op_poss = subsubscript->getOriginalOpPoss();
                for (const op_pos_t &original_op_pos : original_op_poss) {
                    sub_slice_keys.emplace_back(slice_keys.at(original_op_pos));
                    sub_tries.emplace_back(tries.at(original_op_pos));
                }
            }

            return {sub_slices_keys, sub_triess};
        }

        yield_pull <RESULT_TYPE> get() const override {
            return yield_pull<RESULT_TYPE>(boost::bind(&CrossProduct<RESULT_TYPE>::get, this, _1));
        }


        /**
         * Constructor
         * @param subscript the passed Subscript must represent the dimension layout of the HyperTrieTenors (HyperTrieTenor)
         * that will be passed to getResult() . It will be bracketing out cross product factors will be done internally.
         */
        void get(yield_push <RESULT_TYPE> &yield) const;

        std::vector<op2result_pos_t> calcMapping(const size_t op_count) const {
            std::vector<std::vector<std::tuple<size_t, size_t>>> pos_mappings{};
            // calculate result mapping
            const std::vector<label_t> &res_labels = subscript->getResultLabels();
            for (const auto &op_pos : range(op_count)) {
                std::vector<std::tuple<size_t, size_t>> op_to_res_pos{};
                for (const auto &[label_pos_in_op, label] : enumerate(subscript->operandLabels(op_pos)))
                    if (const size_t label_pos_in_res = util::container::search(res_labels, label);
                            label_pos_in_res != util::container::NOT_FOUND)
                        op_to_res_pos.emplace_back(label_pos_in_op, label_pos_in_res);

                pos_mappings.emplace_back(op_to_res_pos);
            }
            return pos_mappings;
        }
    };

    void rek_set_key(yield_push <INT_VALUES> &yield,
                     Key_t &result_key,
                     const std::vector<std::map<Key_t, size_t>> &results,
                     const std::vector<CrossProduct<INT_VALUES>::op2result_pos_t> &pos_mappings,
                     size_t &total_count,
                     const size_t last_op_pos, const size_t op_pos = 0) {
        const CrossProduct<INT_VALUES>::op2result_pos_t &pos_mapping = pos_mappings.at(op_pos);
        if (op_pos != last_op_pos) {
            for (const auto &[key, count] : results.at(op_pos)) {
                total_count *= count;
                for (const auto[pos, key_part] : zip(pos_mapping, key)) {
                    result_key[std::get<1>(pos)] = key_part; // TODO: correct?
                }
                rek_set_key(yield, result_key, results, pos_mappings, total_count, op_pos - 1);
            }
        } else {
            for (const auto &[key, count] : results.at(op_pos)) {
                total_count *= count;
                for (const auto[pos, key_part] : zip(pos_mapping, key)) {
                    result_key[std::get<1>(pos)] = key_part; // TODO: correct?
                }
                yield({result_key, total_count});
            }
        }
    }

    template<>
    void CrossProduct<BOOL_VALUES>::get(operators::yield_push<BOOL_VALUES> &yield) const {
        // TODO: implement
    }

    template<>
    void CrossProduct<INT_VALUES>::get(operators::yield_push<INT_VALUES> &yield) const {

        const size_t op_count = subscript->getResultLabels().size();

        std::vector<key_part_t> result_key(op_count);
        std::vector<std::map<Key_t, size_t>> results(op_count - 1);

        // calculate all predecessors results except for the last one
        for (const size_t &i : range(op_count - 1)) {
            // TODO: keep the one with highest card, not the last
            for (const auto binding : predecessor_generators.at(i))
                results.at(i)[getKey<INT_VALUES>(binding)] += getCount<INT_VALUES>(binding);
            // check that each generator has at least one result
            // TODO: cache empty results
            if (results.at(i).empty())
                return;
        }

        std::vector<op2result_pos_t> pos_mappings = calcMapping(op_count);

        // iterate the last one
        size_t total_count = 0;
        const op2result_pos_t &pos_mapping = pos_mappings.at(op_count - 1);
        for (auto &result : predecessor_generators.at(op_count - 1)) {
            Key_t key = getKey<INT_VALUES>(result);
            total_count = getCount<INT_VALUES>(result);
            for (const auto[pos, key_part] : zip(pos_mapping, key)) {
                key[std::get<1>(pos)] = key_part; // TODO: correct?
            }
            rek_set_key(yield, result_key, results, pos_mappings, total_count, op_count - 2);

        }
    }


}

#endif //SPARSETENSOR_EINSUM_OPERATOR_CROSSPRODUCT_HPP
