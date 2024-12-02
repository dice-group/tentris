#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#define nsel_CONFIG_SELECT_EXPECTED nsel_EXPECTED_NONSTD
#include <restinio/all.hpp>
#include <taskflow/taskflow.hpp>

#include <dice/triple-store/TripleStore.hpp>

#include <dice/endpoint/EndpointCfg.hpp>
#include <dice/endpoint/SparqlQueryCache.hpp>

namespace dice::endpoint {

	class Endpoint {
	protected:
		tf::Executor &executor_;

		triple_store::TripleStore &triplestore_;

		SparqlQueryCache &sparql_query_cache_;

		EndpointCfg const cfg_;

	protected:
		virtual void handle_query(restinio::request_handle_t req, std::chrono::steady_clock::time_point timeout) = 0;

	public:
		Endpoint(tf::Executor &executor, triple_store::TripleStore &triplestore, SparqlQueryCache &sparql_query_cache, EndpointCfg const &endpoint_cfg);
		virtual ~Endpoint() = default;
		virtual restinio::request_handling_status_t operator()(restinio::request_handle_t req, restinio::router::route_params_t params) final;
	};
}// namespace dice::endpoint

#endif//ENDPOINT_HPP