#include <chrono>
#include <filesystem>
#include <fstream>

#include <dice/hash/DiceHash.hpp>
#include <dice/sparse-map/sparse_set.hpp>
#include <dice/tentris/tentris_version.hpp>

#include <cxxopts.hpp>
#include <fmt/format.h>

#include <rdf4cpp/rdf.hpp>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
	using namespace dice;
	namespace fs = std::filesystem;

	/*
	 * Parse Commandline Arguments
	 */
	std::string version = fmt::format("deduplicate_nt v{} is based on rdf4cpp {}.", dice::tentris::version, dice::tentris::rdf4cpp_version);

	cxxopts::Options options("deduplicate_nt",
							 fmt::format("{}\nDeduplicating RDF files (TURTLE, NTRIPLE). Result is serialized in NTRIPLE on console out. Logs are written to console error.", version));
	options.add_options()                                                                                                                                                                              //
			("f,file", "TURTLE or NTRIPLE RDF file that should be processed.", cxxopts::value<std::string>())                                                                                          //
			("m,limit", "Maximum number of result triples. When the limit is reached, the tool quits.", cxxopts::value<size_t>()->default_value(fmt::format("{}", std::numeric_limits<size_t>::max())))//
			("l,loglevel", fmt::format("Details of logging. Available values are: [{}, {}, {}, {}, {}, {}, {}]",                                                                                       //
									   spdlog::level::to_string_view(spdlog::level::trace),                                                                                                            //
									   spdlog::level::to_string_view(spdlog::level::debug),                                                                                                            //
									   spdlog::level::to_string_view(spdlog::level::info),                                                                                                             //
									   spdlog::level::to_string_view(spdlog::level::warn),                                                                                                             //
									   spdlog::level::to_string_view(spdlog::level::err),                                                                                                              //
									   spdlog::level::to_string_view(spdlog::level::critical),                                                                                                         //
									   spdlog::level::to_string_view(spdlog::level::off)),                                                                                                             //
			 cxxopts::value<std::string>()->default_value("info"))                                                                                                                                     //
			("v,version", "Version info.")                                                                                                                                                             //
			("h,help", "Print this help page.")                                                                                                                                                        //
			;

	auto parsed_args = options.parse(argc, argv);
	if (parsed_args.count("help")) {
		std::cerr << options.help() << std::endl;
		exit(EXIT_SUCCESS);
	} else if (parsed_args.count("version")) {
		std::cerr << version << std::endl;
		exit(EXIT_SUCCESS);
	}

	/*
	 * Initialize logger
	 */
	const auto log_level = spdlog::level::from_str(parsed_args["loglevel"].as<std::string>());
	spdlog::set_default_logger(spdlog::stderr_color_mt("rdf2ids logger"));
	spdlog::set_level(log_level);
	spdlog::set_pattern("%Y-%m-%dT%T.%e%z | %n | %t | %l | %v");
	spdlog::info(version);

	auto const limit = parsed_args["limit"].as<size_t>();
	size_t count = 0;

	// write TSV to std::cout
	{
		// terminate when the limit is reached
		auto terminate_at_limit = [&count, &limit] {
			if (++count > limit) {
				std::cout.flush();
				spdlog::info("Limit of {} triples reached.", limit);
				spdlog::info("Shutdown successful.");
				exit(EXIT_SUCCESS);
			}
		};

		auto file_path = parsed_args["file"].as<std::string>();
		std::ifstream ifs{file_path};

		if (!ifs.is_open()) {
			throw std::runtime_error{"unable to open provided file " + file_path};
		}

		dice::sparse_map::sparse_set<uint64_t> deduplication;
		for (rdf4cpp::rdf::parser::IStreamQuadIterator qit{ifs}; qit != std::default_sentinel; ++qit) {
			if (qit->has_value()) {
				auto const &quad = qit->value();
				auto const hash = hash::dice_hash_templates<hash::Policies::xxh3>::dice_hash(std::array<uint64_t, 3>{
						quad.subject().backend_handle().raw(),
						quad.predicate().backend_handle().raw(),
						quad.object().backend_handle().raw()});
				if (not deduplication.contains(hash)) {
					terminate_at_limit();
					std::cout << fmt::format("{} {} {} . \n", std::string(quad.subject()), std::string(quad.predicate()), std::string(quad.object()));
					deduplication.insert(hash);
				}
			} else {
				std::cerr << qit->error() << '\n';
			}
		}
	}

	spdlog::info("Shutdown successful.");
	return EXIT_SUCCESS;
}
