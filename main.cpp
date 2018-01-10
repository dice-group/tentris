#include <iostream>
#include "HyperTrie.hpp"
#include "Trie.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Trie<int> trie{3};
    std::vector<uint64_t> coords{1, 2, 3};
    trie.add(coords, 15);
    std::vector<uint64_t> coords2{1, 2, 4};
    trie.add(coords2, 16);
    std::cout << trie.get(coords) << std::endl;
    std::cout << trie.get(coords2) << std::endl;
    std::vector<uint64_t> coords3{1, 2, 2};
    std::cout << trie.get(coords3) << std::endl;

    return 0;
}