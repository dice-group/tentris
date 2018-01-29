find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
        )

link_libraries(libsparsetensor
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_definitions(-DBOOST_TEST_DYN_LINK)
add_executable(TestUndirectedGraph TestUndirectedGraph.cpp)
add_executable(TestHyperTrieTensor TestHyperTrieTensor.cpp)
add_executable(TestHyperTrie TestHyperTrie.cpp)
add_executable(TestMapTensor TestMapTensor.cpp)
add_executable(TestSubscript TestSubscript.cpp)
add_executable(TestPosCalc TestPosCalc.cpp)
add_executable(TestEinsum TestEinsum.cpp)
