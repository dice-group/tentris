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

    namespace {
        using namespace Pistache;
        using namespace Pistache::Http;
        using namespace tnt::util::types;
        using namespace tensor::einsum;
        using namespace tensor::einsum::operators;
        using namespace tnt::store::sparql;
        using namespace Pistache::Http;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
        using Operands =  typename std::vector<BoolHyperTrie *>;
        using key_part_t = tnt::util::types::key_part_t;
        template<typename V>
        using NDMap = tnt::util::container::NDMap<V>;
    }


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
                            const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();
                            switch (sparqlQuery.getSelectModifier()) {
                                case SelectModifier::NONE: {
                                    yield_pull<INT_VALUES> results = _store->query<INT_VALUES>(sparqlQuery);
                                    auto stream = response.stream(Code::Ok);
                                    stream_out<INT_VALUES>(vars, results, stream);
                                    break;
                                }
                                case SelectModifier::DISTINCT: {
                                    yield_pull<BOOL_VALUES> results = _store->query<BOOL_VALUES>(sparqlQuery);
                                    auto stream = response.stream(Code::Ok);
                                    stream_out<BOOL_VALUES>(vars, results, stream);
                                    break;
                                }
                                default:
                                    response.send(Code::Not_Implemented);
                            }


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

        template<typename VALUE_TYPE>
        void stream_out(const std::vector<Variable> &vars, yield_pull<VALUE_TYPE> &results,
                        ResponseStream &stream) const {
            stream << "{\"head\":{\"vars\":[";
            bool firstTime = true;
            for (const auto &var : vars) {
                if (firstTime) {
                    firstTime = false;
                    stream << "\"" << var._var_name.c_str() << "\"";
                } else {
                    stream << ",\"" << var._var_name.c_str() << "\"";
                }
            }
            stream << "]},\"results\":{\"bindings\":[";
            bool firstResult = true;
            for (const auto &result : results) {
                const Key_t &key = getKey<VALUE_TYPE>(result);
                std::stringstream json_result{};
                json_result << "{";
                bool firstKey = true;
                for (const auto &[binding, var] : zip(key, vars)) {
                    store::Term &term = *_store->getTermIndex().inv().at(binding);


                    if (firstKey) {
                        firstKey = false;
                    } else {
                        json_result << ",";
                    }

                    json_result << "\"" << var._var_name << "\":{";

                    const store::Term::NodeType &termType = term.type();
                    switch (termType) {
                        case store::Term::URI:
                            json_result << "\"type\":\"uri\"";
                            break;
                        case store::Term::BNode:
                            json_result << "\"type\":\"bnode\"";
                            break;
                        case store::Term::Literal:
                            json_result << "\"type\":\"literal\"";
                            break;
                    } //todo check default

                    json_result << ",\"value\":\"" << term.get_value();
                    if (termType == store::Term::Literal) {
                        const store::Literal &literal = static_cast<store::Literal &>(term);
                        if (literal.hasType())
                            json_result << "\",\"datatype\":\"" << literal.getType();
                        else if (literal.hasLang())
                            json_result << "\",\"xml:lang\":" << literal.getLang();
                    }
                    json_result << "\"}";
                }
                json_result << "}";

                std::string json_result_binding = json_result.str();
                std::cout << json_result_binding << std::endl;
                for ([[maybe_unused]]  const auto &c : range(getCount<VALUE_TYPE>(result))) {
                    if (firstResult) {
                        firstResult = false;
                        stream << json_result_binding.c_str();
                    } else
                        stream << "," << json_result_binding.c_str();
                }
            }
            stream << "]}}\n" << ends;
        }
    };


}
#endif //HEALTHCHECK_HPP
