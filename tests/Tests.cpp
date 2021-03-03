#include <gtest/gtest.h>

#include "TestGraphqlExecutionPackage.cpp"
#include "TestGraphqlResponse.cpp"
//#include "TestSPARQLParser.cpp"
//#include "TestTermStore.cpp"


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}