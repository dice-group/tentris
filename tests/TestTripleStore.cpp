#include <gtest/gtest.h>

#include <filesystem>

#include <tnt/store/TripleStore.hpp>
#include <tnt/store/RDF/NTripleParser.hpp>
#include <tnt/util/FmtHelper.hpp>

namespace {
    namespace fs = std::filesystem;

    using namespace tnt::store;
    using namespace tnt::util::types;
}


TEST(TestTripleStore, load_data) {
    TripleStore store{};
    std::string path = fs::current_path().string();
    path.append("/../../tests/ntriplefiles/ntriples.nt");
    store.loadRDF(path);
    for (auto &&[subject, predicate, object] :NTripleParser{path}) {
        fmt::print("{} {} {}\n", subject, predicate, object);
        ASSERT_TRUE(store.contains({subject->getIdentifier(), predicate->getIdentifier(), object->getIdentifier()}));
    }
}

