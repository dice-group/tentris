#ifndef MACRO_HPP
#define MACRO_HPP

#include <boost/log/trivial.hpp>
#include <iostream>
#include <sstream>

#define DEBUG 1 //todo Find an appropriate place for enabling DEBUG
#define TRACE 1 //todo Find an appropriate place for enabling DEBUG

#define GET_VARIABLE_NAME(Variable) (#Variable)

boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

template<typename ...Args>
inline void logDebug(std::string msg, Args &&... args) {
    std::ostringstream oss;
    oss << msg;
    ((oss << ", " << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << oss.str();
}

template<typename ...Args>
inline void logDebugOptimized(std::string msg, Args &&... args) {
#if DEBUG
    std::ostringstream oss;
    oss << msg;
    ((oss << ", " << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << oss.str();
#endif
}


template<typename ...Args>
inline void logTraceOptimized(std::string msg, Args &&... args) {
#if TRACE
    std::ostringstream oss;
    oss << msg;
    ((oss << ", " << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::trace) << oss.str();
#endif
}


template<typename ...Args>
inline void logError(std::string msg, Args &&... args) {
    std::ostringstream oss;
    oss << msg;
    ((oss << ", " << args), ...);
    BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::error) << oss.str();
}


#endif