#ifndef TENTRIS_SPARQLENDPOINT_HPP
#define TENTRIS_SPARQLENDPOINT_HPP

#include <dice/endpoint/Endpoint.hpp>

namespace dice::endpoint {

	class SPARQLEndpoint final : public Endpoint {
	public:
		SPARQLEndpoint(tf::Executor &executor, triple_store::TripleStore &triplestore, SparqlQueryCache &sparql_query_cache, EndpointCfg const &endpoint_cfg);

	protected:
		void handle_query(restinio::request_handle_t req, std::chrono::steady_clock::time_point timeout) override;
	};

}// namespace dice::endpoint
#endif//TENTRIS_SPARQLENDPOINT_HPP