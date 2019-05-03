#include <gtest/gtest.h>
#include "tentris/util/container/BinarySearch.hpp"
using namespace tentris::util::container;
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
    ASSERT_EQ(insert_pos<int>(sorted, 2, 4), 5);
    // at value 0
    ASSERT_EQ(insert_pos<int>(sorted, 2, -7), 2);
    // at value -7
    ASSERT_EQ(insert_pos<int>(sorted, 0, -7), 1);

    // find a value in range
    ASSERT_EQ(insert_pos<int>(sorted, 2, 10 + 1, 6), 6);
    ASSERT_EQ(insert_pos<int>(sorted, 2, 10 + 1, 9), 9);
    ASSERT_EQ(insert_pos<int>(sorted, 2, 10 + 1, 15), 10);

    // values not in range or not sequence not found
    ASSERT_EQ(insert_pos<int>(sorted, 2, 10 + 1, 18), 11);
    ASSERT_EQ(insert_pos<int>(sorted, 2, 10 + 1, 16), 11);
    ASSERT_EQ(insert_pos<int>(sorted, 2, 10 + 1, 14), 10);
}

TEST(TestBinarySearch, findInRange) {
    // Data
    std::vector<int> sorted{-10, -7, 0, 2, 3, 5, 6, 7, 8, 9, 15, 18, 123, 143, 199, 255};

    // start position at 2 and 4 not in sequence
    ASSERT_EQ(search<int>(sorted, 2, 4), NOT_FOUND);
    // start position at 2, so -7 before start position
    ASSERT_EQ(search<int>(sorted, 2, -7), NOT_FOUND);
    // start position at 0 so -7 in search area.
    ASSERT_EQ(search<int>(sorted, 0, -7), 1);

    // find a value in range
    ASSERT_EQ(search<int>(sorted, 2, 10 + 1, 6), 6);
    ASSERT_EQ(search<int>(sorted, 2, 10 + 1, 9), 9);
    ASSERT_EQ(search<int>(sorted, 2, 10 + 1, 15), 10);

    // values not in range or not sequence not found
    ASSERT_EQ(search<int>(sorted, 2, 10 + 1, 18), NOT_FOUND);
    ASSERT_EQ(search<int>(sorted, 2, 10 + 1, 16), NOT_FOUND);
    ASSERT_EQ(search<int>(sorted, 2, 10 + 1, 14), NOT_FOUND);
}

//todo Have more critical cases tests

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}