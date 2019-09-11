#ifndef TENTRIS_TERMINALCONFIG_HPP
#define TENTRIS_TERMINALCONFIG_HPP

#include "ExecutableConfig.hpp"


struct TerminalConfig : public ExecutableConfig {
protected:

	mutable bool onlystdout;

	/**
	 * Initialization of command argument parser.
	 */
	TerminalConfig() {
		options = {"tentris_terminal", "Tentris SPARQL endpoint with terminal interface. "
									   "Just type your query and get the result."};
		addOptions();

		options.add_options()
				("s,onlystdout", "Print non-payload info messages to stdout instead of stderr.",
				 cxxopts::value<bool>()->default_value("false"));
	}


	TerminalConfig(int argc, char **argv) : ExecutableConfig{} {
		initConfig(argc, argv);
	}

protected:

	void parseArguments(const cxxopts::ParseResult &arguments) override {
		ExecutableConfig::parseArguments(arguments);

		onlystdout = arguments["onlystdout"].as<bool>();

	}
};

#endif //TENTRIS_TERMINALCONFIG_HPP
