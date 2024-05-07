#ifndef TENTRIS_HTTPSERVER_HPP
#define TENTRIS_HTTPSERVER_HPP

#define _LARGEFILE64_SOURCE
#include <restinio/all.hpp>
#include <taskflow/taskflow.hpp>

#include <dice/node-store/metall_manager.hpp>
#include <dice/triple-store/TripleStore.hpp>

#include <dice/endpoint/SparqlQueryCache.hpp>


namespace dice::endpoint {

	struct EndpointCfg {
		uint16_t port;
		uint16_t threads;
		std::chrono::seconds timeout_duration;
	};

	class HTTPServer {
		tf::Executor &executor_;
		triple_store::TripleStore &triplestore_;
		SparqlQueryCache sparql_query_cache_;
		std::unique_ptr<restinio::router::express_router_t<>> router_;
		EndpointCfg cfg_;

	public:
		HTTPServer(tf::Executor &executor, triple_store::TripleStore &triplestore, EndpointCfg const &cfg);

		restinio::router::express_router_t<> &router(){
			return *router_;
		}

		void operator()();
	};
}// namespace dice::endpoint

#endif//TENTRIS_HTTPSERVER_HPP
