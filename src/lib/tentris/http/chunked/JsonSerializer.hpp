#ifndef TENTRIS_JSONSERIALIZER_HPP
#define TENTRIS_JSONSERIALIZER_HPP

#include "tentris/store/RDF/Term.hpp"
#include "tentris/store/SPARQL/Variable.hpp"
#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/util/HTTPUtils.hpp"
#include "tentris/http/QueryResultState.hpp"


#include <iostream>


namespace tentris::http {
	namespace {
		using namespace tentris::tensor;
		using Variable = tentris::store::sparql::Variable;
		using namespace std::chrono;
		using QueryExecutionPackage = tentris::store::cache::QueryExecutionPackage;
		using Status = tentris::http::ResultState;
		using namespace tentris::store::rdf;
	}; // namespace

	auto asio_error_callback = [](const auto &ec) {
		if (ec)
			logError("ASIO Error Category {}, Code {}, Message {}"_format(
					ec.category().name(), ec.value(), ec.message()));
	};

	template<typename RESULT_TYPE>
	Status streamJSON(std::shared_ptr<QueryExecutionPackage> &query_package,
					  restinio::response_builder_t<restinio::chunked_output_t> &stream, const system_clock::time_point &timeout) {
		using namespace std::string_literals;

		ulong result_count = 0;
		ulong result_count_multiplyer = 0;
		const ulong flush_result_count = 500;

		const std::vector<Variable> &vars = query_package->getQueryVariables();

		stream.append_chunk(R"({"head":{"vars":[)");
		bool firstTime = true;
		for (const auto &var : vars) {
			if (firstTime) {
				firstTime = false;
				stream.append_chunk("\"{}\""_format(var.name));
			} else {
				stream.append_chunk(",\"{}\""_format(var.name));
			}
		}
		stream.append_chunk(R"(]},"results":{"bindings":[)");
		bool firstResult = true;

		if (not query_package->is_trivial_empty) {
			std::shared_ptr<void> raw_results = query_package->getEinsum();
			auto &results = *static_cast<Einsum<RESULT_TYPE> *>(raw_results.get());


			for (const EinsumEntry<RESULT_TYPE> &result : results) {
				std::stringstream json_result{};
				json_result << "{";
				bool firstKey = true;
				for (const auto[term, var] : zip(result.key, vars)) {
					if (term == nullptr)
						continue;


					if (firstKey) {
						firstKey = false;
					} else {
						json_result << ",";
					}

					json_result << "\"" << var.name << "\":{";

					const Term::NodeType termType = term->type();
					switch (termType) {
						case Term::URI:
							json_result << R"("type":"uri")";
							break;
						case Term::BNode:
							json_result << R"("type":"bnode")";
							break;
						case Term::Literal:
							json_result << R"("type":"literal")";
							break;
						case Term::None:
							log("Uncomplete term with no type (Literal, BNode, URI) detected.");
							assert(false);
					}

					json_result << R"(,"value":")" << escapeJsonString(term->value());
					if (termType == Term::Literal) {
						if (term->hasDataType())
							json_result << R"(","datatype":")" << term->dataType();
						else if (term->hasLang())
							json_result << R"(","xml:lang":")" << term->lang();
					}
					json_result << "\"}";
				}
				json_result << "}";

				std::string json_result_binding = json_result.str();
				for ([[maybe_unused]] const auto &c : range(result.value)) {
					result_count += vars.size();
					if (firstResult) {
						firstResult = false;
						stream.append_chunk(json_result_binding);
					} else {
						stream.append_chunk(",");
						stream.append_chunk(json_result_binding);
						// flush the content from time to time.
						if (result_count > flush_result_count) {
							if (system_clock::now() > timeout) {
								auto bindings_count = (result_count + result_count_multiplyer * flush_result_count);
								log(" ## wrote {} results with {} vars, total of {} entries"_format(
										bindings_count / vars.size(), vars.size(), bindings_count)
								);
								stream.append_chunk("]}}\n");
								stream.done(asio_error_callback);
								return Status::SERIALIZATION_TIMEOUT;
							} else {
								result_count = 0;
								++result_count_multiplyer;
								stream.flush(asio_error_callback);
							}
						}
					}
				}
			}
		}
		auto bindings_count = (result_count + result_count_multiplyer * flush_result_count);
		log(" ## wrote {} results with {} vars, total of {} entries"_format(
				bindings_count / vars.size(), vars.size(), bindings_count)
		);
		if (bindings_count == 0)
			query_package->is_trivial_empty = true;

		stream.append_chunk("]}}\n");

		stream.done(asio_error_callback);
		return Status::OK;
	}
} // namespace tentris::http

#endif // TENTRIS_JSONSERIALIZER_HPP
