#ifndef TENTRIS_SPARQLSTREAMINGENDPOINT_HPP
#define TENTRIS_SPARQLSTREAMINGENDPOINT_HPP

#include <restinio/all.hpp>
#include <taskflow/taskflow.hpp>

#include <dice/node-store/metall_manager.hpp>
#include <dice/triple-store/TripleStore.hpp>

#include <dice/endpoint/SparqlQueryCache.hpp>

namespace dice::endpoint {

	class SPARQLStreamingEndpoint {
		tf::Executor &executor_;

		triple_store::TripleStore &triplestore_;

		SparqlQueryCache &sparql_query_cache_;

		std::chrono::seconds timeout_duration_;

	public:
		SPARQLStreamingEndpoint(tf::Executor &executor, triple_store::TripleStore &triplestore, SparqlQueryCache &sparql_query_cache, std::chrono::seconds timeoutDuration);

		restinio::request_handling_status_t operator()(
				restinio::request_handle_t req,
				restinio::router::route_params_t params);
	};
}// namespace dice::endpoint

#endif//TENTRIS_SPARQLSTREAMINGENDPOINT_HPP
