#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include "tnt/store/TripleStore.hpp"
#include <string>
#include <pistache/http.h>
#include <tuple>
#include <utility>
#include <c++/8/charconv>

tnt::store::TripleStore *_store;

namespace tnt::http {


    /**
     * Content-Type for SPARQL OUTPUT JSON FORMAT
     */
    class SPARQLJSON : public Pistache::Http::Header::Header {
    public:
        NAME("Content-Type");

        SPARQLJSON() {}

        void parse([[maybe_unused]] const std::string &str) {
        }

        void write(std::ostream &os) const {
            os << "application/sparql-results+json";
        }
    };

    /**
     * Decode Query parameter values from HTTP requests.
     * @param urlEncoded url encoded string
     * @return decoded string
     */
    auto urlDecode(const std::string &urlEncoded) -> std::string {
        std::stringstream out;
        size_t i = 0;
        const char *data = urlEncoded.data();
        while (i < urlEncoded.size()) {
            switch (data[i]) {
                case ' ':
                    out << '+';
                    ++i;
                    break;
                case '%':
                    assert(i < urlEncoded.size() - 2);
                    char c;
                    std::from_chars(data + i + 1, data + i + 3, c, 16);
                    out << c;
                    i += 3;
                    break;
                default:
                    out << data[i];
                    ++i;
            }
        }
        return out.str();
    }


    /**
     * Main SPARQL endpoint. Parses HTTP queries and returns SPARQL JSON Results.
     */
    class SPARQLEndpoint : public Pistache::Http::Handler {
    public:
    HTTP_PROTOTYPE(SPARQLEndpoint)

        SPARQLEndpoint() {}

        void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
            using namespace Pistache;
            using namespace Pistache::Http;
            try { // if something fails return an internal server error
                // only answer sparql querys to GET /sparql
                if (request.method() == Method::Get and request.resource().compare("/sparql") == 0) {
                    response.headers().add<SPARQLJSON>();

                    const Optional<std::string> &hasQuery = request.query().get("query");
                    // check if there is actually an query
                    if (not hasQuery.isEmpty()) {
                        const std::string sparqlQuery = urlDecode(hasQuery.get());
                            try { // execute the query
                                tnt::util::container::NDMap<size_t> result = _store->query(sparqlQuery);
                                // TODO: use json instead
                                auto stream = response.stream(Code::Ok);
                                for (const auto &item : result) {
                                    for (const auto &binding : item.first) {
                                        const std::string &materilizedBinding = _store->getTermIndex().inv().at(
                                                binding).operator*().getIdentifier();
                                        stream << materilizedBinding.c_str() << " ";
                                    }

                                    stream << ".\n";
                                }
                                stream << ends;
                            } catch (
                                    const std::exception &exc) { // if the execution of the query should fail return an internal server error
                                std::cout << exc.what() << std::endl;
                                response.send(Code::Internal_Server_Error);
                            }


                    } else {
                        auto stream = response.send(Code::Not_Found);
                    }
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
