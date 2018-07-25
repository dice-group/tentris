
#include "tnt/store/TripleStore.hpp"
#include "tnt/http/SPARQLEndpoint.hpp"
#include "config/InitialConfig.cpp"


int main(int argc, char *argv[]) {
    using namespace std;
    using namespace Pistache;
    using namespace tnt::http;

    InitialConfig initialConfig{argc, argv};

    Port port(initialConfig.port);
    std::string path_to_nt_file{initialConfig.dataBaseFile};

    int thr = std::thread::hardware_concurrency();
    Address addr(Ipv4::any(), port);
    cout << "Serving at " << addr.host() << ":" << addr.port() << " ." << endl;
    cout << "Using " << thr << " threads to handle Requests." << endl;
    _store = new tnt::store::TripleStore{};
    std::unique_ptr<tnt::store::TripleStore> _store_ptr(_store);
    _store->loadRDF(path_to_nt_file);
    cout << "nt-file: " << path_to_nt_file << endl;
    cout << "URI: " << "http://127.0.0.1:" << addr.port() << "/sparql?query" << endl;

    auto server = std::make_shared<Http::Endpoint>(addr);

    auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::InstallSignalHandler | Tcp::Options::ReuseAddr);
    server->init(opts);
    server->setHandler(Http::make_handler<SPARQLEndpoint>());
    server->serve();

    std::cout << "Shutdowning server" << std::endl;
    server->shutdown();

}