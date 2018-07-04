#include <gtest/gtest.h>

#include "store/RDF/NTripleParser.hpp"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

TEST(TestRDFParser, da) {
    std::string path = fs::current_path().string();
    path.append("/../../tests/ntriplefiles/ntriples.nt");
    path = fs::path{path}.string();
    std::cout << path << std::endl;
    for (auto i : range(100)) {
        tnt::store::NTripleParser triple_it{path};

        for (auto &&triple : triple_it)
            std::cout << triple << std::endl;
    }

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
