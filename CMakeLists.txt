cmake_minimum_required(VERSION 3.9)
project(libsparsetensor)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # enable C++17 standard

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()


add_executable(libsparsetensor main.cpp HyperTrie.hpp Trie.cpp Trie.hpp PosCalc.hpp Tensor.hpp MapTensor.cpp MapTensor.hpp)

add_executable(testhypertrie tests/TestHyperTrie.cpp HyperTrie.hpp)

add_executable(testposcalc tests/TestPosCalc.cpp PosCalc.hpp)