#ifndef TIMEOUTCHECK_HPP
#define TIMEOUTCHECK_HPP

#include <chrono>
#include <stdexcept>

namespace dice::endpoint {
    inline void check_timeout(std::chrono::steady_clock::time_point timeout) {
        if (timeout <= std::chrono::steady_clock::now())
            throw std::runtime_error{"timeout reached"};
    }
}// namespace dice::endpoint
#endif//TIMEOUTCHECK_HPP