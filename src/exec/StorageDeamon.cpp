#include <string>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <cstdint>

#include "config/DaemonConfig.hpp"

#include <tnt/store/TripleStore.hpp>

namespace {
    using namespace tnt::store;
    using namespace std::filesystem;
}

static std::string handle_name = ".~triplestore_handle";

int main(int argc, char *argv[]) {
	DaemonConfig cfg{argc, argv};

    TripleStore triplestore{cfg.cache_size, cfg.cache_bucket_capacity, cfg.timeout};
    triplestore.loadRDF(cfg.rdf_file);

    std::ofstream triplestore_handle(handle_name, std::ofstream::out);

    triplestore_handle << reinterpret_cast<uintptr_t>(&triplestore);
    triplestore_handle.close();



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
            logDebug("Exiting by Signal {}."_format(strsignal(number)));
            break;
        }
    }
    remove(handle_name);
}
