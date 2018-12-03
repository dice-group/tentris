
#include <experimental/filesystem>
#include <csignal>

#include "tnt/store/TripleStore.hpp"
#include "tnt/config/Config.cpp"
#include "tnt/http/AtomicTripleStore.hpp"
#include "tnt/http/SPARQLEndpoint.hpp"



namespace {
    using namespace Pistache;
    using namespace tnt::http;
    using namespace tnt::config;
    namespace fs = std::experimental::filesystem;
}

int main(int argc, char *argv[]) {
    init_logging();

    tnt::config::init_config(argc, argv);

    Port port(AtomicConfig::getInstance().port);
    std::string path_to_nt_file{AtomicConfig::getInstance().dataBaseFile};

    int thr = std::thread::hardware_concurrency();
    Address addr(Ipv4::any(), port);
    log("Serving at ", addr.host(), ":", addr.port(), " .");
    log("Using ", thr, " threads to handle Requests.");
    auto loading_start_time = log_health_data();
    if (not AtomicConfig::getInstance().dataBaseFile.empty()) {
        if (fs::is_regular_file(path_to_nt_file)) {
            log("nt-file: ", path_to_nt_file, " loading ...");
            AtomicTripleStore::getInstance().loadRDF(path_to_nt_file);
        } else {
            log("nt-file: ", path_to_nt_file, " not found." );
            return EXIT_FAILURE;
        }

    } else {
        log("No file loaded. Use '-f myntfile.nt' if you want to bulkload a file.");
    }
    log("Loaded ", AtomicTripleStore::getInstance().size(), " triples.");
    auto loading_end_time = log_health_data();
    log_duration(loading_start_time, loading_end_time);

    log("URI: ", "http://127.0.0.1:", addr.port(), "/sparql?query");

    auto server = std::make_shared<Http::Endpoint>(addr);

    auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::InstallSignalHandler | Tcp::Options::ReuseAddr);
    server->init(opts);
    server->setHandler(Http::make_handler<SPARQLEndpoint>());
    server->serveThreaded();

    while (true) {
        sigset_t wset;
        sigemptyset(&wset);
        sigaddset(&wset,SIGINT);
        int number;

        if (int status = sigwait(&wset, &number); status != 0) {
            log("Set contains an invalid signal number.");
            break;
        }
        if (number == SIGINT){
            logDebug("Exiting by Signal ", std::string{strsignal(number)}, ".");
            break;
        }
    }

    log("Shutdowning server ...");
    server->shutdown();
    log("Shutdownn successful.");
    return EXIT_SUCCESS;
}