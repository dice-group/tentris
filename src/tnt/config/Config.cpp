#ifndef TNT_CONFIG_HPP
#define TNT_CONFIG_HPP

#include <string>

#include <cxxopts.hpp>

#include "tnt/util/LogHelper.hpp"
#include "tnt/util/SingletonFactory.hpp"


namespace {
    using namespace ::tnt::logging;
}

namespace tnt::config {
    /**
     * The configuration of a TNT instance.
     */
    struct Config;

    /**
     * A Factory that assures that only one instance of Config is created per running TNT instance.
     */
    class AtomicConfig : public util::sync::SingletonFactory<Config> {
    public:
        AtomicConfig() : SingletonFactory<Config>{} {}
    };

    struct Config {
        cxxopts::Options options{"TNT", "TNT, a tensor-based triple store with an HTTP sparql enpoint"};

        /**
         * The where TNT runs.
         */
        mutable uint16_t port = 9080;
        /**
         * The relative or absolute path to the RDF file that TNT loads on startup.
         */
        mutable std::string dataBaseFile{};
        /**
         * The timeout for query processing of single queries.
         */
        mutable uint timeout = 180;
        /**
         * Number of threads used to serve http results. Each thread may use multiple others while calculating the result.
         */
        mutable uint8_t threads = std::thread::hardware_concurrency();


        /**
         * Initialization of command argument parser.
         */
        Config() {
            options.add_options()
                    ("p,port", "port to run server", cxxopts::value<uint16_t>()->default_value("9080"))
                    ("f,file", "ntriple file to load at startup", cxxopts::value<std::string>())
                    ("t,timeout", "time in seconds until processing a request is canceled by the server",
                     cxxopts::value<uint>()->default_value("180"))
                    ("c,threads", "How many threads are used for handling http requests", cxxopts::value<uint8_t>()->default_value("0"));
        }

        friend void init_config(int argc, char *argv[]);
    };

    /**
     * Parses the command line arguments that were passed to TNT into the Config instance managed by AtomicConfig.
     * @param argc number of arguments
     * @param argv array of char arrays with arguments
     */
    void init_config(int argc, char **argv) {
        Config &config = AtomicConfig::getInstance();

        try {
            auto arguments = config.options.parse(argc, argv);

            if (arguments.count("port") == 1) {
                auto port = arguments["port"].as<uint16_t>();

                config.port = port;
            }

            if (arguments.count("file") == 1) {
                auto file = arguments["file"].as<std::string>();

                config.dataBaseFile = file;
            }

            if (arguments.count("timeout") == 1) {
                auto timeout = arguments["timeout"].as<uint>();

                if (timeout == 0)
                    config.timeout = UINT_MAX;
                else
                    config.timeout = timeout;
            }

            if (arguments.count("threads") == 1) {
                auto threads = arguments["threads"].as<uint8_t>();

                if (threads != 0)
                    config.threads = threads;
            }
        } catch (cxxopts::option_not_exists_exception &ex) {
            if (std::string{"Option ‘help’ does not exist"}.compare(ex.what()) == 0) {
                log(config.options.help());
                exit(EXIT_SUCCESS);
            } else {
                log(ex.what());
                exit(EXIT_FAILURE);
            }
        } catch (cxxopts::argument_incorrect_type &ex) {
            log(ex.what());
            exit(EXIT_FAILURE);
        }
    }

};

#endif //TNT_CONFIG_HPP
