cmake_minimum_required(VERSION 3.13)
project(tentris CXX)
set(CMAKE_CXX_STANDARD 17)
set(tentris_VERSION_MAJOR 1)
set(tentris_VERSION_MINOR 0)
set(tentris_VERSION_PATCH 1)


SET(Boost_USE_STATIC_LIBS ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -momit-leaf-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer")
endif ()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# build antlr4
add_definitions(-DANTLR4CPP_STATIC)
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.2-complete.jar)
include(ExternalAntlr4Cpp)

find_package(ANTLR REQUIRED)

antlr_target(SPARQLGrammar
        Sparql.g4
        LEXER PARSER LISTENER VISITOR
        PACKAGE tentris::a4grammar::sparql)

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
find_package(fmt CONFIG REQUIRED)
add_subdirectory(thirdparty/sparse-map)

add_library(sparsepp INTERFACE)
target_include_directories(sparsepp INTERFACE
        thirdparty/sparsepp/
        )

add_library(boolhypertrie INTERFACE)
target_include_directories(boolhypertrie INTERFACE
        src/boolhypertrie
        )


find_package(Boost COMPONENTS system log thread log_setup REQUIRED)

target_link_libraries(boolhypertrie
        INTERFACE
        tsl::sparse_map
        cppitertools
        fmt::fmt
        absl_hash
        http_parser
        ${conan_libs}
        ${Boost_LIBRARIES}
        )

set(libraries_to_link
        stdc++fs # for #include <filesystem>
        serd-0 # for parsing ntriples
        cxxopts
        antlr4_sparql
        boolhypertrie
        )

# make a library of the code
add_library(tentris INTERFACE)

target_link_libraries(tentris
        INTERFACE
        ${libraries_to_link}
        )

target_include_directories(tentris INTERFACE
        src/lib/
        )

# main executable target
add_executable(tentris_server src/exec/TentrisServer.cpp src/exec/config/ServerConfig.hpp src/exec/config/TerminalConfig.hpp)

target_link_libraries(tentris_server
        PRIVATE
        tentris
        )

if(CMAKE_BUILD_TYPE MATCHES "Release")
    set_property(TARGET tentris_server PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

add_dependencies(tentris_server tentris)

add_executable(tentris_terminal src/exec/TentrisTerminal.cpp src/exec/config/ServerConfig.hpp src/exec/config/TerminalConfig.hpp)
target_link_libraries(tentris_terminal
        PRIVATE
        tentris
        )

option(TENTRIS_BUILD_TESTS "build tests alongside the project" OFF)
if (TENTRIS_BUILD_TESTS)
    enable_testing() # enable this to enable testing
    add_subdirectory(tests)
endif ()
