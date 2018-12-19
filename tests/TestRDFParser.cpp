#include <gtest/gtest.h>

#include <filesystem>

#include <tnt/store/RDF/NTripleParser.hpp>
#include <tnt/util/FmtHelper.hpp>


namespace {
    using namespace tnt::store::rdf;
    namespace fs = std::filesystem;
}

TEST(TestRDFParser, da) {


    std::string path = fs::current_path().string();
    path.append("/../../tests/ntriplefiles/ntriples.nt");
    path = fs::path{path}.string();
    std::cout << path << std::endl;
    for ([[maybe_unused]] auto i : range(100)) {
        NTripleParser triple_it{path};

        for (auto &&triple : triple_it)
            fmt::print("{}\n", triple);
    }
    std::cout << std::endl;

}

