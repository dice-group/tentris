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

namespace {
    using namespace tnt::util::sync;
    struct option long_options[] = {
            {"port",    required_argument, nullptr, 'p'},
            {"file",    required_argument, nullptr, 'f'},
            {"timeout", required_argument, nullptr, 't'},
    };
};
namespace tnt::config {
    struct Config;

    class AtomicConfig : public SingletonFactory<Config> {
    public:
        AtomicConfig() : SingletonFactory<Config>{} {}
    };

    struct Config {
        mutable uint16_t port = 9080;
        mutable std::string dataBaseFile{};
        mutable uint timeout = 180;

        friend void init_config(int argc, char *argv[]);
    };

    void init_config(int argc, char **argv) {

        const std::string &argValues = ArrayHelper::ArrayToString(argv, argc);
        logDebug("Called", __PRETTY_FUNCTION__, GET_VARIABLE_NAME(argc), argc, GET_VARIABLE_NAME(argv),
                 argValues);

        int opt = 0;
        int long_index = 0;

        while ((opt = getopt_long(argc, argv, "p:f:t:", long_options, &long_index)) != -1) {
            switch (opt) {
                case 'p': {
                    const int raw_port_number = atoi(optarg);
                    if (0 > raw_port_number or raw_port_number > 65535) {
                        logDebug("Port must be in range [0,65535].");
                        logDebug("Finished", __PRETTY_FUNCTION__, GET_VARIABLE_NAME(argc), argc,
                                 GET_VARIABLE_NAME(argv), argValues);
                        exit(EXIT_FAILURE);
                    }
                    AtomicConfig::getInstance().port = uint16_t(raw_port_number);
                }
                    break;
                case 'f':
                    AtomicConfig::getInstance().dataBaseFile = optarg;
                    break;
                case 't': {
                    const int raw_timeout = atoi(optarg);
                    if (raw_timeout < 0) {
                        AtomicConfig::getInstance().timeout = UINT_MAX;
                    } else {
                        AtomicConfig::getInstance().timeout = uint(raw_timeout);
                    }
                }
                    break;
                default: {
                    logDebug("Port must be in range [0,65535].");
                    logDebug("Finished", __PRETTY_FUNCTION__, GET_VARIABLE_NAME(argc), argc,
                             GET_VARIABLE_NAME(argv), argValues);
                    exit(EXIT_FAILURE);
                }
            }
        }
        logDebug("Finished", __PRETTY_FUNCTION__, GET_VARIABLE_NAME(argc), argc, GET_VARIABLE_NAME(argv),
                 argValues);
    }


};

#endif //TNT_CONFIG_HPP
