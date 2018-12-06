#include <gtest/gtest.h>

#include <filesystem>

#include <tnt/store/TripleStore.hpp>
#include <tnt/store/RDF/NTripleParser.hpp>

namespace {
    namespace fs = std::filesystem;

    using namespace tnt::store;
    using namespace tnt::util::types;
}


TEST(TestTripleStore, double_write) {
    TripleStore store{};
    std::string path = fs::current_path().string();
    path.append("/../../tests/ntriplefiles/ntriples.nt");
    store.loadRDF(path);
    for (auto &&[subject, predicate, object] :NTripleParser{path}) {
        ASSERT_TRUE(store.contains({subject->getIdentifier(), predicate->getIdentifier(), object->getIdentifier()}));
    }

}

