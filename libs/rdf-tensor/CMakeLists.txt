# Find cmake packages
find_package(Boost REQUIRED)
find_package(hypertrie REQUIRED)
find_package(Metall REQUIRED)
find_package(rdf4cpp REQUIRED)
find_package(dice-hash REQUIRED)

# Define the library
add_library(rdf-tensor INTERFACE)
add_library(tentris::rdf-tensor ALIAS rdf-tensor)
target_link_libraries(rdf-tensor INTERFACE
        rdf4cpp::rdf4cpp
        hypertrie::hypertrie
        Metall::Metall
        Boost::headers
        dice-hash::dice-hash
        )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/dice/tentris/tentris_version.hpp)
target_include_directories(rdf-tensor INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        )

include(${PROJECT_SOURCE_DIR}/cmake/install_library.cmake)
install_interface_component(rdf-tensor src)