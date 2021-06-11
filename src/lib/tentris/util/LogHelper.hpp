#ifndef LOGHELPER_HPP
#define LOGHELPER_HPP

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <iostream>
#include <sstream>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <chrono>
#include <ctime>
#include <filesystem>

#include "tentris/util/FmtHelper.hpp"


namespace tentris::logging {

	namespace {
		using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;

		using namespace fmt::literals;

		using PhysicalMem = uint32_t;


		inline int parseLine(char *line) {
			// This assumes that a digit will be found and the line ends in " Kb".
			int i = strlen(line);
			const char *p = line;
			while (*p < '0' || *p > '9') p++;
			line[i - 3] = '\0';
			i = atoi(p);
			return i;
		}

		inline PhysicalMem get_memory_usage() {
			FILE *file = fopen("/proc/self/status", "r");
			char line[128];
			PhysicalMem physicalMem{};

			while (fgets(line, 128, file) != NULL) {

				if (strncmp(line, "VmRSS:", 6) == 0) {
					physicalMem = parseLine(line);
				}
			}
			fclose(file);
			return physicalMem;
		}
	}

	inline void init_logging(bool logstdout, bool logfile, std::string logfiledir,
					  boost::log::trivial::severity_level severity = boost::log::trivial::info) {
		namespace logging = boost::log;
		namespace src = boost::log::sources;
		namespace sinks = boost::log::sinks;
		namespace keywords = boost::log::keywords;
		namespace fs = std::filesystem;
		logging::add_common_attributes();
		boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

		logging::core::get()->set_filter(logging::trivial::severity >= severity);
		static const auto log_format = "%LineID% | %TimeStamp% | %ThreadID% | %Severity% | %Message%";
		if (not logfile and not logstdout)
			logging::core::get()->set_logging_enabled(false);
		if (logfile)
			logging::add_file_log(
					keywords::file_name = fs::path(logfiledir).append("tentris_%N.log"),
					keywords::rotation_size = 5 * 512 * 1024,
					keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
					keywords::auto_flush = true,
					keywords::format = log_format
			);
		if (logstdout)
			auto stdout_sink = logging::add_console_log(std::cout, keywords::format = log_format);
	}


	inline void log(std::string msg) {
		BOOST_LOG_TRIVIAL(info) << msg;
	}

	inline time_point_t log_health_data() {
		using namespace std::chrono;
		auto mem = get_memory_usage();
		const auto time = steady_clock::now();
		const std::time_t t = system_clock::to_time_t(system_clock::now());
		log(fmt::format("time: {:%F_%T}", *std::localtime(&t)));
		log("Mem: {} kB"_format(mem));
		return time;
	}

	inline void log_duration(std::chrono::time_point<std::chrono::steady_clock> start_time,
							 std::chrono::time_point<std::chrono::steady_clock> end_time) {
		using namespace std::chrono;
		const auto duration = end_time - start_time;

		// xx h xx min xx s
		log("duration: {} h {} min {}.{:03d} s = {} ms\n"_format(
				(std::chrono::duration_cast<std::chrono::hours>(duration)).count(),
				(std::chrono::duration_cast<std::chrono::minutes>(duration) % 60).count(),
				(std::chrono::duration_cast<std::chrono::seconds>(duration) % 60).count(),
				(std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000).count(),
				std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()));
	}

	/**
	 * Make a duration string out of the distance between two time_points
	 * @tparam clock the clock used by the time_points. Should be inferred automatically in most cases.
	 * @param start_time start time
	 * @param end_time end time
	 * @return a string
	 */
	template<typename clock>
	inline std::string
	toDurationStr(std::chrono::time_point<clock> start_time, std::chrono::time_point<clock> end_time) {
		using namespace std::chrono;
		auto duration = end_time - start_time;
		return "{:02d}h{:02d}m{:02d}s.{:03d}'{:03d}'{:03d}"_format(
				duration_cast<hours>(duration).count(),
				duration_cast<minutes>(duration).count() % 60,
				duration_cast<seconds>(duration).count() % 60,
				duration_cast<milliseconds>(duration).count() % 1000,
				duration_cast<microseconds>(duration).count() % 1000,
				duration_cast<nanoseconds>(duration).count() % 1000);
	}

	inline void logDebug(std::string msg) {
		BOOST_LOG_TRIVIAL(debug) << msg;
	}

	inline void logTrace(std::string msg) {
		BOOST_LOG_TRIVIAL(trace) << msg;
	}


	inline void logError(std::string msg) {
		BOOST_LOG_TRIVIAL(error) << msg;
	}
}

#endif // LOGHELPER_HPP