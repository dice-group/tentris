
#include "tnt/store/TripleStore.hpp"
#include "tnt/http/SPARQLEndpoint.hpp"

using namespace Pistache;

int main() {
    using namespace std;
    using namespace Pistache;
    Port port(9080);

    int thr = std::thread::hardware_concurrency();
    Address addr(Ipv4::any(), port);
    cout << "Serving at " << addr.host() << ":" << addr.port() << " ." << endl;
    cout << "Using " << thr << " threads to handle Requests." << endl;

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