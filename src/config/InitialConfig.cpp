#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <getopt.h>

namespace tnt::config {
    namespace {
        struct option long_options[] = {
                {"port", required_argument, nullptr, 'p'},
                {"file", required_argument, nullptr, 'f'},
        };
    };

    class InitialConfig {

    public:
        mutable uint16_t port = 9080;
        mutable std::string dataBaseFile{};

        InitialConfig(int argc, char *argv[]) {
            int opt = 0;
            int long_index = 0;
            while ((opt = getopt_long(argc, argv, "p:f:", long_options, &long_index)) != -1) {
                switch (opt) {
                    case 'p': {
                        const int raw_port_number = atoi(optarg);
                        if (0 > raw_port_number or raw_port_number > 65535) {
                            printf("Port must be in range [0,65535].");
                            exit(EXIT_FAILURE);
                        }
                        port = uint16_t(raw_port_number);
                    }
                        break;
                    case 'f':
                        dataBaseFile = optarg;
                        break;
                    default:
                        printf("Wrong option is set for running the program");
                        exit(EXIT_FAILURE);
                }
            }
        }
    };
};