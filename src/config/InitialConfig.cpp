#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <getopt.h>

class InitialConfig {
public:
    uint16_t port = 9080;
    std::string dataBaseFile;

    InitialConfig(int argc, char *argv[]) {
        static struct option long_options[] = {
                {"port", required_argument, nullptr, 'p'},
                {"file", required_argument, nullptr, 'f'},
        };
        int opt = 0;
        int long_index = 0;
        while ((opt = getopt_long(argc, argv, "p:f:",
                                  long_options, &long_index)) != -1) {
            switch (opt) {
                case 'p':
                    port = atoi(optarg);
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

    InitialConfig() = default;
};
