#ifndef SPARSETENSOR_UTIL_EXTENDEDCONTAINERPRINT_HPP
#define SPARSETENSOR_UTIL_EXTENDEDCONTAINERPRINT_HPP

#include <iostream>
#include <vector>
#include <numeric>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cinttypes>

using std::ostream;
using std::tuple;
using std::pair;
using std::hash;
using std::vector;
using std::unordered_set;

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

//template<typename Value>
ostream &operator<<(ostream &os, const unordered_set<uint8_t> &elements) {
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


#endif //SPARSETENSOR_UTIL_EXTENDEDCONTAINERPRINT_HPP
