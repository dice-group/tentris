include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.8.1
        GIT_SHALLOW    TRUE
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest ALIAS gtest)
add_library(GTest::Main ALIAS gtest_main)

include(GoogleTest)
# add the executable for all tests
add_executable(tests Tests.cpp)
target_link_libraries(tests
        GTest::GTest
        GTest::Main
        tentris
        )
add_executable(tests_graphql TestGraphQL.cpp)
target_link_libraries(tests_graphql
        GTest::GTest
        GTest::Main
        tentris
        )

set_property(TARGET tests PROPERTY CXX_STANDARD 20)
set_property(TARGET tests_graphql PROPERTY CXX_STANDARD 20)

# detect the tests
gtest_discover_tests(tests)
gtest_discover_tests(tests_graphql)

# copy files for testing to the binary folder
file(COPY queries DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

