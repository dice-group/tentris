#ifndef TENTRIS_GRAPHQLENDPOINT_HPP
#define TENTRIS_GRAPHQLENDPOINT_HPP

#include <chrono>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <random>

#include <fmt/ostream.h>
#include <restinio/all.hpp>

#include "tentris/http/QueryResultState.hpp"
#include "tentris/store/graphql/ParsedGraphql.hpp"
#include "tentris/store/AtomicQueryExecutionPackageCache.hpp"
#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/util/LogHelper.hpp"

namespace tentris::http::graphql_endpoint {

    using AtomicTripleStoreConfig = ::tentris::store::config::AtomicTripleStoreConfig;
    using AtomicQueryExecutionCache = ::tentris::store::AtomicGraphqlExecutionCache;
    using QueryExecutionPackage = ::tentris::store::cache::GraphqlExecutionPackage;
    using Status = ResultState;

    using namespace ::tentris::logging;
    using namespace ::tentris::store;
    using namespace ::tentris::store::graphql;
    using namespace ::tentris::tensor;

    using namespace std::string_literals;
    using namespace ::std::chrono;

    struct GraphqlEndpoint {
		using EinsumCounted_t = Einsum<COUNTED_t>;
		using EinsumEntryCounted_t = EinsumEntry<COUNTED_t>;
		constexpr static size_t chunk_size = 100'000'000UL;

        auto operator()(restinio::request_handle_t req,
                        [[maybe_unused]] auto params) -> restinio::request_handling_status_t {
            using namespace std::string_literals;
            auto start_time = steady_clock::now();
            log("request started.");
            auto start_memory = get_memory_usage();
            logDebug("ram: {:d} kB"_format(start_memory));
            auto timeout = start_time + AtomicTripleStoreConfig::getInstance().timeout;
            restinio::request_handling_status_t handled = restinio::request_rejected();
            Status status = Status::OK;
            std::string error_message{};
            std::shared_ptr<QueryExecutionPackage> query_package;
            std::string query_string{};
            try {
                const auto query_params = restinio::parse_query<restinio::parse_query_traits::javascript_compatible>(
                        req->header().query());
                if (query_params.has("query")) {
                    query_string = std::string(query_params["query"]);
                    log("query: {}"_format(query_string));
                    // check if there is actually an query
                    try {
                        query_package = AtomicQueryExecutionCache::getInstance()[query_string];
                    } catch (const std::invalid_argument &exc) {
                        status = Status::UNPARSABLE;
                        error_message = exc.what();
                    }
                    if (status == Status::OK) {
                        status = runQuery(req, query_package, timeout);
                    }
                } else {
                    status = Status::UNPARSABLE;
                }
            } catch (const std::exception &exc) {
                // if the execution of the query should fail return an internal server error
                status = Status::UNEXPECTED;
                error_message = exc.what();
            }
            catch (...) {
                // if the execution of the query should fail return an internal server error
                status = Status::SEVERE_UNEXPECTED;
            }

            switch (status) {
                case OK:
                    handled = restinio::request_accepted();
                    break;
                case UNPARSABLE:
                    logError(" ## unparsable query\n"
                             "    query_string: {}"_format(query_string)
                    );
                    handled = req->create_response(restinio::http_status_line_t{restinio::status_code::bad_request,
                                                                                "Could not parse the requested query."s}).done();
                    break;
                case UNKNOWN_REQUEST:
                    logError("unknown HTTP command. Only HTTP GET and POST are supported.");
                    handled = req->create_response(restinio::status_not_implemented()).done();
                    break;
                case PROCESSING_TIMEOUT:
                    logError("timeout during request processing");
                    handled = req->create_response(restinio::status_request_time_out()).done();
                    break;
                case SERIALIZATION_TIMEOUT:
                    // no REQUEST TIMEOUT response can be sent here because we stream results directly to the client.
                    // Thus, the code was already written to the header.
                    logError("timeout during writing the result");
                    handled = restinio::request_accepted();
                    break;
                case UNEXPECTED:
                    logError(" ## unexpected internal error, exception_message: {}"_format(error_message)
                    );
                    handled = req->create_response(
                            restinio::status_internal_server_error()).connection_close().done();
                    break;
                case SEVERE_UNEXPECTED:
                    logError(" ## severe unexpected internal error,  exception_message: {}"_format(error_message)
                    );
                    handled = req->create_response(
                            restinio::status_internal_server_error()).connection_close().done();
                    break;
            }
            if (handled == restinio::request_rejected())
                logError(fmt::format("Handling the request was rejected."));
            auto end_memory = get_memory_usage();
            logDebug("ram: {:d} kB"_format(end_memory));
            logDebug("ram diff: {:+3d} kB"_format(long(end_memory) - long(start_memory)));
            logDebug("request duration: {}"_format(toDurationStr(start_time, steady_clock::now())));
            log("request ended.");
            return handled;
        };

        static void async_cleanup(std::shared_ptr<void> raw_results) {
            std::thread([raw_results{move(raw_results)}]() {
              auto &results = *static_cast<EinsumCounted_t *>(raw_results.get());
              results.clear();
            }).detach();
        }

        static Status
        runQuery(restinio::request_handle_t &req, std::shared_ptr<QueryExecutionPackage> &query_package,
                 const time_point_t timeout) {
            if (steady_clock::now() >= timeout) {
                return Status::PROCESSING_TIMEOUT;
            }

            // create HTTP response object
            restinio::response_builder_t<restinio::chunked_output_t> resp = req->create_response<restinio::chunked_output_t>();
            resp.append_header(restinio::http_field::content_type, "application/graphql-results+json");

            std::shared_ptr<void> raw_results = query_package->getEinsum(timeout);
            auto &results = *static_cast<EinsumCounted_t *>(raw_results.get());

            auto timout_check = 0;
            for (const EinsumEntryCounted_t &result : results) {
                if (++timout_check == 100) {
                    if (steady_clock::now() >= timeout) {
                        async_cleanup(std::move(raw_results));
                        return Status::PROCESSING_TIMEOUT;
                    }
                    timout_check = 0;
                }
            }

            if (steady_clock::now() >= timeout) {
                async_cleanup(std::move(raw_results));
                return Status::PROCESSING_TIMEOUT;
            }

            async_cleanup(std::move(raw_results));
            return Status::OK;
        }

	};

} //tentris::http::graphql_endpoint

#endif //TENTRIS_GRAPHQLENDPOINT_HPP
