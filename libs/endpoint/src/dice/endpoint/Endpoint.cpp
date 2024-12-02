#include "dice/endpoint/Endpoint.hpp"

namespace dice::endpoint {
    Endpoint::Endpoint(tf::Executor &executor,
                       triple_store::TripleStore &triplestore,
                       SparqlQueryCache &sparql_query_cache,
                       EndpointCfg const &endpoint_cfg)
        : executor_{executor},
          triplestore_{triplestore},
          sparql_query_cache_{sparql_query_cache},
          cfg_{endpoint_cfg} {}// endpoint


    restinio::request_handling_status_t Endpoint::operator()(
            restinio::request_handle_t req,
            [[maybe_unused]] restinio::router::route_params_t params) {
        auto const timeout = (cfg_.opt_timeout_duration)
                                     ? std::chrono::steady_clock::now() + cfg_.opt_timeout_duration.value()
                                     : std::chrono::steady_clock::time_point::max();
        if (executor_.num_topologies() < executor_.num_workers()) {
            executor_.silent_async([req = std::move(req), this, timeout]() mutable {
                try {
                    this->handle_query(std::move(req), timeout);
                } catch (std::runtime_error const &) {
                    const auto timeout_message = fmt::format("Request processing timed out after {}.",
                                                             this->cfg_.opt_timeout_duration.value());
                    spdlog::warn("HTTP response {}: {}", restinio::status_gateway_time_out(), timeout_message);
                    req->create_response(restinio::status_gateway_time_out())
                            .connection_close()
                            .set_body(timeout_message)
                            .done();
                }
            });
            spdlog::debug("Request was accepted.");
            return restinio::request_accepted();
        } else {
            spdlog::warn("Handling request was rejected. All workers are busy.");
            return restinio::request_rejected();
        }
    }

}// namespace dice::endpoint