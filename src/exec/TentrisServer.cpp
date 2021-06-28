#include <csignal>
#include <filesystem>

#include "config/ServerConfig.hpp"
#include <restinio/all.hpp>
#include <tentris/http/GraphQLEndpoint.hpp>
#include <tentris/http/SPARQLEndpoint.hpp>
#include <tentris/store/AtomicGraphqlSchema.hpp>
#include <tentris/store/AtomicTripleStore.hpp>
#include <tentris/store/config/AtomicTripleStoreConfig.cpp>
#include <tentris/store/graphql/GraphQLParser.hpp>

#include <fmt/format.h>

#include "VersionStrings.hpp"
#include "config/ServerConfig.hpp"

void bulkload(const std::string &triple_file, size_t bulksize) {
	namespace fs = std::filesystem;
	using namespace fmt::literals;
	using namespace tentris::logging;

	// log the starting time and print resource usage information
	auto loading_start_time = log_health_data();

	if (fs::is_regular_file(triple_file)) {
		log("nt-file: {} loading ..."_format(triple_file));
		::tentris::store::AtomicTripleStore::getInstance().bulkloadRDF(triple_file, bulksize);
	} else {
		log("nt-file {} was not found."_format(triple_file));
		log("Exiting ...");
		std::exit(EXIT_FAILURE);
	}
	// log the end time and print resource usage information
	auto loading_end_time = log_health_data();
	// log the time it tool to load the file
	log_duration(loading_start_time, loading_end_time);
}

struct tentris_restinio_traits : public restinio::traits_t<
										 restinio::null_timer_manager_t,
#ifdef DEBUG
										 restinio::shared_ostream_logger_t,
#else
										 restinio::null_logger_t,
#endif
										 restinio::router::express_router_t<>> {
	static constexpr bool use_connection_count_limiter = true;
};


int main(int argc, char *argv[]) {
	using namespace tentris::http;
	using namespace tentris::store::config;
	using namespace fmt::literals;
	using namespace tentris::logging;

	ServerConfig cfg{argc, argv};

	init_logging(cfg.logstdout, cfg.logfile, cfg.logfiledir, cfg.loglevel);

	log("Running {} with {}"_format(tentris_version_string, hypertrie_version_string));

	auto &store_cfg = AtomicTripleStoreConfig::getInstance();
	store_cfg.rdf_file = cfg.rdf_file;
	store_cfg.timeout = cfg.timeout;
	store_cfg.cache_size = cfg.cache_size;
	store_cfg.threads = cfg.threads;

	// bulkload file
	if (not cfg.rdf_file.empty()) {
		bulkload(cfg.rdf_file, cfg.bulksize);
	} else {
		log("No file loaded.");
	}
	// parse graphql schema if provided
	if (not cfg.graphql_schema.empty()) {
		// save contents of the document to a string
		std::ifstream schema_file;
		schema_file.open(cfg.graphql_schema);
		std::stringstream schema_str_stream;
		schema_str_stream << schema_file.rdbuf();
		tentris::store::graphql::GraphQLParser::parseSchema(schema_str_stream.str(),
															::tentris::store::AtomicGraphqlSchema::getInstance());
	}

	// create endpoint
	using namespace restinio;
	auto router = std::make_unique<router::express_router_t<>>();
	router->http_get(
			R"(/sparql)",
			tentris::http::sparql_endpoint::SparqlEndpoint<restinio::restinio_controlled_output_t>{});
	router->http_get(
			R"(/stream)",
			tentris::http::sparql_endpoint::SparqlEndpoint<restinio::chunked_output_t>{});
	router->http_get(
			R"(/graphql)",
			tentris::http::graphql_endpoint::GraphQLEndpoint{});

	router->non_matched_request_handler(
			[](auto req) -> restinio::request_handling_status_t {
				return req->create_response(restinio::status_not_found()).connection_close().done();
			});

	// Launching a server with custom traits.

	log("SPARQL endpoint serving sparkling linked data treasures on {} threads at http://0.0.0.0:{}/sparql?query="_format(cfg.threads, cfg.port));

	restinio::run(
			restinio::on_thread_pool<tentris_restinio_traits>(cfg.threads)
					.max_parallel_connections(cfg.threads)
					.address("0.0.0.0")
					.port(cfg.port)
					.request_handler(std::move(router))
					.handle_request_timeout(cfg.timeout)
					.write_http_response_timelimit(cfg.timeout));
	log("Shutdown successful.");
	return EXIT_SUCCESS;
}
