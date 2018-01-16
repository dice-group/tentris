#include "../HyperTrie.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestHyperTrie)

    BOOST_AUTO_TEST_CASE(test_single_write_read) {
        vector<uint64_t> key{5, 10, 8};
        short value = 1;

        HyperTrie<short> trie{uint8_t(key.size())};

        trie.set(key, value);

        optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

        BOOST_CHECK(safed_value__);

        short safed_value = std::get<short>(*safed_value__);
        BOOST_CHECK_EQUAL(safed_value, value);
    }

    BOOST_AUTO_TEST_CASE(test_double_write_read) {
        vector<uint64_t> key{4, 10, 8};
        short value1 = 3;
        short value2 = 1;

        HyperTrie<short> trie{uint8_t(key.size())};

        trie.set(key, value1);

        trie.set(key, value2);

        optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

        BOOST_CHECK(safed_value__);

        short safed_value = std::get<short>(*safed_value__);
        BOOST_CHECK_EQUAL(safed_value, value2);
    }


BOOST_AUTO_TEST_SUITE_END()