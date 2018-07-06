cmake_minimum_required(VERSION 3.9)
project(tnt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp") # add openmp if it is needed

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
# set variable pointing to the antlr tool that supports C++
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.1-complete.jar)
include( ExternalAntlr4Cpp )


# Include the runtime to compile against
add_library(antlr4_interface INTERFACE)
target_include_directories(antlr4_interface INTERFACE ${ANTLR4CPP_INCLUDE_DIRS})
# target_link_libraries(antlr4_interface INTERFACE  ${ANTLR4CPP_LIBS} )
#include_directories( ${ANTLR4CPP_INCLUDE_DIRS} )
link_directories( ${ANTLR4CPP_LIBS} )
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")

# Call macro to add lexer and grammar to your build dependencies.
# NOTE: Here, we define "antlrcpptest" as our project's namespace


# include generated files in project environment
include_directories(grammar)

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
        antlr4-runtime
        antlr4_interface
)

# the main executable target
add_executable(tnt_exec src/Main.cpp)
# add_dependencies(tnt_exec antlr4cpp)

# enable_testing() # enable this to enable testing
if (CMAKE_TESTING_ENABLED)
    add_subdirectory(tests)
endif ()
