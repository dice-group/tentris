#include <gtest/gtest.h>
#include "store/RDF/TermStore.hpp"

using namespace tnt::store;

TEST(TestTermStore, write) {
    TermStore store{};

    const TermStore::key_part_t &id_ = store[std::string{"\"xx\""}];
    const TermStore::key_part_t &id2_ = store[std::string{"\"xx\""}];
    ASSERT_EQ(id_, id2_);
    ASSERT_EQ(store.size(), 1);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
