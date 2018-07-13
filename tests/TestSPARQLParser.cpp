#include <gtest/gtest.h>

#include "tnt/store/SPARQL/SPARQLParser.hpp"


TEST(TestSPARQLParser, da) {
    using namespace tnt::store::sparql;
    SPARQLParser{"PREFIX  dc:  <http://purl.org/dc/elements/1.1/>"
                 "PREFIX  ns:  <http://example.org/ns#>"
                 "SELECT  ?title ?price "
                 "WHERE   { ?x ns:price ?price ."
                 "FILTER (?price < 30.5)"
                 "?x a ?title . }"}.getSubscript();
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
