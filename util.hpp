//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_UTIL_HPP
#define LIBSPARSETENSOR_UTIL_HPP

#include <iostream>
#include <vector>
using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::endl;

/**
 * Adds operator<< to std::vector.<br/>
 * source: http://www.cplusplus.com/forum/beginner/104130/
 * @tparam TElem Type of std::vector's elements
 * @param os stream to write to
 * @param vec vector to be printed
 * @return input stream for chaining
 */
template<typename TElem>
std::ostream &operator<<(std::ostream &os, const std::vector<TElem> &vec){
    typename vector<TElem>::const_iterator iter_begin = vec.begin();
    typename vector<TElem>::const_iterator iter_end = vec.end();
    os << "(";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        cout << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << ")";
    return os;
}

#endif //LIBSPARSETENSOR_UTIL_HPP
