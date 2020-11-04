cmake_minimum_required(VERSION 3.13)

# load CMake Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Bootstrap Vcpkg and download dependencies
include(AutoVcpkg)
vcpkg_install(
        boost-core boost-system boost-log boost-thread boost-lockfree
        cppitertools
        restinio http-parser
        tsl-hopscotch-map tsl-sparse-map robin-hood-hashing
        abseil
        fmt
        catch2
        pegtl
        )

message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}\n-----\n## Please add this to your CMake invocation `-D${CMAKE_TOOLCHAIN_FILE}` ## \n-----")

###
project(tentris CXX)
set(CMAKE_CXX_STANDARD 20)
set(tentris_VERSION_MAJOR 1)
set(tentris_VERSION_MINOR 1)
set(tentris_VERSION_PATCH 0)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    endif ()
endif ()

if(NOT DEFINED ${TENTRIS_MARCH})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=x86-64")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=${TENTRIS_MARCH}")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -momit-leaf-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer")
endif ()
if (TENTRIS_BUILD_WITH_TCMALLOC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Ltcmalloc-minimal")
    if(TENTRIS_STATIC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--eh-frame-hdr")
    endif()
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")

include(FetchContent)
FetchContent_Declare(hypertrie
        GIT_REPOSITORY https://github.com/dice-group/hypertrie.git
        GIT_TAG origin/vcpkg
        )

FetchContent_MakeAvailable(hypertrie)

FetchContent_Declare(sparql-parser-basex
        GIT_REPOSITORY https://github.com/dice-group/sparql-parser-base.git
        GIT_TAG origin/vcpkg
        )

FetchContent_MakeAvailable(sparql-parser-basex)

FetchContent_Declare(rdf-parser
        GIT_REPOSITORY https://github.com/dice-group/rdf-parser.git
        GIT_TAG origin/vcpkg
        )

FetchContent_MakeAvailable(rdf-parser)



# Lightweight C++ command line option parser   https://github.com/jarro2783/cxxopts
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE
        thirdparty/cxxopts/include
        )

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE
        thirdparty/RapidJSON/include
        )

find_package(absl CONFIG REQUIRED)
find_package(tsl-hopscotch-map CONFIG REQUIRED)
find_package(fmt REQUIRED)
find_package(cppitertools CONFIG REQUIRED)

if (TENTRIS_STATIC)
    SET(Boost_USE_STATIC_LIBS ON)
endif ()

find_package(Boost REQUIRED COMPONENTS system log_setup log thread)
if (TENTRIS_STATIC)
    SET(RESTINIO_USE_BOOST_ASIO=static)
else()
    SET(RESTINIO_USE_BOOST_ASIO=shared)
endif ()
find_package(restinio REQUIRED)
find_package(unofficial-http-parser CONFIG REQUIRED)

# make a library of the code
add_library(tentris INTERFACE)

target_link_libraries(tentris
        INTERFACE
        stdc++fs # for #include <filesystem>
        cxxopts
        rapidjson
        sparql-parser-base
        absl::hash
        tsl::hopscotch_map
        fmt::fmt
        hypertrie::hypertrie
        rdf_parser
        Boost::headers
        Boost::log_setup
        Boost::log
        Boost::thread
        serd-0
        )

target_include_directories(tentris INTERFACE
        # todo: move code to include/Dice/tentris
        src/lib/
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
