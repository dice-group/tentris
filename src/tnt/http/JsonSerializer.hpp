#ifndef TNT_JSONSERIALIZER_HPP
#define TNT_JSONSERIALIZER_HPP

#include "tnt/http/TimeoutException.hpp"
#include "tnt/store/RDF/Term.hpp"
#include "tnt/store/SPARQL/Variable.hpp"
#include "tnt/store/TripleStore.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/util/HTTPUtils.hpp"

#include <pistache/http.h>

#include <iostream>

namespace {
	using Variable = tnt::store::sparql::Variable;
	using ResponseStream = Pistache::Http::ResponseStream;
	using namespace tnt::tensor::einsum::operators;
	using namespace std::chrono;
	using TripleStore = tnt::store::TripleStore;
	using namespace Pistache::Http;
	using namespace tnt::store::rdf;
}; // namespace
namespace tnt::http {
	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	void steamJSON(const std::vector<Variable> &vars, yield_pull<RESULT_TYPE> results, ResponseStream &stream,
	               const TripleStore &store, const system_clock::time_point &timeout) {
		ulong result_count = 0;

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
			for (const auto &[binding, var] : zip(key, vars)) {
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

			if (result_count += RESULT_TYPE::getCount(result); result_count > 20) {
				if (system_clock::now() > timeout) {
					stream << "]}}\n" << ends;
					throw TimeoutException{result_count};
				} else {
					result_count = 0;
				}
			}

			std::string json_result_binding = json_result.str();
			for ([[maybe_unused]] const auto &c : range(RESULT_TYPE::getCount(result))) {
				if (firstResult) {
					firstResult = false;
					stream << json_result_binding.c_str();
				} else {
					stream << "," << json_result_binding.c_str();
				}
			}
		}
		stream << "]}}\n" << ends;
	}
} // namespace tnt::http

#endif // TNT_JSONSERIALIZER_HPP
