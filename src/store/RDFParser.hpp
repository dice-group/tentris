#ifndef TEST_RDFPARSER_HPP
#define TEST_RDFPARSER_HPP

#include <serd-0/serd/serd.h>
#include <memory>

#include "../util/All.hpp"
#include "RDF/Node.hpp"
#include <stdexcept>
#include <optional>

namespace tnt::store {
    class RDFParser {
        struct CallBack {
            std::unique_ptr<Node> subject;
            std::unique_ptr<Node> predicate;
            std::unique_ptr<Node> object;
        };

        CallBack cb{};

        SerdStatementSink writeTriple = [](void *handle,
                                           SerdStatementFlags flags,
                                           const SerdNode *graph,
                                           const SerdNode *subject,
                                           const SerdNode *predicate,
                                           const SerdNode *object,
                                           const SerdNode *object_datatype,
                                           const SerdNode *object_lang) -> SerdStatus {

            CallBack *cb = (CallBack *) handle;
            switch (subject->type) {
                case SERD_URI:
                    cb->subject = std::unique_ptr<Node>{
                            new URIRef{std::string{(char *) (subject->buf), size_t(subject->n_chars)}}};
                    break;
                case SERD_BLANK:
                    cb->subject = std::unique_ptr<Node>{
                            new BNode{std::string{(char *) (subject->buf), size_t(subject->n_chars)}}};
                    break;
                default:
                    return SERD_ERR_BAD_SYNTAX;
            }

            switch (predicate->type) {
                case SERD_URI:
                    cb->predicate = std::unique_ptr<Node>{
                            new URIRef{std::string{(char *) (predicate->buf), size_t(predicate->n_chars)}}};
                    break;
                default:
                    return SERD_ERR_BAD_SYNTAX;
            }

            switch (object->type) {
                case SERD_LITERAL: {
                    std::optional<std::string> type = (object_datatype != nullptr)
                                                      ? std::optional<std::string>{{(char *) (object_datatype->buf),
                                                                                           size_t(object_datatype->n_chars)}}
                                                      : std::nullopt;
                    std::optional<std::string> lang = (object_lang != nullptr)
                                                      ? std::optional<std::string>{{(char *) (object_lang->buf),
                                                                                           size_t(object_lang->n_chars)}}
                                                      : std::nullopt;

                    cb->object = std::unique_ptr<Node>{
                            new Literal{std::string{(char *) (object->buf), size_t(object->n_chars)}, type, lang}};
                }
                    break;
                case SERD_BLANK:
                    cb->object = std::unique_ptr<Node>{
                            new BNode{std::string{(char *) (object->buf), size_t(object->n_chars)}}};
                    break;
                case SERD_URI:
                    cb->object = std::unique_ptr<Node>{
                            new URIRef{std::string{(char *) (object->buf), size_t(object->n_chars)}}};
                    break;
                default:
                    return SERD_ERR_BAD_SYNTAX;
            }
            std::cout << (*cb->subject).getIdentifier() << " "
                      << (*cb->predicate).getIdentifier() << " "
                      << (*cb->object).getIdentifier() << " . " << std::endl;
            return SERD_SUCCESS;
        };

        SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) &cb, NULL, NULL, NULL, writeTriple, NULL);

    public:

        RDFParser(std::string file) {
            SerdStatus read_file = serd_reader_read_file(sr, (uint8_t *) (file.data()));
            std::cout << read_file << std::endl;
        }

        RDFParser(const RDFParser &) = delete;


        ~ RDFParser() {
            serd_reader_free(sr);
        }


    };

};
#endif //TEST_RDFPARSER_HPP
