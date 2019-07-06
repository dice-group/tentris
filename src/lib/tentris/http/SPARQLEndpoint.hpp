#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <chrono>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

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
			try {
				try {
					log("### Request begin ###");
					log("Resources at the beginning:");
					log_health_data();


					Status status = Status::OK;
					std::string error_message = "";
					std::shared_ptr<QueryExecutionPackage> query_package;

					try { // if something fails return an internal server error
						// only answer sparql querys to GET /sparql
						switch (request.method()) {
							case Method::Get:
								[[fallthrough]];
							case Method::Post: {
								if (request.resource().compare("/sparql") == 0) {
									const Optional<std::string> &hasQuery = request.query().get("query");
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

									log("query-string: {}\n"_format(query_package->getSparqlStr()));
									log("");

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
							processingDone(query_package);
							break;
						case UNPARSABLE:
							logCanceled("unparsable query", error_message);
							response.send(Code::Bad_Request, "Could not parse the requested query.");
							break;
						case UNKNOWN_REQUEST:
							log("Unknown HTTP command. Only HTTP GET and POST are supported.");
							response.send(Http::Code::Not_Implemented);
							break;
						case PROCESSING_TIMEOUT:
							logCanceled("timeout during request processing");
							processingCanceled(query_package);
							response.send(Http::Code::Request_Timeout);
							break;
						case SERIALIZATION_TIMEOUT:
							// no REQUEST TIMEOUT response can be sent here because we stream results directly to the client.
							// Thus, the code was already written to the header.
							logCanceled("timeout during writing the result");
							processingDone(query_package);
							break;
						case UNEXPECTED:
							logCanceled("unexpected internal Error", error_message);
							processingCanceled(query_package);
							response.send(Code::Internal_Server_Error);
							break;
						case SEVERE_UNEXPECTED:
							logCanceled("severe internal error.");
							processingCanceled(query_package);
							response.send(Code::Internal_Server_Error);
							break;
					}

					log("Resources at the end:");
					log_health_data();
					log("### Request end ###\n");
					log("");
					return;
				} catch (const std::exception &exc) {
					log("severe error. ");
					log(fmt::format("exception: {}", exc.what()));
				}
			}
			catch (...) {
				// if the execution of the query should fail return an internal server error
				log("severe error. ");
			}
		}

		static void processingDone(std::shared_ptr<QueryExecutionPackage> query_package, bool canceled = false) {
			try {
				try {
					AtomicCleanupTaskGroup::getInstance().run([=] {
						try {
							try {
								if (query_package) { // check if the pointer is valid
									if (not canceled)
										query_package->done();
									else
										query_package->canceled();
								}
							} catch (const std::exception &exc) {
								log("severe error while cleanup. ");
								log(fmt::format("exception: {}", exc.what()));
								log(fmt::format("query: {}", query_package->getSparqlStr()));
							}

							catch (...) {
								// if the execution of the query should fail return an internal server error
								log("severe error while cleanup. ");
								log(fmt::format("query: {}", query_package->getSparqlStr()));
							}
						} catch (...) {
							log("severe error while cleanup exception handling.");
						}
					});
				} catch (const std::exception &exc) {
					log("severe error while cleanup start. ");
					log(fmt::format("exception: {}", exc.what()));
					log(fmt::format("query: {}", query_package->getSparqlStr()));
				}

				catch (...) {
					// if the execution of the query should fail return an internal server error
					log("severe error while cleanup start. ");
					log(fmt::format("query: {}", query_package->getSparqlStr()));
				}
			} catch (...) {
				log("severe error while cleanup start exception handling.");
			}
		}

		static void processingCanceled(std::shared_ptr<QueryExecutionPackage> query_package) {
			processingDone(query_package, true);
		}


		static void logCanceled(const std::string &reason) {
			log("canceled: {}"_format(reason));
		}

		static void logCanceled(const std::string &reason, const std::string &exc) {
			log("canceled: {}"_format(reason));
			if (not exc.empty())
				log("exception: {}"_format(exc));
		}

	}; // namespace tentris::http

} // namespace tentris::http
#endif // HEALTHCHECK_HPP
