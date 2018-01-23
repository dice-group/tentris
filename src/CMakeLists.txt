cmake_minimum_required(VERSION 3.9)
project(sparsetensor CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)

file(GLOB lib_SRC
        "*.hpp"
        )

add_library(libsparsetensor ${lib_SRC})
SET_TARGET_PROPERTIES(libsparsetensor PROPERTIES LINKER_LANGUAGE CXX)

# add_executable(libsparsetensor main.cpp hypertrie/HyperTrie.hpp hypertrie/PosCalc.hpp tensor/Tensor.hpp tensor/MapTensor.hpp util.hpp tensor/HyperTrieTensor.hpp einsum/Einsum.hpp einsum/Subscript.hpp einsum/operator/Operator.hpp einsum/operator/Einsum.hpp einsum/operator/CrossProduct.hpp einsum/util/UndirectedGraph.hpp)

#add_executable(testhypertrie tests/TestHyperTrie.cpp src/hypertrie/HyperTrie.hpp)
#
#add_executable(testposcalc tests/TestPosCalc.cpp src/hypertrie/PosCalc.hpp)
#
#add_executable(testmaptensor tests/TestMapTensor.cpp src/tensor/Tensor.hpp src/tensor/MapTensor.hpp util.hpp)
#
#add_executable(testhypertrietensor tests/TestHyperTrieTensor.cpp src/tensor/Tensor.hpp src/tensor/HyperTrieTensor.hpp util.hpp)
#
#add_executable(testundirectedgraph src/einsum/util/UndirectedGraph.hpp tests/TestUndirectedGraph.cpp util.hpp)
#
#add_executable(testsubscript src/einsum/Subscript.hpp src/einsum/util/UndirectedGraph.hpp tests/TestSubScript.cpp util.hpp)