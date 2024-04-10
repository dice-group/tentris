# Find cmake packages
find_package(sparql-parser-base REQUIRED)
find_package(robin_hood REQUIRED)
find_package(dice-hash REQUIRED)

# Define the library
add_library(sparql2tensor
        src/dice/sparql2tensor/parser/exception/SPARQLErrorListener.cpp
        src/dice/sparql2tensor/parser/visitors/PrologueVisitor.cpp
        src/dice/sparql2tensor/parser/visitors/SelectAskQueryVisitor.cpp
        src/dice/sparql2tensor/SPARQLQuery.cpp
        src/dice/sparql2tensor/UPDATEQuery.cpp
        )

add_library(tentris::sparql2tensor ALIAS sparql2tensor)

target_include_directories(sparql2tensor PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        PRIVATE
        private-include
        )

target_link_libraries(sparql2tensor PUBLIC
        tentris::rdf-tensor
        robin_hood::robin_hood
        PRIVATE
        sparql-parser-base::sparql-parser-base
        )


set_target_properties(sparql2tensor PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

include(${PROJECT_SOURCE_DIR}/cmake/install_library.cmake)
install_component(sparql2tensor src)
