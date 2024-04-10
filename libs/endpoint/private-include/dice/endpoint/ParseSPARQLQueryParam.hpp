#ifndef TENTRIS_PARSESPARQLQUERYPARAM_HPP
#define TENTRIS_PARSESPARQLQUERYPARAM_HPP

#include <spdlog/spdlog.h>

#include <restinio/request_handler.hpp>
#include <restinio/uri_helpers.hpp>

#include <dice/sparql2tensor/SPARQLQuery.hpp>

#include <dice/endpoint/SparqlQueryCache.hpp>

namespace dice::endpoint {

	inline std::shared_ptr<sparql2tensor::SPARQLQuery const> parse_sparql_query_param(restinio::request_handle_t &req, SparqlQueryCache &cache) {
		using namespace dice::sparql2tensor;
		using namespace restinio;
		const auto qp = parse_query<restinio::parse_query_traits::javascript_compatible>(req->header().query());
		if (not qp.has("query")) {
			static auto const message = "Query parameter 'query' is missing.";
			spdlog::warn("HTTP response {}: {}", status_bad_request(), message);
			req->create_response(status_bad_request()).set_body(message).done();
			return {};
		}
		std::string sparql_query_str = std::string{qp["query"]};
		SPARQLQuery sparql_query;
		try {
			return cache[sparql_query_str];
		} catch (std::exception &ex) {
			static auto const message = "Value of query parameter 'query' is not parsable.";
			spdlog::warn("HTTP response {}: {} (detail: {})", status_bad_request(), message, ex.what());
			req->create_response(status_bad_request()).set_body(message).done();
			return {};
		}
	}
}// namespace dice::endpoint
#endif//TENTRIS_PARSESPARQLQUERYPARAM_HPP
