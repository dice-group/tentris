#ifndef TENTRIS_SPARQLREQUESTTHREAD_HPP
#define TENTRIS_SPARQLREQUESTTHREAD_HPP

#include "tentris/http/JsonSerializer.hpp"
#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/tensor/einsum/operator/GeneratorInterface.hpp"

#include <chrono>
#include <restinio/all.hpp>


#include <tentris/util/LogHelper.hpp>

namespace {
	using ParsedSPARQL = tentris::store::sparql::ParsedSPARQL;
	using Variable = tentris::store::sparql::Variable;
	using SelectModifier = tentris::store::sparql::SelectModifier;
	using TripleStore = tentris::store::TripleStore;
	using namespace tentris::store::cache;
	using namespace tentris::tensor::einsum::operators;
	using namespace std::chrono;
	using namespace ::tentris::logging;
}; // namespace
namespace tentris::http {
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
		const auto timeout = query_package->getTimeout();
		switch (sparqlQuery.getSelectModifier()) {
			case SelectModifier::NONE: {
				logTrace("Running select query.");
				// calculate the result
				auto result_generator = query_package->getRegularGenerator();
				// check if it timed out
				if (system_clock::now() < timeout) {
					auto resp = req->create_response<restinio::chunked_output_t>(restinio::status_ok());
					resp.append_header( restinio::http_field::content_type, "application/sparql-results+json" );
					resp.flush();
					return streamJSON<counted_binding>(vars, std::move(result_generator), resp, store, timeout);
				} else{
				    return Status::PROCESSING_TIMEOUT;
				}
			}
			case SelectModifier::DISTINCT: {
				logTrace("Running select distinct query.");
                auto result_generator = query_package->getDistinctGenerator();
                // check if it timed out
                if (system_clock::now() < timeout) {
	                auto resp = req->create_response<restinio::chunked_output_t>(restinio::status_ok());
	                resp.append_header( restinio::http_field::content_type, "application/sparql-results+json" );
	                resp.flush();
                    return streamJSON<distinct_binding>(vars, std::move(result_generator), resp, store, timeout);
                } else{
                    return Status::PROCESSING_TIMEOUT;
                }
			}
            default:
                break;
		}
		logTrace("Query type is not yet supported.");
        return Status::UNPARSABLE;
	}
} // namespace tentris::http

#endif // TENTRIS_SPARQLREQUESTTHREAD_HPP
