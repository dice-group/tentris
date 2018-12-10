#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <string>
#include <sstream>
#include <tuple>
#include <utility>
#include <chrono>

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/http.h>

#include "tnt/store/TripleStore.hpp"
#include "tnt/http/JSONSparqlResultsHeader.hpp"
#include "tnt/store/AtomicTripleStore.hpp"
#include "tnt/config/Config.cpp"
#include "tnt/util/HTTPUtils.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/http/JsonSerializer.hpp"
#include "tnt/http/RunQuery.hpp"
#include "tnt/util/LogHelper.hpp"

namespace {
    using namespace Pistache;
    using namespace Pistache::Http;
    using namespace tnt::util::types;
    using namespace tnt::config;
    using namespace tnt::tensor::einsum;
    using namespace tnt::tensor::hypertrie;
    using namespace tnt::tensor::einsum::operators;
    using namespace tnt::store::sparql;
    using namespace tnt::store::cache;
    using AtomicTripleStore = tnt::store::AtomicTripleStore;
    using namespace std::chrono;
}
namespace tnt::http {
    /**
     * Counts the number of open connections.
     */
    static std::atomic_uint open_connections{0};

    /**
     * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
     */
    class SPARQLEndpoint : public Handler {
    public:
    HTTP_PROTOTYPE(SPARQLEndpoint)

        SPARQLEndpoint() {}

        void onRequest(const Request &request, ResponseWriter response) override {
            log("### Request begin ###");
            if (open_connections > 100) {
                response.send(Code::Service_Unavailable);
                log("canceled: more than 100 connections are open.\n");
                return;
            } else {
                ++open_connections;
            }

            try { // if something fails return an internal server error
                // only answer sparql querys to GET /sparql
                if (request.method() == Method::Get) {
                    if (request.resource().compare("/sparql") == 0) {
                        const Optional<std::string> &hasQuery = request.query().get("query");
                        // check if there is actually an query
                        if (not hasQuery.isEmpty()) {

                            const std::string sparqlQueryStr = urlDecode(hasQuery.get());
                            log("query-string: ", sparqlQueryStr, "\n");
                            log("Resources at the beginning:");
                            log_health_data();
                            log("");

                            try { // execute the query
                                try {
                                    const auto query_package = AtomicTripleStore::getInstance().query(sparqlQueryStr);
                                    response.headers().add<SPARQLJSON>();
                                    const time_point<high_resolution_clock> time_out =
                                            high_resolution_clock::now() + seconds(AtomicConfig::getInstance().timeout);
                                    runQuery(response, query_package, AtomicTripleStore::getInstance(), time_out);
                                    --open_connections;
                                    log("Resources at the end:");
                                    log_health_data();
                                    log("### Request end ###\n");
                                    return;
                                } catch (const std::invalid_argument &exc) {
                                    --open_connections;
                                    response.send(Code::Bad_Request, "Could not parse the requested query.");
                                    logCanceled("unparsable query", &exc);

                                    return;
                                }
                            } catch (const TimeoutException &exc) {
                                response.timeoutAfter(std::chrono::seconds(0));
                                response.headers().add<SPARQLJSON>();
                                response.send(Http::Code::Request_Timeout);
                                --open_connections;
                                logCanceled("timeout", &exc);
                                return;
                            } catch (const std::exception &exc) {
                                // if the execution of the query should fail return an internal server error
                                --open_connections;
                                response.send(Code::Internal_Server_Error);
                                logCanceled("internal server error", &exc);
                                return;
                            }
                        }
                    }
                }
                response.send(Code::I_m_a_teapot);
                --open_connections;
                log("Request to something that doesn't exist.\n");
                log("### Request end ###\n");
                log("");
                return;
            } catch (std::exception &exc) {
                response.send(Code::Internal_Server_Error);
                --open_connections;
                log("Unhandled exception", &exc);

                return;
            }
            catch (...) {
                std::exception_ptr exc = std::current_exception();
                response.send(Code::Internal_Server_Error);
                --open_connections;
                logCanceled("Unhandled exception");
                return;
            }
        }

        static void logCanceled(std::string const reason, std::exception const *const exc = nullptr) {
            log("Resources at the end:");
            log_health_data();
            log("");
            log("canceled: ", reason);
            if (exc)
                logError(exc->what());
            log("### Request end ###");
        }

    };


}
#endif //HEALTHCHECK_HPP
