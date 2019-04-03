#ifndef ARRAYHELPER_HPP
#define ARRAYHELPER_HPP

#include <string>
#include <sstream>

class ArrayHelper {
public:
    template<class T>
    static std::string ArrayToString(T array[], int len, const std::string &separator = ",") {
        std::ostringstream oss;
        for (int i = 0; i < len; ++i)
            oss << (i == 0 ? "" : separator) << array[i];
        std::string ret = oss.str();
        return ret;
    }
};

#endif