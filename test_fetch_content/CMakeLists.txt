cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

# find out the current tag of the tentris-private repos in the folder above
execute_process(COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../
        OUTPUT_VARIABLE parent_git_tag
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
# fetch the tentris-private repo from the local copy
include(FetchContent)
FetchContent_Declare(
        tentris
        GIT_REPOSITORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
        GIT_TAG ${parent_git_tag}
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tentris)
add_executable(example example.cpp)
# With FetchContent_Declare we don't need find_package
target_link_libraries(example PRIVATE tentris::node-store)
