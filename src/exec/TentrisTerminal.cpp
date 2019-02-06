#include <string>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "config/TerminalConfig.hpp"

#include <tnt/tensor/einsum/operator/GeneratorInterface.hpp>
#include <tnt/store/QueryExecutionPackage.hpp>
#include <tnt/store/TripleStore.hpp>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <itertools.hpp>

namespace {
	using namespace tnt::store;
	using namespace std::filesystem;
	using namespace tnt::tensor::einsum::operators;
	using namespace iter;
}

template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
size_t writeNTriple(std::ostream &stream, const std::vector<Variable> &vars, yield_pull<RESULT_TYPE> results,
					const TripleStore &store, const system_clock::time_point &timeout) {
	stream << fmt::format("{}\n", fmt::join(vars, ","));

	auto invTermINdex = store.getTermIndex().inv();

	int timeout_check = 0;
	size_t result_count = 0;

	std::vector<Term const *> binding(size(vars));

	for (const auto &result : results) {

		const Key_t &key = RESULT_TYPE::getKey(result);

		for (const auto[pos, id] : enumerate(key))
			binding[pos] = invTermINdex.at(id).get();
		auto binding_string = fmt::format("{}\n", fmt::join(binding.begin(), binding.end(), ","));

		for ([[maybe_unused]] const auto c : range(RESULT_TYPE::getCount(result))) {
			stream << binding_string;
			++result_count;
			if (++timeout_check == 500) {
				timeout_check = 0;
				stream.flush();
				if (system_clock::now() > timeout) {
					return result_count;
				}
			}
		}
	}
	return result_count;
}

[[noreturn]]
void commandlineInterface(TripleStore &triple_store) {
	while (true) {

		std::string sparql_str;

		std::getline(std::cin, sparql_str);

		high_resolution_clock::time_point start = high_resolution_clock::now();

		try {
			std::shared_ptr<QueryExecutionPackage> query_package = triple_store.query(sparql_str);
			const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
			const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();

			const auto timeout = query_package->getTimeout();

			switch (sparqlQuery.getSelectModifier()) {
				case SelectModifier::NONE: {
					// calculate the result
					auto result_generator = query_package->getRegularGenerator();
					// check if it timed out
					if (system_clock::now() < timeout) {
						auto nb = writeNTriple<counted_binding>(std::cout, vars, std::move(result_generator),
																triple_store,
																timeout);
						std::cout.flush();
						std::cerr << "number_of_bindings: " << nb << std::endl;
						query_package->done();
					} else {
						query_package->canceled();
						continue;
					}
					break;
				}
				case SelectModifier::REDUCE:
					[[fallthrough]];
				case SelectModifier::DISTINCT: {
					logDebug("Running select distinct query.");
					// calculate the result
					auto result_generator = query_package->getDistinctGenerator();
					// check if it timed out
					if (system_clock::now() < timeout) {
						writeNTriple<distinct_binding>(std::cout, vars, std::move(result_generator), triple_store,
													   timeout);
						query_package->done();
					} else {
						query_package->canceled();
						continue;
					}
					break;
				}
			}
		} catch (const std::invalid_argument &e) {
			std::cerr << e.what() << std::endl;
		}
		std::cout.flush();
		auto time_span = duration_cast<std::chrono::nanoseconds>(high_resolution_clock::now() - start);
		std::cerr << "totaltime: " << time_span.count() << " ns" << std::endl;
		std::cerr << "totaltime: " << duration_cast<std::chrono::milliseconds>(time_span).count() << " ms" << std::endl;
		std::cerr.flush();
	}
}


int main(int argc, char *argv[]) {
	TerminalConfig cfg{argc, argv};

	TripleStore triplestore{cfg.cache_size, cfg.cache_bucket_capacity, cfg.timeout};
	if (not cfg.rdf_file.empty()) {
		std::cerr << "Loading file " << cfg.rdf_file << " ..." << std::endl;
		triplestore.loadRDF(cfg.rdf_file);
		std::cerr << "loading done." << std::endl;
	}

	std::thread commandline_client{commandlineInterface, std::ref(triplestore)};
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
}
