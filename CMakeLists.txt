cmake_minimum_required(VERSION 3.10)
project(tnt CXX)
set(tnt_VERSION_MAJOR 0)
set(tnt_VERSION_MINOR pinwheel)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS OFF)
SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -momit-leaf-frame-pointer")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# find boost
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS context system log thread log_setup)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF ()

# build antlr4
add_definitions(-DANTLR4CPP_STATIC)
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.2-complete.jar)
include(ExternalAntlr4Cpp)

find_package(ANTLR REQUIRED)


antlr_target(SPARQLGrammar
        Sparql.g4
        LEXER PARSER LISTENER VISITOR
        PACKAGE tnt::a4grammar::sparql)

#TODO: seperate sources and headers
#set(ANTLR_SPARQLGrammar_headers ${ANTLR_SPARQLGrammar_CXX_OUTPUTS})
#LIST(FILTER ANTLR_SPARQLGrammar_headers INCLUDE REGEX ".+\\.h$")
#
#set(ANTLR_SPARQLGrammar_sources ${ANTLR_SPARQLGrammar_CXX_OUTPUTS})
#LIST(FILTER ANTLR_SPARQLGrammar_sources INCLUDE REGEX ".+\\.cpp$")

#make an interface library from the SPARQL grammar
add_library(antlr4_sparql STATIC
        ${ANTLR_SPARQLGrammar_CXX_OUTPUTS}
        )
target_include_directories(antlr4_sparql
        PUBLIC ${ANTLR_SPARQLGrammar_OUTPUT_DIR}
        ${ANTLR4_INCLUDE_DIRS}
        )
target_link_libraries(antlr4_sparql antlr4_static)
add_dependencies(antlr4_sparql antlr4_static)

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

#fmt
add_subdirectory(thirdparty/fmt)

set(libraries_to_link
        stdc++fs # for #include <filesystem>
        serd-0 # for parsing ntriples
        pthread
        # Threads::Threads # for use of threads
        cppitertools
        datetimelib
        cxxopts
        antlr4_sparql
        Boost::boost # header only boost libraries
        fmt::fmt
        ${Boost_LIBRARIES}
        ${Pistache_LIBRARY}
        )

# make a library of the code
add_library(libsparsetensor STATIC
        src/
        )

target_link_libraries(libsparsetensor
        ${libraries_to_link}
        )

target_include_directories(libsparsetensor PUBLIC
        src/
        )

# main executable target
add_executable(tnt_exec src/Main.cpp)
target_include_directories(tnt_exec PRIVATE
        src/
        )

target_link_libraries(tnt_exec
        PRIVATE ${libraries_to_link}
        )

enable_testing() # enable this to enable testing
if (CMAKE_TESTING_ENABLED)
    add_subdirectory(tests)
endif ()
