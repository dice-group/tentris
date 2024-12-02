#ifndef TENTRIS_COUNTENDPOINT_HPP
#define TENTRIS_COUNTENDPOINT_HPP

#include <dice/endpoint/Endpoint.hpp>

namespace dice::endpoint {

	class CountEndpoint final : public Endpoint {
	public:
		CountEndpoint(tf::Executor &executor, triple_store::TripleStore &triplestore, SparqlQueryCache &sparql_query_cache, EndpointCfg const &endpoint_cfg);

	protected:
		void handle_query(restinio::request_handle_t req, std::chrono::steady_clock::time_point timeout) override;
	};
}// namespace dice::endpoint
#endif//TENTRIS_COUNTENDPOINT_HPP