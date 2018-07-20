#include <gtest/gtest.h>

#include "tnt/store/SPARQL/ParsedSPARQL.hpp"


TEST(TestSPARQLParser, da) {
    using namespace tnt::store::sparql;
    ParsedSPARQL{"PREFIX  dc:  <http://purl.org/dc/elements/1.1/>"
                 "PREFIX  ns:  <http://example.org/ns#>"
                 "SELECT  ?title ?price "
                 "WHERE   { ?x ns:price true ."
                 "?x a 1.123 . "
                 "?v ?p \"cat\".  "
                 "?v ?price \"cat\"@en . "
                 "?title ?p \"abc\"^^<http://example.org/datatype#specialDatatype> . }"};
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
