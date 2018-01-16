#include "../HyperTrie.hpp"

#define BOOST_TEST_MODULE LibSparseTensorTest

#include <boost/test/included/unit_test.hpp>
#include <boost/range/combine.hpp>

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
        // data
        vector<uint64_t> key{4, 10, 8};
        short value1 = 3;
        short value2 = 1;

        // init
        HyperTrie<short> trie{uint8_t(key.size())};

        // load data
        trie.set(key, value1);
        trie.set(key, value2);

        // validate
        optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

        BOOST_CHECK(safed_value__);

        short safed_value = std::get<short>(*safed_value__);
        BOOST_CHECK_EQUAL(safed_value, value2);
    }

    BOOST_AUTO_TEST_CASE(test_mult_write_read3) {
        // data
        vector<vector<uint64_t>> keys{
                vector<uint64_t>{0, 10, 8},
                vector<uint64_t>{0, 10, 9}
        };
        vector<short> values{
                1,
                1
        };
        uint8_t key_length = 3;

        // init
        HyperTrie<short> trie{key_length};

        // load data
        for (int i = 0; i < keys.size(); ++i) {
            vector<uint64_t> &key = keys[i];
            short &value = values[i];

            trie.set(key, value);
        }

        // validate
        for (int i = 0; i < keys.size(); ++i) {
            vector<uint64_t> &key = keys[i];
            short &value = values[i];

            optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

            BOOST_CHECK(safed_value__);

            short safed_value = std::get<short>(*safed_value__);

            BOOST_CHECK_EQUAL(safed_value, value);
        }
    }


BOOST_AUTO_TEST_SUITE_END()