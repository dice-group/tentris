
execute_process(COMMAND "bash" "clone_googletest.sh"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_subdirectory(googletest)
include_directories(googlemock/include googletest/include)
link_libraries(
        gmock
        gtest)

#add_executable(TestEvalPlan TestEvalPlan.cpp)
add_executable(TestBoolHyperTrie TestBoolHyperTrie.cpp)
add_executable(TestPosCalc TestPosCalc.cpp)
add_executable(TestSubscript TestSubscript.cpp)
add_executable(TestDoubleVector VecMapTest.cpp)
add_executable(TestSetVector VecSetTest.cpp)
add_executable(TestBinarySearch BinarySearchTest.cpp)
add_executable(TestRDFParser TestRDFParser.cpp)
add_executable(TestSPARQLParser TestSPARQLParser.cpp)
add_executable(TestRDFNode TestRDFNode.cpp)
add_executable(TestTermStore TestTermStore.cpp)
add_executable(TestJoin TestJoin.cpp)
add_executable(TestTripleStore TestTripleStore.cpp)
