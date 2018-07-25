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
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"

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
            using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
            try { // if something fails return an internal server error
                // only answer sparql querys to GET /sparql
                if (request.method() == Method::Get and request.resource().compare("/sparql") == 0) {
                    response.headers().add<SPARQLJSON>();
                    const Optional<std::string> &hasQuery = request.query().get("query");
                    // check if there is actually an query
                    if (not hasQuery.isEmpty()) {
                        const std::string sparqlQuery = urlDecode(hasQuery.get());
                        try { // execute the query
                            ParsedSPARQL parsedSPARQL{sparqlQuery};
                            const std::vector<Variable> &vars = parsedSPARQL.getQueryVariables();
                            tnt::util::container::NDMap<size_t> result = _store->query(sparqlQuery);
                            auto stream = response.stream(Code::Ok);
                            stream_out(vars, result, stream);
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

        void stream_out(const std::vector<Variable> &vars, const util::container::NDMap<unsigned long> &result,
                        Pistache::Http::ResponseStream &stream) const {
            stream << "{\n";
            std::string commaSeparatedVars = "";
            bool firstTime = true;
            for (const auto &var : vars) {
                                std::string separator = " , ";
                                if (firstTime) {
                                    firstTime = false;
                                    separator = "";
                                }
                                commaSeparatedVars.append(separator + "\"" + var._var_name + "\"");
                            }
            std::string s = "\"head\": { \"vars\": [" + commaSeparatedVars +
                                     "] },\n\"results\": {\n\"bindings\": [";
            stream << s.c_str();
            bool firstResult = true;
            for (const auto &[key, count] : result) {
                                s = "{";
                                bool firstKey = true;
                                for (const auto &[binding, var] : zip(key, vars)) {
                                    store::Term &term = *_store->getTermIndex().inv().at(
                                            binding);
                                    const std::string &materializedBinding = term.getIdentifier();

                                    const store::Term::NodeType &termType = term.type();
                                    std::string type = "\"\"";
                                    switch (termType) {
                                        case store::Term::URI:
                                            type = "\"uri\"";
                                            break;
                                        case store::Term::BNode:
                                            type = "\"bnode\"";
                                            break;
                                        case store::Term::Literal:
                                            type = "\"literal\"";
                                            break;
                                    } //todo check default

                                    std::string t = ",\n";
                                    if (firstKey) {
                                        t = "";
                                        firstKey = false;
                                    }

                                    s += t + "\"" + var._var_name + "\": { ";
                                    s += "\"type\": " + type;
                                    s += ", \"value\":" + materializedBinding;
                                    if (termType == store::Term::Literal) {
                                        const store::Literal &literal = static_cast<store::Literal &>(term);
                                        if (literal.hasType())
                                            s += "\", datatype\":" + (std::string) literal.getType();
                                        else if (literal.hasLang())
                                            s += "\", xml:lang\":" + (std::string) literal.getLang();
                                    }
                                    s += "}";
                                }
                                s += "}";

                                for ([[maybe_unused]]  const auto& c : range(count+1)) {
                                    if (firstResult) {
                                        firstResult = false;
                                        stream << "\n" << s.c_str();
                                    } else
                                        stream << ",\n" << s.c_str();
                                }
                            }
            stream << "\n]}\n}\n";
            stream << Pistache::Http::ends;
        }
    };

}
#endif //HEALTHCHECK_HPP
