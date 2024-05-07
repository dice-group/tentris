#include <chrono>
#include <filesystem>

#include <cxxopts.hpp>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include <dice/node-store/PersistentNodeStorageBackend.hpp>
#include <dice/triple-store/TripleStore.hpp>

#include <dice/tentris/tentris_version.hpp>

int main(int argc, char *argv[]) {
	using namespace dice;
	namespace fs = std::filesystem;

	std::string version = fmt::format("tentris-loader v{} is using hypertrie v{} and rdf4cpp {}.", dice::tentris::version, hypertrie::version, dice::tentris::rdf4cpp_version);
	cxxopts::Options options("tentris-loader",
							 fmt::format("{}\nA tensor-based triple store.", version));
	options.add_options()                                                                                                                                                                                                                //
			("s,storage", "Location where the index is stored.", cxxopts::value<std::string>()->default_value(fs::current_path().string()))
			("f,file", "A N-Triples or Turtle file.", cxxopts::value<std::string>())                      //
			("b,bulksize", "Bulk-size for loading RDF files. A larger value results in a higher memory consumption during loading RDF data but may result in shorter loading times.", cxxopts::value<uint32_t>()->default_value("1000000"))//
			("l,loglevel", fmt::format("Details of logging. Available values are: [{}, {}, {}, {}, {}, {}, {}]",                                                                                                                         //
									   spdlog::level::to_string_view(spdlog::level::trace),                                                                                                                                              //
									   spdlog::level::to_string_view(spdlog::level::debug),                                                                                                                                              //
									   spdlog::level::to_string_view(spdlog::level::info),                                                                                                                                               //
									   spdlog::level::to_string_view(spdlog::level::warn),                                                                                                                                               //
									   spdlog::level::to_string_view(spdlog::level::err),                                                                                                                                                //
									   spdlog::level::to_string_view(spdlog::level::critical),                                                                                                                                           //
									   spdlog::level::to_string_view(spdlog::level::off)),                                                                                                                                               //
			 cxxopts::value<std::string>()->default_value("info"))                                                                                                                                                                       //
			("logfile", "If log is written to files.", cxxopts::value<bool>()->default_value("true"))                                                                                                                                    //
			("logstdout", "If log is written to stdout.", cxxopts::value<bool>()->default_value("false"))                                                                                                                                //
			("logfiledir", "A folder path where to write the logfiles. Default is the current working directory.", cxxopts::value<std::string>()->default_value(fs::current_path().string()))                                            //
			("v,version", "Version info.")                                                                                                                                                                                               //
			("h,help", "Print this help page.")                                                                                                                                                                                          //
			;

	auto parsed_args = options.parse(argc, argv);
	if (parsed_args.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	} else if (parsed_args.count("version")) {
		std::cout << version << std::endl;
		exit(0);
	}
	if (not parsed_args.count("file")) {
		std::cout << "Please provide an RDF file." << std::endl;
		std::cout << options.help() << std::endl;
		exit(0);
	}

	using metall_manager = rdf_tensor::metall_manager;

	auto const storage_path = fs::absolute(fs::path{parsed_args["storage"].as<std::string>()}).append("tentris_data");
	if (fs::exists(storage_path)) {
		std::cout << "Index storage path (-s,--storage) " << storage_path.string() << " already exists. Please provide a different path.";
		exit(0);
	}

	// init logger
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

	// init storage
	{
		metall_manager{metall::create_only, storage_path.c_str()};
	}
	metall_manager storage_manager{metall::open_only, storage_path.c_str()};
	// set up node store
	{
		using namespace rdf4cpp::rdf::storage::node;
		using namespace dice::node_store;
		auto *nodestore_backend = storage_manager.find_or_construct<PersistentNodeStorageBackendImpl>("node-store")(storage_manager.get_allocator());
		NodeStorage::set_default_instance(
				NodeStorage::new_instance<PersistentNodeStorageBackend>(nodestore_backend));
	}
	// setup triple store
	auto &ht_context = *storage_manager.find_or_construct<rdf_tensor::HypertrieContext>("hypertrie-context")(storage_manager.get_allocator());
	auto &rdf_tensor = *storage_manager.find_or_construct<rdf_tensor::BoolHypertrie>("rdf-tensor")(3, rdf_tensor::HypertrieContext_ptr{&ht_context});
	triple_store::TripleStore triplestore{rdf_tensor};
	fs::path ttl_file(parsed_args["file"].as<std::string>());

	{// load data
		spdlog::info("Loading triples from file {}.", fs::absolute(ttl_file).string());
		spdlog::stopwatch loading_time;
		spdlog::stopwatch batch_loading_time;
		size_t total_processed_entries = 0;
		size_t total_inserted_entries = 0;
		size_t final_hypertrie_size_after = 0;

		triplestore.load_ttl(
				parsed_args["file"].as<std::string>(),
				parsed_args["bulksize"].as<uint32_t>(),
				[&](size_t processed_entries,
					size_t inserted_entries,
					size_t hypertrie_size_after) noexcept {
					std::chrono::duration<double> batch_duration = batch_loading_time.elapsed();
					spdlog::info("batch: {:>10.3} mio triples processed, {:>10.3} mio triples added, {} elapsed, {:>10.3} mio triples in storage.",
								 (double(processed_entries) / 1'000'000),
								 (double(inserted_entries) / 1'000'000),
								 (batch_duration.count()),
								 (double(hypertrie_size_after) / 1'000'000));
					total_processed_entries = processed_entries;
					total_inserted_entries = inserted_entries;
					final_hypertrie_size_after = hypertrie_size_after;
					batch_loading_time.reset();
				},
				[](rdf_tensor::parser::ParsingError const &error) noexcept {
					std::ostringstream oss;
					oss << error;
					spdlog::warn(oss.str());// spdlog does not want to use the ostream operator for ParsingError
				});
		spdlog::info("loading finished: {} triples processed, {} triples added, {} elapsed, {} triples in storage.",
					 total_processed_entries, total_inserted_entries, std::chrono::duration<double>(loading_time.elapsed()).count(), final_hypertrie_size_after);
		const auto cards = triplestore.get_hypertrie().get_cards({0, 1, 2});
		spdlog::info("Storage stats: {} triples ({} distinct subjects, {} distinct predicates, {} distinct objects)",
					 triplestore.size(), cards[0], cards[1], cards[2]);
	}

	// create snapshot
	spdlog::info("Creating snapshot: {}_snapshot", storage_path.string());
	auto snapshot_path = fs::absolute(storage_path.string().append("_snapshot"));
	storage_manager.snapshot(snapshot_path.c_str());
	spdlog::info("Finished loading: {}.", ttl_file.string());
}