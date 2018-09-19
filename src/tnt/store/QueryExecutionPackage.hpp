#ifndef TNT_QUERYEXECUTIONPACKAGE_HPP
#define TNT_QUERYEXECUTIONPACKAGE_HPP

#include <any>
#include <exception>

#include "tnt/store/RDF/TermStore.hpp"
#include "tnt/util/SycronizedFactory.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/tensor/einsum/operator/CrossProduct.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/tensor/einsum/operator/OperatorNode.hpp"

namespace {
    using namespace tnt::tensor::einsum::operators;
    using namespace tnt::store::sparql;
    using namespace tnt::util::types;
    using namespace tnt::tensor::einsum;
    using namespace tnt::tensor::hypertrie;
    using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
};

namespace tnt::store::cache {


    /**
     * A QueryExecutionPackage contains everything that is necessary to execute a given sparql query for a state of the
     * RDF graph.
     */
    struct QueryExecutionPackage {
    private:
        ParsedSPARQL parsedSPARQL;
    public:
        /**
         * Indicates if the QueryExecutionPackage represents an distinct query or not. If it is distinct use only
         * the methods with distinct in their names. Otherwise use only the methods with regular in their names
         */
        bool is_distinct;
        bool is_trivial_emtpy;
    private:
        std::unique_ptr<OperatorNode<counted_binding>> regular_operator_tree;
        std::unique_ptr<OperatorNode<distinct_binding>> distinct_operator_tree;
    public:

        /**
         *
         * @param sparql_string sparql query to be parsed
         * @param trie current try holding the data
         * @param termIndex term store attached to the trie
         * @throw std::invalid_argument the sparql query was not parsable
         */
        QueryExecutionPackage(const std::string sparql_string, BoolHyperTrie &trie, const TermStore &termIndex)
                : parsedSPARQL{sparql_string},
                  is_distinct{(parsedSPARQL.getSelectModifier() == SelectModifier::DISTINCT)} {

            const std::vector<SliceKey_t> slice_keys = calc_keys(parsedSPARQL.getOperandKeys(), trie, termIndex);


            is_trivial_emtpy = slice_keys.empty();
            if (not is_trivial_emtpy) {

                const std::shared_ptr<const Subscript> subscript = parsedSPARQL.getSubscript();

                const std::vector<BoolHyperTrie *> hypertries = std::vector<BoolHyperTrie *>(slice_keys.size(),
                                                                                             &const_cast<BoolHyperTrie &>(trie));
                if (not is_distinct)
                    regular_operator_tree = getOpTree<counted_binding>(slice_keys, subscript, hypertries);
                else
                    distinct_operator_tree = getOpTree<distinct_binding>(slice_keys, subscript, hypertries);

            }
        }

        template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
        std::unique_ptr<OperatorNode<RESULT_TYPE>>
        getOpTree(const std::vector<SliceKey_t> &slice_keys, const std::shared_ptr<const Subscript> subscript,
                  const std::vector<BoolHyperTrie *> &hypertries) {
            if (auto subsubscripts = subscript->getSubSubscripts();not subsubscripts.empty()) {
                return std::unique_ptr<OperatorNode<RESULT_TYPE>>{
                        new CrossProduct<RESULT_TYPE>{subscript, slice_keys, hypertries}};
            } else {
                return std::unique_ptr<OperatorNode<RESULT_TYPE>>{
                        new Einsum<RESULT_TYPE>{subscript, slice_keys, hypertries}};
            }
        }

        const ParsedSPARQL &getParsedSPARQL() const {
            return parsedSPARQL;
        }

        const OperatorNode<distinct_binding> &getDistinctOpTree() const {
            if (is_distinct)
                return *distinct_operator_tree.get();
            else
                throw std::domain_error("This Packackage holds a non-distinct Operator tree.");
        }

        const OperatorNode<counted_binding> &getRegularOpTree() const {
            if (not is_distinct)
                return *regular_operator_tree.get();
            else
                throw std::domain_error("This Packackage holds a distinct Operator tree.");
        }

        /**
         * Returns an generator for the result if this->is_distinct
         * @return result generator
         */
        yield_pull<distinct_binding> getDistinctGenerator() const {
            if (is_distinct) {
                if (not is_trivial_emtpy)
                    return distinct_operator_tree->get();
                else
                    return yield_pull<distinct_binding>([&]([[maybe_unused]]yield_push<distinct_binding> &yield) { return; });
            } else
                throw std::domain_error("This Packackage holds a non-distinct Operator tree.");

        }

        /**
         * Returns an generator for the result if not this->is_distinct
         * @return result generator
         */
        yield_pull<counted_binding> getRegularGenerator() const {
            if (not is_distinct) {
                if (not is_trivial_emtpy)
                    return regular_operator_tree->get();
                else
                    return yield_pull<counted_binding>([&]([[maybe_unused]]yield_push<counted_binding> &yield) { return; });
            } else
                throw std::domain_error("This Packackage holds a distinct Operator tree.");


        }

    private:
        /**
         * Calculates the slice keys for the BoolHyperTrie from a basic graph pattern. If the result is clearly empty
         * an empty list of slice keys is returned.
         * @param bgp basic graph pattern of the query
         * @param trie BoolHyperTrie
         * @param termIndex intex for trie
         * @return slice keys for the BoolHyperTrie
         */
        static std::vector<SliceKey_t>
        calc_keys(const std::vector<std::vector<std::optional<Term>>> &bgp, BoolHyperTrie &trie,
                  const TermStore &termIndex) {
            std::vector<SliceKey_t> slice_keys{};
            for (const auto &op_key : bgp) {
                SliceKey_t slice_key(3, std::nullopt);
                bool no_slices = true;
                for (const auto &[pos, op_key_part] : enumerate(op_key))
                    if (op_key_part.has_value())
                        try {
                            key_part_t ind = termIndex.at(*op_key_part);
                            slice_key[pos] = {ind};
                        } catch (...) { // a keypart was not in the index so the result is zero anyways.
                            return {};
                        }
                    else
                        no_slices = false;

                if (no_slices) {
                    if (not std::get<bool>(
                            trie.get(slice_key))) // one triple without variables was not in storeF
                        return {};
                } else
                    slice_keys.push_back(slice_key);
            }

            return slice_keys;
        }
    };

}

#endif //TNT_QUERYEXECUTIONPACKAGE_HPP
