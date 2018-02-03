#include <gtest/gtest.h>

#include <numeric>
#include "hypertrie/HyperTrie.hpp"


TEST(TestHyperTrie, test_single_write_read) {
    using namespace sparsetensor::hypertrie;
    vector<uint64_t> key{5, 10, 8};
    short value = 1;

    HyperTrie<short> trie{uint8_t(key.size())};

    trie.set(key, value);

    optional<variant<HyperTrie<short> *, short>> safed_value__ = trie.get(key);

    ASSERT_TRUE(safed_value__);

    short safed_value = std::get<short>(*safed_value__);
    ASSERT_EQ(safed_value, value);
}

TEST(TestHyperTrie, test_double_write_read0) {
    using namespace sparsetensor::hypertrie;
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

    ASSERT_TRUE(bool(safed_value__));

    short safed_value = std::get<short>(*safed_value__);
    ASSERT_EQ(safed_value, value2);
}

TEST(TestHyperTrie, test_mult_write_read1) {
    using namespace sparsetensor::hypertrie;
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

        ASSERT_TRUE(safed_value__);

        short safed_value = ::std::get<short>(*safed_value__);

        ASSERT_EQ(safed_value, value);
    }
}

TEST(TestHyperTrie, test_read_empty_key) {
    using namespace sparsetensor::hypertrie;
    vector<uint64_t> key{5, 10, 8};
    short value = 1;

    HyperTrie<short> trie{uint8_t(key.size())};

    trie.set(key, value);

    vector<uint64_t> empty_key{};
    optional<variant<HyperTrie<short> *, short>> this_trie_ = trie.get(empty_key);

    ASSERT_TRUE(this_trie_);

    HyperTrie<short> *this_trie = std::get<HyperTrie<short> *>(*this_trie_);
    ASSERT_EQ(this_trie, &trie);
}

TEST(TestHyperTrie, test_mult_write_read4) {
    using namespace sparsetensor::hypertrie;
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

        ASSERT_TRUE(safed_value__);

        short safed_value = std::get<short>(*safed_value__);

        ASSERT_EQ(safed_value, value);
    }
}

TEST(TestHyperTrie, test_multi_level_read) {
    using namespace sparsetensor::hypertrie;
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
    ASSERT_TRUE(subtrie_0_);

    HyperTrie<short> *subtrie_0 = std::get<HyperTrie<short> *>(*subtrie_0_);

    ASSERT_EQ(subtrie_0->depth, 3);
    ASSERT_EQ(subtrie_0->leafsum, 2);
    ASSERT_EQ(subtrie_0->leafcount, 2);


    vector<uint64_t> subkey_4 = vector<uint64_t>{4};
    optional<variant<HyperTrie<short> *, short>> subtrie_04_ = subtrie_0->get(subkey_4);
    ASSERT_TRUE(subtrie_04_);

    HyperTrie<short> *subtrie_04 = std::get<HyperTrie<short> *>(*subtrie_04_);

    ASSERT_EQ(subtrie_04->depth, 2);
    ASSERT_EQ(subtrie_04->leafsum, 1);
    ASSERT_EQ(subtrie_04->leafcount, 1);


    vector<uint64_t> subkey_8 = vector<uint64_t>{8};
    optional<variant<HyperTrie<short> *, short>> subtrie_048_ = subtrie_04->get(subkey_8);
    ASSERT_TRUE(subtrie_048_);

    HyperTrie<short> *subtrie_048 = std::get<HyperTrie<short> *>(*subtrie_048_);

    ASSERT_EQ(subtrie_048->depth, 1);
    ASSERT_EQ(subtrie_048->leafsum, 1);
    ASSERT_EQ(subtrie_048->leafcount, 1);


    vector<uint64_t> subkey_9 = vector<uint64_t>{9};
    optional<variant<HyperTrie<short> *, short>> subtrie_0489_ = subtrie_048->get(subkey_9);
    ASSERT_TRUE(subtrie_0489_);

    short safed_value = std::get<short>(*subtrie_0489_);

    ASSERT_EQ(safed_value, value);
}

TEST(TestHyperTrie, sum) {
    using namespace sparsetensor::hypertrie;
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

    ASSERT_EQ(std::accumulate(values.begin(), values.end(), 0), trie.leafsum);

}

TEST(TestHyperTrie, all_slices) {
    using namespace sparsetensor::hypertrie;
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

    // get identity
    vector<uint64_t> key = vector<uint64_t>{};
    const optional<variant<HyperTrie<short> *, short>> &safed_value__ = trie.get(key);

    ASSERT_TRUE(safed_value__);

    HyperTrie<short> *safed_value = std::get<HyperTrie<short> *>(*safed_value__);

    ASSERT_EQ(safed_value, &trie);

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}