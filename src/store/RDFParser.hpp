#ifndef TEST_RDFPARSER_HPP
#define TEST_RDFPARSER_HPP

#include <serd-0/serd/serd.h>
#include <memory>

#include "../util/All.hpp"
#include "RDF/Node.hpp"
#include <stdexcept>
#include <optional>
#include <future>
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable>

namespace tnt::store {


    template<typename R>
    bool is_ready(std::future<R> const &f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

    class RDFParser {
        struct CallBack {
            bool producer_runs{false};
            bool fresh_result{false};
            std::mutex mutex;
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
            while (true) {
                std::lock_guard guard{cb->mutex};
                if (cb->producer_runs)
                    break;
            }
            std::lock_guard guard{cb->mutex};


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
                    cb->producer_runs = false;
                    cb->fresh_result = true;
                    return SERD_ERR_BAD_SYNTAX;
            }

            switch (predicate->type) {
                case SERD_URI:
                    cb->predicate = std::unique_ptr<Node>{
                            new URIRef{std::string{(char *) (predicate->buf), size_t(predicate->n_chars)}}};
                    break;
                default:

                    cb->producer_runs = false;
                    cb->fresh_result = true;
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

                    cb->producer_runs = false;
                    cb->fresh_result = true;
                    return SERD_ERR_BAD_SYNTAX;
            }
            std::cout << (*cb->subject).getIdentifier() << " "
                      << (*cb->predicate).getIdentifier() << " "
                      << (*cb->object).getIdentifier() << " . " << std::endl;
            cb->producer_runs = false;
            cb->fresh_result = true;
            return SERD_SUCCESS;
        };

        SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) &cb, NULL, NULL, NULL, writeTriple, NULL);

        std::string _file;
        std::future<SerdStatus> _future;


    public:

        explicit RDFParser(std::string file) :
                _file(std::move(file)) {
            cb.mutex.lock();
            _future = std::async(std::launch::async, serd_reader_read_file, sr, (uint8_t *) (_file.data()));
        }

        RDFParser(const RDFParser &) = delete;


        ~ RDFParser() {

            SerdStatus get1 = _future.get();
            serd_reader_free(sr);
        }

        class iterator {
            RDFParser &_parser;
            bool _ended;
        public:
            explicit iterator(RDFParser &parser, bool ended = false) : _parser{parser}, _ended{ended} {
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
                // ergebnis wird berechnet
                while (true) {
                    std::lock_guard guard{_parser.cb.mutex};
                    if (not _parser.cb.producer_runs) {
                        if (not _parser.cb.fresh_result) {
                            _ended = true;
                        }
                        break;
                    }
                    if (is_ready(_parser._future)){
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

            std::tuple<std::unique_ptr<Node>, std::unique_ptr<Node>, std::unique_ptr<Node>> operator*() {

                return {std::move(_parser.cb.subject), std::move(_parser.cb.subject), std::move(_parser.cb.subject)};
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
