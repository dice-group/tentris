cmake_minimum_required(VERSION 3.18)
project(tentris
        VERSION 1.3.1
        DESCRIPTION "tensor-based triplestore")

include(cmake/boilerplate_init.cmake)
boilerplate_init()

option(CONAN_CMAKE "If this should use conan cmake to fetch dependencies" On)
if (IS_TOP_LEVEL AND CONAN_CMAKE)
    include(cmake/conan_cmake.cmake)
    install_packages_via_conan("${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py" "")
endif ()

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    endif ()
endif ()



add_subdirectory(libs)
