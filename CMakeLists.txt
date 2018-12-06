cmake_minimum_required(VERSION 3.10)
project(tnt CXX)
set(tnt_VERSION_MAJOR 0)
set(tnt_VERSION_MINOR firecracker)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS OFF)
SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# find boost
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS context system log thread log_setup)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF ()
message("Boost libraries: ${Boost_LIBRARIES}")

# build antlr4
# set variable pointing to the antlr tool that supports C++
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.1-complete.jar)
include(ExternalAntlr4Cpp)
#load libraries
link_directories(${ANTLR4CPP_LIBS})

# add header interface for antlr4-runtime
add_library(antlr4_interface INTERFACE)
target_include_directories(antlr4_interface INTERFACE ${ANTLR4CPP_INCLUDE_DIRS})

#make an interface library from the SPARQL grammar
add_library(antlr4_sparql INTERFACE)
target_include_directories(antlr4_sparql INTERFACE
        grammar/
        )

#include pistache
find_package(Pistache REQUIRED)

#include threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#include cppitertools, see https://github.com/ryanhaining/cppitertools
add_library(cppitertools INTERFACE)
target_include_directories(cppitertools INTERFACE
        thirdparty/cppitertools
        )

# Lightweight C++ command line option parser   https://github.com/jarro2783/cxxopts
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE
        thirdparty/cxxopts/include
        )


add_library(datetimelib INTERFACE)
target_include_directories(datetimelib INTERFACE
        thirdparty/date/include
        )

#make an interface library from the code
add_library(libsparsetensor INTERFACE)
target_include_directories(libsparsetensor INTERFACE
        src/
        )

target_link_libraries(libsparsetensor
        INTERFACE
        stdc++fs # for #include <filesystem>
        serd-0 # for parsing ntriples
        Threads::Threads # for use of threads
        cppitertools
        datetimelib
        cxxopts
        antlr4_sparql
        antlr4-runtime
        antlr4_interface
        Boost::boost # header only boost libraries
        ${Boost_LIBRARIES}
        ${Pistache_LIBRARY}
        )

# main executable target
add_executable(tnt_exec src/Main.cpp)
target_link_libraries(tnt_exec
        PRIVATE libsparsetensor)
add_dependencies(tnt_exec antlr4cpp)

enable_testing() # enable this to enable testing
if (CMAKE_TESTING_ENABLED)
    add_subdirectory(tests)
endif ()
