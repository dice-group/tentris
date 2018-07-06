
#include "tnt/store/TripleStore.hpp"

int main() {
    tnt::store::TripleStore store{};
    store.loadRDF(std::string{"/home/me/Downloads/swdfu8.nt"});
    int i;
    std::cin >> i;
}