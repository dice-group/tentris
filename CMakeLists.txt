cmake_minimum_required(VERSION 3.13)
project(tentris CXX)
set(CMAKE_CXX_STANDARD 20)
set(tentris_VERSION_MAJOR 1)
set(tentris_VERSION_MINOR 1)
set(tentris_VERSION_PATCH 0)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

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
if (TENTRIS_BUILD_WITH_TCMALLOC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ltcmalloc -Wl,--eh-frame-hdr")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")



# Lightweight C++ command line option parser   https://github.com/jarro2783/cxxopts
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE
        thirdparty/cxxopts/include
        )

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE
        thirdparty/RapidJSON/include
        )

find_package(absl REQUIRED)
find_package(tsl-hopscotch-map REQUIRED)
find_package(fmt REQUIRED)
find_package(hypertrie REQUIRED)
find_package(sparql-parser-base REQUIRED)
find_package(rdf-parser REQUIRED)
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system log_setup log thread)
SET(RESTINIO_USE_BOOST_ASIO=static)
find_package(restinio REQUIRED)
find_package(http-parser REQUIRED)
find_package(string-view-lite REQUIRED)
find_package(optional-lite REQUIRED)

# make a library of the code
add_library(tentris INTERFACE)

target_link_libraries(tentris
        INTERFACE
        stdc++fs # for #include <filesystem>
        cxxopts
        rapidjson
        sparql-parser-base::sparql-parser-base
        absl::absl
        tsl-hopscotch-map::tsl-hopscotch-map
        fmt::fmt
        hypertrie::hypertrie
        rdf-parser::rdf-parser
        Boost::Boost
        serd-0
        )

target_include_directories(tentris INTERFACE
        # todo: move code to include/Dice/tentris
        src/lib/
        ${Boost_INCLUDE_DIRS}
        )


# main executable targets
add_executable(tentris_server src/exec/TentrisServer.cpp src/exec/config/ServerConfig.hpp src/exec/config/TerminalConfig.hpp)

target_link_libraries(tentris_server
        PRIVATE
        -static
        -Wl,--whole-archive -lrt -lpthread -Wl,--no-whole-archive
        tentris
        restinio::restinio
        http-parser::http-parser
        nonstd::string-view-lite
        nonstd::optional-lite
        nonstd::variant-lite
        )

set_target_properties(tentris_server PROPERTIES LINK_FLAGS_RELEASE -s)

add_dependencies(tentris_server tentris)

add_executable(tentris_terminal src/exec/TentrisTerminal.cpp src/exec/config/ServerConfig.hpp src/exec/config/TerminalConfig.hpp)
target_link_libraries(tentris_terminal
        -static
        tentris
        -Wl,--whole-archive -lrt -lpthread -Wl,--no-whole-archive
        )

set_target_properties(tentris_terminal PROPERTIES LINK_FLAGS_RELEASE -s)

add_dependencies(tentris_terminal tentris)

add_executable(rdf2ids src/exec/tools/RDF2IDs.cpp)
target_link_libraries(rdf2ids
        -static
        tentris
        -Wl,--whole-archive -lrt -lpthread -Wl,--no-whole-archive
        )

set_target_properties(rdf2ids PROPERTIES LINK_FLAGS_RELEASE -s)

add_dependencies(rdf2ids tentris)

# for rt and pthread linkage see:
# * https://stackoverflow.com/questions/58848694/gcc-whole-archive-recipe-for-static-linking-to-pthread-stopped-working-in-rec
# * https://stackoverflow.com/questions/35116327/when-g-static-link-pthread-cause-segmentation-fault-why
add_executable(ids2hypertrie src/exec/tools/IDs2Hypertrie.cpp)
target_link_libraries(ids2hypertrie
        -static
        tentris
        -Wl,--whole-archive -lrt -lpthread -Wl,--no-whole-archive
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
