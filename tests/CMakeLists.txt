set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp -D_GLIBCXX_PARALLEL")

set (CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

find_package(GTest REQUIRED)
include_directories(
        ${TEST_SOURCE_DIR}/src
        ${GTEST_INCLUDE_DIRS})

link_libraries(
       #libsparsetensor
        ${GTEST_LIBRARIES}
        pthread
)

#add_executable(TestCrossProductTensor TestCrossProductTensor.cpp)
#add_executable(TestDiagonal TestDiagonal.cpp)
#add_executable(TestEinsum TestEinsum.cpp)
#add_executable(TestEvalPlan TestEvalPlan.cpp)
#add_executable(TestHyperTrie TestHyperTrie.cpp)
add_executable(TestBoolHyperTrie TestBoolHyperTrie.cpp)
#add_executable(TestHyperTrieTensor TestHyperTrieTensor.cpp)
#add_executable(TestJoin TestJoin.cpp)
add_executable(TestNewJoin TestNewJoin.cpp)
#add_executable(TestMapTensor TestMapTensor.cpp)
#add_executable(TestPosCalc TestPosCalc.cpp)
#add_executable(TestSubscript TestSubscript.cpp)
#add_executable(TestUndirectedGraph TestUndirectedGraph.cpp)
add_executable(TestDoubleVector VecMapTest.cpp )
add_executable(TestBinarySearch BinarySearchTest.cpp )
