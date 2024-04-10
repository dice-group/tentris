#include "SparqlStreamingEndpoint.hpp"

#include <spdlog/spdlog.h>

#include "dice/endpoint/ParseSPARQLQueryParam.hpp"
#include "dice/endpoint/SparqlJsonResultSAXWriter.hpp"

namespace dice::endpoint {


	SPARQLStreamingEndpoint::SPARQLStreamingEndpoint(tf::Executor &executor,
													 triple_store::TripleStore &triplestore,
													 SparqlQueryCache &sparql_query_cache,
													 std::chrono::seconds timeoutDuration)
		: executor_(executor),
		  triplestore_(triplestore),
		  sparql_query_cache_(sparql_query_cache),
		  timeout_duration_(timeoutDuration) {
	}
	restinio::request_handling_status_t SPARQLStreamingEndpoint::operator()(
			restinio::request_handle_t req,
			[[maybe_unused]] restinio::router::route_params_t params) {
		auto timeout = (timeout_duration_.count()) ? std::chrono::steady_clock::now() + this->timeout_duration_ : std::chrono::steady_clock::time_point::max();
		if (executor_.num_topologies() < executor_.num_workers()) {
			executor_.silent_async([this, timeout](restinio::request_handle_t req) {
				using namespace dice::sparql2tensor;
				using namespace restinio;

				std::shared_ptr<SPARQLQuery const> sparql_query = parse_sparql_query_param(req, this->sparql_query_cache_);
				if (not sparql_query)
					return;

				bool asio_write_failed = false;

				endpoint::SparqlJsonResultSAXWriter json_writer{sparql_query->projected_variables_, 100'000};

				response_builder_t<chunked_output_t> resp = req->template create_response<chunked_output_t>();
				resp.append_header(http_field::content_type, "application/sparql-results+json");

				try {
					for (auto const &entry : this->triplestore_.eval_select(*sparql_query, timeout)) {
						json_writer.add(entry);
						if (json_writer.full()) {
							resp.append_chunk(std::string{json_writer.string_view()});
							resp.flush([&](auto const &status) { asio_write_failed = status.failed(); });
							if (asio_write_failed) {
								spdlog::warn("Writing chunked HTTP response failed.");
								return;
							}
							json_writer.clear();
						}
					}
					json_writer.close();
					resp.append_chunk(std::string{json_writer.string_view()});
					resp.done();
					spdlog::info("HTTP response {}: {} variables, {} solutions, {} bindings",
								 status_ok(),
								 sparql_query->projected_variables_.size(),
								 json_writer.number_of_written_solutions(),
								 json_writer.number_of_written_bindings());
				} catch (std::runtime_error const &timeout_exception) {
					const auto timeout_message = fmt::format("Request processing timed out after {}.", this->timeout_duration_);
					spdlog::warn("HTTP response {}: {}", status_gateway_time_out(), timeout_message);
					req->create_response(status_gateway_time_out()).set_body(timeout_message).done();
				}
			},
								   std::move(req));
			return restinio::request_accepted();
		} else {
			spdlog::warn("Handling request was rejected. All workers are busy.");
			return restinio::request_rejected();
		}
	}
}// namespace dice::endpoint