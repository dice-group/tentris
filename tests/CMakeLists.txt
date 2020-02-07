cmake_minimum_required(VERSION 3.13) # since 3.10 gtest_discover_tests is included
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(GoogleTest)

# add the exectutable for all tests
add_executable(tests Tests.cpp)
target_link_libraries(tests
        ${CONAN_LIBS}
        tentris
        )

set_property(TARGET tests PROPERTY CXX_STANDARD 17)

# detect the tests
gtest_discover_tests(tests)

# copy files for testing to the binary folder
file(COPY query DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY queries DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY dataset DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

