
# Find cmake packages
find_package(hypertrie REQUIRED)
find_package(robin_hood REQUIRED)
find_package(dice-hash REQUIRED)
find_package(Boost REQUIRED)

# Define the library
add_library(node-store
        src/dice/node-store/PersistentNodeStorageBackendImpl.cpp
        src/dice/node-store/PersistentNodeStorageBackend.cpp
        src/dice/node-store/MetallBNodeBackend.cpp
        src/dice/node-store/MetallIRIBackend.cpp
        src/dice/node-store/MetallLiteralBackend.cpp
        src/dice/node-store/MetallVariableBackend.cpp
        )
add_library(tentris::node-store ALIAS node-store)

target_link_libraries(node-store PUBLIC
        tentris::rdf-tensor
        )

target_include_directories(node-store PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        )

set_target_properties(node-store PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

include(${PROJECT_SOURCE_DIR}/cmake/install_library.cmake)
install_component(node-store src)
