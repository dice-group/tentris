#ifndef TENTRIS_CHUNKED_SPARQLENDPOINT_HPP
#define TENTRIS_CHUNKED_SPARQLENDPOINT_HPP

#include <chrono>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <random>

#include <fmt/ostream.h>
#include <restinio/all.hpp>

#include "tentris/http/AtomicCleanupTaskGroup.hpp"
#include "tentris/http/chunked/JsonSerializer.hpp"
#include "tentris/http/chunked/RunQuery.hpp"
#include "tentris/store/AtomicQueryExecutionPackageCache.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/util/HTTPUtils.hpp"
#include "tentris/util/LogHelper.hpp"


namespace tentris::http {
	namespace {
		using namespace ::tentris::store::sparql;
		using namespace ::tentris::store::cache;
		using AtomicQueryExecutionCache = ::tentris::store::AtomicQueryExecutionCache;
		using namespace ::std::chrono;
		using namespace ::tentris::logging;
		using namespace std::string_literals;
	} // namespace

	std::string randomString(std::string::size_type length) {

		thread_local static auto chrs = "qwertzuiopasdfghjklyxcvbnm"s;
		thread_local static std::random_device rd;
		thread_local static std::mt19937 gen(rd());
		thread_local static std::uniform_int_distribution<> dis(0, size(chrs) - 1);

		std::string s;
		s.reserve(length);
		while (length--)
			s += chrs[dis(gen)];
		return s;
	}

	/**
	 * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
	 */
	auto chunked_sparql_endpoint = [](restinio::request_handle_t req,
									  [[maybe_unused]] auto params) -> restinio::request_handling_status_t {
		using namespace std::string_literals;
		auto timeout = system_clock::now() + AtomicTripleStoreConfig::getInstance().timeout;
		restinio::request_handling_status_t handled = restinio::request_rejected();
		std::string request_id = randomString(10);
		auto start_ram = get_memory_usage().physicalMem;
		auto start_time = std::chrono::system_clock::now();
		log("### request begin\n"s +
			"request_id: {}\n"_format(request_id) +
			"date: {}\n"_format(toDateStr(start_time)) +
			"start_time: {}\n"_format(logging::toTimestampStr(start_time)) +
			"ram: {:d} kB\n"_format(start_ram)
		);
		Status status = Status::OK;
		std::string error_message = "";
		std::shared_ptr<QueryExecutionPackage> query_package;
		std::string query_string = "";
		try {

			try {

				const auto query_params = restinio::parse_query<restinio::parse_query_traits::javascript_compatible>(
						req->header().query());
				if (query_params.has("query")) {
					query_string = std::string(query_params["query"]);
					// check if there is actually an query
					try {
						query_package = AtomicQueryExecutionCache::getInstance()[query_string];
					} catch (const std::invalid_argument &exc) {
						status = Status::UNPARSABLE;
						error_message = exc.what();
					}
					if (status == Status::OK) {
						log(" ## query parsed\n"
							"request_id: {}\n"_format(request_id) +
							"query_string: {}\n"_format(query_package->getSparqlStr())
						);

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

			try {
				switch (status) {
					case OK:
						handled = restinio::request_accepted();
						break;
					case UNPARSABLE:
						logError(" ## unparsable query\n"
								 "request_id: {}\n"_format(request_id) +
								 "query_string: {}\n"_format(query_string)
						);
						handled = req->create_response(restinio::http_status_line_t{restinio::status_code::bad_request,
																					"Could not parse the requested query."s}).connection_close().done();
						break;
					case UNKNOWN_REQUEST:
						logError(" ## unknown HTTP command. Only HTTP GET and POST are supported.\n"
								 "request_id: {}\n"_format(request_id)
						);
						handled = req->create_response(restinio::status_not_implemented()).connection_close().done();
						break;
					case PROCESSING_TIMEOUT:
						logError(" ## timeout during request processing\n"
								 "request_id: {}\n"_format(request_id)
						);
						handled = req->create_response(restinio::status_request_time_out()).connection_close().done();
						break;
					case SERIALIZATION_TIMEOUT:
						// no REQUEST TIMEOUT response can be sent here because we stream results directly to the client.
						// Thus, the code was already written to the header.
						logError(" ## timeout during writing the result\n"
								 "request_id: {}\n"_format(request_id)
						);
						handled = restinio::request_accepted();
						break;
					case UNEXPECTED:
						logError(" ## unexpected internal error\n"
								 "request_id: {}\n"_format(request_id) +
								 "exception_message: {}"_format(error_message)
						);
						handled = req->create_response(
								restinio::status_internal_server_error()).connection_close().done();
						break;
					case SEVERE_UNEXPECTED:
						logError(" ## severe internal error\n"
								 "request_id: {}\n"_format(request_id) +
								 "exception_message: {}"_format(error_message)
						);
						handled = req->create_response(
								restinio::status_internal_server_error()).connection_close().done();
						break;
				}
			}
			catch (const std::exception &exc) {
				logError(" ## error during response state handling\n"
						 "request_id: {}\n"_format(request_id) +
						 "exception_message: {}"_format(exc.
								 what()
						 )
				);
			}
		}
		catch (...) {
			// if the execution of the query should fail return an internal server error
			logError(
					" ## severe, unhandled error during request processing or response state handling. System may be in an unsafe state. \n"
					"request_id: {}\n"_format(request_id)
			);
		}

		auto end_ram = get_memory_usage().physicalMem;
		auto end_time = std::chrono::system_clock::now();
		log("### request end\n"s +
			"request_id: {}\n"_format(request_id) +
			"end_time: {}\n"_format(
					logging::toTimestampStr(end_time)
			) +
			"status: {}\n"_format(status) +
			"request_duration: {}\n"_format(
					logging::toDurationStr(start_time, end_time
					)) +
			"ram: {:d} kB\n"_format(end_ram) +
			"ram_delta: {:+3d} kB"_format(
					int32_t(end_ram)
					-
					int32_t(start_ram)
			)
		);
		return handled;
	};


} // namespace tentris::http
#endif // TENTRIS_CHUNKED_SPARQLENDPOINT_HPP
