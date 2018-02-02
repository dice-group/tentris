#include "einsum/util/UndirectedGraph.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestUndirectedGraph)


    BOOST_AUTO_TEST_CASE(addEdge) {
        using namespace ::sparsetensor::einsum::util;
        UndirectedGraph<uint8_t> graph{};

        graph.addEdge(1, 1);
        graph.addEdge(2, 3);
        graph.addEdge(3, 4);

        const unordered_set<unordered_set<uint8_t>> &connectedComponents = graph.getConnectedComponents();

        BOOST_CHECK(connectedComponents.count(std::unordered_set<uint8_t> {1}));

        BOOST_CHECK(connectedComponents.count(std::unordered_set<uint8_t> {2, 3, 4}));

        BOOST_CHECK(not connectedComponents.count(std::unordered_set<uint8_t> {1, 2, 3, 4}));

        BOOST_CHECK_EQUAL(size(connectedComponents), 2);
    }

    BOOST_AUTO_TEST_CASE(addFullGraph) {
        using namespace ::sparsetensor::einsum::util;
        UndirectedGraph<uint8_t> graph{};

        graph.addCompleteGraph({1, 2, 3});
        graph.addCompleteGraph({3, 4});
        graph.addCompleteGraph({5, 6, 7});

        const unordered_set<unordered_set<uint8_t>> &connectedComponents = graph.getConnectedComponents();

        BOOST_CHECK(connectedComponents.count(std::unordered_set<uint8_t> {1, 2, 3, 4}));

        BOOST_CHECK(connectedComponents.count(std::unordered_set<uint8_t> {5, 6, 7}));

        BOOST_CHECK(not connectedComponents.count(std::unordered_set<uint8_t> {1, 2, 3}));

        BOOST_CHECK_EQUAL(size(connectedComponents), 2);
    }


BOOST_AUTO_TEST_SUITE_END()