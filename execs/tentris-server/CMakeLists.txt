add_executable(tentris_server
        src/dice/tentris-server/TentrisServer.cpp
        )

find_package(Threads REQUIRED)
find_package(Taskflow REQUIRED)
find_package(spdlog REQUIRED)
find_package(cxxopts REQUIRED)
find_package(restinio REQUIRED)
find_package(Metall REQUIRED)

target_link_libraries(tentris_server PRIVATE
        Threads::Threads
        tentris::tentris
        restinio::restinio
        Taskflow::Taskflow
        spdlog::spdlog
        cxxopts::cxxopts
        Metall::Metall
        )

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_target_properties(tentris_server PROPERTIES LINK_FLAGS_RELEASE -s)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result LANGUAGES CXX) # fatal error if IPO is not supported
    set_property(TARGET tentris_server PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif ()
