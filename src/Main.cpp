
#include <experimental/filesystem>
#include <boost/log/core.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>

#include "tnt/store/TripleStore.hpp"
#include "tnt/config/Config.cpp"
#include "tnt/http/AtomicTripleStore.hpp"
#include "tnt/http/SPARQLEndpoint.hpp"

namespace {
    using namespace Pistache;
    using namespace tnt::http;
    using namespace tnt::config;
    using namespace boost::log::trivial;
    namespace fs = std::experimental::filesystem;
}

// TODO: move out of main
void init_logging() {
    using namespace boost::log;
    add_file_log
            (
                    keywords::file_name = "TNT_%N.log",
                    keywords::rotation_size = 10 * 1024 * 1024,
                    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                    keywords::format = (
                            expressions::stream
                                    << std::setw(8) << std::setfill('0')
                                    << expressions::attr< unsigned int >("LineID")
                                    << "\t"
                                    << expressions::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d_%H:%M:%S.%f")
                                    << "\t: <" << trivial::severity
                                    << "> \t"
                                    << expressions::smessage
                    ),
                    keywords::auto_flush = true
            );
//
    core::get()->set_filter
            (
                    trivial::severity >= trivial::debug
            );
}

int main(int argc, char *argv[]) {
    init_logging();
    boost::log::add_common_attributes();

    tnt::config::init_config(argc, argv);

    Port port(AtomicConfig::getInstance().port);
    std::string path_to_nt_file{AtomicConfig::getInstance().dataBaseFile};

    int thr = std::thread::hardware_concurrency();
    Address addr(Ipv4::any(), port);
    std::cout << "Serving at " << addr.host() << ":" << addr.port() << " ." << std::endl;
    std::cout << "Using " << thr << " threads to handle Requests." << std::endl;
    if (not AtomicConfig::getInstance().dataBaseFile.empty()) {
        if (fs::is_regular_file(path_to_nt_file)) {
            std::cout << "nt-file: " << path_to_nt_file << std::endl;
            AtomicTripleStore::getInstance().loadRDF(path_to_nt_file);
        } else {
            std::cout << "nt-file: " << path_to_nt_file << " not found." << std::endl;
            return EXIT_FAILURE;
        }

    } else {
        std::cout << "No file loaded. Use '-f myntfile.nt' if you want to bulkload a file." << std::endl;
    }
    std::cout << "URI: " << "http://127.0.0.1:" << addr.port() << "/sparql?query" << std::endl;

    auto server = std::make_shared<Http::Endpoint>(addr);

    auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::InstallSignalHandler | Tcp::Options::ReuseAddr);
    server->init(opts);
    server->setHandler(Http::make_handler<SPARQLEndpoint>());
    server->serve();

    std::cout << "Shutdowning server" << std::endl;
    server->shutdown();
    return EXIT_SUCCESS;
}