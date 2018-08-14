#ifndef TNT_QUERYEXECUTIONPACKAGE_HPP
#define TNT_QUERYEXECUTIONPACKAGE_HPP

#include <any>

#include "tnt/store/RDF/TermStore.hpp"
#include "tnt/util/SycronizedFactory.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"


namespace tnt::store::cache {
    namespace {
        using namespace tnt::tensor::einsum::operators;
        using namespace tnt::store::sparql;
        using namespace tnt::util::types;
        using namespace tensor::einsum;
        using namespace tensor::einsum::operators;
        using namespace tnt::store::sparql;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
        using Operands =  typename std::vector<BoolHyperTrie *>;
        using key_part_t = tnt::util::types::key_part_t;
    };

    struct QueryExecutionPackage {
    private:
        ParsedSPARQL parsedSPARQL;
    public:
        bool is_distinct;
    private:
        std::vector<SliceKey_t> slice_keys;
        std::shared_ptr<Einsum<INT_VALUES>> regular_operator_tree;
        std::shared_ptr<Einsum<BOOL_VALUES>> distinct_operator_tree;
    public:

        QueryExecutionPackage(const std::string sparql_string, BoolHyperTrie &trie, const TermStore &termIndex)
                : parsedSPARQL{sparql_string},
                  is_distinct{(parsedSPARQL.getSelectModifier() == SelectModifier::DISTINCT)},
                  slice_keys{calc_keys(parsedSPARQL.getOperandKeys(), trie, termIndex)},
                  regular_operator_tree{
                          ((not slice_keys.empty()) and (not is_distinct))
                          ? new Einsum<INT_VALUES>{
                                  parsedSPARQL.getSubscript(), slice_keys,
                                  std::vector<BoolHyperTrie *>(slice_keys.size(), &const_cast<BoolHyperTrie &>(trie))}
                          : (Einsum<INT_VALUES> *) nullptr},
                  distinct_operator_tree{
                          ((not slice_keys.empty()) and is_distinct)
                          ? new Einsum<BOOL_VALUES>{
                                  parsedSPARQL.getSubscript(), slice_keys,
                                  std::vector<BoolHyperTrie *>(slice_keys.size(), &const_cast<BoolHyperTrie &>(trie))}
                          : (Einsum<BOOL_VALUES> *) nullptr} {
        }

        const ParsedSPARQL &getParsedSPARQL() const {
            return parsedSPARQL;
        }

        const Einsum<BOOL_VALUES> &getDistinctOpTree() const {
            return *distinct_operator_tree.get();
        }

        const Einsum<INT_VALUES> &getRegularOpTree() const {
            return *regular_operator_tree.get();
        }

        yield_pull<BOOL_VALUES> getDistinctGenerator() const {
            if (not slice_keys.empty())
                return distinct_operator_tree->get();
            else
                return yield_pull<BOOL_VALUES>([&]([[maybe_unused]]yield_push<BOOL_VALUES> &yield) { return; });
        }

        yield_pull<INT_VALUES> getRegularGenerator() const {
            if (not slice_keys.empty())
                return regular_operator_tree->get();
            else
                return yield_pull<INT_VALUES>([&]([[maybe_unused]]yield_push<INT_VALUES> &yield) { return; });

        }


        static std::vector<SliceKey_t>
        calc_keys(const std::vector<std::vector<std::optional<Term>>> &op_keys, BoolHyperTrie &trie,
                  const TermStore &termIndex) {
            std::vector<SliceKey_t> slice_keys{};
            for (const auto &op_key : op_keys) {
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
