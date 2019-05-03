//
// Created by usr on 03.08.18.
//

#ifndef TENTRIS_HTTPUTILS_HPP
#define TENTRIS_HTTPUTILS_HPP

#include <string>
#include <sstream>
#include <c++/8/charconv>
#include <cassert>
#include <map>
#include <exception>

namespace tentris::http {

/**
 * Decode Query parameter values from HTTP requests.
 * @param urlEncoded url encoded string
 * @return decoded string
 */
    auto urlDecode(const std::string &urlEncoded) -> std::string {
        std::stringstream out;
        size_t i = 0;
        const char *data = urlEncoded.data();
        while (i < urlEncoded.size()) {
            switch (data[i]) {
                case '+':
                    out << ' ';
                    ++i;
                    break;
                case '%':
                    assert(i < urlEncoded.size() - 2);
                    char c;
                    std::from_chars(data + i + 1, data + i + 3, c, 16);
                    out << c;
                    i += 3;
                    break;
                default:
                    out << data[i];
                    ++i;
            }
        }
        return out.str();
    }

    inline char hexchar2char(const char &input) {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + char(10);
        if (input >= 'a' && input <= 'f')
            return input - 'a' + char(10);
        throw std::invalid_argument("Invalid input string");
    }

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
    inline char two_hexchars2char(const char src[]) {
        return hexchar2char(src[0]) * char(16) + hexchar2char(src[1]);
    }

    std::map<std::string, std::string> decodeFormParam(const std::string &x_www_form_urlencoded) {
        std::map<std::string, std::string> decoded_params{};
        enum state {
            key,
            value
        };
        state current_state = key;
        std::ostringstream ss;
        std::string entry_key;
        for (auto iter = x_www_form_urlencoded.cbegin(), _end = x_www_form_urlencoded.cend(); iter != _end; ++iter) {
            const char &current_char = *iter;

            switch (current_char) {
                case '=': {
                    if (current_state != key)
                        throw std::invalid_argument("Two '=' must always be seperated by a '&'");

                    entry_key = ss.str();

                    ss.clear();
                    ss.seekp(0);

                    current_state = value;
                    break;
                }

                case '&': {
                    if (current_state != value)
                        throw std::invalid_argument("Two '&' must always be seperated by a '='");
                    decoded_params.emplace(entry_key, ss.str());

                    ss.clear();
                    ss.seekp(0);

                    current_state = key;
                    break;
                }

                case '+': {
                    ss << ' ';
                    break;
                }

                case '%': {
                    char next_char;
                    bool first = true;
                    uint runs = 0;
                    do {
                        char hex_chars[2];
                        if (_end == ++iter)
                            throw std::invalid_argument("string must have at least two chars after '%'");
                        hex_chars[0] = *iter;
                        if (_end == ++iter)
                            throw std::invalid_argument("string must have at least two chars after '%'");
                        hex_chars[1] = *iter;

                        next_char = two_hexchars2char(hex_chars);
                        ss << next_char;
                        if (first) {
                            first = false;
                            if ((0b11110000 & next_char) == 0b11110000 and not(0b00001000 & next_char))
                                runs = 3;
                            else if ((0b11100000 & next_char) == 0b11100000 and not(0b00010000 & next_char))
                                runs = 2;
                            else if ((0b11000000 & next_char) == 0b11000000 and not(0b00100000 & next_char))
                                runs = 1;
                        }
                    } while (runs-- > 0);
                    break;
                }
                default:
                    ss << current_char;
                    break;
            }
        }
        decoded_params.emplace(entry_key, ss.str());
        return decoded_params;
    }


// TODO: stream to ouptput instead of writing a stream
    template<typename S>
    std::string escapeJsonString(const S &input) {
        std::ostringstream ss;
        for (const char &current_char : input) {
            switch (current_char) {
                case '\\':
                    ss << "\\\\";
                    break;
                case '"':
                    ss << "\\\"";
                    break;
                case '/':
                    ss << "\\/";
                    break;
                case '\b':
                    ss << "\\b";
                    break;
                case '\f':
                    ss << "\\f";
                    break;
                case '\n':
                    ss << "\\n";
                    break;
                case '\r':
                    ss << "\\r";
                    break;
                case '\t':
                    ss << "\\t";
                    break;
                default:
                    ss << current_char;
                    break;
            }
        }
        return ss.str();
    }
};
#endif //TENTRIS_HTTPUTILS_HPP
