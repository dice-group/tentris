#include <gtest/gtest.h>

#include <tentris/util/UndirectedGraph.hpp>

TEST(TestUndirectedGraph, addEdge) {
    using namespace ::tentris::util;
    UndirectedGraph<uint8_t> graph{};

    graph.addEdge(1, 1);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);

    const std::vector<std::set<uint8_t>> &connComp = graph.getConnectedComponents();

    ASSERT_EQ(std::count(connComp.begin(), connComp.end(), std::set<uint8_t>{1}), 1);

    ASSERT_EQ(std::count(connComp.begin(), connComp.end(), std::set<uint8_t>{2, 3, 4}), 1);

    ASSERT_EQ(std::count(connComp.begin(), connComp.end(), std::set<uint8_t>{1, 2, 3, 4}), 0);

    ASSERT_EQ(size(connComp), 2);
}

TEST(TestUndirectedGraph, addCompleteGraph) {
    using namespace ::tentris::util;
    UndirectedGraph<uint8_t> graph{};

    graph.addCompleteGraph({1, 2, 3});
    graph.addCompleteGraph({3, 4});
    graph.addCompleteGraph({5, 6, 7});

    const std::vector<std::set<uint8_t>> &connComp = graph.getConnectedComponents();

    ASSERT_EQ(std::count(connComp.begin(), connComp.end(), std::set<uint8_t>{1, 2, 3, 4}), 1);

    ASSERT_EQ(std::count(connComp.begin(), connComp.end(), std::set<uint8_t>{5, 6, 7}), 1);

    ASSERT_EQ(std::count(connComp.begin(), connComp.end(), std::set<uint8_t>{1, 2, 3}), 0);

    ASSERT_EQ(size(connComp), 2);
}
