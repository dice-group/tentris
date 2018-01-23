#include "../einsum/Subscript.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestSubscript)

    BOOST_AUTO_TEST_CASE(test_me) {
        vector<vector<Subscript::label_t >> raw_op_sc{
                {3, 2, 1},
                {1, 2, 3},
                {0}
        };

        vector<Subscript::label_t> raw_res_sc{
                {3, 0}
        };
        Subscript sc{raw_op_sc, raw_res_sc};


    }


BOOST_AUTO_TEST_SUITE_END()