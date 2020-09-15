#include <string>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "config/TerminalConfig.hpp"

#include <tentris/store/QueryExecutionPackage.hpp>
#include <tentris/store/QueryExecutionPackageCache.hpp>
#include <tentris/store/TripleStore.hpp>
#include <tentris/util/LogHelper.hpp>
#include <tentris/tensor/BoolHypertrie.hpp>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <itertools.hpp>
#include "tentris/http/QueryResultState.hpp"

namespace {
	using namespace tentris::store;
	using namespace tentris::store::cache;
	using namespace std::filesystem;
	using namespace iter;
	using namespace tentris::tensor;
	using namespace std::chrono;
}

TerminalConfig cfg;

bool onlystdout = false;

using Errors = tentris::http::ResultState;

std::ostream &logsink() {
	if (onlystdout)
		return std::cout;
	else
		return std::cerr;
}

inline std::string tp2s(time_point_t timepoint) {
	auto in_time_t = system_clock::to_time_t(
			system_clock::now() + duration_cast<system_clock::duration>(timepoint - steady_clock::now()));

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
};

time_point_t query_start;
time_point_t query_end;
time_point_t parse_start;
time_point_t parse_end;
time_point_t execute_start;
time_point_t execute_end;

Errors error;
size_t number_of_bindings;

time_point_t timeout;
time_point_t actual_timeout;

template<typename RESULT_TYPE>
void
writeNTriple(std::ostream &stream, const std::shared_ptr<QueryExecutionPackage> &query_package) {
	const std::vector<Variable> &vars = query_package->getQueryVariables();
	stream << fmt::format("{}\n", fmt::join(vars, ","));

	uint timeout_check = 0;
	size_t result_count = 0;

	bool first = true;

	if (not query_package->is_trivial_empty) {
		std::shared_ptr<void> raw_results = query_package->getEinsum(timeout);
		auto &results = *static_cast<Einsum<RESULT_TYPE> *>(raw_results.get());
		for (const auto &result : results) {
			if (first) {
				first = false;
				execute_end = steady_clock::now();
			}

			std::stringstream ss;
			bool inner_first = true;
			for (auto binding : result.key) {
				if (inner_first)
					inner_first = false;
				else
					ss << ",";
				if (binding != nullptr)
					ss << binding->getIdentifier();
			}
			ss << "\n";

			std::string binding_string = ss.str();

			for ([[maybe_unused]] const auto c : iter::range(result.value)) {
				stream << binding_string;
				++result_count;
				if (++timeout_check == 500) {
					timeout_check = 0;
					stream.flush();
					if (auto current_time = steady_clock::now(); current_time > timeout) {
						::error = Errors::SERIALIZATION_TIMEOUT;
						actual_timeout = current_time;
						number_of_bindings = result_count;
						return;
					}
				}
			}
		}
	}
	if (first) { // if no bindings are returned
		execute_end = steady_clock::now();
	}
	number_of_bindings = result_count;
}

template<typename RESULT_TYPE>
inline void runCMDQuery(const std::shared_ptr<QueryExecutionPackage> &query_package,
						const time_point_t timeout) {
	// calculate the result
	// check if it timed out
	if (steady_clock::now() < timeout) {
		writeNTriple<RESULT_TYPE>(std::cout, query_package);
	} else {
		::error = Errors::PROCESSING_TIMEOUT;
		actual_timeout = steady_clock::now();
	}
}

void commandlineInterface(QueryExecutionPackage_cache &querypackage_cache) {
	std::string sparql_str;
	while (std::getline(std::cin, sparql_str)) {

		query_start = steady_clock::now();


		number_of_bindings = 0;
		::error = Errors::OK;


		try {
			parse_start = steady_clock::now();
			std::shared_ptr<QueryExecutionPackage> query_package = querypackage_cache[sparql_str];

			timeout = steady_clock::now() + cfg.timeout;

			parse_end = steady_clock::now();
			execute_start = steady_clock::now();

			switch (query_package->getSelectModifier()) {
				case SelectModifier::NONE: {
					runCMDQuery<COUNTED_t>(query_package, timeout);
					break;
				}
				case SelectModifier::REDUCE:
					[[fallthrough]];
				case SelectModifier::DISTINCT: {
					runCMDQuery<DISTINCT_t>(query_package, timeout);
					break;
				}
				default:
					break;
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
		query_end = steady_clock::now();


		auto parsing_time = duration_cast<std::chrono::nanoseconds>(parse_end - parse_start);
		auto execution_time = duration_cast<std::chrono::nanoseconds>(execute_end - execute_start);
		auto total_time = duration_cast<std::chrono::nanoseconds>(query_end - query_start);
		auto serialization_time = total_time - execution_time - parsing_time;
		switch (::error) {
			case Errors::OK:
				logsink() << "SUCCESSFUL\n";
				break;
			case Errors::UNPARSABLE:
				logsink() << "ERROR: UNPARSABLE QUERY\n";
				break;
			case Errors::PROCESSING_TIMEOUT:
				logsink() << "ERROR: TIMEOUT DURING PROCESSING\n";
				break;
			case Errors::SERIALIZATION_TIMEOUT:
				logsink() << "ERROR: TIMEOUT DURING SERIALIZATION\n";
				break;
			case Errors::UNEXPECTED:
				logsink() << "ERROR: UNEXPECTED\n";
				break;
			case Errors::SEVERE_UNEXPECTED:
				logsink() << "ERROR: SEVERE UNEXPECTED\n";
				break;
			default:
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
	std::raise(SIGINT);
}


int main(int argc, char *argv[]) {
	auto const_argv = const_cast<const char **>(argv);
	cfg = TerminalConfig{argc, const_argv};
	tentris::logging::init_logging(cfg.logstdout, cfg.logfile, cfg.logfiledir, cfg.loglevel);

	TripleStore triplestore{};

	QueryExecutionPackage_cache executionpackage_cache{cfg.cache_size};


	onlystdout = cfg.onlystdout;

	if (not cfg.rdf_file.empty()) {
		logsink() << "Loading file " << cfg.rdf_file << " ..." << std::endl;
		auto start_time = steady_clock::now();
		AtomicTripleStore::getInstance().loadRDF(cfg.rdf_file);
		auto duration = steady_clock::now() - start_time;
		logsink() << fmt::format("... loading finished. {} triples loaded.", AtomicTripleStore::getInstance().size())
				  << std::endl;
		logsink() << "duration: {} h {} min {} s"_format(
				(duration_cast<hours>(duration) % 24).count(),
				(duration_cast<minutes>(duration) % 60).count(),
				(duration_cast<seconds>(duration) % 60).count()) << std::endl;
	}


	std::thread commandline_client{commandlineInterface, std::ref(executionpackage_cache)};
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
