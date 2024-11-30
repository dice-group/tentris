#ifndef TENTRIS_HTTPSERVER_HPP
#define TENTRIS_HTTPSERVER_HPP

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


	class HTTPServer {
		tf::Executor &executor_;
		triple_store::TripleStore &triplestore_;
		SparqlQueryCache sparql_query_cache_;
		std::unique_ptr<restinio::router::express_router_t<>> router_;
		EndpointCfg cfg_;

	public:
		HTTPServer(tf::Executor &executor, triple_store::TripleStore &triplestore, EndpointCfg const &cfg);

		restinio::router::express_router_t<> &router() {
			return *router_;
		}

		void operator()();
	};
}// namespace dice::endpoint

#endif//TENTRIS_HTTPSERVER_HPP