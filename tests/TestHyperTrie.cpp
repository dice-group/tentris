#include "hypertrie/HyperTrie.hpp"

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

    BOOST_AUTO_TEST_CASE(test_double_write_read0) {
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

    BOOST_AUTO_TEST_CASE(test_mult_write_read1) {
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
        for (unsigned int i = 0; i < keys.size(); ++i) {
            vector<uint64_t> &key = keys[i];
            short &value = values[i];

            trie.set(key, value);
        }

        // validate
        for (unsigned int i = 0; i < keys.size(); ++i) {
            vector<uint64_t> &key = keys[i];
            short &value = values[i];

            optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

            BOOST_CHECK(safed_value__);

            short safed_value = std::get<short>(*safed_value__);

            BOOST_CHECK_EQUAL(safed_value, value);
        }
    }

    BOOST_AUTO_TEST_CASE(test_read_empty_key) {
        vector<uint64_t> key{5, 10, 8};
        short value = 1;

        HyperTrie<short> trie{uint8_t(key.size())};

        trie.set(key, value);

        vector<uint64_t> empty_key{};
        optional<variant<HyperTrie<short> *, short>> this_trie_ = trie.get(empty_key);

        BOOST_CHECK(this_trie_);

        HyperTrie<short> *this_trie = std::get<HyperTrie<short> *>(*this_trie_);
        BOOST_CHECK_EQUAL(this_trie, &trie);
    }

    BOOST_AUTO_TEST_CASE(test_mult_write_read4) {
        // data
        vector<vector<uint64_t>> keys{
                vector<uint64_t>{0, 10, 8, 2},
                vector<uint64_t>{2, 5, 10, 9},
                vector<uint64_t>{1, 0, 10, 5},
                vector<uint64_t>{3, 4, 3, 0},
                vector<uint64_t>{4, 2, 2, 6},
                vector<uint64_t>{2, 3, 3, 2},
                vector<uint64_t>{1, 5, 3, 0},
                vector<uint64_t>{6, 5, 6, 8},
                vector<uint64_t>{1, 9, 7, 3}
        };
        short value = 1;

        uint8_t key_length = 4;

        // init
        HyperTrie<short> trie{key_length};

        // load data
        for (vector<uint64_t> &key :keys) {
            trie.set(key, value);
        }

        // validate
        for (vector<uint64_t> &key :keys) {
            optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

            BOOST_CHECK(safed_value__);

            short safed_value = std::get<short>(*safed_value__);

            BOOST_CHECK_EQUAL(safed_value, value);
        }
    }

    BOOST_AUTO_TEST_CASE(test_multi_level_read) {
        // data
        vector<vector<uint64_t>> keys{
                vector<uint64_t>{0, 10, 8, 2},
                vector<uint64_t>{2, 5, 10, 9},
                vector<uint64_t>{0, 4, 8, 9},
                vector<uint64_t>{1, 0, 10, 5},
                vector<uint64_t>{3, 4, 3, 0},
                vector<uint64_t>{4, 2, 2, 6},
                vector<uint64_t>{2, 3, 3, 2},
                vector<uint64_t>{1, 5, 3, 0},
                vector<uint64_t>{6, 5, 6, 8},
                vector<uint64_t>{1, 9, 7, 3}
        };
        short value = 1;

        uint8_t key_length = 4;

        // init
        HyperTrie<short> trie{key_length};

        // load data
        for (vector<uint64_t> &key :keys) {
            trie.set(key, value);
        }

        vector<uint64_t> subkey_0 = vector<uint64_t>{0};
        optional<variant<HyperTrie<short> *, short>> subtrie_0_ = trie.get(subkey_0);
        BOOST_CHECK(subtrie_0_);

        HyperTrie<short> *subtrie_0 = std::get<HyperTrie<short> *>(*subtrie_0_);

        BOOST_CHECK_EQUAL(subtrie_0->depth, 3);
        BOOST_CHECK_EQUAL(subtrie_0->leafsum, 2);
        BOOST_CHECK_EQUAL(subtrie_0->leafcount, 2);


        vector<uint64_t> subkey_4 = vector<uint64_t>{4};
        optional<variant<HyperTrie<short> *, short>> subtrie_04_ = subtrie_0->get(subkey_4);
        BOOST_CHECK(subtrie_04_);

        HyperTrie<short> *subtrie_04 = std::get<HyperTrie<short> *>(*subtrie_04_);

        BOOST_CHECK_EQUAL(subtrie_04->depth, 2);
        BOOST_CHECK_EQUAL(subtrie_04->leafsum, 1);
        BOOST_CHECK_EQUAL(subtrie_04->leafcount, 1);


        vector<uint64_t> subkey_8 = vector<uint64_t>{8};
        optional<variant<HyperTrie<short> *, short>> subtrie_048_ = subtrie_04->get(subkey_8);
        BOOST_CHECK(subtrie_048_);

        HyperTrie<short> *subtrie_048 = std::get<HyperTrie<short> *>(*subtrie_048_);

        BOOST_CHECK_EQUAL(subtrie_048->depth, 1);
        BOOST_CHECK_EQUAL(subtrie_048->leafsum, 1);
        BOOST_CHECK_EQUAL(subtrie_048->leafcount, 1);


        vector<uint64_t> subkey_9 = vector<uint64_t>{9};
        optional<variant<HyperTrie<short> *, short>> subtrie_0489_ = subtrie_048->get(subkey_9);
        BOOST_CHECK(subtrie_0489_);

        short safed_value = std::get<short>(*subtrie_0489_);

        BOOST_CHECK_EQUAL(safed_value, value);
    }

    BOOST_AUTO_TEST_CASE(sum) {
        // data
        vector<vector<uint64_t>> keys{
                vector<uint64_t>{0, 10, 8, 2},
                vector<uint64_t>{0, 11, 8, 2},
                vector<uint64_t>{1, 10, 8, 2},
                vector<uint64_t>{2, 10, 8, 1},
                vector<uint64_t>{2, 10, 8, 2}
        };

        vector<short> values{
                1,
                1,
                3,
                1,
                1
        };

        uint8_t key_length = 4;

        // init
        HyperTrie<short> trie{key_length};

        // load data
        for (unsigned int i = 0; i < keys.size(); ++i) {
            vector<uint64_t> &key = keys[i];
            short &value = values[i];

            trie.set(key, value);
        }

        BOOST_CHECK_EQUAL(std::accumulate(values.begin(), values.end(), 0), trie.leafsum);

    }


BOOST_AUTO_TEST_SUITE_END()