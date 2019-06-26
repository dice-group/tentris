#ifndef TENTRIS_JSONSERIALIZER_HPP
#define TENTRIS_JSONSERIALIZER_HPP

#include "tentris/http/TimeoutException.hpp"
#include "tentris/store/RDF/Term.hpp"
#include "tentris/store/SPARQL/Variable.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tentris/util/HTTPUtils.hpp"
#include "QueryResultState.hpp"


#include <pistache/http.h>

#include <iostream>

namespace {
	using Variable = tentris::store::sparql::Variable;
	using ResponseStream = Pistache::Http::ResponseStream;
	using namespace tentris::tensor::einsum::operators;
	using namespace std::chrono;
	using TripleStore = tentris::store::TripleStore;
	using namespace Pistache::Http;
    using Status = tentris::http::ResultState ;
    using namespace tentris::store::rdf;
}; // namespace
namespace tentris::http {
	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	Status streamJSON(const std::vector<Variable> &vars, yield_pull<RESULT_TYPE> results, ResponseStream &stream,
				   const TripleStore &store, const system_clock::time_point &timeout) {
		ulong result_count = 0;
		const ulong flush_result_count = 500;

		stream << "{\"head\":{\"vars\":[";
		bool firstTime = true;
		for (const auto &var : vars) {
			if (firstTime) {
				firstTime = false;
				stream << "\"" << var.name.c_str() << "\"";
			} else {
				stream << ",\"" << var.name.c_str() << "\"";
			}
		}
		stream << "]},\"results\":{\"bindings\":[";
		bool firstResult = true;

		for (const auto &result : results) {
			const Key_t &key = RESULT_TYPE::getKey(result);
			std::stringstream json_result{};
			json_result << "{";
			bool firstKey = true;
			for (const auto[binding, var] : zip(key, vars)) {
				Term &term = *store.getTermIndex().inv().at(binding);

				if (firstKey) {
					firstKey = false;
				} else {
					json_result << ",";
				}

				json_result << "\"" << var.name << "\":{";

				const Term::NodeType &termType = term.type();
				switch (termType) {
					case Term::URI:
						json_result << "\"type\":\"uri\"";
						break;
					case Term::BNode:
						json_result << "\"type\":\"bnode\"";
						break;
					case Term::Literal:
						json_result << "\"type\":\"literal\"";
						break;
				} // todo check default

				json_result << ",\"value\":\"" << escapeJsonString(term.get_value());
				if (termType == Term::Literal) {
					const Literal &literal = static_cast<Literal &>(term);
					if (literal.hasType())
						json_result << "\",\"datatype\":\"" << literal.getType();
					else if (literal.hasLang())
						json_result << "\",\"xml:lang\":\"" << literal.getLang();
				}
				json_result << "\"}";
			}
			json_result << "}";

			std::string json_result_binding = json_result.str();
			for ([[maybe_unused]] const auto &c : range(RESULT_TYPE::getCount(result))) {
                result_count += vars.size();
				if (firstResult) {
					firstResult = false;
					stream << json_result_binding.c_str();
				} else {
					stream << "," << json_result_binding.c_str();
					// flush the content from time to time.
                    if (result_count > flush_result_count) {
                        if (system_clock::now() > timeout) {
                            stream << "]}}\n" << ends;
                            return Status::SERIALIZATION_TIMEOUT;
                        } else {
                            result_count = 0;
                            stream << flush;
                        }
                    }
				}
			}
		}
		stream << "]}}\n" << ends;
		return Status::OK;
	}
} // namespace tentris::http

#endif // TENTRIS_JSONSERIALIZER_HPP
