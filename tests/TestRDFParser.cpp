#include <gtest/gtest.h>

#include <filesystem>

#include <tnt/store/RDF/NTripleParser.hpp>

TEST(TestRDFParser, da) {
    namespace fs = std::filesystem;

    std::string path = fs::current_path().string();
    path.append("/../../tests/ntriplefiles/ntriples.nt");
    path = fs::path{path}.string();
    std::cout << path << std::endl;
    for ([[maybe_unused]] auto i : range(100)) {
        tnt::store::NTripleParser triple_it{path};

        for (auto &&triple : triple_it)
            std::cout << triple << "\n";
    }
    std::cout << std::endl;

}

