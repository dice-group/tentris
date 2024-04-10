# Find cmake packages

# Define the library
add_library(tentris INTERFACE)
add_library(tentris::tentris ALIAS tentris)
target_link_libraries(tentris INTERFACE
        tentris::endpoint
        )

target_include_directories(tentris INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        )

include(${PROJECT_SOURCE_DIR}/cmake/install_library.cmake)
install_interface_component(tentris src)