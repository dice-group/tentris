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

#include <date/tz.h>

#include "tentris/util/FmtHelper.hpp"



// #define DEBUG 1 // todo: Find an appropriate place for enabling DEBUG
// #define TRACE 1 // todo: Find an appropriate place for enabling DEBUG

// #define GET_VARIABLE_NAME(Variable) (#Variable)

namespace {
	using namespace std::chrono;
}

namespace tentris::logging {
	using namespace fmt::literals;
	namespace {


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

	void init_logging() {
//		using namespace boost::log;
		namespace logging = boost::log;
		namespace src = boost::log::sources;
		namespace sinks = boost::log::sinks;
		namespace keywords = boost::log::keywords;
		logging::add_common_attributes();
		boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

		logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
		auto file_log = logging::add_file_log(
				keywords::file_name = "TENTRIS_%N.log",
				keywords::rotation_size = 5 * 512 * 1024,
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::auto_flush = true,
				keywords::format = "[%LineID%] [%TimeStamp%] [%ThreadID%] <%Severity%>: %Message%"
		);
		auto console_sink = logging::add_console_log(std::cout,
		                                             keywords::format = "[%LineID%] [%TimeStamp%] [%ThreadID%] <%Severity%>: %Message%");
	}


	inline void log(std::string msg) {
		BOOST_LOG_TRIVIAL(info) << msg;
	}

	inline time_point<system_clock> log_health_data() {
		using namespace std::chrono;
		auto mem = get_memory_usage();
		const auto time = std::chrono::system_clock::now();
		const std::time_t t = std::chrono::system_clock::to_time_t(time);
		log(fmt::format("time: {:%F_%T}", *std::localtime(&t)));
		// log("virtMem: ", mem.virtualMem, "kB");
		log("Mem: {} kB"_format(mem));
		return time;
	}

	inline void log_duration(time_point<system_clock> start_time, time_point<system_clock> end_time) {
		using namespace std::chrono;
		auto duration = end_time - start_time;

		// xx h xx min xx s
		log("duration: {} h {} min {} s"_format(
				(duration_cast<hours>(duration) % 24).count(),
				(duration_cast<minutes>(duration) % 60).count(),
				(duration_cast<seconds>(duration) % 60).count()));
	}

	/**
	 * Make a Timestamp string without the date out of a time_point
	 * @tparam clock the clock used by the time_point. Should be inferred automatically in most cases.
	 * @param time_point the time_point of interest
	 * @return a string
	 */
	template<typename clock>
	inline std::string toTimestampStr(time_point<clock> time_point) {
		using namespace std::chrono;
		auto const time_point_t = system_clock::to_time_t(time_point);
		auto tse = time_point.time_since_epoch();
		return fmt::format("{:%H:%M:%S}.{:03d}'{:03d}'{:03d}", *std::localtime(&time_point_t),
		                   duration_cast<milliseconds>(tse).count() % 1000,
		                   duration_cast<microseconds>(tse).count() % 1000,
		                   duration_cast<nanoseconds>(tse).count() % 1000);
	}

	/**
	 * Make a date string out of a time_point
	 * @tparam clock the clock used by the time_point. Should be inferred automatically in most cases.
	 * @param time_point the time_point of interest
	 * @return a string
	 */
	template<typename clock>
	inline std::string toDateStr(time_point<clock> time_point) {
		auto const time_point_t = std::chrono::system_clock::to_time_t(time_point);
		return fmt::format("{:%Y-%m-%d}", *std::localtime(&time_point_t));
	}

	/**
	 * Make a duration string out of the distance between two time_points
	 * @tparam clock the clock used by the time_points. Should be inferred automatically in most cases.
	 * @param start_time start time
	 * @param end_time end time
	 * @return a string
	 */
	template<typename clock>
	inline std::string toDurationStr(time_point<clock> start_time, time_point<clock> end_time) {
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

	inline void logDebug([[maybe_unused]]std::string msg) {
		BOOST_LOG_TRIVIAL(debug) << msg;
	}

	inline void logTrace([[maybe_unused]]std::string msg) {
		BOOST_LOG_TRIVIAL(trace) << msg;
	}


	inline void logError(std::string msg) {
		BOOST_LOG_TRIVIAL(error) << msg;
	}
}

#endif // LOGHELPER_HPP