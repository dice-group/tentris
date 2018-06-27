#include <gtest/gtest.h>

#include <numeric>
#include "hypertrie/HyperTrie.hpp"


TEST(TestHyperTrie, test_single_write_read
) {
using namespace sparsetensor::hypertrie;
Key_t key{5, 10, 8};
int value = 1;

HyperTrie<int> trie{uint8_t(key.size())};

trie.
set(key, value
);

std::variant<HyperTrie < int> *, int> &
safed_value_ = trie.get(key);

short safed_value = std::get<int>(safed_value_);
ASSERT_EQ(safed_value, value
);
}

TEST(TestHyperTrie, test_double_write_read0
) {
using namespace sparsetensor::hypertrie;
// data
Key_t key{4, 10, 8};
int value1 = 3;
int value2 = 1;

// init
HyperTrie<int> trie{uint8_t(key.size())};

// load data
trie.
set(key, value1
);
trie.
set(key, value2
);

// validate
variant<HyperTrie < int> *, int>
safed_value_ = trie.get(key);

short safed_value = std::get<int>(safed_value_);
ASSERT_EQ(safed_value, value2
);
}

TEST(TestHyperTrie, test_mult_write_read1
) {
using namespace sparsetensor::hypertrie;
// data
vector <Key_t> keys{
        {0, 10, 8},
        {0, 10, 9}
};
vector<short> values{
        1,
        1
};
uint8_t key_length = 3;

// init
HyperTrie<short> trie{key_length};

// load data
for (
unsigned int i = 0;
i<keys.

size();

++i) {
Key_t &key = keys[i];
short &value = values[i];

trie.
set(key, value
);
}

// validate
for (
unsigned int i = 0;
i<keys.

size();

++i) {
Key_t &key = keys[i];
short &value = values[i];

variant<HyperTrie < short> *, short>
safed_value_ = trie.get(key);

short safed_value = ::std::get<short>(safed_value_);

ASSERT_EQ(safed_value, value
);
}
}

TEST(TestHyperTrie, DISABLED_test_read_empty_key
) {
using namespace sparsetensor::hypertrie;
Key_t key{5, 10, 8};
short value = 1;

HyperTrie<short> trie{uint8_t(key.size())};

trie.
set(key, value
);

Key_t empty_key{};

variant<HyperTrie < short> *, short>
this_trie_ = trie.get(empty_key);

HyperTrie<short> *this_trie = std::get < HyperTrie < short > * > (this_trie_);

ASSERT_EQ(this_trie, &trie
);
}

TEST(TestHyperTrie, test_mult_write_read4
) {
using namespace sparsetensor::hypertrie;
// data
vector <Key_t> keys{
        {0, 10, 8,  2},
        {2, 5,  10, 9},
        {1, 0,  10, 5},
        {3, 4,  3,  0},
        {4, 2,  2,  6},
        {2, 3,  3,  2},
        {1, 5,  3,  0},
        {6, 5,  6,  8},
        {1, 9,  7,  3}
};
short value = 1;

uint8_t key_length = 4;

// init
HyperTrie<short> trie{key_length};

// load data
for (
Key_t &key
:keys) {
trie.
set(key, value
);
}

// validate
for (
Key_t &key
:keys) {
variant<HyperTrie < short> *, short>
safed_value_ = trie.get(key);

short safed_value = std::get<short>(safed_value_);

ASSERT_EQ(safed_value, value
);
}
}

TEST(TestHyperTrie, test_multi_level_read
) {
using namespace sparsetensor::hypertrie;
// data
vector <Key_t> keys{
        {0, 10, 8,  2},
        {2, 5,  10, 9},
        {0, 4,  8,  9},
        {1, 0,  10, 5},
        {3, 4,  3,  0},
        {4, 2,  2,  6},
        {2, 3,  3,  2},
        {1, 5,  3,  0},
        {6, 5,  6,  8},
        {1, 9,  7,  3}
};
short value = 1;

uint8_t key_length = 4;

// init
HyperTrie<short> trie{key_length};

// load data
for (
Key_t &key
:keys) {
trie.
set(key, value
);
}

Key_t subkey_0 = Key_t{0};
variant<HyperTrie < short> *, short>
subtrie_0_ = trie.get(subkey_0);

HyperTrie<short> *subtrie_0 = std::get < HyperTrie < short > * > (subtrie_0_);

ASSERT_EQ(subtrie_0
->depth, 3);
ASSERT_EQ(subtrie_0
->leafsum, 2);
ASSERT_EQ(subtrie_0
->leafcount, 2);


Key_t subkey_4 = Key_t{4};
variant<HyperTrie < short> *, short>
subtrie_04_ = subtrie_0->get(subkey_4);

HyperTrie<short> *subtrie_04 = std::get < HyperTrie < short > * > (subtrie_04_);

ASSERT_EQ(subtrie_04
->depth, 2);
ASSERT_EQ(subtrie_04
->leafsum, 1);
ASSERT_EQ(subtrie_04
->leafcount, 1);


Key_t subkey_8 = Key_t{8};
variant<HyperTrie < short> *, short>
subtrie_048_ = subtrie_04->get(subkey_8);

HyperTrie<short> *subtrie_048 = std::get < HyperTrie < short > * > (subtrie_048_);

ASSERT_EQ(subtrie_048
->depth, 1);
ASSERT_EQ(subtrie_048
->leafsum, 1);
ASSERT_EQ(subtrie_048
->leafcount, 1);


Key_t subkey_9 = Key_t{9};
variant<HyperTrie < short> *, short>
subtrie_0489_ = subtrie_048->get(subkey_9);

short safed_value = std::get<short>(subtrie_0489_);

ASSERT_EQ(safed_value, value
);
}

TEST(TestHyperTrie, sum
) {
using namespace sparsetensor::hypertrie;
// data
vector <Key_t> keys{
        {0, 10, 8, 2},
        {0, 11, 8, 2},
        {1, 10, 8, 2},
        {2, 10, 8, 1},
        {2, 10, 8, 2}
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
for (
unsigned int i = 0;
i<keys.

size();

++i) {
Key_t &key = keys[i];
short &value = values[i];

trie.
set(key, value
);
}

ASSERT_EQ(std::accumulate(values.begin(), values.end(), 0), trie
.leafsum);

}

TEST(TestHyperTrie, calc_card
) {
using namespace sparsetensor::hypertrie;
// data
vector <Key_t> keys{
        {0, 1},
        {0, 2},
        {0, 3},
};

int value = 1;

uint8_t key_length = 2;

// init
HyperTrie<int> trie{key_length};

// load data
for (
auto &&key
: keys) {
trie.
set(key, value
);
}

ASSERT_EQ(trie
.getCard(0), 1);

ASSERT_EQ(trie
.getCard(1), 3);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}