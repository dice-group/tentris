#include <gtest/gtest.h>

#include <filesystem>

#include <tentris/store/TripleStore.hpp>
#include <tentris/store/RDF/NTripleParser.hpp>
#include <tentris/util/FmtHelper.hpp>

namespace {
    namespace fs = std::filesystem;

    using namespace tentris::store;
    using namespace tentris::util::types;
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

