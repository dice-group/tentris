#include <gtest/gtest.h>

#include "store/RDFParser.hpp"
#include <filesystem>
namespace fs = std::filesystem;
TEST(TestRDFParser, da) {
    std::string path = fs::current_path().string();
    path.append("/../../tests/ntriplefiles/ntriples.nt");
    path = fs::path{path}.string();
    std::cout << path << std::endl;
    tnt::store::RDFParser da{path};

    for(auto x : da);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
