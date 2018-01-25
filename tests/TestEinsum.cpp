#include "einsum/Einsum.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestEinsum)

    BOOST_AUTO_TEST_CASE(simple_call) {
        einsum(vector<HyperTrieTensor<int>>{}, vector<raw_subscript>{}, raw_subscript{});
    }

BOOST_AUTO_TEST_SUITE_END()