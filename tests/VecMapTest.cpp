#include <gtest/gtest.h>
#include "container/VecMap.hpp"

TEST(TestDoubleVector, loadAndRead) {
    using sparsetensor::container::VecMap;
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',};

    // Load data
    VecMap<int, char> map{};
    for (size_t i = 0; i < keys.size(); ++i) {
        map.setItem(keys[i], values[i]);
    }

    // check if data was loaded correctly
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(map.get(keys[i]), values[i]);
    }

    ASSERT_EQ(keys.size(), map.size());
}

TEST(TestDoubleVector, loadDeleteRead) {
    using sparsetensor::container::VecMap;
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',};

    // Load data
    VecMap<int, char> map{};
    for (size_t i = 0; i < keys.size(); ++i) {
        map.setItem(keys[i], values[i]);
    }

    // check if data was loaded correctly
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(map.get(keys[i]), values[i]);
    }
    ASSERT_EQ(keys.size(), map.size());

    map.delItem(6);
    for (size_t i = 0; i < keys.size(); ++i) {
        if (keys[i] != 6) {
            ASSERT_EQ(map.get(keys[i]), values[i]);
        }
    }
    ASSERT_EQ(map.get(keys[6]), std::numeric_limits<char>().max());

    ASSERT_EQ(map.size(), keys.size() - 1);
}

TEST(TestDoubleVector, probeElements) {
    using sparsetensor::container::VecMap;
    // Data
    std::vector<int> keys{0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14};
    std::vector<char> values{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',};

    // Load data
    VecMap<int, char> map{};
    for (size_t i = 0; i < keys.size(); ++i) {
        map.setItem(keys[i], values[i]);
    }
    // std::cout << map.keys[0] << std::endl;
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
