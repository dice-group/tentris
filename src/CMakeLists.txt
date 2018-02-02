cmake_minimum_required(VERSION 3.9)
project(sparsetensor CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(libsparsetensor util/ExtendContainerPrint.hpp einsum/util/UndirectedGraph.hpp einsum/Subscript.hpp einsum/Einsum.hpp einsum/operator/Einsum.hpp einsum/operator/CrossProduct.hpp einsum/operator/Operator.hpp hypertrie/HyperTrie.hpp hypertrie/PosCalc.hpp tensor/Tensor.hpp tensor/MapTensor.hpp tensor/HyperTrieTensor.hpp tensor/CrossProductTensor.hpp einsum/EvalPlan.hpp hypertrie/Join.hpp util/ExtendHash.hpp hypertrie/Types.hpp einsum/Types.hpp util/All.hpp)
add_executable(rundiag Main.cpp)
SET_TARGET_PROPERTIES(libsparsetensor PROPERTIES LINKER_LANGUAGE CXX)