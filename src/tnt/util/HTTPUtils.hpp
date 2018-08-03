//
// Created by usr on 03.08.18.
//

#ifndef TNT_HTTPUTILS_HPP
#define TNT_HTTPUTILS_HPP

#include <string>
#include <sstream>
#include <ostream>
#include <c++/8/charconv>
#include <cassert>

namespace tnt::http {

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
#endif //TNT_HTTPUTILS_HPP
