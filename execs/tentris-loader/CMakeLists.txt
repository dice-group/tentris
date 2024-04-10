add_executable(tentris_loader
        src/dice/tentris-loader/TentrisLoader.cpp
        )

find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)
find_package(cxxopts REQUIRED)
find_package(Metall REQUIRED)

target_link_libraries(tentris_loader PRIVATE
        Threads::Threads
        tentris::triple-store
        tentris::node-store
        spdlog::spdlog
        cxxopts::cxxopts
        Metall::Metall
        )

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_target_properties(tentris_loader PROPERTIES LINK_FLAGS_RELEASE -s)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result LANGUAGES CXX) # fatal error if IPO is not supported
    set_property(TARGET tentris_loader PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif ()
