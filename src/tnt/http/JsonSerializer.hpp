#ifndef TNT_JSONSERIALIZER_HPP
#define TNT_JSONSERIALIZER_HPP

#include "tnt/store/RDF/Term.hpp"
#include "tnt/store/SPARQL/Variable.hpp"
#include "tnt/util/HTTPUtils.hpp"
#include <pistache/http.h>
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"

#include "tnt/store/TripleStore.hpp"

namespace tnt::http {
    namespace {
        using Variable = tnt::store::sparql::Variable;
        using ResponseStream = Pistache::Http::ResponseStream;
        using namespace tnt::tensor::einsum::operators;
    };

    template<typename VALUE_TYPE>
    void stream_out(const std::vector<Variable> &vars, yield_pull<VALUE_TYPE> &results, ResponseStream &stream, const tnt::store::TripleStore &store) {
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
                store::Term &term = *store.getTermIndex().inv().at(binding);


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

                json_result << ",\"value\":\"" << escapeJsonString(term.get_value());
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
            for ([[maybe_unused]]  const auto &c : range(getCount<VALUE_TYPE>(result))) {
                if (firstResult) {
                    firstResult = false;
                    stream << json_result_binding.c_str();
                } else
                    stream << "," << json_result_binding.c_str();
            }
        }
        stream << "]}}\n" << Pistache::Http::ends;
    }
}

#endif //TNT_JSONSERIALIZER_HPP
