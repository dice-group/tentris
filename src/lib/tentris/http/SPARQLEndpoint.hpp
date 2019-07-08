#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <chrono>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <random>

#include <fmt/ostream.h>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "tentris/http/AtomicCleanupTaskGroup.hpp"
#include "tentris/http/JSONSparqlResultsHeader.hpp"
#include "tentris/http/JsonSerializer.hpp"
#include "tentris/http/RunQuery.hpp"
#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/util/HTTPUtils.hpp"
#include "tentris/util/LogHelper.hpp"

namespace {
	using namespace ::Pistache;
	using namespace ::Pistache::Http;
	using namespace ::tentris::util::types;
	using namespace ::tentris::tensor::einsum;
	using namespace ::tentris::tensor::hypertrie;
	using namespace ::tentris::tensor::einsum::operators;
	using namespace ::tentris::store::sparql;
	using namespace ::tentris::store::cache;
	using AtomicTripleStore = ::tentris::store::AtomicTripleStore;
	using namespace ::std::chrono;
	using namespace ::tentris::logging;
	using namespace std::string_literals;
} // namespace
namespace tentris::http {

	/**
	 * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
	 */
	class SPARQLEndpoint : public Handler {
	public:
	HTTP_PROTOTYPE(SPARQLEndpoint)


		SPARQLEndpoint() {}

		void onRequest(const Request &request, ResponseWriter response) override {
			using namespace std::string_literals;
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
			try {
				try {
					std::shared_ptr<QueryExecutionPackage> query_package;

					try { // if something fails return an internal server error
						// only answer sparql querys to GET /sparql
						switch (request.method()) {
							case Method::Get:
								[[fallthrough]];
							case Method::Post: {
								if (request.resource().compare("/sparql"s) == 0) {
									const Optional<std::string> &hasQuery = request.query().get("query"s);
									// check if there is actually an query
									if (hasQuery.isEmpty()) {
										status = Status::UNPARSABLE;
										break;
									}
									try {
										query_package = AtomicTripleStore::getInstance().query(
												urlDecode(hasQuery.get()));
									} catch (const TimeoutException &exc) {
										status = Status::PROCESSING_TIMEOUT;
										break;
									} catch (const std::invalid_argument &exc) {
										status = Status::UNPARSABLE;
										error_message = exc.what();
										break;
									}

									log(" ## query parsed\n"
									    "request_id: {}\n"_format(request_id) +
									    "query_string: {}\n"_format(query_package->getSparqlStr())
									);

									response.headers().add<SPARQLJSON>();
									status = runQuery(response, query_package, AtomicTripleStore::getInstance());
								}
								break;
							}
							default: {
								status = Status::UNKNOWN_REQUEST;
								break;
							}
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
							processingDone(query_package, request_id);
							break;
						case UNPARSABLE:
							logError(" ## unparsable query\n"
							         "request_id: {}\n"_format(request_id) +
							         "query_string: {}\n"_format(query_package->getSparqlStr())
							);
							response.send(Code::Bad_Request, "Could not parse the requested query."s);
							break;
						case UNKNOWN_REQUEST:
							logError(" ## unknown HTTP command. Only HTTP GET and POST are supported.\n"
							         "request_id: {}\n"_format(request_id)
							);
							response.send(Http::Code::Not_Implemented);
							break;
						case PROCESSING_TIMEOUT:
							logError(" ## timeout during request processing\n"
							         "request_id: {}\n"_format(request_id)
							);
							processingCanceled(query_package, request_id);
							response.send(Http::Code::Request_Timeout);
							break;
						case SERIALIZATION_TIMEOUT:
							// no REQUEST TIMEOUT response can be sent here because we stream results directly to the client.
							// Thus, the code was already written to the header.
							logError(" ## timeout during writing the result\n"
							         "request_id: {}\n"_format(request_id)
							);
							processingDone(query_package, request_id);
							break;
						case UNEXPECTED:
							logError(" ## unexpected internal error\n"
							         "request_id: {}\n"_format(request_id) +
							         "exception_message: {}"_format(error_message)
							);
							processingCanceled(query_package, request_id);
							response.send(Code::Internal_Server_Error);
							break;
						case SEVERE_UNEXPECTED:
							logError(" ## severe internal error\n"
							         "request_id: {}\n"_format(request_id) +
							         "exception_message: {}"_format(error_message)
							);
							processingCanceled(query_package, request_id);
							response.send(Code::Internal_Server_Error);
							break;
					}
				} catch (const std::exception &exc) {
					logError(" ## error during response state handling\n"
					         "request_id: {}\n"_format(request_id) +
					         "exception_message: {}"_format(exc.what())
					);
				}
			}
			catch (...) {
				// if the execution of the query should fail return an internal server error
				logError(" ## severe, unhandled error during request processing. System may be in an unsafe state. \n"
				         "request_id: {}\n"_format(request_id)
				);
			}

			auto end_ram = get_memory_usage().physicalMem;
			auto end_time = std::chrono::system_clock::now();
			log("### request end\n"s +
			    "request_id: {}\n"_format(request_id) +
			    "end_time: {}\n"_format(logging::toTimestampStr(end_time)) +
			    "status: {}\n"_format(status) +
			    "request_duration: {}\n"_format(logging::toDurationStr(start_time, end_time)) +
			    "ram: {:d} kB\n"_format(end_ram) +
			    "ram_delta: {:+3d} kB"_format(int32_t(end_ram) - int32_t(start_ram))
			);
		}

		inline static void processingDone(std::shared_ptr<QueryExecutionPackage> query_package, std::string request_id,
		                                  bool canceled = false) {
			using namespace std::string_literals;
			try {
				try {
					AtomicCleanupTaskGroup::getInstance().run([=] {
#if DEBUG
						auto start_ram = get_memory_usage().physicalMem;
						auto start_time = std::chrono::system_clock::now();

						logDebug("### cleanup begin\n"s +
						         "request_id: {}\n"_format(request_id) +
						         "start_time: {}\n"_format(logging::toTimestampStr(start_time)) +
						         "ram: {:d} kB\n"_format(start_ram) +
						         "query_package_valid: {}\n"_format(bool(query_package)) +
						         "processing_canceled: {}"_format(canceled)
						);
#endif
						try {
							try {
								if (query_package) { // check if the pointer is valid
									if (not canceled)
										query_package->done();
									else
										query_package->canceled();
								}
							} catch (const std::exception &exc) {
								logError(" ## error during cleanup\n"s +
								         "request_id: {}\n"_format(request_id) +
								         "exception_message: {}"_format(exc.what())
								);
							}

							catch (...) {
								logError(
										" ## severe, unhandled error during cleanup. System may be in an unsafe state. \n"
										"request_id: {}"_format(request_id)
								);
							}
						} catch (...) {
							logError(
									" ## severe, unhandled error during cleanup error handling. System may be in an unsafe state. \n"
									"request_id: {}"_format(request_id)
							);
						}
#if DEBUG
						auto end_ram = get_memory_usage().physicalMem;
						auto end_time = std::chrono::system_clock::now();

						logDebug("### cleanup end\n"s +
						         "request_id: {}\n"_format(request_id) +
						         "end_time: {}\n"_format(logging::toTimestampStr(end_time)) +
						         "cleanup_duration: {}\n"_format(logging::toDurationStr(start_time, end_time)) +
						         "ram: {:d} kB\n"_format(end_ram) +
						         "ram_delta: {:+3d} kB"_format(int32_t(end_ram) - int32_t(start_ram))
						);
#endif
					});
				} catch (const std::exception &exc) {
					logError(
							" ## error while starting cleanup.\n" +
							"request_id: {}\n"_format(request_id) +
							"exception_message: {}"_format(exc.what())
					);
				}

			} catch (...) {
				logError(
						" ## severe, unhandled error while starting cleanup. System may be in an unsafe state. \n"
						"request_id: {}"_format(request_id)
				);
			}
		}

		inline static void
		processingCanceled(std::shared_ptr<QueryExecutionPackage> query_package, std::string request_id) {
			processingDone(query_package, request_id, true);
		}

		static std::string randomString(std::string::size_type length) {

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


	}; // namespace tentris::http

} // namespace tentris::http
#endif // HEALTHCHECK_HPP
