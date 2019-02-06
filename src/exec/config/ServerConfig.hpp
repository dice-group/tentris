#ifndef TNT_SERVERCONFIG_HPP
#define TNT_SERVERCONFIG_HPP

#include <string>

#include <cxxopts.hpp>
#include <chrono>

#include "DaemonConfig.hpp"


namespace {
	using namespace ::tnt::logging;
}

struct ServerConfig : public DaemonConfig {

	/**
	 * The where TNT runs.
	 */
	mutable uint16_t port = 9080;

	/**
		 * Number of threads used to serve http results. Each thread may use multiple others while calculating the result.
		 */
	mutable uint threads = std::thread::hardware_concurrency();

	ServerConfig() : DaemonConfig{} {
		options = cxxopts::Options{"tentris_server", "Tentris SPARQL HTTP endpoint"};
		options.add_options()
				("p,port", "port to run server", cxxopts::value<uint16_t>()->default_value("9080"))
				("c,threads", "How many threads are used for handling http requests",
				 cxxopts::value<uint8_t>()->default_value("0"));
	}

	ServerConfig(int argc, char **argv) : ServerConfig{} {
		initConfig(argc, argv);
	}

protected:
	void parseArguments(const cxxopts::ParseResult &arguments) {
		DaemonConfig::parseArguments(arguments);
		if (arguments.count("port") == 1) {
			port = arguments["port"].as<uint16_t>();
		}

		if (arguments.count("threads") == 1) {
			auto threads_ = arguments["threads"].as<uint>();

			if (threads_ != 0)
				threads = threads_;
		}

	}

};

#endif //TNT_SERVERCONFIG_HPP
