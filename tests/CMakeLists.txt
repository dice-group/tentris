cmake_minimum_required(VERSION 3.13) # since 3.10 gtest_discover_tests is included
set(CMAKE_CXX_STANDARD 20)


include(GoogleTest)

# add the exectutable for all tests
add_executable(tests Tests.cpp)
target_link_libraries(tests
        ${CONAN_LIBS}
        tentris
        )

set_property(TARGET tests PROPERTY CXX_STANDARD 20)

# detect the tests
gtest_discover_tests(tests)

# copy files for testing to the binary folder
file(COPY queries DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

