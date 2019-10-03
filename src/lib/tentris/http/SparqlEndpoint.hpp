#ifndef TENTRIS_SPARQLENDPOINT_HPP
#define TENTRIS_SPARQLENDPOINT_HPP

#include <chrono>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <random>

#include <fmt/ostream.h>
#include <restinio/all.hpp>

#include "tentris/http/QueryResultState.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/store/AtomicQueryExecutionPackageCache.hpp"
#include "tentris/store/JsonQueryResult.hpp"
#include "tentris/util/LogHelper.hpp"


namespace tentris::http {
	namespace {
		using namespace ::tentris::store::sparql;
		using namespace ::tentris::store::cache;
		using AtomicQueryExecutionCache = ::tentris::store::AtomicQueryExecutionCache;
		using namespace ::std::chrono;
		using namespace ::tentris::logging;
		using namespace std::string_literals;
		using Status = ResultState;
	} // namespace


	namespace sparql_endpoint {

		Status
		runQuery(restinio::request_handle_t &req, std::shared_ptr<QueryExecutionPackage> &query_package,
				 const QueryExecutionPackage::TimeoutType timeout);

		template<typename RESULT_TYPE>
		Status runQuery(restinio::request_handle_t &req, std::shared_ptr<QueryExecutionPackage> &query_package,
						const QueryExecutionPackage::TimeoutType timeout);


		/**
		 * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
		 */
		auto sparql_endpoint = [](restinio::request_handle_t req,
								  [[maybe_unused]] auto params) -> restinio::request_handling_status_t {
			using namespace std::string_literals;
			auto start_time = system_clock::now();
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
																				"Could not parse the requested query."s}).connection_close().done();
					break;
				case UNKNOWN_REQUEST:
					logError("unknown HTTP command. Only HTTP GET and POST are supported.");
					handled = req->create_response(restinio::status_not_implemented()).connection_close().done();
					break;
				case PROCESSING_TIMEOUT:
					logError("timeout during request processing");
					handled = req->create_response(restinio::status_request_time_out()).connection_close().done();
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
			logDebug("request duration: {}"_format(toDurationStr(start_time, system_clock::now())));
			logTrace("subscripts: {}"_format(subscript_count));
			log("request ended.");
			return handled;
		};

		Status
		runQuery(restinio::request_handle_t &req, std::shared_ptr<QueryExecutionPackage> &query_package,
				 const QueryExecutionPackage::TimeoutType timeout) {

			switch (query_package->getSelectModifier()) {
				case SelectModifier::NONE: {
					return runQuery<COUNTED_t>(req, query_package, timeout);
				}
				case SelectModifier::DISTINCT: {
					return runQuery<DISTINCT_t>(req, query_package, timeout);
				}
				default:
					break;
			}
			logTrace("Query type is not yet supported.");
			return Status::UNPARSABLE;
		};

		template<typename RESULT_TYPE>
		Status runQuery(restinio::request_handle_t &req, std::shared_ptr<QueryExecutionPackage> &query_package,
						const QueryExecutionPackage::TimeoutType timeout) {
			// check if it timed out
			if (system_clock::now() >= timeout) {
				return Status::PROCESSING_TIMEOUT;
			}
			const std::vector<Variable> &vars = query_package->getQueryVariables();
			JsonQueryResult<RESULT_TYPE> json_result{vars};
			if (not query_package->is_trivial_empty) {
				std::shared_ptr<void> raw_results = query_package->getEinsum();
				auto &results = *static_cast<Einsum<RESULT_TYPE> *>(raw_results.get());


				auto timout_check = 0;
				for (const EinsumEntry<RESULT_TYPE> &result : results) {
					json_result.add(result);
					if (++timout_check == 100) {
						if (system_clock::now() >= timeout)
							return Status::PROCESSING_TIMEOUT;
						timout_check = 0;
					}
				}
			}

			if (system_clock::now() >= timeout) {
				return Status::PROCESSING_TIMEOUT;
			}

			auto resp = req->create_response();
			resp.append_header(restinio::http_field::content_type, "application/sparql-results+json");
			resp.connection_close();
			resp.set_body(json_result.str()).done();
			return Status::OK;
		}

	};


} // namespace tentris::http
#endif // TENTRIS_SPARQLENDPOINT_HPP
