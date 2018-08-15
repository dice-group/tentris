#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <getopt.h>
#include <sstream>
#include "tnt/util/ArrayHelper.hpp"
#include "tnt/util/LogHelper.hpp"

namespace tnt::config {
    namespace {
        struct option long_options[] = {
                {"port",    required_argument, nullptr, 'p'},
                {"file",    required_argument, nullptr, 'f'},
                {"timeout", required_argument, nullptr, 't'},
        };
    };

    class InitialConfig {
    public:
        mutable uint16_t port = 9080;
        mutable std::string dataBaseFile{};
        mutable uint timeout = 180;

        InitialConfig(int argc, char *argv[]) {
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
                        port = uint16_t(raw_port_number);
                    }
                        break;
                    case 'f':
                        dataBaseFile = optarg;
                        break;
                    case 't': {
                        const int raw_timeout = atoi(optarg);
                        if (raw_timeout < 0) {
                            timeout = UINT_MAX;
                        } else {
                            timeout = uint(raw_timeout);
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
};