#include <gtest/gtest.h>

#include "TestBoolHyperTrie.cpp"
// #include "TestEinsum.cpp"
#include "TestJoin.cpp"
#include "TestPosCalc.cpp"
#include "TestRDFNode.cpp"
#include "TestRDFParser.cpp"
#include "TestSPARQLParser.cpp"
#include "TestSubscript.cpp"
#include "TestTermStore.cpp"
#include "TestTripleStore.cpp"
#include "TestUndirectedGraph.cpp"
#include "VecMapTest.cpp"
#include "VecSetTest.cpp"


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}