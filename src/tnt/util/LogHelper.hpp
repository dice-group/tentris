#ifndef MACRO_HPP
#define MACRO_HPP

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

#define DEBUG 1 // todo: Find an appropriate place for enabling DEBUG
#define TRACE 1 // todo: Find an appropriate place for enabling DEBUG

#define GET_VARIABLE_NAME(Variable) (#Variable)

namespace {
    using namespace boost::log::trivial;
}

boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

template<typename ...Args>
inline void log(std::string msg, Args &&... args) {
    std::ostringstream oss;
    oss << msg;
    ((oss << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::info) << oss.str();
}

struct processMem_t{
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

inline std::chrono::time_point<std::chrono::system_clock> log_health_data() {
    using namespace std::chrono;
    processMem_t mem = get_memory_usage();
    auto time = system_clock::now();
    log("time: ", date::format("%F %T", date::floor<milliseconds>(time)));
    // log("virtMem: ", mem.virtualMem, "kB");
    log("physMem: ", mem.physicalMem, "kB");
    return time;
}

inline void log_duration(std::chrono::time_point<std::chrono::system_clock> date1, std::chrono::time_point<std::chrono::system_clock> date2) {
    using namespace std::chrono;
    auto duration = date2 - date1;

    log("duration: ", (duration_cast<hours>(duration) % 24).count(), "h ",
            (duration_cast<minutes>(duration) % 60).count(), "min ",
            (duration_cast<seconds>(duration) % 60).count(), "s");
}



template<typename ...Args>
inline void logDebug(std::string msg, Args &&... args) {
#if DEBUG
    std::ostringstream oss;
    oss << msg;
    ((oss << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << oss.str();
#endif
}

template<typename ...Args>
inline void logDebugOptimized(std::string msg, Args &&... args) {
#if DEBUG
    std::ostringstream oss;
    oss << msg;
    ((oss << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << oss.str();
#endif
}


template<typename ...Args>
inline void logTraceOptimized(std::string msg, Args &&... args) {
#if TRACE
    std::ostringstream oss;
    oss << msg;
    ((oss << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::trace) << oss.str();
#endif
}


template<typename ...Args>
inline void logError(std::string msg, Args &&... args) {
    std::ostringstream oss;
    oss << msg;
    ((oss << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::error) << oss.str();
}


void init_logging() {
    using namespace boost::log;
    add_file_log(
            keywords::file_name = "TNT_%N.log",
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
//
    core::get()->set_filter(
            trivial::severity >= trivial::debug
    );
    boost::log::add_common_attributes();
    add_console_log(std::cout, boost::log::keywords::format = "%Message%");
}

#endif