# ~~~
# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~


set(AUTO_VCPKG_GIT_REPOSITORY "https://github.com/Microsoft/vcpkg.git")

function (vcpkg_download)
    if (DEFINED AUTO_VCPKG_ROOT)
        return()
    endif ()
    # message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
    # message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
    set(AUTO_VCPKG_ROOT "${CMAKE_BINARY_DIR}/vcpkg")
    set(vcpkg_download_contents [===[
cmake_minimum_required(VERSION 3.12)
project(vcpkg-download)

include(ExternalProject)
ExternalProject_Add(vcpkg
            GIT_REPOSITORY @AUTO_VCPKG_GIT_REPOSITORY@
            GIT_SHALLOW ON
            SOURCE_DIR @AUTO_VCPKG_ROOT@
            PATCH_COMMAND ""
            CONFIGURE_COMMAND  ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_INSTALL ON)
    ]===])
    string(REPLACE "@AUTO_VCPKG_GIT_REPOSITORY@" "${AUTO_VCPKG_GIT_REPOSITORY}" vcpkg_download_contents "${vcpkg_download_contents}")
    string(REPLACE "@AUTO_VCPKG_ROOT@" "${AUTO_VCPKG_ROOT}" vcpkg_download_contents "${vcpkg_download_contents}")
    file(WRITE "${CMAKE_BINARY_DIR}/vcpkg-download/CMakeLists.txt" "${vcpkg_download_contents}")
    if(WIN32)
        get_filename_component(VC_COMPILER_PATH ${CMAKE_C_COMPILER} DIRECTORY)
        set(VCRT_LIB_PATH "${VC_COMPILER_PATH}/../../../lib/x86")
        # message(STATUS "VCRT_LIB_PATH=${VCRT_LIB_PATH}")
        execute_process(COMMAND "${CMAKE_COMMAND}"
                "-H${CMAKE_BINARY_DIR}/vcpkg-download"
                "-B${CMAKE_BINARY_DIR}/vcpkg-download"
                "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}"
                "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}"
                "-DCMAKE_EXE_LINKER_FLAGS=/LIBPATH:\"${VCRT_LIB_PATH}\"")
        execute_process(COMMAND "${CMAKE_COMMAND}"
                "--build" "${CMAKE_BINARY_DIR}/vcpkg-download")
    else()
        execute_process(COMMAND "${CMAKE_COMMAND}"
                "-H${CMAKE_BINARY_DIR}/vcpkg-download"
                "-B${CMAKE_BINARY_DIR}/vcpkg-download")
        execute_process(COMMAND "${CMAKE_COMMAND}"
                "--build" "${CMAKE_BINARY_DIR}/vcpkg-download")
    endif()
endfunction ()

function (vcpkg_bootstrap)
    find_program(AUTO_VCPKG_EXECUTABLE vcpkg PATHS ${AUTO_VCPKG_ROOT})
    if (NOT AUTO_VCPKG_EXECUTABLE)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_LIST_DIR}/cmake/VcpkgBootstrap.cmake" "${AUTO_VCPKG_ROOT}")
        execute_process(COMMAND ${CMAKE_COMMAND} -P "${AUTO_VCPKG_ROOT}/VcpkgBootstrap.cmake"
                WORKING_DIRECTORY ${AUTO_VCPKG_ROOT})
    endif ()
endfunction ()

function (vcpkg_download_and_bootstrap)
    if (NOT DEFINED AUTO_VCPKG_ROOT)
        if (NOT DEFINED ENV{AUTO_VCPKG_ROOT})
            vcpkg_download()
            set(AUTO_VCPKG_ROOT "${CMAKE_BINARY_DIR}/vcpkg" CACHE STRING "")
        else ()
            set(AUTO_VCPKG_ROOT "$ENV{AUTO_VCPKG_ROOT}" CACHE STRING "")
        endif ()
        message(STATUS "Setting AUTO_VCPKG_ROOT to ${AUTO_VCPKG_ROOT}")
        mark_as_advanced(AUTO_VCPKG_ROOT)
    endif ()
    vcpkg_bootstrap()
endfunction ()

function (vcpkg_configure)
    if (AUTO_VCPKG_EXECUTABLE AND DEFINED AUTO_VCPKG_ROOT)
        set(CMAKE_TOOLCHAIN_FILE
                "${AUTO_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        return()
    endif ()

    vcpkg_download_and_bootstrap()
    message("Searching for vcpkg in ${AUTO_VCPKG_ROOT}")
    find_program(AUTO_VCPKG_EXECUTABLE vcpkg PATHS ${AUTO_VCPKG_ROOT})
    if (NOT AUTO_VCPKG_EXECUTABLE)
        message(FATAL_ERROR "Cannot find vcpkg executable")
    endif ()
    mark_as_advanced(AUTO_VCPKG_EXECUTABLE)
    set(CMAKE_TOOLCHAIN_FILE "${AUTO_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endfunction ()

function (vcpkg_install)
    # Ensure the vcpkg system is configured.
    vcpkg_configure()

    cmake_parse_arguments(_vcpkg_install "" "TRIPLET" "" ${ARGN})
    if (NOT ARGN)
        message(STATUS "vcpkg_install() called with no packages to install")
        return()
    endif ()

    if (NOT _vcpkg_install_TRIPLET)
        set(packages ${ARGN})
    else ()
        string(APPEND ":${_vcpkg_install_TRIPLET}" packages ${ARGN})
    endif ()
    string(CONCAT join ${packages})
    message(STATUS "vcpkg_install() called to install: ${join}")

    execute_process (COMMAND "${AUTO_VCPKG_EXECUTABLE}" "install" ${packages})
endfunction ()
