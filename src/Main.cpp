
#include "tnt/store/TripleStore.hpp"

int main() {
    tnt::store::TripleStore store{};
    store.loadRDF(std::string{"/home/me/CLionProjects/tnt/tests/ntriplefiles/ntriples.nt"});
}