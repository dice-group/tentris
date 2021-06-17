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
#include "tentris/store/AtomicQueryExecutionPackageCache.hpp"
#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/store/graphql/GraphQLResponseSAX.hpp"
#include "tentris/util/LogHelper.hpp"

namespace tentris::http::graphql_endpoint {

    using AtomicTripleStoreConfig = ::tentris::store::config::AtomicTripleStoreConfig;
    using AtomicQueryExecutionCache = ::tentris::store::AtomicGraphQLExecutionCache;
    using QueryExecutionPackage = ::tentris::store::cache::GraphQLExecutionPackage;
    using Status = ResultState;

    using namespace ::tentris::logging;
    using namespace ::tentris::store;
    using namespace ::tentris::store::graphql;
    using namespace ::tentris::tensor;

    using namespace std::string_literals;
    using namespace ::std::chrono;

    struct GraphQLEndpoint {
		using Einsum_t = Einsum<COUNTED_t>;
		using EinsumEntry_t = EinsumEntry<COUNTED_t>;
        constexpr static size_t chunk_size = 100'000'000UL;

        auto operator()(restinio::request_handle_t req,
                        [[maybe_unused]] auto params) -> restinio::request_handling_status_t {
            using namespace std::string_literals;
            auto start_time = steady_clock::now();
            log("request started.");
            auto start_memory = get_memory_usage();
            logDebug("ram: {:d} kB"_format(start_memory));
            auto timeout = start_time + AtomicTripleStoreConfig::getInstance().timeout;
            Status status = Status::OK;
            std::string error_message{};
            std::shared_ptr<QueryExecutionPackage> query_package;
            std::string request_str{};
            std::string op_name{};
            try {
                const auto query_params = restinio::parse_query<restinio::parse_query_traits::javascript_compatible>(
                        req->header().query());
                // check if there is actually an query
                if (query_params.has("query")) {
					request_str = std::string(query_params["query"]);
					if (query_params.has("operation"))
						op_name = std::string(query_params["operation"]);
                    log("request: {}"_format(request_str));
					if (not op_name.empty())
                        log("operation: {}"_format(op_name));
                    try {
                        query_package = AtomicQueryExecutionCache::getInstance()[std::make_pair(request_str, op_name)];
                    } catch (const std::invalid_argument &exc) {
                        status = Status::UNPARSABLE;
                        error_message = exc.what();
                    }
                    if (status == Status::OK) {
                        status = runQuery(req, query_package, timeout);
                    }
                } else {
                    status = Status::UNPARSABLE;
					error_message = "No query document was provided";
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
					break;
                case UNPARSABLE:
                    logError(" ## unparsable query \n query_string: {}"_format(request_str));
                    break;
                case UNKNOWN_REQUEST:
                    logError("unknown HTTP command. Only HTTP GET and POST are supported.");
                    break;
                case PROCESSING_TIMEOUT:
                    logError("timeout during request processing");
                    break;
                case SERIALIZATION_TIMEOUT:
                    logError("timeout during writing the result");
					error_message = "reached timeout";
                    break;
                case UNEXPECTED:
                    logError(" ## unexpected internal error, exception_message: {}"_format(error_message));
                    break;
                case SEVERE_UNEXPECTED:
                    logError(" ## severe unexpected internal error,  exception_message: {}"_format(error_message));
					break;
            }
			if (status != Status::OK) {
                auto resp = req->create_response();
                resp.append_header(restinio::http_field::content_type, "application/json");
				resp.append_body(fmt::format(R"({{"error":"{}"}})", error_message));
                resp.done();
			}
            auto end_memory = get_memory_usage();
            logDebug("ram: {:d} kB"_format(end_memory));
            logDebug("ram diff: {:+3d} kB"_format(long(end_memory) - long(start_memory)));
            logDebug("request duration: {}"_format(toDurationStr(start_time, steady_clock::now())));
            log("request ended.");
            return restinio::request_accepted();
        };

        static void async_cleanup(std::shared_ptr<void> raw_results) {
            std::thread([raw_results{move(raw_results)}]() {
              auto &results = *static_cast<Einsum_t *>(raw_results.get());
              results.clear();
            }).detach();
        }

		Status runQuery(restinio::request_handle_t &req,
				 std::shared_ptr<QueryExecutionPackage> &query_package,
                 const time_point_t timeout) {
            if (steady_clock::now() >= timeout) {
                return Status::PROCESSING_TIMEOUT;
            }
            // create HTTP response object
			GraphQLResponseSAX json_response{chunk_size};
            auto resp = req->create_response<restinio::chunked_output_t>();
            resp.append_header(restinio::http_field::content_type, "application/json");
            auto timout_check = 0;
			auto einsums = query_package->generateEinsums(timeout);
			for(auto i : iter::range(einsums.size())) {
                std::shared_ptr<void> raw_results = einsums[i];
				auto &results = *static_cast<Einsum_t *>(raw_results.get());
				json_response.begin_root_field(&query_package->getParsedSubGraphQL(i));
				// iterate over einsum results and populate json response
				for (const EinsumEntry_t &result : results) {
					json_response.add(result);
					if (++timout_check == 100) {
						if (steady_clock::now() >= timeout) {
							async_cleanup(std::move(raw_results));
							return Status::PROCESSING_TIMEOUT;
						}
						timout_check = 0;
					}
                    if (json_response.full()) {
                        resp.append_chunk(std::string{json_response.string_view()});
                        resp.flush();
                        json_response.clear();
                    }
				}
				json_response.end_root_field();
				if (steady_clock::now() >= timeout) {
					async_cleanup(std::move(raw_results));
					return Status::PROCESSING_TIMEOUT;
				}
				async_cleanup(std::move(raw_results));
			}
			json_response.close();
            resp.append_chunk(std::string{json_response.string_view()});
			resp.done();
            return Status::OK;
        }

	};

} //tentris::http::graphql_endpoint

#endif //TENTRIS_GRAPHQLENDPOINT_HPP
