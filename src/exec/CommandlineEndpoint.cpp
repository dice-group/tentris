#include <string>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "config/DaemonConfig.hpp"
#include "config/TermStorePointerFilename.hpp"

#include <tnt/tensor/einsum/operator/GeneratorInterface.hpp>
#include <tnt/store/QueryExecutionPackage.hpp>
#include <tnt/store/TripleStore.hpp>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/time.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>

namespace {
	using namespace tnt::store;
	using namespace std::filesystem;
	using namespace tnt::tensor::einsum::operators;
}

template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
std::ostream &streamoutNTriple(std::ostream &stream, const std::vector<Variable> &vars, yield_pull<RESULT_TYPE> results,
							   const TripleStore &store, const system_clock::time_point &timeout) {
	stream << fmt::format("{}\n", fmt::join(vars, ","));

	auto invTermINdex = store.getTermIndex().inv();

	int timeout_check = 0;

	for (const auto &result : results) {

		const Key_t &key = RESULT_TYPE::getKey(result);
		std::stringstream s_binding{};
		bool first = true;
		for (const auto[pos, id] : enumerate(key)) {
			if (first) {
				first = false;
			} else {
				s_binding << ",";
			}
			s_binding << invTermINdex.at(id)->getIdentifier();
		}
		s_binding << "\n";
		std::string binding_string = s_binding.str();

		for ([[maybe_unused]] const auto c : range(RESULT_TYPE::getCount(result))) {
			stream << binding_string;
			if (++timeout_check == 400) {
				timeout_check = 0;
				if (system_clock::now() > timeout) {
					return stream;
				}
			}
		}
	}
	return stream;
}

[[noreturn]]
void commandlineInterface(TripleStore &triple_store) {
	while (true) {
		std::string sparql_str;

		std::getline(std::cin, sparql_str);
		// std::cin >> sparql_str;
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
					streamoutNTriple<counted_binding>(std::cout, vars, std::move(result_generator), triple_store,
													  timeout);
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
					streamoutNTriple<distinct_binding>(std::cout, vars, std::move(result_generator), triple_store,
													   timeout);
					query_package->done();
				} else {
					query_package->canceled();
					continue;
				}
				break;
			}
		}
	}
}


int main(int argc, char *argv[]) {
	DaemonConfig cfg{argc, argv};

	TripleStore triplestore{cfg.cache_size, cfg.cache_bucket_capacity, cfg.timeout};
	if (not cfg.rdf_file.empty())
		triplestore.loadRDF(cfg.rdf_file);
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
