#ifndef TENTRIS_TIMEOUTEXCEPTION_HPP
#define TENTRIS_TIMEOUTEXCEPTION_HPP

#include <exception>
#include <string>


namespace tentris::http {
    class TimeoutException : public std::exception {
        ulong _number_of_results;
        const std::string _what;
    public:
        explicit TimeoutException(const ulong &number_of_results = 0)
                : std::exception{},
                  _number_of_results{number_of_results},
                  _what{std::string{"Timed out after writing "} + std::to_string(_number_of_results).c_str() +
                        " entries"} {}

        const char *what() const throw() {
            return _what.c_str();
        }
    };
}
#endif //TENTRIS_TIMEOUTEXCEPTION_HPP
