#ifndef ENDOINTCFG_HPP
#define ENDOINTCFG_HPP

#include <chrono>
#include <cstdint>
#include <optional>

namespace dice::endpoint {

    struct EndpointCfg {
        uint16_t port;
        uint16_t threads;
        std::optional<std::chrono::steady_clock::duration> opt_timeout_duration;
    };
}// namespace dice::endpoint
#endif//ENDOINTCFG_HPP