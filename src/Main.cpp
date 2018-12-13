
#include <filesystem>
#include <csignal>

#include <tnt/store/TripleStore.hpp>
#include <tnt/config/Config.cpp>
#include <tnt/store/AtomicTripleStore.hpp>
#include <tnt/http/SPARQLEndpoint.hpp>



namespace {
    using namespace Pistache;
    using namespace tnt::http;
    using namespace tnt::config;
    namespace fs = std::filesystem;
}

void bulkload(std::string triple_file){

    // log the starting time and print resource usage informations
    auto loading_start_time = log_health_data();

    if (fs::is_regular_file(triple_file)) {
        log("nt-file: ", triple_file, " loading ...");
        AtomicTripleStore::getInstance().loadRDF(triple_file);
    } else {
        log("nt-file ", triple_file, " was not found." );
        log("Exiting ..." );
        std::exit(EXIT_FAILURE);
    }
    log("Loaded ", AtomicTripleStore::getInstance().size(), " triples.\n");

    // log the end time and print resource usage informations
    auto loading_end_time = log_health_data();
    // log the time it tool to load the file
    log_duration(loading_start_time, loading_end_time);
}

int main(int argc, char *argv[]) {
    init_logging();

    tnt::config::init_config(argc, argv);

    // bulkload file
    if(std::string triple_file = AtomicConfig::getInstance().dataBaseFile; not triple_file.empty()){
        bulkload(AtomicConfig::getInstance().dataBaseFile);
    } else{
        log("No file loaded. Use '-f yourfile.nt' to bulkload a file.");
    }

    // create endpoint
    Address address(Ipv4::any(), {AtomicConfig::getInstance().port});
    auto opts = Http::Endpoint::options()
            .threads(AtomicConfig::getInstance().threads)
            .flags(Tcp::Options::InstallSignalHandler | Tcp::Options::ReuseAddr);
    std::shared_ptr<Http::Endpoint> server = std::make_shared<Http::Endpoint>(address);
    server->init(opts);
    server->setHandler(Http::make_handler<SPARQLEndpoint>());
    log("Server \n"
        "  threads: ", AtomicConfig::getInstance().threads, "\n",
        "  IRI:     ", "http://127.0.0.1:", address.port(), "/sparql?query=");
    // start endpoint
    server->serveThreaded();

    // wait for keyboard interrupt
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

    log("Shutting down server ...");
    server->shutdown();
    log("Shutdown successful.");
    return EXIT_SUCCESS;
}