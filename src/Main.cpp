
#include "tnt/store/TripleStore.hpp"
#include "tnt/http/StoreInstance.hpp"
#include "tnt/http/SPARQLEndpoint.hpp"
#include "config/InitialConfig.cpp"
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

void init() {
    boost::log::add_file_log
            (
                    boost::log::keywords::file_name = "TNT_%N.log",
                    boost::log::keywords::rotation_size = 10 * 1024 * 1024,
                    boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
                    boost::log::keywords::format = (
                            boost::log::expressions::stream
                                    << std::setw(8) << std::setfill('0')
                                    << boost::log::expressions::attr< unsigned int >("LineID")
                                    << "\t"
                                    << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d_%H:%M:%S.%f")
                                    << "\t: <" << boost::log::trivial::severity
                                    << "> \t"
                                    << boost::log::expressions::smessage
                    ),
                    boost::log::keywords::auto_flush = true
            );
//
    boost::log::core::get()->set_filter
            (
                    boost::log::trivial::severity >= boost::log::trivial::debug
            );
}

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace Pistache;
    using namespace tnt::http;
    using namespace tnt::config;
    using namespace boost::log::trivial;
    namespace fs = std::experimental::filesystem;

    init();
    boost::log::add_common_attributes();

    InitialConfig initialConfig{argc, argv};

    Port port(initialConfig.port);
    std::string path_to_nt_file{initialConfig.dataBaseFile};
    __timeout = initialConfig.timeout;

    int thr = std::thread::hardware_concurrency();
    Address addr(Ipv4::any(), port);
    cout << "Serving at " << addr.host() << ":" << addr.port() << " ." << endl;
    cout << "Using " << thr << " threads to handle Requests." << endl;
    _store = new tnt::store::TripleStore{};
    std::unique_ptr<tnt::store::TripleStore> _store_ptr(_store);
    if (not initialConfig.dataBaseFile.empty()) {
        if (fs::is_regular_file(path_to_nt_file)) {
            cout << "nt-file: " << path_to_nt_file << endl;
            _store->loadRDF(path_to_nt_file);
        } else {
            cout << "nt-file: " << path_to_nt_file << " not found." << endl;
            return EXIT_FAILURE;
        }

    } else {
        cout << "No file loaded. Use '-f myntfile.nt' if you want to bulkload a file." << endl;
    }
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
    return EXIT_SUCCESS;
}