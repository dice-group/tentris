cmake_minimum_required(VERSION 3.9)
project(libsparsetensor)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # enable C++11 standard

add_executable(libsparsetensor main.cpp HyperTrie.cpp HyperTrie.hpp Trie.cpp Trie.hpp PosCalc.hpp)