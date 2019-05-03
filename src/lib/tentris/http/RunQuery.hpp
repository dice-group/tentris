#ifndef TENTRIS_SPARQLREQUESTTHREAD_HPP
#define TENTRIS_SPARQLREQUESTTHREAD_HPP

#include "tentris/http/JsonSerializer.hpp"
#include "tentris/http/JSONSparqlResultsHeader.hpp"
#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/tensor/einsum/operator/GeneratorInterface.hpp"

#include <chrono>
#include <pistache/http.h>

#include <tentris/util/LogHelper.hpp>

namespace {
	using ParsedSPARQL = tentris::store::sparql::ParsedSPARQL;
	using ResponseWriter = Pistache::Http::ResponseWriter;
	using Variable = tentris::store::sparql::Variable;
	using SelectModifier = tentris::store::sparql::SelectModifier;
	using TripleStore = tentris::store::TripleStore;
	using namespace tentris::store::cache;
	using namespace tentris::tensor::einsum::operators;
	using Code = Pistache::Http::Code;
	using namespace std::chrono;
	using namespace ::tentris::logging;
}; // namespace
namespace tentris::http {
	/**
	 * Executes a QueryExecutionPackage and writes its as JSON to a Pistache ResponseWriter. The execution is stopped
	 * when time_out is reached.
	 * @param response the writer to write the result of the query to.
	 * @param query_package a query package that is executed to produce the result
	 * @param store a instance of a triple store
	 * @param time_out a time stamp after that the execution must be canceled
	 */
	void runQuery(ResponseWriter &response, const std::shared_ptr<QueryExecutionPackage> &query_package,
				  TripleStore &store) {

		const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
		const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();
		const auto timeout = query_package->getTimeout();
		switch (sparqlQuery.getSelectModifier()) {
			case SelectModifier::NONE: {
				logDebug("Running select query.");
				// calculate the result
				auto result_generator = query_package->getRegularGenerator();
				// check if it timed out
				if (system_clock::now() < timeout) {
					response.headers().add<SPARQLJSON>();
					auto stream = response.stream(Code::Ok);
					steamJSON<counted_binding>(vars, std::move(result_generator), stream, store, timeout);
				} else {
					response.send(Code::Request_Timeout);
					throw TimeoutException{};
				}
				break;
			}
			case SelectModifier::DISTINCT: {
				logDebug("Running select distinct query.");
				// calculate the result
				auto result_generator = query_package->getDistinctGenerator();
				// check if it timed out
				if (system_clock::now() < timeout) {
					response.headers().add<SPARQLJSON>();
					auto stream = response.stream(Code::Ok);
					steamJSON<distinct_binding>(vars, std::move(result_generator), stream, store, timeout);
				} else {
					response.send(Code::Request_Timeout);
					throw TimeoutException{};
				}
				break;
			}
			default:
				logDebug("Query type is not yet supported.");
				response.send(Code::Not_Implemented);
		}
	}
} // namespace tentris::http

#endif // TENTRIS_SPARQLREQUESTTHREAD_HPP
