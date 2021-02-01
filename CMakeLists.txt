cmake_minimum_required(VERSION 3.13)
project(tentris CXX)
set(CMAKE_CXX_STANDARD 20)
set(tentris_VERSION_MAJOR 1)
set(tentris_VERSION_MINOR 0)
set(tentris_VERSION_PATCH 7)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    endif ()
endif ()

if(DEFINED ${TENTRIS_MARCH})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=${TENTRIS_MARCH}")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -momit-leaf-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer")
endif ()

if (TENTRIS_STATIC)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif ()

if (TENTRIS_BUILD_WITH_TCMALLOC)
    find_library(TCMALLOCMINIMAL tcmalloc_minimal)
    if (NOT TCMALLOCMINIMAL)
        find_library(TCMALLOCMINIMAL tcmalloc-minimal)
    endif()
    if (NOT TCMALLOCMINIMAL)
        message(FATAL_ERROR "Neither tcmalloc-minimal nor tcmalloc_minimal was found")
    endif()
    message("tcmalloc minimal ${TCMALLOCMINIMAL}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TCMALLOCMINIMAL}")
    if(TENTRIS_STATIC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--eh-frame-hdr")
    endif()
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")

# Lightweight C++ command line option parser   https://github.com/jarro2783/cxxopts
include(FetchContent)
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG        v2.2.1
        GIT_SHALLOW    TRUE
)
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_WARNINGS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cxxopts)

add_library(csv-parser INTERFACE)
target_include_directories(csv-parser INTERFACE
        thirdparty/csv-parser
        )


add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE
        thirdparty/RapidJSON/include
        )

find_package(tsl-hopscotch-map REQUIRED)
find_package(fmt REQUIRED)
find_package(hypertrie REQUIRED)
find_package(sparql-parser-base REQUIRED)
find_package(rdf-parser REQUIRED)
if (TENTRIS_STATIC)
    SET(Boost_USE_STATIC_LIBS ON)
endif ()

find_package(Boost REQUIRED COMPONENTS system log_setup log thread)
if (TENTRIS_STATIC)
    SET(RESTINIO_USE_BOOST_ASIO=static)
endif ()
find_package(restinio REQUIRED)
find_package(string-view-lite REQUIRED)
find_package(optional-lite REQUIRED)

# make a library of the code
add_library(tentris INTERFACE)

include(FetchContent)
FetchContent_Declare(
        cppitertools
        GIT_REPOSITORY https://github.com/ryanhaining/cppitertools.git
        GIT_TAG v2.1
        GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(cppitertools)

target_link_libraries(tentris
        INTERFACE
        stdc++fs # for #include <filesystem>
        cxxopts
        rapidjson
        sparql-parser-base::sparql-parser-base
        tsl-hopscotch-map::tsl-hopscotch-map
        fmt::fmt
        hypertrie::hypertrie
        rdf-parser::rdf-parser
        Boost::Boost
        serd-0
        cppitertools::cppitertools
        )

target_include_directories(tentris INTERFACE
        # todo: move code to include/Dice/tentris
        src/lib/
        ${Boost_INCLUDE_DIRS}
        )

# for rt and pthread linkage see:
# * https://stackoverflow.com/questions/58848694/gcc-whole-archive-recipe-for-static-linking-to-pthread-stopped-working-in-rec
# * https://stackoverflow.com/questions/35116327/when-g-static-link-pthread-cause-segmentation-fault-why
if (TENTRIS_STATIC)
    set(TENTRIS_STATIC_DEF -static)
    set(TENTRIS_PTHREAD_DEF -Wl,--whole-archive -lrt -lpthread -Wl,--no-whole-archive)
else()
    set(TENTRIS_PTHREAD_DEF -lpthread)
endif ()


# main executable targets
add_executable(tentris_server src/exec/TentrisServer.cpp src/exec/config/ServerConfig.hpp src/exec/config/TerminalConfig.hpp)

target_link_libraries(tentris_server
        PRIVATE
        ${TENTRIS_STATIC_DEF}
        ${TENTRIS_PTHREAD_DEF}
        tentris
        restinio::restinio
        nonstd::string-view-lite
        nonstd::optional-lite
        nonstd::variant-lite
        )

set_target_properties(tentris_server PROPERTIES LINK_FLAGS_RELEASE -s)

add_dependencies(tentris_server tentris)

add_executable(tentris_terminal src/exec/TentrisTerminal.cpp src/exec/config/ServerConfig.hpp src/exec/config/TerminalConfig.hpp)
target_link_libraries(tentris_terminal
        ${TENTRIS_STATIC_DEF}
        ${TENTRIS_PTHREAD_DEF}
        tentris
        )

set_target_properties(tentris_terminal PROPERTIES LINK_FLAGS_RELEASE -s)

add_dependencies(tentris_terminal tentris)

add_executable(rdf2ids src/exec/tools/RDF2IDs.cpp)
target_link_libraries(rdf2ids
        ${TENTRIS_STATIC_DEF}
        ${TENTRIS_PTHREAD_DEF}
        tentris
        )

set_target_properties(rdf2ids PROPERTIES LINK_FLAGS_RELEASE -s)

add_dependencies(rdf2ids tentris)


add_executable(ids2hypertrie src/exec/tools/IDs2Hypertrie.cpp)
target_link_libraries(ids2hypertrie
        ${TENTRIS_STATIC_DEF}
        ${TENTRIS_PTHREAD_DEF}
        tentris
        csv-parser
        )

add_dependencies(ids2hypertrie tentris)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_property(TARGET tentris_server PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
    set_property(TARGET tentris_terminal PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

    set_property(TARGET ids2hypertrie PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
    set_property(TARGET rdf2ids PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif ()

option(TENTRIS_BUILD_TESTS "build tests alongside the project" OFF)
if (TENTRIS_BUILD_TESTS)
    enable_testing() # enable this to enable testing
    add_subdirectory(tests)
endif ()
