//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_UTIL_HPP
#define LIBSPARSETENSOR_UTIL_HPP

#include <iostream>
#include <vector>
#include <numeric>
#include <set>
#include <unordered_set>

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
std::ostream &operator<<(std::ostream &os, const std::vector<TElem> &vec) {
    typename vector<TElem>::const_iterator iter_begin = vec.begin();
    typename vector<TElem>::const_iterator iter_end = vec.end();
    os << "(";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        cout << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << ")";
    return os;
}

namespace std {
    template<typename Tt>
    struct hash<std::unordered_set<Tt>> {
        size_t operator()(const std::unordered_set<Tt> &v) const {
            std::hash<Tt> hasher;
            size_t seed = 0;
            for (int i : v) {
                seed ^= hasher(i); // + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    // Code from https://stackoverflow.com/a/7115547
    namespace
    {

        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     http://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
            static void apply(size_t& seed, Tuple const& tuple)
            {
                HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
                hash_combine(seed, std::get<Index>(tuple));
            }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
            static void apply(size_t& seed, Tuple const& tuple)
            {
                hash_combine(seed, std::get<0>(tuple));
            }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>>
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
            return seed;
        }

    };
}

#endif //LIBSPARSETENSOR_UTIL_HPP
