#ifndef TENTRIS_EXECUTABLECONFIG_HPP
#define TENTRIS_EXECUTABLECONFIG_HPP

#include <string>
#include <filesystem>
#include <chrono>
#include <iostream>

#include <fmt/core.h>
#include <fmt/format.h>
#include <boost/log/trivial.hpp>

#include <cxxopts.hpp>


namespace {
	using namespace fmt::literals;
	namespace fs =    std::filesystem;
	namespace logging = boost::log;
}

struct ExecutableConfig {
protected:

	static std::map<std::string, logging::trivial::severity_level> log_severity_mapping;

	cxxopts::Options options{"tentris", "to be overwritten"};
public:
	/**
	 * The relative or absolute path to the RDF file that TENTRIS loads on startup.
	 */
	mutable std::string rdf_file{};
	/**
	 * The relative or absolute path to the file containing the graphql schema -- optional
	 */
    mutable std::string graphql_schema{};
	/**
	 * The timeout for query processing of single queries.
	 */
	mutable std::chrono::steady_clock::duration timeout;
	/**
	 * Max number queries that may be cached.
	 */
	mutable size_t cache_size;

	mutable logging::trivial::severity_level loglevel;

	mutable bool logfile;

	mutable size_t bulksize;

	mutable bool logstdout;

	mutable std::string logfiledir;

	/**
	 * Initialization of command argument parser.
	 */
	ExecutableConfig() {
		addOptions();
	}

protected:
	void addOptions() {
		options.add_options()
				("f,file", "ntriple file to load at startup", cxxopts::value<std::string>())
                ("s,gql_schema", "graphql schema", cxxopts::value<std::string>())
				("t,timeout", "time in seconds until processing a request is canceled by the server",
				 cxxopts::value<uint>()->default_value("180"))
				("l,cache_size", "Max number queries that may be cached.",
				 cxxopts::value<size_t>()->default_value("500"))
				("loglevel", "Sets the logging level. Valid values are: [trace, debug, info, warning, error, fatal]",
				 cxxopts::value<std::string>()->default_value("info"))
				("logfile",
				 "If log is written to file.",
				 cxxopts::value<bool>()->default_value("true"))
				("b,bulksize",
				 "Number of triples that are inserted at once. A larger value results in a higher memory consumption during loading RDF data but may result in shorter loading times.",
				 cxxopts::value<size_t>()->default_value("1000000"))
				("logstdout",
				 "If log is written to stdout.",
				 cxxopts::value<bool>()->default_value("false"))
				("logfiledir",
				 "A folder path where to write the logfiles. Default is the current working directory.",
				 cxxopts::value<std::string>()->default_value(fs::current_path().string()));
	}

public:

	ExecutableConfig(int argc, char ** &argv) : ExecutableConfig{} {
		initConfig(argc, argv);
	}

	/**
	 * Parses the command line arguments.
	 * @param argc number of arguments
	 * @param argv array of char arrays with arguments
	 */

	void initConfig(int argc, char **&argv) {
		try {
			cxxopts::ParseResult arguments = options.parse(argc, argv);
			parseArguments(arguments);

		} catch (cxxopts::option_not_exists_exception &ex) {
			if (std::string{"Option ‘help’ does not exist"} == ex.what()) {
				std::cout << options.help() << std::endl;
				exit(EXIT_SUCCESS);
			} else {
				std::cout << ex.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		} catch (cxxopts::argument_incorrect_type &ex) {
			std::cout << ex.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}

protected:
	virtual void parseArguments(const cxxopts::ParseResult &arguments) {
		if (arguments.count("file"))
			rdf_file = arguments["file"].as<std::string>();

        if (arguments.count("gql_schema"))
            graphql_schema = arguments["gql_schema"].as<std::string>();

		auto timeout_ = arguments["timeout"].as<uint>();
		if (timeout_ == 0)
			timeout = std::chrono::hours(24 * 7); // one week is default processing time.
		else
			timeout = std::chrono::seconds(timeout_);


		auto cache_size_ = arguments["cache_size"].as<size_t>();

		if (cache_size_ == 0)
			cache_size = 1;
		else
			cache_size = cache_size_;


		auto loglevel_str = arguments["loglevel"].as<std::string>();
		auto found = log_severity_mapping.find(loglevel_str);
		if (found != log_severity_mapping.end()) {
			loglevel = found->second;
		} else {
			throw cxxopts::argument_incorrect_type(
					"{} is not a valid log level. Valid values are: [trace, debug, info, warning, error, fatal]"_format(
							loglevel_str));
		}


		logfile = arguments["logfile"].as<bool>();


		logstdout = arguments["logstdout"].as<bool>();

		bulksize = arguments["bulksize"].as<size_t>();


		logfiledir = arguments["logfiledir"].as<std::string>();
		if (not fs::exists(logfiledir)) {
			try {
				fs::create_directories(logfiledir);
			} catch (const fs::filesystem_error &exc) {
				throw cxxopts::argument_incorrect_type(
						"Could not creat directory {}. Error: {}"_format(logfiledir, exc.what()));
			}
		}
		if (not fs::is_directory(logfiledir))
			throw cxxopts::argument_incorrect_type("{} is not a valid directory path"_format(logfiledir));
	}
};

std::map<std::string, logging::trivial::severity_level>  ExecutableConfig::log_severity_mapping{
		{"trace",   logging::trivial::trace},
		{"debug",   logging::trivial::debug},
		{"info",    logging::trivial::info},
		{"warning", logging::trivial::warning},
		{"error",   logging::trivial::error},
		{"fatal",   logging::trivial::fatal}
};


#endif //TENTRIS_EXECUTABLECONFIG_HPP
