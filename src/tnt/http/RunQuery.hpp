#ifndef TNT_SPARQLREQUESTTHREAD_HPP
#define TNT_SPARQLREQUESTTHREAD_HPP

#include "tnt/http/JsonSerializer.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/store/TripleStore.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"

#include <chrono>
#include <pistache/http.h>

#include <tnt/util/LogHelper.hpp>

namespace {
	using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
	using ResponseWriter = Pistache::Http::ResponseWriter;
	using Variable = tnt::store::sparql::Variable;
	using SelectModifier = tnt::store::sparql::SelectModifier;
	using TripleStore = tnt::store::TripleStore;
	using namespace tnt::store::cache;
	using namespace tnt::tensor::einsum::operators;
	using Code = Pistache::Http::Code;
	using namespace std::chrono;
	using namespace ::tnt::logging;
}; // namespace
namespace tnt::http {
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
		const auto timeout = query_package->timeout;
		switch (sparqlQuery.getSelectModifier()) {
			case SelectModifier::NONE: {
				logDebug("Running select query.");
				// calculate the result
				auto result_generator = query_package->getRegularGenerator();
				// check if it timed out
				if (system_clock::now() > timeout) {
					response.headers().add<SPARQLJSON>();
					auto stream = response.stream(Code::Ok);
					steamJSON<counted_binding>(vars, std::move(result_generator), stream, store, timeout);
					query_package->done();
				} else {
					query_package->canceled();
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
				if (timeout < system_clock::now()) {
					response.headers().add<SPARQLJSON>();
					auto stream = response.stream(Code::Ok);
					steamJSON<distinct_binding>(vars, std::move(result_generator), stream, store, timeout);
					query_package->done();
				} else {
					query_package->canceled();
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
} // namespace tnt::http

#endif // TNT_SPARQLREQUESTTHREAD_HPP
