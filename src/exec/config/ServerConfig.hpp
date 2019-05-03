#ifndef TENTRIS_SERVERCONFIG_HPP
#define TENTRIS_SERVERCONFIG_HPP

#include <string>

#include <cxxopts.hpp>
#include <chrono>

#include "TerminalConfig.hpp"


namespace {
	using namespace ::tentris::logging;
}

struct ServerConfig : public TerminalConfig {

	/**
	 * The where TENTRIS runs.
	 */
	mutable uint16_t port = 9080;

	/**
		 * Number of threads used to serve http results. Each thread may use multiple others while calculating the result.
		 */
	mutable uint threads = std::thread::hardware_concurrency();

	ServerConfig() {
		options = {"tentris_server", "Tentris SPARQL endpoint with terminal interface. "
									   "Just type your query and get the result."};
		addOptions();

		options.add_options()
				("p,port", "port to run server", cxxopts::value<uint16_t>()->default_value("9080"))
				("c,threads", "How many threads are used for handling http requests",
				 cxxopts::value<uint8_t>()->default_value("0"));
	}

	ServerConfig(int argc, char **argv) : ServerConfig{} {
		initConfig(argc, argv);
	}

protected:
	void parseArguments(const cxxopts::ParseResult &arguments) override {
		TerminalConfig::parseArguments(arguments);
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

#endif //TENTRIS_SERVERCONFIG_HPP
