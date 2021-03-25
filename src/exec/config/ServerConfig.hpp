#ifndef TENTRIS_SERVERCONFIG_HPP
#define TENTRIS_SERVERCONFIG_HPP

#include "ExecutableConfig.hpp"

#include <thread>


struct ServerConfig : public ExecutableConfig {

	/**
	 * The network port where Tentris is available.
	 */
	mutable uint16_t port;

	/**
	 * Number of threads used to serve http results. Each thread may use multiple others while calculating the result.
	 */
	mutable uint threads;

	ServerConfig() {
		options = cxxopts::Options{
				"tentris_server",
				"Tentris HTTP SPARQL endpoint"
	"\n  {}\n  {}\n"_format(tentris_version_string, hypertrie_version_string)};
		addOptions();

		options.add_options()
				("p,port", "port to run server", cxxopts::value<uint16_t>()->default_value("9080"))
				("c,threads", "How many threads are used for handling http requests",
				 cxxopts::value<uint>()->default_value("{}"_format(std::thread::hardware_concurrency())));
	}

	ServerConfig(int argc, char **&argv) : ServerConfig{} {
		initConfig(argc, argv);
	}

protected:
	void parseArguments(const cxxopts::ParseResult &arguments) override {
		ExecutableConfig::parseArguments(arguments);


		port = arguments["port"].as<uint16_t>();


		auto threads_ = arguments["threads"].as<uint>();

		if (threads_ != 0)
			threads = threads_;

	}

};

#endif //TENTRIS_SERVERCONFIG_HPP
