#ifndef HEALTHCHECK_HPP
#define HEALTHCHECK_HPP

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include "tnt/store/TripleStore.hpp"
#include <string>


class SPARQLJSON : public Pistache::Http::Header::Header {
public:
    NAME("Content-Type");

    SPARQLJSON() {}

    void parse(const std::string &str) {
    }

    void write(std::ostream &os) const {
        os << "application/sparql-results+json";
    }
};

class SPARQLEndpoint : public Pistache::Http::Handler {
public:
HTTP_PROTOTYPE(SPARQLEndpoint)
    tnt::store::TripleStore _store{};

    void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace Pistache;using namespace Pistache::Http;
        if (request.method() == Method::Get and request.resource().compare("/sparql") == 0) {

            const Optional<std::string> &hasQuery = request.query().get("query");

            response.headers().add<SPARQLJSON>();

            if (not hasQuery.isEmpty()) {
                const std::string sparqlQuery = hasQuery.get();
                const tnt::util::container::NDMap<unsigned long> result = _store.query(sparqlQuery);
                for (const auto &item : sparqlQuery) {
                    std::cout << item << std::endl;
                }

                auto stream = response.stream(Code::Ok);
                stream << "{\"found\":\"";
                stream << sparqlQuery.data();
                stream << "\"}";
//            for (const auto &[key, value] : result) {
//                stream << key << " " << value << "\n";
//            }
                stream << ends;
            } else {
                auto stream = response.stream(Code::Ok);
                stream << "{\"found\":\"None\"}";
                stream << ends;
            }
        } else {
            response.send(Code::I_m_a_teapot);
        }
    }
};

#endif //HEALTHCHECK_HPP
