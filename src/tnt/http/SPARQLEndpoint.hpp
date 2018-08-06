#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <string>
#include <sstream>
#include "tnt/store/TripleStore.hpp"
#include "tnt/http/StoreInstance.hpp"
#include <pistache/http.h>
#include "tnt/http/Headers.hpp"
#include "tnt/util/HTTPUtils.hpp"
#include <tuple>
#include <utility>
#include <c++/8/charconv>
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include <chrono>
#include "tnt/http/JsonSerializer.hpp"
#include "tnt/http/RunQuery.hpp"
#include "tnt/http/JsonSerializer.hpp"


namespace tnt::http {

    namespace {
        using namespace Pistache;
        using namespace Pistache::Http;
        using namespace tnt::util::types;
        using namespace tensor::einsum;
        using namespace tensor::einsum::operators;
        using namespace tnt::store::sparql;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
        using Operands =  typename std::vector<BoolHyperTrie *>;
        using key_part_t = tnt::util::types::key_part_t;
    }

    std::atomic_uint open_connections{0};

    /**
     * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
     */
    class SPARQLEndpoint : public Pistache::Http::Handler {
    public:
    HTTP_PROTOTYPE(SPARQLEndpoint)

        SPARQLEndpoint() {}

        void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
            response.timeoutAfter(std::chrono::seconds(__timeout));
            if (open_connections > 100) {
                response.send(Code::Service_Unavailable);
                return;
            } else {
                ++open_connections;
            }
            using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
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
                                    const ParsedSPARQL &sparqlQuery = _store->parseSPARQL(sparqlQueryStr);
                                    response.headers().add<SPARQLJSON>();
                                    const std::chrono::time_point<std::chrono::high_resolution_clock> time_out =
                                            std::chrono::high_resolution_clock::now() + std::chrono::seconds(__timeout);
                                    runQuery(response, sparqlQuery, *_store, time_out);
                                    --open_connections;
                                    return;
                                } catch (const std::invalid_argument &exc) {
                                    --open_connections;
                                    response.send(Code::Bad_Request, "Could not parse the requested query.");
                                    std::cout << "unparsable query: \n" << sparqlQueryStr << "\n";
                                    const uint oc = open_connections;
                                    std::cout << "open connections: " << oc << std::endl;
                                    return;
                                }
                            } catch (const TimeoutException exc) {
                                response.timeoutAfter(std::chrono::seconds(0));
                                std::cout << exc.what() << std::endl;
                                // open_connections is set by onTimeout
                                return;
                            } catch (const std::exception &exc) {
                                // if the execution of the query should fail return an internal server error
                                std::cout << exc.what() << std::endl;
                                --open_connections;
                                response.send(Code::Internal_Server_Error);
                                const uint oc = open_connections;
                                std::cout << "open connections: " << oc << std::endl;
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
                std::cout << exc.what() << std::endl;
                response.send(Code::Internal_Server_Error);
                --open_connections;
                const uint oc = open_connections;
                std::cout << "open connections: " << oc;
                return;
            }
        }

        void onTimeout(const Http::Request &req, Http::ResponseWriter response) {
            response.headers().add<SPARQLJSON>();
            response.send(Http::Code::Request_Timeout);
            --open_connections;
            const uint oc = open_connections;
            std::cout << "open connections: " << oc;
            return;
        }

    };


}
#endif //HEALTHCHECK_HPP
