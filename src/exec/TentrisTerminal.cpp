#include <string>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "config/TerminalConfig.hpp"

#include <tentris/tensor/einsum/operator/GeneratorInterface.hpp>
#include <tentris/store/QueryExecutionPackage.hpp>
#include <tentris/store/TripleStore.hpp>
#include <tentris/util/LogHelper.hpp>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/time.h>
#include <itertools.hpp>

namespace {
	using namespace tentris::store;
	using namespace std::filesystem;
	using namespace tentris::tensor::einsum::operators;
	using namespace iter;
}

bool onlystdout = false;

enum Errors {
	OK, UNPARSABLE, PROCESSING_TIMEOUT, SERIALIZATION_TIMEOUT, UNEXPECTED, SEVERE_UNEXPECTED
};

std::ostream &logsink() {
	if (onlystdout)
		return std::cout;
	else
		return std::cerr;
}

inline std::string tp2s(system_clock::time_point timepoint) {
	auto in_time_t = system_clock::to_time_t(timepoint);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
};

system_clock::time_point query_start;
system_clock::time_point query_end;
system_clock::time_point parse_start;
system_clock::time_point parse_end;
system_clock::time_point execute_start;
system_clock::time_point execute_end;

Errors error;
size_t number_of_bindings;

std::chrono::system_clock::time_point timeout;
std::chrono::system_clock::time_point actual_timeout;

template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
void
writeNTriple(std::ostream &stream, const std::vector<Variable> &vars, yield_pull<RESULT_TYPE> results,
			 const TripleStore &store) {
	stream << fmt::format("{}\n", fmt::join(vars, ","));

	auto invTermIndex = store.getTermIndex().inv();

	uint timeout_check = 0;
	size_t result_count = 0;

	std::vector<Term const *> binding(size(vars));

	bool first = true;

	for (const auto &result : results) {
		if (first) {
			first = false;
			execute_end = system_clock::now();
		}

		const Key_t &key = RESULT_TYPE::getKey(result);

		for (const auto[pos, id] : enumerate(key))
			binding[pos] = invTermIndex.at(id).get();
		auto binding_string = fmt::format("{}\n", fmt::join(binding.begin(), binding.end(), ","));

		for ([[maybe_unused]] const auto c : range(RESULT_TYPE::getCount(result))) {
			stream << binding_string;
			++result_count;
			if (++timeout_check == 500) {
				timeout_check = 0;
				stream.flush();
				if (auto current_time = system_clock::now(); current_time > timeout) {
					::error = Errors::SERIALIZATION_TIMEOUT;
					actual_timeout = current_time;
					number_of_bindings = result_count;
					return;
				}
			}
		}
	}
	if (first) { // if no bindings are returned
		execute_end = system_clock::now();
	}
	number_of_bindings = result_count;
}

[[noreturn]]
void commandlineInterface(TripleStore &triple_store) {
	while (true) {


		std::string sparql_str;

		std::getline(std::cin, sparql_str);

		query_start = system_clock::now();


		number_of_bindings = 0;
		::error = Errors::OK;


		try {
			parse_start = system_clock::now();
			std::shared_ptr<QueryExecutionPackage> query_package = triple_store.query(sparql_str);
			const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
			const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();

			timeout = query_package->getTimeout();

			parse_end = system_clock::now();
			execute_start = system_clock::now();

			switch (sparqlQuery.getSelectModifier()) {
				case SelectModifier::NONE: {
					// calculate the result
					auto result_generator = query_package->getRegularGenerator();
					// check if it timed out
					if (system_clock::now() < timeout) {
						writeNTriple<counted_binding>(std::cout, vars, std::move(result_generator), triple_store);
						std::thread cleanup_thread([=] { query_package->done(); });
						cleanup_thread.detach();
					} else {
						::error = Errors::PROCESSING_TIMEOUT;
						actual_timeout = system_clock::now();
						std::thread cleanup_thread([=] { query_package->canceled(); });
						cleanup_thread.detach();
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
						writeNTriple<distinct_binding>(std::cout, vars, std::move(result_generator), triple_store);
						std::thread cleanup_thread([=] { query_package->done(); });
						cleanup_thread.detach();
						break;
					} else {
						::error = Errors::PROCESSING_TIMEOUT;
						actual_timeout = system_clock::now();
						std::thread cleanup_thread([=] { query_package->canceled(); });
						cleanup_thread.detach();
					}
					break;
				}
			}
		} catch (const std::invalid_argument &e) {
			::error = Errors::UNPARSABLE;
			logDebug(fmt::format("UNPARSABLE reason: {}", e.what()));
		} catch (const std::exception &e) {
			::error = Errors::UNEXPECTED;
			logDebug(fmt::format("UNEXPECTED reason: {}", e.what()));
		} catch (...) {
			::error = Errors::SEVERE_UNEXPECTED;
		}
		query_end = system_clock::now();


		auto parsing_time = duration_cast<std::chrono::nanoseconds>(parse_end - parse_start);
		auto execution_time = duration_cast<std::chrono::nanoseconds>(execute_end - execute_start);
		auto total_time = duration_cast<std::chrono::nanoseconds>(query_end - query_start);
		auto serialization_time = total_time - execution_time - parsing_time;
		switch (::error) {
			case OK:
				logsink() << "SUCCESSFUL\n";
				break;
			case UNPARSABLE:
				logsink() << "ERROR: UNPARSABLE QUERY\n";
				break;
			case PROCESSING_TIMEOUT:
				logsink() << "ERROR: TIMEOUT DURING PROCESSING\n";
				break;
			case SERIALIZATION_TIMEOUT:
				logsink() << "ERROR: TIMEOUT DURING SERIALIZATION\n";
				break;
			case UNEXPECTED:
				logsink() << "ERROR: UNEXPECTED\n";
				break;
			case SEVERE_UNEXPECTED:
				logsink() << "ERROR: SEVERE UNEXPECTED\n";
				break;
		}


		logsink() << fmt::format("start:              {}\n", tp2s(query_start));
		logsink() << fmt::format("planned timeout:    {}\n", tp2s(timeout));
		if (::error == Errors::PROCESSING_TIMEOUT or ::error == Errors::SERIALIZATION_TIMEOUT)
			logsink() << fmt::format("actual timeout:     {}\n", tp2s(actual_timeout));
		logsink() << fmt::format("end:                {}\n", tp2s(query_end));

		if (::error == Errors::OK or ::error == Errors::PROCESSING_TIMEOUT or
			::error == Errors::SERIALIZATION_TIMEOUT) {
			logsink() << "number of bindings: " << fmt::format("{:18}", number_of_bindings) << "\n";

			logsink() << "parsing time:       " << fmt::format("{:18}", parsing_time.count()) << " ns\n";


			logsink() << "execution time:     " << fmt::format("{:18}", execution_time.count()) << " ns\n";
			if (::error != Errors::PROCESSING_TIMEOUT)
				logsink() << "serialization time: " << fmt::format("{:18}", serialization_time.count()) << " ns\n";
		}

		logsink() << "total time:         " << fmt::format("{:18}", total_time.count()) << " ns\n";
		logsink() << "total time:         "
				  << fmt::format("{:12}", duration_cast<std::chrono::milliseconds>(total_time).count())
				  << "       ms\n";

		logsink().flush();
	}
}


int main(int argc, char *argv[]) {
	tentris::logging::init_logging();
	TerminalConfig cfg{argc, argv};

	TripleStore triplestore{cfg.cache_size, cfg.cache_bucket_capacity, cfg.timeout};

	onlystdout = cfg.onlystdout;

	if (not cfg.rdf_file.empty()) {
		logsink() << "Loading file " << cfg.rdf_file << " ..." << std::endl;
		auto start_time = system_clock::now();
		triplestore.loadRDF(cfg.rdf_file);
		auto duration = system_clock::now() - start_time;
		logsink() << fmt::format("... loading finished. {} triples loaded.", triplestore.size()) << std::endl;
		logsink() << "duration: {} h {} min {} s"_format(
				(duration_cast<hours>(duration) % 24).count(),
				(duration_cast<minutes>(duration) % 60).count(),
				(duration_cast<seconds>(duration) % 60).count()) << std::endl;
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
