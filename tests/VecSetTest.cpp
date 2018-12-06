#include <gtest/gtest.h>
#include <tnt/util/container/VecSet.hpp>

namespace {
    using namespace tnt::util::container;
}


TEST(TestDoubleVector, loadAndRead) {
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};

    // Load data
    VecSet<int> set{};
    for (size_t i = 0; i < keys.size(); ++i) {
        set.add(keys[i]);
    }

    // check if data was loaded correctly
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_TRUE(set.contains(keys[i]));
    }
    // check if other data is not in
    ASSERT_FALSE(set.contains({15}));

    ASSERT_EQ(keys.size(), set.size());
}

TEST(TestDoubleVector, loadDeleteRead) {
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};

    // Load data
    VecSet<int> set{};
    for (size_t i = 0; i < keys.size(); ++i) {
        set.add(keys[i]);
    }

    // check if data was loaded correctly
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_TRUE(set.contains(keys[i]));
    }
    ASSERT_EQ(keys.size(), set.size());

    set.del(6);
    for (size_t i = 0; i < keys.size(); ++i) {
        if (keys[i] != 6) {
            ASSERT_TRUE(set.contains(keys[i]));
        }
    }
    ASSERT_FALSE(set.contains(keys[6]));

    ASSERT_EQ(set.size(), keys.size() - 1);
}

TEST(TestDoubleVector, probeElements) {
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};

    // Load data
    VecSet<int> set{};
    for (size_t i = 0; i < keys.size(); ++i) {
        set.contains(keys[i]);
    }
    // std::cout << map.keys[0] << std::endl;
}


