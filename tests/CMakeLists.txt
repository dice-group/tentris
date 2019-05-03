cmake_minimum_required(VERSION 3.10) # since 3.10 gtest_discover_tests is included

# clone gtest
execute_process(COMMAND "bash" "clone_googletest.sh"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
add_subdirectory(googletest)

# include for gtest_discover_tests
include(GoogleTest)

# add the exectutable for all tests
add_executable(tests Tests.cpp)
target_link_libraries(tests gmock
        gtest
        tentris
        )
target_include_directories(tests PUBLIC googlemock/include googletest/include)
# detect the tests
gtest_discover_tests(tests)

# copy files for testing to the binary folder
file(COPY query DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY queries DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY dataset DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

