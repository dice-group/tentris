#ifndef TENTRIS_SPARQLREQUESTTHREAD_HPP
#define TENTRIS_SPARQLREQUESTTHREAD_HPP

#include "tentris/http/JsonSerializer.hpp"
#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

#include <chrono>
#include <restinio/all.hpp>


#include <tentris/util/LogHelper.hpp>


namespace tentris::http {

	namespace {
		using ParsedSPARQL = tentris::store::sparql::ParsedSPARQL;
		using Variable = tentris::store::sparql::Variable;
		using SelectModifier = tentris::store::sparql::SelectModifier;
		using TripleStore = tentris::store::TripleStore;
		using namespace tentris::store::cache;
		using namespace std::chrono;
		using namespace ::tentris::logging;
		using namespace tentris::tensor;

	}; // namespace

	template<typename RESULT_TYPE>
	inline Status runQuery(restinio::request_handle_t &req, const std::shared_ptr<QueryExecutionPackage> &query_package,
	                       TripleStore &store,
	                       const QueryExecutionPackage::TimeoutType timeout, const std::vector<Variable> &vars) {
		logTrace("Running select query.");
		// calculate the result

		// check if it timed out
		if (system_clock::now() < timeout) {
			auto resp = req->create_response<restinio::chunked_output_t>(restinio::status_ok());
			resp.append_header(restinio::http_field::content_type, "application/sparql-results+json");
			resp.flush();
			return streamJSON<RESULT_TYPE>(vars, query_package, resp, store, timeout);
		} else {
			return Status::PROCESSING_TIMEOUT;
		}
	}

	/**
	 * Executes a QueryExecutionPackage and writes its as JSON to a result. The execution is stopped
	 * when time_out is reached.
	 * @param response the writer to write the result of the query to.
	 * @param query_package a query package that is executed to produce the result
	 * @param store a instance of a triple store
	 * @param time_out a time stamp after that the execution must be canceled
	 */
	Status runQuery(restinio::request_handle_t &req, const std::shared_ptr<QueryExecutionPackage> &query_package,
	                TripleStore &store) {

		const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
		const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();
		const QueryExecutionPackage::TimeoutType timeout = query_package->getTimeout();
		switch (sparqlQuery.getSelectModifier()) {
			case SelectModifier::NONE: {
				return runQuery<COUNTED_t>(req, query_package, store, timeout, vars);
			}
			case SelectModifier::DISTINCT: {
				return runQuery<DISTINCT_t>(req, query_package, store, timeout, vars);
			}
			default:
				break;
		}
		logTrace("Query type is not yet supported.");
		return Status::UNPARSABLE;
	}


} // namespace tentris::http


#endif // TENTRIS_SPARQLREQUESTTHREAD_HPP
