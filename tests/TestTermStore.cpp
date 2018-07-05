#include <gtest/gtest.h>
#include "store/RDF/TermStore.hpp"

using namespace tnt::store;
using namespace tnt::util::types;

TEST(TestTermStore, double_write) {
    TermStore store{};
    const key_part_t &id_ = store[std::string{"\"xx\""}];
    const key_part_t &id2_ = store[std::string{"\"xx\""}];
    ASSERT_EQ(id_, id2_);
    ASSERT_EQ(store.size(), 1);
}

TEST(TestTermStore, inverse) {
    TermStore store{};
    const key_part_t &id_ = store[std::string{"\"xx\""}];
    store[std::string{"\"xx\""}];
    TermStore::RevTermStore &inv = store.inv();

    const std::unique_ptr<Term> &ptr = inv.at(id_);
    ASSERT_EQ(ptr->getIdentifier(), std::string{"\"xx\""});
    ASSERT_EQ(inv.size(), 1);
    ASSERT_EQ(&inv.inv(), &store);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
