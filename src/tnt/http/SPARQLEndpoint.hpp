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


class SPARQLEndpoint : public Pistache::Http::Handler {
public:
HTTP_PROTOTYPE(SPARQLEndpoint)
    tnt::store::TripleStore _store{};

    SPARQLEndpoint() { _store.loadRDF("/home/usr/Code/tnt/tests/dataset/sp2b.nt"); }

    void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response) {
        using namespace Pistache;
        using namespace Pistache::Http;
        if (request.method() == Method::Get and request.resource().compare("/sparql") == 0) {

            const Optional<std::string> &hasQuery = request.query().get("query");
            Uri::Query query = request.query();


            response.headers().add<SPARQLJSON>();

            if (not hasQuery.isEmpty()) {
                const std::string sparqlQuery = urlDecode(hasQuery.get());
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
