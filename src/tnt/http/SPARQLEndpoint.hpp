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
#include "tnt/http/Headers.hpp"
#include "tnt/http/AtomicTripleStore.hpp"
#include "tnt/config/Config.cpp"
#include "tnt/util/HTTPUtils.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/http/JsonSerializer.hpp"
#include "tnt/http/RunQuery.hpp"

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
}
namespace tnt::http {
    static std::atomic_uint open_connections{0};

    /**
     * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
     */
    class SPARQLEndpoint : public Pistache::Http::Handler {
    public:
    HTTP_PROTOTYPE(SPARQLEndpoint)

        SPARQLEndpoint() {}

        void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
            if (open_connections > 100) {
                response.send(Code::Service_Unavailable);
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

                            try { // execute the query
                                try {
                                    const std::shared_ptr<QueryExecutionPackage> query_package = AtomicTripleStore::getInstance().query(
                                            sparqlQueryStr);
                                    response.headers().add<SPARQLJSON>();
                                    const std::chrono::time_point<std::chrono::high_resolution_clock> time_out =
                                            std::chrono::high_resolution_clock::now() +
                                            std::chrono::seconds(AtomicConfig::getInstance().timeout);
                                    runQuery(response, query_package, AtomicTripleStore::getInstance(), time_out);
                                    --open_connections;
                                    return;
                                } catch (const std::invalid_argument &exc) {
                                    --open_connections;
                                    response.send(Code::Bad_Request, "Could not parse the requested query.");
                                    std::cout << "unparsable query: \n" << sparqlQueryStr << "\n";
                                    const uint oc = open_connections;
                                    std::cout << "open connections: " << oc << "\n";
                                    return;
                                }
                            } catch (const TimeoutException exc) {
                                response.timeoutAfter(std::chrono::seconds(0));
                                std::cout << exc.what() << "\n";
                                response.headers().add<SPARQLJSON>();
                                response.send(Http::Code::Request_Timeout);
                                --open_connections;
                                const uint oc = open_connections;
                                std::cout << "open connections: " << oc;
                                return;
                            } catch (const std::exception &exc) {
                                // if the execution of the query should fail return an internal server error
                                std::cout << exc.what() << "\n";
                                --open_connections;
                                response.send(Code::Internal_Server_Error);
                                const uint oc = open_connections;
                                std::cout << "open connections: " << oc << "\n";
                                return;
                            }
                        }
                    }
                }
                response.send(Code::I_m_a_teapot);
                --open_connections;
                const uint oc = open_connections;
                std::cout << "open connections: " << oc;
                return;
            } catch (const std::exception &exc) {
                std::cout << exc.what() << "\n";
                response.send(Code::Internal_Server_Error);
                --open_connections;
                const uint oc = open_connections;
                std::cout << "open connections: " << oc;
                return;
            }
        }

    };


}
#endif //HEALTHCHECK_HPP
