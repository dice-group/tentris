#ifndef TNT_TERMINALCONFIG_HPP
#define TNT_TERMINALCONFIG_HPP

#include <string>

#include <cxxopts.hpp>
#include <chrono>

#include <tnt/util/LogHelper.hpp>
#include <tnt/util/SingletonFactory.hpp>


namespace {
	using namespace ::tnt::logging;
}

struct TerminalConfig {
protected:
	cxxopts::Options options{"tentris_terminal", "Tentris SPARQL endpoint with terminal interface. "
												 "Just type your query and get the result."};
public:
	/**
	 * The relative or absolute path to the RDF file that TNT loads on startup.
	 */
	mutable std::string rdf_file{};
	/**
	 * The timeout for query processing of single queries.
	 */
	mutable std::chrono::system_clock::duration timeout = std::chrono::seconds(180);
	/**
	 * Max number queries that may be cached.
	 */
	mutable size_t cache_size = 500;
	/**
	 * Max number of bindings per query result that is cached.
	 */
	mutable size_t cache_bucket_capacity = 500;
	/**
	 *
	 */
	mutable bool onlystdout = true;

	/**
	 * Initialization of command argument parser.
	 */
	TerminalConfig() {
		options.add_options()
				("f,file", "ntriple file to load at startup", cxxopts::value<std::string>())
				("t,timeout", "time in seconds until processing a request is canceled by the server",
				 cxxopts::value<uint>()->default_value("180"))
				("l,cache_size", "Max number queries that may be cached.",
				 cxxopts::value<size_t>()->default_value("500"))
				("b,cache_bucket_capacity", "Max number of bindings per query result that is cached.",
				 cxxopts::value<size_t>()->default_value("500"))
				("s,onlystdout", "Print non-payload info messages to stdout instead of stderr.",
				cxxopts::value<bool>()->default_value("false"));
	}

	TerminalConfig(int argc, char **argv) : TerminalConfig{} {
		initConfig(argc, argv);
	}

	/**
	 * Parses the command line arguments.
	 * @param argc number of arguments
	 * @param argv array of char arrays with arguments
	 */

	void initConfig(int argc, char **argv) {
		try {
			cxxopts::ParseResult arguments = options.parse(argc, argv);
			parseArguments(arguments);

		} catch (cxxopts::option_not_exists_exception &ex) {
			if (std::string{"Option ‘help’ does not exist"}.compare(ex.what()) == 0) {
				log(options.help());
				exit(EXIT_SUCCESS);
			} else {
				log(ex.what());
				exit(EXIT_FAILURE);
			}
		} catch (cxxopts::argument_incorrect_type &ex) {
			log(ex.what());
			exit(EXIT_FAILURE);
		}
	}

protected:
	void parseArguments(const cxxopts::ParseResult &arguments) {
		if (arguments.count("file") == 1) {
			auto file = arguments["file"].as<std::string>();

			this->rdf_file = file;
		}

		if (arguments.count("timeout") == 1) {
			auto timeout_ = arguments["timeout"].as<uint>();

			if (timeout_ == 0)
				timeout = std::chrono::hours(24 * 7); // one week is default processing time.
			else
				timeout = std::chrono::seconds(timeout_);
		}

		if (arguments.count("cache_size") == 1) {
			auto cache_size_ = arguments["cache_size"].as<size_t>();

			if (cache_size_ != 0)
				cache_size = cache_size_;
		}

		if (arguments.count("cache_bucket_capacity") == 1) {
			auto cache_bucket_capacity_ = arguments["cache_bucket_capacity"].as<size_t>();

			if (cache_bucket_capacity_ != 0)
				cache_bucket_capacity = cache_bucket_capacity_;
		}

		if (arguments.count("onlystdout") == 1) {
			onlystdout = arguments["onlystdout"].as<bool>();
		}
	}
};


#endif //TNT_TERMINALCONFIG_HPP
