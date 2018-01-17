cmake_minimum_required(VERSION 3.9)
project(libsparsetensor)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # enable C++17 standard

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()


add_executable(libsparsetensor main.cpp hypertrie/HyperTrie.hpp Trie.cpp Trie.hpp hypertrie/PosCalc.hpp tensor/Tensor.hpp tensor/MapTensor.hpp util.hpp tensor/HyperTrieTensor.hpp einsum/Einsum.hpp einsum/Subscript.hpp einsum/operator/Operator.hpp einsum/operator/Einsum.hpp einsum/operator/CrossProduct.hpp einsum/util/UndirectedGraph.hpp)

add_executable(testhypertrie tests/TestHyperTrie.cpp hypertrie/HyperTrie.hpp)

add_executable(testposcalc tests/TestPosCalc.cpp hypertrie/PosCalc.hpp)

add_executable(testmaptensor tests/TestMapTensor.cpp tensor/Tensor.hpp tensor/MapTensor.hpp util.hpp)

add_executable(testhypertrietensor tests/TestHyperTrieTensor.cpp tensor/Tensor.hpp tensor/HyperTrieTensor.hpp util.hpp)