#include <chrono>
#include <filesystem>

#include <cxxopts.hpp>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <taskflow/taskflow.hpp>

#include <dice/endpoint/HTTPServer.hpp>
#include <dice/node-store/PersistentNodeStorageBackend.hpp>
#include <dice/triple-store/TripleStore.hpp>

#include <dice/tentris/tentris_version.hpp>

int main(int argc, char *argv[]) {
	using namespace dice;
	namespace fs = std::filesystem;

	/*
	 * Parse Commandline Arguments
	 */
	std::string version = fmt::format("tentris-server v{} is based on hypertrie v{} and rdf4cpp {}.", dice::tentris::version, hypertrie::version, dice::tentris::rdf4cpp_version);

	cxxopts::Options options("tentris-server",
							 fmt::format("{}\nA tensor-based triple store.", version));
	options.add_options()                                                                                                                                                                    //
			("s,storage", "Location where the index is stored.", cxxopts::value<std::string>()->default_value(fs::current_path().string()))                                                  //
			("t,timeout", "Time out in seconds for answering requests.", cxxopts::value<uint>()->default_value("180"))                                                                       //
			("j,threads", "Number of threads used by the endpoint.", cxxopts::value<uint16_t>()->default_value(std::to_string(std::thread::hardware_concurrency())))                         //
			("p,port", "Port to be used by the endpoint.", cxxopts::value<uint16_t>()->default_value("9080"))                                                                                //
			("l,loglevel", fmt::format("Details of logging. Available values are: [{}, {}, {}, {}, {}, {}, {}]",                                                                             //
									   spdlog::level::to_string_view(spdlog::level::trace),                                                                                                  //
									   spdlog::level::to_string_view(spdlog::level::debug),                                                                                                  //
									   spdlog::level::to_string_view(spdlog::level::info),                                                                                                   //
									   spdlog::level::to_string_view(spdlog::level::warn),                                                                                                   //
									   spdlog::level::to_string_view(spdlog::level::err),                                                                                                    //
									   spdlog::level::to_string_view(spdlog::level::critical),                                                                                               //
									   spdlog::level::to_string_view(spdlog::level::off)),                                                                                                   //
			 cxxopts::value<std::string>()->default_value("info"))                                                                                                                           //
			("logfile", "If log is written to files.", cxxopts::value<bool>()->default_value("true"))                                                                                        //
			("logstdout", "If log is written to stdout.", cxxopts::value<bool>()->default_value("false"))                                                                                    //
			("logfiledir", "A folder path where to write the logfiles. Default is the current working directory.", cxxopts::value<std::string>()->default_value(fs::current_path().string()))//
			("v,version", "Version info.")                                                                                                                                                   //
			("h,help", "Print this help page.")                                                                                                                                              //
			;

	auto parsed_args = options.parse(argc, argv);
	if (parsed_args.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	} else if (parsed_args.count("version")) {
		std::cout << version << std::endl;
		exit(0);
	}

	/*
	 * Initialize logger
	 */
	const auto log_level = spdlog::level::from_str(parsed_args["loglevel"].as<std::string>());
	spdlog::set_level(log_level);

	std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;

	if (parsed_args["logfile"].as<bool>()) {
		// Create a file rotating logger with 5mb size max and 10 rotated files
		const auto max_size = 1048576 * 5;
		const auto max_files = 10;
		auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(parsed_args["logfiledir"].as<std::string>() + "/tentris.log", max_size, max_files);
		file_sink->set_level(log_level);
		sinks.emplace_back(std::move(file_sink));
	}

	if (parsed_args["logstdout"].as<bool>()) {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(log_level);
		sinks.emplace_back(std::move(console_sink));
	}

	auto logger = std::make_shared<spdlog::logger>("tentris logger", sinks.begin(), sinks.end());
	logger->set_level(log_level);
	spdlog::set_default_logger(logger);
	spdlog::set_pattern("%Y-%m-%dT%T.%e%z | %n | %t | %l | %v");
	spdlog::info(version);
	spdlog::flush_every(std::chrono::seconds{5});


	/*
	 * Initialize storage, executor and endpoints
	 */
	const endpoint::EndpointCfg endpoint_cfg{
			.port = parsed_args["port"].as<uint16_t>(),
			.threads = parsed_args["threads"].as<uint16_t>(),
			.timeout_duration = std::chrono::seconds{parsed_args["timeout"].as<uint>()}};

	using metall_manager = rdf_tensor::metall_manager;

	auto const storage_path = fs::absolute(fs::path{parsed_args["storage"].as<std::string>()}).append("tentris_data");
	if (not metall_manager::consistent(storage_path.c_str())) {
		spdlog::info("No index storage or corrupted index storage found at {}. Checking for snapshot.", storage_path);
		auto const snapshot_path = storage_path.string().append("_snapshot");
		if (metall_manager::consistent(snapshot_path.c_str())) {
			spdlog::info("Found snapshot at {}.", storage_path.string());
			spdlog::info("Reconstructing index.");
			metall_manager storage_manager{metall::open_only, snapshot_path.c_str()};
			storage_manager.snapshot(storage_path.c_str());
			spdlog::info("Reconstructed index at {}.", storage_path.string());
		} else {
			spdlog::info("No snapshot found. Exiting.");
			std::cout << "No snapshot found. Please create a new index using tentris_loader." << std::endl;
			exit(0);
		}
	} else {
		spdlog::info("Existing index storage at {}.", storage_path.string());
	}
	metall_manager storage_manager{metall::open_only, storage_path.c_str()};


	{// set up node store
		using namespace rdf4cpp::rdf::storage::node;
		using namespace dice::node_store;
		auto *nodestore_backend = storage_manager.find_or_construct<PersistentNodeStorageBackendImpl>("node-store")(storage_manager.get_allocator());
		NodeStorage::default_instance(
				NodeStorage::new_instance<PersistentNodeStorageBackend>(nodestore_backend));
	}

	// setup triple store
	auto &rdf_tensor = [&storage_manager]() -> rdf_tensor::BoolHypertrie & {
		auto [ptr, cnt] = storage_manager.find<rdf_tensor::BoolHypertrie>("rdf-tensor");
		if (cnt != 1UL) {
			spdlog::error("Storage is readable but contains no rdf-tensor with index data. Please create a new index using tentris_loader.");
			exit(0);
		}
		return *ptr;
	}();
	{
		triple_store::TripleStore triplestore{rdf_tensor};
		// initialize task runners
		tf::Executor executor(endpoint_cfg.threads);
		// setup and configure endpoints
		endpoint::HTTPServer http_server{executor, triplestore, endpoint_cfg};
		const auto cards = triplestore.get_hypertrie().get_cards({0, 1, 2});
		spdlog::info("Storage stats: {} triples ({} distinct subjects, {} distinct predicates, {} distinct objects)",
					 triplestore.size(), cards[0], cards[1], cards[2]);
		spdlog::info("SPARQL endpoint serving sparkling linked data treasures on {} threads at http://0.0.0.0:{}/ with {} request timeout.",
					 endpoint_cfg.threads, endpoint_cfg.port, endpoint_cfg.timeout_duration);

		// start http server
		http_server();
	}

	// warping up node storage
	spdlog::info("Shutdown successful.");
	return EXIT_SUCCESS;
}
