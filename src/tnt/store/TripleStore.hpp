#ifndef TNT_STORE_TRIPLESTORE
#define TNT_STORE_TRIPLESTORE


#include <serd-0/serd/serd.h>
#include <string>
#include <optional>

#include "tnt/store/RDF/TermStore.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/util/container/NDMap.hpp"
#include "tnt/store/SPARQL/SPARQLParser.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/tensor/einsum/operator/CrossProduct.hpp"


namespace tnt::store {
    class TripleStore {
        using key_part_t = tnt::util::types::key_part_t;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
        template<typename V>
        using NDMap = tnt::util::container::NDMap<V>;
        TermStore termIndex{};
        BoolHyperTrie trie{3};

    public:
        TermStore &getTermIndex() {
            return termIndex;
        }

        void loadRDF(std::string file_path);

        void add(std::tuple<std::string, std::string, std::string> triple) {
            add(parse(std::get<0>(triple)), parse(std::get<1>(triple)), parse(std::get<2>(triple)));
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
            const std::unique_ptr<Term> &subject = parse(std::get<0>(triple));
            const std::unique_ptr<Term> &predicate = parse(std::get<1>(triple));
            const std::unique_ptr<Term> &object = parse(std::get<2>(triple));
            return termIndex.contains(subject) and termIndex.contains(predicate) and termIndex.contains(object);
        }

        NDMap<size_t> query(std::string sparql) {
            using namespace tnt::util::types;
            using Operands =  typename std::vector<BoolHyperTrie *>;
            using namespace tensor::einsum;
            sparql::SPARQLParser parser{sparql};

            std::vector<std::vector<std::optional<Term>>> op_keys = parser.getOperandKeys();
            Operands operands;
            for (const auto &op_key : op_keys) {
                std::vector<std::optional<key_part_t >> id_op_key(3);
                int count = 0;
                for (const auto &[pos, op_key_part] : enumerate(op_key)) {
                    if (op_key_part) {
                        ++count;
                        try {
                            key_part_t ind = termIndex.at(*op_key_part);
                            id_op_key[pos] = {ind};
                        } catch (...) {
                            return {}; // a keypart was not in the index so the result is zero anyways.
                        }
                    }
                }

                if (count)
                    try {
                        BoolHyperTrie *operand = std::get<BoolHyperTrie *>(trie.get(id_op_key));
                        operands.push_back(operand);


                    } catch (...) {
                        return {}; // a triple pattern has an empty solution.
                    }
            }
            // TODO: add support for distinct
            Subscript subscript = parser.getSubscript();
            Subscript optimized = subscript.optimized();
            if (optimized.getSubSubscripts().empty()) {
                operators::Einsum<size_t> einsumOp{optimized};
                return einsumOp.getResult(operands);
            } else {
                operators::CrossProduct<size_t> crossprodOp{optimized};
                operators::CrossProductResult<size_t> result = crossprodOp.getResult(operands);
                NDMap<size_t> result_array{};
                for (const auto &[key, count] : result) {
                    result_array[key] = count;

                }
                return result_array;
            }
            return {};
        }

        inline size_t size() const {
            return trie.size();
        }
    };

    auto getBNode(const SerdNode *node) -> std::unique_ptr<Term> {
        std::ostringstream bnode_str;
        bnode_str << "_:" << std::string_view{(char *) (node->buf), size_t(node->n_chars)};
        return std::unique_ptr<Term>{new BNode{bnode_str.str()}};
    };

    auto getURI(const SerdNode *node) -> std::unique_ptr<Term> {
        std::ostringstream uri_str;
        uri_str << "<" << std::string_view{(char *) (node->buf), size_t(node->n_chars)} << ">";
        return std::unique_ptr<Term>{new URIRef{uri_str.str()}};
    };

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
    };

    void TripleStore::loadRDF(std::string file_path) {
        SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) this, NULL, NULL, NULL, &serd_callback, NULL);
        serd_reader_read_file(sr, (uint8_t *) (file_path.data()));
    }
};

#endif //TNT_STORE_TRIPLESTORE
