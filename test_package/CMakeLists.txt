cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(tentris REQUIRED node-store)

add_executable(example example.cpp)

target_link_libraries(example PRIVATE
        tentris::node-store
        )

set_target_properties(
        example PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
