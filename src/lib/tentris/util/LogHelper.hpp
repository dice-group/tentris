#ifndef LOGHELPER_HPP
#define LOGHELPER_HPP

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
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
	using namespace boost::log::trivial;
	using namespace std::chrono;
}

namespace tentris::logging {
	using namespace fmt::literals;
	namespace {
		boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;


		struct processMem_t {
			uint32_t virtualMem;
			uint32_t physicalMem;
		};


		inline int parseLine(char *line) {
			// This assumes that a digit will be found and the line ends in " Kb".
			int i = strlen(line);
			const char *p = line;
			while (*p < '0' || *p > '9') p++;
			line[i - 3] = '\0';
			i = atoi(p);
			return i;
		}

		inline processMem_t get_memory_usage() {
			FILE *file = fopen("/proc/self/status", "r");
			char line[128];
			processMem_t processMem{};

			while (fgets(line, 128, file) != NULL) {
				// std::cout << line << std::endl;
				if (strncmp(line, "VmSize:", 7) == 0) {
					processMem.virtualMem = parseLine(line);
				}

				if (strncmp(line, "VmRSS:", 6) == 0) {
					processMem.physicalMem = parseLine(line);
				}
			}
			fclose(file);
			return processMem;
		}
	}

	void init_logging() {
		using namespace boost::log;
		add_file_log(
				keywords::file_name = "TENTRIS_%N.log",
				keywords::rotation_size = 10 * 1024 * 1024,
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::format = (
						expressions::stream
								<< std::setw(8) << std::setfill('0')
								<< expressions::attr<unsigned int>("LineID")
								<< "\t"
								<< expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",
																						   "%Y-%m-%d_%H:%M:%S.%f")
								<< "\t: <" << trivial::severity
								<< "> \t"
								<< expressions::smessage
				),
				keywords::auto_flush = true
		);
		core::get()->set_filter(
				trivial::severity >= trivial::trace
		);
		boost::log::add_common_attributes();
		add_console_log(std::cout, boost::log::keywords::format = "%Message%");
	}


	inline void log(std::string msg) {
		BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::info) << msg;
	}

	inline time_point<system_clock> log_health_data() {
		using namespace std::chrono;
		processMem_t mem = get_memory_usage();
		const auto time = std::chrono::system_clock::now();
		const std::time_t t = std::chrono::system_clock::to_time_t(time);
		log(fmt::format("time: {:%F_%T}", *std::localtime(&t)));
		// log("virtMem: ", mem.virtualMem, "kB");
		log("physMem: {} kB"_format(mem.physicalMem));
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
#if DEBUG
		BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << msg;
#endif
	}

	inline void logTrace([[maybe_unused]]std::string msg) {
#if TRACE
		BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::trace) << msg;
#endif
	}


	inline void logError(std::string msg) {
		BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::error) << msg;
	}
}

#endif // LOGHELPER_HPP