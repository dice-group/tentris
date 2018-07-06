#ifndef TEST_RDFPARSER_HPP
#define TEST_RDFPARSER_HPP

#include "tnt/util/All.hpp"
#include "tnt/store/RDF/Term.hpp"

#include <serd-0/serd/serd.h>
#include <memory>
#include <stdexcept>
#include <optional>
#include <future>
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock

namespace tnt::store {


    template<typename R>
    bool is_ready(std::future<R> const &f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

    class NTripleParser {
        struct CallBack {
            bool producer_runs{false};
            bool fresh_result{false};
            std::mutex mutex;
            std::unique_ptr<Term> subject;
            std::unique_ptr<Term> predicate;
            std::unique_ptr<Term> object;

        };

        CallBack cb{};

        constexpr static const auto getBNode = [](const SerdNode *node) -> std::unique_ptr<Term> {
            std::ostringstream bnode_str;
            bnode_str << "_:" << std::string_view{(char *) (node->buf), size_t(node->n_chars)};
            return std::unique_ptr<Term>{new BNode{bnode_str.str()}};
        };

        constexpr static const auto getURI = [](const SerdNode *node) -> std::unique_ptr<Term> {
            std::ostringstream uri_str;
            uri_str << "<" << std::string_view{(char *) (node->buf), size_t(node->n_chars)} << ">";
            return std::unique_ptr<Term>{new URIRef{uri_str.str()}};
        };

        constexpr static const auto getLiteral = [](const SerdNode *literal, const SerdNode *type_node,
                                                    const SerdNode *lang_node) -> std::unique_ptr<Term> {
            std::optional<std::__cxx11::string> type = (type_node != nullptr)
                                                       ? std::optional<std::__cxx11::string>{{(char *) (type_node->buf),
                                                                                                     size_t(type_node->n_chars)}}
                                                       : std::nullopt;
            std::optional<std::__cxx11::string> lang = (lang_node != nullptr)
                                                       ? std::optional<std::__cxx11::string>{{(char *) (lang_node->buf),
                                                                                                     size_t(lang_node->n_chars)}}
                                                       : std::nullopt;

            return std::unique_ptr<Term>{
                    new Literal{std::__cxx11::string{(char *) (literal->buf), size_t(literal->n_chars)}, lang, type}};
        };

        constexpr static const SerdStatementSink writeTriple = [](void *handle,
                                                                  SerdStatementFlags flags,
                                                                  const SerdNode *graph,
                                                                  const SerdNode *subject,
                                                                  const SerdNode *predicate,
                                                                  const SerdNode *object,
                                                                  const SerdNode *object_datatype,
                                                                  const SerdNode *object_lang) -> SerdStatus {
            CallBack *cb = (CallBack *) handle;
            while (true) {
                std::lock_guard guard{cb->mutex};
                if (cb->producer_runs)
                    break;
            }
            std::lock_guard guard{cb->mutex};


            switch (subject->type) {
                case SERD_URI:
                    cb->subject = std::move(getURI(subject));
                    break;
                case SERD_BLANK: {
                    cb->subject = std::move(getBNode(subject));
                }
                    break;
                default:
                    cb->producer_runs = false;
                    cb->fresh_result = true;
                    return SERD_ERR_BAD_SYNTAX;
            }

            switch (predicate->type) {
                case SERD_URI:
                    cb->predicate = std::move(getURI(predicate));
                    break;
                default:

                    cb->producer_runs = false;
                    cb->fresh_result = true;
                    return SERD_ERR_BAD_SYNTAX;
            }

            switch (object->type) {
                case SERD_LITERAL:
                    cb->object = getLiteral(object, object_datatype, object_lang);
                    break;
                case SERD_BLANK:
                    cb->object = getBNode(object);
                    break;
                case SERD_URI:
                    cb->object = getURI(object);
                    break;
                default:

                    cb->producer_runs = false;
                    cb->fresh_result = true;
                    return SERD_ERR_BAD_SYNTAX;
            }
            // todo: remove
//            std::cout << (*cb->subject).getIdentifier() << " "
//                      << (*cb->predicate).getIdentifier() << " "
//                      << (*cb->object).getIdentifier() << " . " << std::endl;
            cb->producer_runs = false;
            cb->fresh_result = true;
            return SERD_SUCCESS;
        };


        SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) &cb, NULL, NULL, NULL, writeTriple, NULL);

        std::string _file;
        std::future<SerdStatus> _future;


    public:

        explicit NTripleParser(std::string file) :
                _file(std::move(file)) {
            cb.mutex.lock();
            _future = std::async(std::launch::async, serd_reader_read_file, sr, (uint8_t *) (_file.data()));
        }

        NTripleParser(const NTripleParser &) = delete;


        ~ NTripleParser() {
            while (not is_ready(_future)) {
                cb.producer_runs = 1;

            }
            _future.get();
            serd_reader_free(sr);
        }

        class iterator {
            NTripleParser &_parser;
            bool _ended;
        public:
            explicit iterator(NTripleParser &parser, bool ended = false) : _parser{parser}, _ended{ended} {
                if (not _ended) {
                    _parser.cb.mutex.unlock();
                    calculateNext();
                }
            }

            void calculateNext() {
                {
                    std::lock_guard guard{_parser.cb.mutex};
                    _parser.cb.producer_runs = true;
                }
                while (true) {
                    std::lock_guard guard{_parser.cb.mutex};
                    if (not _parser.cb.producer_runs) {
                        if (not _parser.cb.fresh_result) {
                            _ended = true;
                        }
                        break;
                    }
                    if (is_ready(_parser._future)) {
                        _ended = true;
                        break;
                    }
                }
                _parser.cb.fresh_result = false;
            }

            iterator &operator++() {
                calculateNext();
                return *this;
            }

            std::tuple<std::unique_ptr<Term>, std::unique_ptr<Term>, std::unique_ptr<Term>> operator*() {

                return make_tuple(std::move(_parser.cb.subject), std::move(_parser.cb.predicate),
                                  std::move(_parser.cb.object));
            }

            bool operator==(iterator &rhs) {
                return _ended == rhs._ended;
            }

            bool operator!=(iterator &rhs) {
                return _ended != rhs._ended;
            }

        };

        iterator begin() {
            return iterator{*this};
        }

        iterator end() {
            return iterator{*this, true};
        }


    };

};
#endif //TEST_RDFPARSER_HPP
