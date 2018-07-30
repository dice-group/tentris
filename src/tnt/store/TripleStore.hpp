#ifndef TNT_STORE_TRIPLESTORE
#define TNT_STORE_TRIPLESTORE


#include <serd-0/serd/serd.h>
#include <string>
#include <optional>
#include <boost/bind.hpp>
#include <boost/coroutine2/all.hpp>
#include <vector>

#include "tnt/store/RDF/TermStore.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"


namespace tnt::store {
    namespace {
        using namespace tnt::util::types;
        using namespace tensor::einsum;
        using namespace tensor::einsum::operators;
        using namespace tnt::store::sparql;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
        using Operands =  typename std::vector<BoolHyperTrie *>;
        using key_part_t = tnt::util::types::key_part_t;
    }
    class TripleStore {
        mutable std::map<std::string, std::unique_ptr<ParsedSPARQL>> parsed_query_cache{};
        mutable std::map<std::string, std::unique_ptr<Einsum<INT_VALUES>>> distinct_operator_tree_cache{};
        mutable std::map<std::string, std::unique_ptr<Einsum<BOOL_VALUES>>> default_operator_tree_cache{};

        TermStore termIndex{};
        BoolHyperTrie trie{3};

    public:
        TermStore &getTermIndex() {
            return termIndex;
        }

        void loadRDF(std::string file_path);

        void add(std::tuple<std::string, std::string, std::string> triple) {
            add(parseTerm(std::get<0>(triple)), parseTerm(std::get<1>(triple)), parseTerm(std::get<2>(triple)));
        }

        void add(std::tuple<std::unique_ptr<Term>, std::unique_ptr<Term>, std::unique_ptr<Term>> triple) {
            add(std::move(std::get<0>(triple)), std::move(std::get<1>(triple)), std::move(std::get<2>(triple)));
        }

        inline void
        add(std::unique_ptr<Term> &&subject, std::unique_ptr<Term> &&predicate, std::unique_ptr<Term> &&object) {
            if (subject->type() != Term::NodeType::Literal and predicate->type() == Term::NodeType::URI) {
                const key_part_t &subject_id = termIndex[std::move(subject)];
                const key_part_t &predicate_id = termIndex[std::move(predicate)];
                const key_part_t &object_id = termIndex[std::move(object)];
                trie.set({subject_id, predicate_id, object_id}, true);
            } else
                throw std::invalid_argument{
                        "Subject or predicate of the triple have a term type that is not allowed there."};
        }

        bool contains(std::tuple<std::string, std::string, std::string> triple) {
            const std::unique_ptr<Term> &subject = parseTerm(std::get<0>(triple));
            const std::unique_ptr<Term> &predicate = parseTerm(std::get<1>(triple));
            const std::unique_ptr<Term> &object = parseTerm(std::get<2>(triple));
            return termIndex.contains(subject) and termIndex.contains(predicate) and termIndex.contains(object);
        }

        const ParsedSPARQL &parseSPARQL(const std::string &sparql) {
            try {
                return *parsed_query_cache.at(sparql).get();
            } catch (...) {
                auto inserted = parsed_query_cache.emplace(sparql,
                                                           std::unique_ptr<ParsedSPARQL>{new ParsedSPARQL{sparql}});
                return *inserted.first->second.get();
            }
        }

        template<typename RETURN_TYPE>
        Einsum<RETURN_TYPE> &getOperatorTree(const std::string &sparql, const Subscript &subscript,
                                             std::vector<SliceKey_t> &slice_keys) const {
            try {
                return *getOperatorTreeCache<RETURN_TYPE>().at(sparql).get();
            } catch (...) {
                const std::vector<BoolHyperTrie *> tries(slice_keys.size(), &const_cast<BoolHyperTrie &>(trie));
                auto inserted = getOperatorTreeCache<RETURN_TYPE>()
                        .emplace(sparql, std::unique_ptr<Einsum<RETURN_TYPE>>{
                                new Einsum<RETURN_TYPE>{subscript, slice_keys, tries}});
                return *inserted.first->second.get();
            }
        }


        template<typename RETURN_TYPE>
        std::map<std::string, std::unique_ptr<Einsum<RETURN_TYPE>>> &getOperatorTreeCache() const;


        template<typename RETURN_TYPE>
        yield_pull<RETURN_TYPE> query(const ParsedSPARQL &sparql_) {
            const ParsedSPARQL &sparql = parseSPARQL(sparql_.getSparqlStr());
            std::vector<std::vector<std::optional<Term>>> op_keys = sparql.getOperandKeys();
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
                            return yield_pull<RETURN_TYPE>(
                                    [&]([[maybe_unused]]yield_push<RETURN_TYPE> &yield) { return; });
                        }
                    else
                        no_slices = false;

                if (no_slices) {
                    if (not std::get<bool>(trie.get(slice_key))) // one triple without variables was not in storeF

                        return yield_pull<RETURN_TYPE>(
                                [&]([[maybe_unused]]yield_push<RETURN_TYPE> &yield) { return; });
                } else
                    slice_keys.push_back(slice_key);
            }
            const Subscript &subscript = sparql.getSubscript();
            const Einsum<RETURN_TYPE> &einsumOp
                    = getOperatorTree<RETURN_TYPE>(sparql.getSparqlStr(), subscript, slice_keys);

            return yield_pull<RETURN_TYPE>(boost::bind(&Einsum<RETURN_TYPE>::get, &einsumOp, _1));
            // TODO: implement cross product
        }


    };

    auto getBNode(const SerdNode *node) -> std::unique_ptr<Term> {
        std::ostringstream bnode_str;
        bnode_str << "_:" << std::string_view{(char *) (node->buf), size_t(node->n_chars)};
        return std::unique_ptr<Term>{new BNode{bnode_str.str()}};
    }

    auto getURI(const SerdNode *node) -> std::unique_ptr<Term> {
        std::ostringstream uri_str;
        uri_str << "<" << std::string_view{(char *) (node->buf), size_t(node->n_chars)} << ">";
        return std::unique_ptr<Term>{new URIRef{uri_str.str()}};
    }

    auto getLiteral(const SerdNode *literal, const SerdNode *type_node,
                    const SerdNode *lang_node) -> std::unique_ptr<Term> {
        std::optional<std::string> type = (type_node != nullptr)
                                          ? std::optional<std::string>{{(char *) (type_node->buf),
                                                                               size_t(type_node->n_chars)}}
                                          : std::nullopt;
        std::optional<std::string> lang = (lang_node != nullptr)
                                          ? std::optional<std::string>{{(char *) (lang_node->buf),
                                                                               size_t(lang_node->n_chars)}}
                                          : std::nullopt;
            return std::unique_ptr<Term>{
                    new Literal{std::string{(char *) (literal->buf), size_t(literal->n_chars)}, lang, type}};
    };

    auto serd_callback(void *handle, SerdStatementFlags flags, const SerdNode *graph, const SerdNode *subject,
                       const SerdNode *predicate, const SerdNode *object, const SerdNode *object_datatype,
                       const SerdNode *object_lang) -> SerdStatus {
        tnt::store::TripleStore *store = (tnt::store::TripleStore *) handle;
        std::unique_ptr<Term> subject_term;
        std::unique_ptr<Term> predicate_term;
        std::unique_ptr<Term> object_term;

        switch (subject->type) {
            case SERD_URI:
                subject_term = getURI(subject);
                break;
            case SERD_BLANK: {
                subject_term = getBNode(subject);
            }
                break;
            default:
                return SERD_ERR_BAD_SYNTAX;
        }

        switch (predicate->type) {
            case SERD_URI:
                predicate_term = getURI(predicate);
                break;
            default:
                return SERD_ERR_BAD_SYNTAX;
        }

        switch (object->type) {
            case SERD_LITERAL:
                object_term = getLiteral(object, object_datatype, object_lang);
                break;
            case SERD_BLANK:
                object_term = getBNode(object);
                break;
            case SERD_URI:
                object_term = getURI(object);
                break;
            default:
                return SERD_ERR_BAD_SYNTAX;
        }
        store->add(std::move(subject_term), std::move(predicate_term), std::move(object_term));
        return SERD_SUCCESS;
    }


    void TripleStore::loadRDF(std::string file_path) {
        SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) this, NULL, NULL, NULL, &serd_callback, NULL);
        serd_reader_read_file(sr, (uint8_t *) (file_path.data()));
    }

    template<>
    std::map<std::string, std::unique_ptr<Einsum<INT_VALUES>>> &TripleStore::getOperatorTreeCache() const {
        return distinct_operator_tree_cache;
    }

    template<>
    std::map<std::string, std::unique_ptr<Einsum<BOOL_VALUES>>> &TripleStore::getOperatorTreeCache() const {
        return default_operator_tree_cache;
    }
};
#endif //TNT_STORE_TRIPLESTORE
