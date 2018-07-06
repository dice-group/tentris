
#include "tnt/store/TripleStore.hpp"
#include "tnt/web/healthCheck.hpp"
#include <antlr4-runtime.h>
#include "SparqlParser.cpp"
#include "SparqlLexer.cpp"
#include "pistache/endpoint.h"
#include "pistache/net.h"
#include "pistache/http.h"
#include "pistache/router.h"

int main() {
    tnt::store::TripleStore store{};
    store.loadRDF(std::string{"/home/me/Downloads/swdfu8.nt"});
    Pistache::Http::listenAndServe<healthCheck>("*:9999");
}