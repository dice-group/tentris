#include <gtest/gtest.h>

#include "TestRDFNode.cpp"
#include "TestSPARQLParser.cpp"
#include "TestTermStore.cpp"
#include "TestTripleStore.cpp"


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}