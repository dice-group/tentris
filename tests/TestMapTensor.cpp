#include "../tensor/MapTensor.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMapTensor)


    BOOST_AUTO_TEST_CASE(write_read_delete_read) {
        MapTensor<int> tensor{vector<uint64_t>{3, 3, 3}};
        std::cout << tensor << std::endl;

        vector<uint64_t> coord{1, 1, 1};
        int value{4};
        tensor.set(coord, value);
        std::cout << tensor << std::endl;

        int retrieved_value = tensor.get(coord);
        BOOST_CHECK_EQUAL(retrieved_value, value);

        int zero{};
        tensor.set(coord, zero);
        std::cout << tensor << std::endl;

        retrieved_value = tensor.get(coord);
        BOOST_CHECK_EQUAL(retrieved_value, zero);
    }


BOOST_AUTO_TEST_SUITE_END()