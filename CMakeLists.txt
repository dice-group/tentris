cmake_minimum_required(VERSION 3.9)
project(tnt CXX)
set (tnt_VERSION_MAJOR 0)
set (tnt_VERSION_MINOR firecracker)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS OFF)
SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# find boost
find_package(Boost  REQUIRED COMPONENTS context)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()
message("Boost libraries: ${Boost_LIBRARIES}")
# build antlr4
# set variable pointing to the antlr tool that supports C++
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.1-complete.jar)
include( ExternalAntlr4Cpp )
#load libraries
link_directories( ${ANTLR4CPP_LIBS} )

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

#include cppitertools, see https://github.com/ryanhaining/cppitertools
add_library(lru INTERFACE)
target_include_directories(lru INTERFACE
        thirdparty/mohaps-lrucache11
        )

#make an interface library from the code
add_library(libsparsetensor INTERFACE)
target_include_directories(libsparsetensor INTERFACE
        src/
        )

# set all linkables
link_libraries(
        stdc++fs # for #include <filesystem>
        serd-0 # for parsing ntriples
        Threads::Threads # for use of threads
        cppitertools
        lru
        libsparsetensor
        antlr4_sparql
        antlr4-runtime
        antlr4_interface
        Boost::boost # header only boost libraries
        boost_context
        ${Pistache_LIBRARY}
)

# main executable target
add_executable(tnt_exec src/Main.cpp)
add_dependencies(tnt_exec antlr4cpp)

enable_testing() # enable this to enable testing
if (CMAKE_TESTING_ENABLED)
    add_subdirectory(tests)
endif ()
