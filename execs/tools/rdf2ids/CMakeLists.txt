find_package(Threads REQUIRED)
find_package(dice-sparse-map REQUIRED)
find_package(robin_hood REQUIRED)
find_package(dice-hash REQUIRED)
find_package(spdlog REQUIRED)
find_package(cxxopts REQUIRED)
find_package(vincentlaucsb-csv-parser REQUIRED)
find_package(rdf4cpp REQUIRED)

add_executable(rdf2ids
        src/dice/tools/rdf2ids/RDF2IDs.cpp
        )

target_link_libraries(rdf2ids PRIVATE
        Threads::Threads
        dice-hash::dice-hash
        rdf4cpp::rdf4cpp
        dice-sparse-map::dice-sparse-map
        robin_hood::robin_hood
        spdlog::spdlog
        cxxopts::cxxopts
        vincentlaucsb-csv-parser::vincentlaucsb-csv-parser
        tentris::rdf-tensor
        )

target_include_directories(rdf2ids PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_target_properties(rdf2ids PROPERTIES LINK_FLAGS_RELEASE -s)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result LANGUAGES CXX) # fatal error if IPO is not supported
    set_property(TARGET rdf2ids PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif ()
