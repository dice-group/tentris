//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_UTIL_HPP
#define LIBSPARSETENSOR_UTIL_HPP

#include <iostream>
#include <vector>

/**
 * Adds operator<< to std::vector.<br/>
 * source: http://www.cplusplus.com/forum/beginner/104130/
 * @tparam TElem Type of std::vector's elements
 * @param os stream to write to
 * @param vec vector to be printed
 * @return input stream for chaining
 */
template<typename TElem>
std::ostream &operator<<(std::ostream &os, const std::vector<TElem> &vec);

#endif //LIBSPARSETENSOR_UTIL_HPP
