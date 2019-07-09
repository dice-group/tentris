#define RESTINIO_USE_BOOST_ASIO

#include <filesystem>
#include <csignal>

#include <tentris/store/TripleStore.hpp>
#include "config/ServerConfig.hpp"
#include <tentris/store/AtomicTripleStore.hpp>
#include <tentris/store/config/AtomicTripleStoreConfig.cpp>
#include <tentris/http/SPARQLEndpoint.hpp>
#include <restinio/all.hpp>

#include <fmt/format.h>


namespace {
	using namespace Pistache;
	using namespace tentris::http;
	using namespace tentris::store::config;
	namespace fs = std::filesystem;
	using namespace fmt::literals;
}

void bulkload(std::string triple_file) {

	// log the starting time and print resource usage information
	auto loading_start_time = log_health_data();

	if (fs::is_regular_file(triple_file)) {
		log("nt-file: {} loading ..."_format(triple_file));
		AtomicTripleStore::getInstance().loadRDF(triple_file);
	} else {
		log("nt-file {} was not found."_format(triple_file));
		log("Exiting ...");
		std::exit(EXIT_FAILURE);
	}
	log("Loaded {} triples.\n"_format(AtomicTripleStore::getInstance().size()));

	// log the end time and print resource usage information
	auto loading_end_time = log_health_data();
	// log the time it tool to load the file
	log_duration(loading_start_time, loading_end_time);
}

int main(int argc, char *argv[]) {
	init_logging();

	ServerConfig cfg{argc, argv};

	auto &store_cfg = AtomicTripleStoreConfig::getInstance();
	store_cfg.rdf_file = cfg.rdf_file;
	store_cfg.timeout = cfg.timeout;
	store_cfg.cache_size = cfg.cache_size;
	store_cfg.cache_bucket_capacity = cfg.cache_bucket_capacity;

	// bulkload file
	if (not cfg.rdf_file.empty()) {
		bulkload(cfg.rdf_file);
	} else {
		log("No file loaded. Use '-f yourfile.nt' to bulkload a file.");
	}

	// create endpoint
	using namespace restinio;
	auto router = std::make_unique<router::express_router_t<>>();
	router->http_get(
			R"(/sparql)",
			tentris::http::sparql_endpoint);

	router->non_matched_request_handler(
			[](auto req) -> restinio::request_handling_status_t {
				return req->create_response(restinio::status_not_found()).connection_close().done();
			});

	// Launching a server with custom traits.

	using traits_t =
	restinio::traits_t<
			restinio::asio_timer_manager_t,
			restinio::shared_ostream_logger_t,
			restinio::router::express_router_t<>
	>;

	restinio::run(
			restinio::on_thread_pool<traits_t>(cfg.threads)
					.address("localhost")
					.port(cfg.port)
					.request_handler(std::move(router))
					.handle_request_timeout(cfg.timeout)
					.write_http_response_timelimit(cfg.timeout));

	log("Server \n"
	    "  threads: {}\n"
	    "  IRI:     http://127.0.0.1:{}/sparql?query="_format(cfg.threads, cfg.port));
	// start endpoint

	// wait for keyboard interrupt
	while (true) {
		sigset_t wset;
		sigemptyset(&wset);
		sigaddset(&wset, SIGINT);
		int number;

		if (int status = sigwait(&wset, &number); status != 0) {
			log("Set contains an invalid signal number.");
			break;
		}
		if (number == SIGINT) {
			logDebug("Exiting by Signal {}."_format(strsignal(number)));
			break;
		}
	}

	log("Shutting down server ...");
	log("Shutdown successful.");
	return EXIT_SUCCESS;
}
