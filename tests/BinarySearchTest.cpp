#include <gtest/gtest.h>
#include "tnt/util/container/BinarySearch.hpp"
using namespace tnt::util::container;
TEST(TestBinarySearch, insertInRightOrder) {

    // Data
    std::vector<int> keys{3, 8, 7, 2, 5, 9, 6};

    // Load data
    std::vector<int> sorted{};
    for (size_t i = 0; i < keys.size(); ++i) {
        size_t pos = insert_pos<int>(sorted, keys[i]);
        sorted.insert(sorted.begin() + pos, keys[i]);
    }

    std::vector<int> correct_sorted{2, 3, 5, 6, 7, 8, 9};
    for (size_t i = 0; i < keys.size(); ++i) {
        ASSERT_EQ(sorted[i], correct_sorted[i]);
    }
}

TEST(TestBinarySearch, findInsertPos) {
    // Data
    std::vector<int> sorted{-10, -7, 0, 2, 3, 5, 6, 7, 8, 9, 15, 18, 123, 143, 199, 255};

    // at value 5
    ASSERT_EQ(insert_pos<int>(sorted, 4, 2), 5);
    // at value 0
    ASSERT_EQ(insert_pos<int>(sorted, -7, 2), 2);
    // at value -7
    ASSERT_EQ(insert_pos<int>(sorted, -7, 0), 1);

    // find a value in range
    ASSERT_EQ(insert_pos<int>(sorted, 6, 2, 10 + 1), 6);
    ASSERT_EQ(insert_pos<int>(sorted, 9, 2, 10 + 1), 9);
    ASSERT_EQ(insert_pos<int>(sorted, 15, 2, 10 + 1), 10);

    // values not in range or not sequence not found
    ASSERT_EQ(insert_pos<int>(sorted, 18, 2, 10 + 1), 11);
    ASSERT_EQ(insert_pos<int>(sorted, 16, 2, 10 + 1), 11);
    ASSERT_EQ(insert_pos<int>(sorted, 14, 2, 10 + 1), 10);
}

TEST(TestBinarySearch, findInRange) {
    // Data
    std::vector<int> sorted{-10, -7, 0, 2, 3, 5, 6, 7, 8, 9, 15, 18, 123, 143, 199, 255};

    // start position at 2 and 4 not in sequence
    ASSERT_EQ(search<int>(sorted, 4, 2), NOT_FOUND);
    // start position at 2, so -7 before start position
    ASSERT_EQ(search<int>(sorted, -7, 2), NOT_FOUND);
    // start position at 0 so -7 in search area.
    ASSERT_EQ(search<int>(sorted, -7, 0), 1);

    // find a value in range
    ASSERT_EQ(search<int>(sorted, 6, 2, 10 + 1), 6);
    ASSERT_EQ(search<int>(sorted, 9, 2, 10 + 1), 9);
    ASSERT_EQ(search<int>(sorted, 15, 2, 10 + 1), 10);

    // values not in range or not sequence not found
    ASSERT_EQ(search<int>(sorted, 18, 2, 10 + 1), NOT_FOUND);
    ASSERT_EQ(search<int>(sorted, 16, 2, 10 + 1), NOT_FOUND);
    ASSERT_EQ(search<int>(sorted, 14, 2, 10 + 1), NOT_FOUND);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}