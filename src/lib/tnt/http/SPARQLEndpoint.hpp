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

#include "tnt/http/JSONSparqlResultsHeader.hpp"
#include "tnt/http/JsonSerializer.hpp"
#include "tnt/http/RunQuery.hpp"
#include "tnt/store/AtomicTripleStore.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/store/TripleStore.hpp"
#include "tnt/util/HTTPUtils.hpp"
#include "tnt/util/LogHelper.hpp"

namespace {
	using namespace ::Pistache;
	using namespace ::Pistache::Http;
	using namespace ::tnt::util::types;
	using namespace ::tnt::tensor::einsum;
	using namespace ::tnt::tensor::hypertrie;
	using namespace ::tnt::tensor::einsum::operators;
	using namespace ::tnt::store::sparql;
	using namespace ::tnt::store::cache;
	using AtomicTripleStore = ::tnt::store::AtomicTripleStore;
	using namespace ::std::chrono;
	using namespace ::tnt::logging;
} // namespace
namespace tnt::http {
	/**
	 * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
	 */
	class SPARQLEndpoint : public Handler {
	public:
	HTTP_PROTOTYPE(SPARQLEndpoint)

		SPARQLEndpoint() {}

		void onRequest(const Request &request, ResponseWriter response) override {
			log("### Request begin ###");

			try { // if something fails return an internal server error
				// only answer sparql querys to GET /sparql
				if (request.method() == Method::Get) {
					if (request.resource().compare("/sparql") == 0) {
						const Optional<std::string> &hasQuery = request.query().get("query");
						// check if there is actually an query
						if (not hasQuery.isEmpty()) {

							const std::string sparqlQueryStr = urlDecode(hasQuery.get());
							log("query-string: {}\n"_format(sparqlQueryStr));
							log("Resources at the beginning:");
							log_health_data();
							log("");

							try { // execute the query
								auto query_package = AtomicTripleStore::getInstance().query(sparqlQueryStr);
								response.headers().add<SPARQLJSON>();
								try {
									runQuery(response, query_package, AtomicTripleStore::getInstance());
								} catch (const TimeoutException &exc) {
									// This catch is only for logging.
									// Returning a HTTP response must be handled in the function that times out, i.e.:
									// runQuery(response, query_package, AtomicTripleStore::getInstance());
									logCanceled("timeout", &exc);
									return;
								}
								log("Resources at the end:");
								log_health_data();
								log("### Request end ###\n");
								return;
							} catch (const std::invalid_argument &exc) {
								// handles if the request was not parsable in:
								// auto query_package = AtomicTripleStore::getInstance().query(sparqlQueryStr);
								response.send(Code::Bad_Request, "Could not parse the requested query.");
								logCanceled("unparsable query", &exc);

								return;

							} catch (const TimeoutException &exc) {
								// handles timeouts of
								// auto query_package = AtomicTripleStore::getInstance().query(sparqlQueryStr);
								response.send(Http::Code::Request_Timeout);
								logCanceled("timeout", &exc);
								return;
							} catch (const std::exception &exc) {
								// if the execution of the query should fail return an internal server error
								response.send(Code::Internal_Server_Error);
								logCanceled("internal server error", &exc);
								return;
							}
						}
					}
				}
				response.send(Code::I_m_a_teapot);
				log("Request to something that doesn't exist.\n");
				log("### Request end ###\n");
				log("");
				return;
			} catch (std::exception &exc) {
				response.send(Code::Internal_Server_Error);
				log("Unhandled exception {}"_format(exc));

				return;
			} catch (...) {
				std::exception_ptr exc = std::current_exception();
				response.send(Code::Internal_Server_Error);
				logCanceled("Unhandled exception");
				return;
			}
		}

		static void logCanceled(std::string const reason, std::exception const *const exc = nullptr) {
			log("Resources at the end:");
			log_health_data();
			log("");
			log("canceled: {}"_format(reason));
			if (exc)
				logError(exc->what());
			log("### Request end ###");
		}
	}; // namespace tnt::http

} // namespace tnt::http
#endif // HEALTHCHECK_HPP
