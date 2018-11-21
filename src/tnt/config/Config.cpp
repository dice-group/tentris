#ifndef TNT_CONFIG_HPP
#define TNT_CONFIG_HPP

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <getopt.h>

#include "tnt/util/ArrayHelper.hpp"
#include "tnt/util/LogHelper.hpp"
#include "tnt/util/SingletonFactory.hpp"
#include <cxxopts.hpp>


namespace tnt::config {
    struct Config;

    class AtomicConfig : public util::sync::SingletonFactory<Config> {
    public:
        AtomicConfig() : SingletonFactory<Config>{} {}
    };

    struct Config {
        cxxopts::Options options{"TNT", "TNT, a tensor-based triple store with an HTTP sparql enpoint"};


        mutable uint16_t port = 9080;
        mutable std::string dataBaseFile{};
        mutable uint timeout = 180;

        Config() {
            options.add_options()
                    ("p,port", "port to run server", cxxopts::value<uint16_t>()->default_value("9080"))
                    ("f,file", "ntriple file to load at startup", cxxopts::value<std::string>())
                    ("t,timeout", "time in seconds until processing a request is canceled by the server",
                     cxxopts::value<uint>()->default_value("180"));
        }

        friend void init_config(int argc, char *argv[]);
    };

    void init_config(int argc, char **argv) {
        Config &config = AtomicConfig::getInstance();

        try {
            auto arguments = config.options.parse(argc, argv);

            if (arguments.count("port") == 1) {
                auto port = arguments["port"].as<uint16_t>();

                if (0 > port or port > 65535) {
                    log("Port must be in range [0,65535].");
                    exit(EXIT_FAILURE);
                }

                config.port = port;
            }

            if (arguments.count("file") == 1) {
                auto file = arguments["file"].as<std::string>();

                config.dataBaseFile = file;
            }

            if (arguments.count("timeout") == 1) {
                auto timeout = arguments["timeout"].as<uint>();

                if (timeout < 0)
                    config.timeout = UINT_MAX;
                else
                    config.timeout = timeout;
            }
        } catch (cxxopts::option_not_exists_exception ex) {
            if (std::string{"Option ‘help’ does not exist"}.compare(ex.what()) == 0) {
                log(config.options.help());
                exit(EXIT_SUCCESS);
            } else {
                log(ex.what());
                exit(EXIT_FAILURE);
            }
        } catch (cxxopts::argument_incorrect_type ex) {
            log(ex.what());
            exit(EXIT_FAILURE);
        }
    }

};

#endif //TNT_CONFIG_HPP
