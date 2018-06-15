#include <gtest/gtest.h>
#include "einsum/util/UndirectedGraph.hpp"

TEST(TestUndirectedGraph, addEdge) {
    using namespace ::sparsetensor::operations::util;
    UndirectedGraph<uint8_t> graph{};

    graph.addEdge(1, 1);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);

    const unordered_set<unordered_set<uint8_t>> &connectedComponents = graph.getConnectedComponents();

    ASSERT_TRUE(connectedComponents.count(std::unordered_set<uint8_t>{1}));

    ASSERT_TRUE(connectedComponents.count(std::unordered_set<uint8_t>{2, 3, 4}));

    ASSERT_TRUE(not connectedComponents.count(std::unordered_set<uint8_t>{1, 2, 3, 4}));

    ASSERT_EQ(size(connectedComponents), 2);
}

TEST(TestUndirectedGraph, addFullGraph) {
    using namespace ::sparsetensor::operations::util;
    UndirectedGraph<uint8_t> graph{};

    graph.addCompleteGraph({1, 2, 3});
    graph.addCompleteGraph({3, 4});
    graph.addCompleteGraph({5, 6, 7});

    const unordered_set<unordered_set<uint8_t>> &connectedComponents = graph.getConnectedComponents();

    ASSERT_TRUE(connectedComponents.count(std::unordered_set<uint8_t>{1, 2, 3, 4}));

    ASSERT_TRUE(connectedComponents.count(std::unordered_set<uint8_t>{5, 6, 7}));

    ASSERT_TRUE(not connectedComponents.count(std::unordered_set<uint8_t>{1, 2, 3}));

    ASSERT_EQ(size(connectedComponents), 2);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}