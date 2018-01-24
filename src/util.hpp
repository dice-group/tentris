#ifndef LIBSPARSETENSOR_UTIL_HPP
#define LIBSPARSETENSOR_UTIL_HPP

#include <iostream>
#include <vector>
#include <numeric>
#include <set>
#include <unordered_set>

using std::vector;
using std::set;
using std::unordered_set;
using std::map;
using std::unordered_map;
using std::ostream;
using std::tuple;
using std::pair;
using std::hash;


ostream &operator<<(ostream &os, const uint8_t &element) {
    os << int(element);
    return os;
}

template<typename Key, typename Value>
ostream &operator<<(ostream &os, const tuple<Key, Value> &t) {
    os << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ")";
    return os;
}

template<typename Key, typename Value>
ostream &operator<<(ostream &os, const pair<const Key, Value> &p) {
    os << p.first << " : " << p.second;
    return os;
}


template<typename Value>
ostream &operator<<(ostream &os, const vector<Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "[";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << "]";
    return os;
}

template<template<class> class SetTyp, typename Value>
ostream &operator<<(ostream &os, const SetTyp<Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "{";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << "}";
    return os;
}

template<template<typename, typename> class SetTyp, typename Key, typename Value>
ostream &operator<<(ostream &os, const SetTyp<Key, Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "{";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << (*iter);
    }
    os << "}";
    return os;
}

namespace std {
    template<typename Tt>
    struct hash<unordered_set<Tt>> {
        size_t operator()(const unordered_set<Tt> &v) const {
            hash<Tt> hasher;
            size_t seed = 0;
            for (int i : v) {
                seed ^= hasher(i); // + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    // Code from https://stackoverflow.com/a/7115547
    namespace {

        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     http://stackoverflow.com/questions/4948780

        template<class T>
        inline void hash_combine(std::size_t &seed, T const &v) {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        // Recursive template code derived from Matthieu M.
        template<class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl {
            static void apply(size_t &seed, Tuple const &tuple) {
                HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
                hash_combine(seed, std::get<Index>(tuple));
            }
        };

        template<class Tuple>
        struct HashValueImpl<Tuple, 0> {
            static void apply(size_t &seed, Tuple const &tuple) {
                hash_combine(seed, std::get<0>(tuple));
            }
        };
    }

    template<typename ... TT>
    struct hash<tuple<TT...>> {
        size_t
        operator()(tuple<TT...> const &tt) const {
            size_t seed = 0;
            HashValueImpl<tuple<TT...> >::apply(seed, tt);
            return seed;
        }

    };
}

#endif //LIBSPARSETENSOR_UTIL_HPP
