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

bool onlystdout = false;

std::ostream &logsink(){
	if (onlystdout)
		return std::cout;
	else
		return std::cerr;
}

high_resolution_clock::time_point query_start;
high_resolution_clock::time_point query_end;
high_resolution_clock::time_point parse_start;
high_resolution_clock::time_point parse_end;
high_resolution_clock::time_point execute_start;
high_resolution_clock::time_point execute_end;

template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
size_t writeNTriple(std::ostream &stream, const std::vector<Variable> &vars, yield_pull<RESULT_TYPE> results,
					const TripleStore &store, const system_clock::time_point &timeout) {
	stream << fmt::format("{}\n", fmt::join(vars, ","));

	auto invTermINdex = store.getTermIndex().inv();

	int timeout_check = 0;
	size_t result_count = 0;

	std::vector<Term const *> binding(size(vars));

	bool first = true;

	for (const auto &result : results) {
		if (first){
			first = false;
			execute_end = high_resolution_clock::now();
		}

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
	if (first){ // if no bindings are returned
		first = false;
		execute_end = high_resolution_clock::now();
	}
	return result_count;
}

[[noreturn]]
void commandlineInterface(TripleStore &triple_store) {
	while (true) {

		std::string sparql_str;

		std::getline(std::cin, sparql_str);

		query_start = high_resolution_clock::now();

		try {
			parse_start = high_resolution_clock::now();
			std::shared_ptr<QueryExecutionPackage> query_package = triple_store.query(sparql_str);
			const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
			const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();

			const auto timeout = query_package->getTimeout();

			parse_end = high_resolution_clock::now();
			execute_start = high_resolution_clock::now();

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
						logsink() << "number_of_bindings: " << nb << std::endl;
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
						auto nb = writeNTriple<distinct_binding>(std::cout, vars, std::move(result_generator), triple_store,
													   timeout);
						std::cout.flush();
						logsink() << "number_of_bindings: " << nb << std::endl;
						query_package->done();
					} else {
						query_package->canceled();
						continue;
					}
					break;
				}
			}
		} catch (const std::invalid_argument &e) {
			logsink() << e.what() << std::endl;
		}
		std::cout.flush();
		query_end = high_resolution_clock::now();

		auto parsing_time = duration_cast<std::chrono::nanoseconds>(parse_end - parse_start);
		auto execution_time = duration_cast<std::chrono::nanoseconds>(execute_end - execute_start);
		auto total_time = duration_cast<std::chrono::nanoseconds>(query_end - query_start);
		auto serialization_time = total_time - execution_time - parsing_time;

		logsink() << "queryparsingtime:  " << fmt::format("{:15}",parsing_time.count()) << " ns\n";


		logsink() << "executiontime:     " << fmt::format("{:15}",execution_time.count()) << " ns\n";

		logsink() << "serializationtime: " << fmt::format("{:15}",serialization_time.count()) << " ns\n";


		logsink() << "totaltime:         " << fmt::format("{:15}",total_time.count()) << " ns\n";
		logsink() << "totaltime:         " << fmt::format("{:15}",duration_cast<std::chrono::milliseconds>(total_time).count()) << " ms\n";

		logsink().flush();
	}
}


int main(int argc, char *argv[]) {
	TerminalConfig cfg{argc, argv};

	TripleStore triplestore{cfg.cache_size, cfg.cache_bucket_capacity, cfg.timeout};

	onlystdout = cfg.onlystdout;

	if (not cfg.rdf_file.empty()) {
		logsink() << "Loading file " << cfg.rdf_file << " ..." << std::endl;
		triplestore.loadRDF(cfg.rdf_file);
		logsink() << "loading done." << std::endl;
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
