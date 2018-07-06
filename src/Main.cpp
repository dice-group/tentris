
#include "tnt/store/TripleStore.hpp"
#include <antlr4-runtime.h>
#include "SparqlParser.cpp"
#include "SparqlLexer.cpp"

int main() {
    tnt::store::TripleStore store{};
    store.loadRDF(std::string{"/home/me/Downloads/swdfu8.nt"});
}