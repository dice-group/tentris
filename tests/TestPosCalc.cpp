#include "../hypertrie/PosCalc.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestPosCalc)


    BOOST_AUTO_TEST_CASE(every_different_instance_must_exist_only_once) {
        vector<bool> vec1 = vector<bool>(3);

        PosCalc *inst000 = PosCalc::getInstance(vec1);
        PosCalc *inst000x = PosCalc::getInstance(vec1);

        BOOST_CHECK_EQUAL(inst000, inst000x);

        PosCalc *inst010 = inst000x->use(1);

        vec1[1] = true;
        PosCalc *inst010x = PosCalc::getInstance(vec1);

        BOOST_CHECK_EQUAL(inst010, inst010x);
    }

    BOOST_AUTO_TEST_CASE(check_values) {
        vector<bool> vec1 = vector<bool>(3);

        PosCalc *inst000 = PosCalc::getInstance(vec1);
        PosCalc *inst100 = inst000->use(0);
        BOOST_CHECK_EQUAL(inst100->key_to_subkey_pos(1), 0);
        BOOST_CHECK_EQUAL(inst100->key_to_subkey_pos(2), 1);

        BOOST_CHECK_EQUAL(inst100->subkey_to_key_pos(0), 1);
        BOOST_CHECK_EQUAL(inst100->subkey_to_key_pos(1), 2);

        PosCalc *inst010 = inst000->use(1);
        BOOST_CHECK_EQUAL(inst010->key_to_subkey_pos(0), 0);
        BOOST_CHECK_EQUAL(inst010->key_to_subkey_pos(2), 1);

        BOOST_CHECK_EQUAL(inst010->subkey_to_key_pos(0), 0);
        BOOST_CHECK_EQUAL(inst010->subkey_to_key_pos(1), 2);
    }


BOOST_AUTO_TEST_SUITE_END()