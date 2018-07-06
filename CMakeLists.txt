cmake_minimum_required(VERSION 3.9)
project(tnt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp") # add openmp if it is needed

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

#include threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#include cppitertools, see https://github.com/ryanhaining/cppitertools
add_library(cppitertools INTERFACE)
target_include_directories(cppitertools INTERFACE
        thirdparty/cppitertools
        )

#make an interface library from the code
add_library(libsparsetensor INTERFACE)
target_include_directories(libsparsetensor INTERFACE
        src/
        )

# set all linkables
link_libraries(
        stdc++fs # for #include <filesystem>
        serd-0 # for parsing ntriples
        Threads::Threads # for use of threads
        cppitertools
        libsparsetensor
)

# the main executable target
add_executable(tnt_exec src/Main.cpp)


enable_testing() # enable this to enable testing
if (CMAKE_TESTING_ENABLED)
    add_subdirectory(tests)
endif ()
