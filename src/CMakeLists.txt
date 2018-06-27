cmake_minimum_required(VERSION 3.9)
project(sparsetensor CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp -D_GLIBCXX_PARALLEL")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

#add_library(libsparsetensor util/ExtendContainerPrint.hpp einsum/util/UndirectedGraph.hpp einsum/Subscript.hpp einsum/Einsum.hpp einsum/operator/Einsum.hpp einsum/operator/CrossProduct.hpp hypertrie/PosCalc.hpp tensor/CrossProductTensor.hpp tensor/Tensor.hpp tensor/MapTensor.hpp tensor/HyperTrieTensor.hpp  einsum/EvalPlan.hpp hypertrie/Join.hpp util/ExtendHash.hpp hypertrie/Types.hpp hypertrie/Diagonal.hpp einsum/Types.hpp util/All.hpp tensor/Types.hpp einsum/ShapeCalc.hpp einsum/operator/Operator.hpp hypertrie/BoolHyperTrie.hpp tensor/BoolHyperTrieTensor.hpp container/VecMap.hpp container/VecSet.hpp container/BinarySearch.hpp)
# add_library(libsparsetensor container/VecMap.hpp container/VecSet.hpp container/BinarySearch.hpp hypertrie/Join.hpp hypertrie/BoolHyperTrie.hpp hypertrie/PosCalc.hpp tensor/Types.hpp hypertrie/Types.hpp util/Sort.hpp einsum/NewSubscript.hpp  einsum/Types.hpp util/cppitertools/itertools.hpp util/cppitertools/enumerate.hpp util/cppitertools/zip.hpp)
add_library(libsparsetensor container/VecMap.hpp container/VecSet.hpp container/BinarySearch.hpp hypertrie/Join.hpp hypertrie/BoolHyperTrie.hpp hypertrie/PosCalc.hpp tensor/Types.hpp hypertrie/Types.hpp util/Sort.hpp einsum/Subscript.hpp einsum/EinsumPlan.hpp einsum/Types.hpp util/Range.hpp util/Enumerate.hpp container/NDMap.hpp einsum/operator/Result.hpp)


add_executable(main Main.cpp)
SET_TARGET_PROPERTIES(libsparsetensor PROPERTIES LINKER_LANGUAGE CXX)
