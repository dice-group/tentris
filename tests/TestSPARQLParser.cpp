#include <gtest/gtest.h>

#include "tnt/store/SPARQL/SPARQLParser.hpp"


TEST(TestSPARQLParser, da) {
    using namespace tnt::store::sparql;
    SPARQLParser{"SELECT ?description ?date ?amount\n"
                 "WHERE\n"
                 "{?meal e:description ?description ;\n"
                 "        e:date ?date ;\n"
                 "        e:amount ?amount ,"
                 "?date }"}.getSubscript();


}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
