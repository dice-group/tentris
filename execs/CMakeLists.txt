cmake_minimum_required(VERSION 3.18)
project(tentris-binaries)

include(${CMAKE_SOURCE_DIR}/../cmake/boilerplate_init.cmake)

boilerplate_init()
if (NOT IS_TOP_LEVEL)
    message(FATAL_ERROR "Must only be used as CMake top-level project.")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    endif ()
endif ()

if (DEFINED ${MARCH})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=${MARCH} -mtune=${MARCH}")
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
if (CMAKE_CXX_COMPILER_ID MATCHES " Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer -momit-leaf-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer")
endif ()

option(STATIC "Build tentris executables statically" OFF)
if (STATIC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--eh-frame-hdr -static")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif ()

option(WITH_TCMALLOC "Build tentris with tcmalloc." OFF)
if (WITH_TCMALLOC)
    find_library(TCMALLOCMINIMAL tcmalloc_minimal)
    if (NOT TCMALLOCMINIMAL)
        find_library(TCMALLOCMINIMAL tcmalloc-minimal)
    endif ()
    if (NOT TCMALLOCMINIMAL)
        message(FATAL_ERROR "Neither tcmalloc-minimal nor tcmalloc_minimal was found")
    endif ()
    message(STATUS "tcmalloc minimal ${TCMALLOCMINIMAL}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TCMALLOCMINIMAL}")
endif ()


# set library options
include(${CMAKE_SOURCE_DIR}/../cmake/conan_cmake.cmake)
install_packages_via_conan("${CMAKE_SOURCE_DIR}/../conanfile.py" "with_exec_deps=True ")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/libtentris)

add_subdirectory(tentris-server)
add_subdirectory(tentris-loader)
add_subdirectory(tools)
