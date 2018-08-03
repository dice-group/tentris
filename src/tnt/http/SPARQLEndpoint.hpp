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
#include <thread>
#include "tnt/http/JsonSerializer.hpp"
#include "tnt/http/RunQuery.hpp"


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

    /**
     * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
     */
    class SPARQLEndpoint : public Pistache::Http::Handler {
    public:
    HTTP_PROTOTYPE(SPARQLEndpoint)

        SPARQLEndpoint() {}

        void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {

            using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
            try { // if something fails return an internal server error
                // only answer sparql querys to GET /sparql
                if (request.method() == Method::Get and request.resource().compare("/sparql") == 0) {
                    response.headers().add<SPARQLJSON>();
                    const Optional<std::string> &hasQuery = request.query().get("query");
                    // check if there is actually an query
                    if (not hasQuery.isEmpty()) {

                        const std::string sparqlQueryStr = urlDecode(hasQuery.get());

                        try { // execute the query
                            const ParsedSPARQL &sparqlQuery = _store->parseSPARQL(sparqlQueryStr);
                            runQuery(response, sparqlQuery, *_store);


                        } catch (
                                const std::exception &exc) { // if the execution of the query should fail return an internal server error
                            std::cout << exc.what() << std::endl;
                            response.send(Code::Internal_Server_Error);
                        }

                    } else {
                        auto stream = response.send(Code::Not_Found);
                    }
                } else if (request.method() == Method::Get and request.resource().compare("/timeout") == 0) {
                    response.timeoutAfter(std::chrono::seconds(10));
                    auto stream = response.stream(Code::Ok);
                    stream << "write something";
                    stream.flush();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    stream << "5 seconds";
                    stream.flush();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    stream << "10 seconds";
                    stream.flush();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    stream << "15 seconds";
                    stream.flush();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    stream << "going into endless wait, but after 30 seconds it should timeout anyways.";
                    stream.flush();
                    stream.ends();
//                    while (true);
                    return;

                } else {
                    response.send(Code::I_m_a_teapot);
                }

            } catch (const std::exception &exc) {
                std::cout << exc.what() << std::endl;
                response.send(Code::Internal_Server_Error);
            }

        }
    };


}
#endif //HEALTHCHECK_HPP
