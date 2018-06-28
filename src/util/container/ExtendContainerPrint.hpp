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


std::ostream &operator<<(std::ostream &os, const uint8_t &element) {
    os << int(element);
    return os;
}

template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::tuple<T1, T2> &t) {
    os << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ")";
    return os;
}

template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<const T1, T2> &p) {
    os << p.first << " : " << p.second;
    return os;
}

template<typename Value>
std::ostream &operator<<(std::ostream &os, const std::vector<Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "[";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << "]";
    return os;
}

//template<template<class> class SetTyp, typename Value>
//ostream &operator<<(ostream &os, const SetTyp<Value> &elements) {
//    auto iter_begin = begin(elements);
//    auto hypertrie_iter_end = end(elements);
//    os << "{";
//    for (auto hypertrie_iter = iter_begin; hypertrie_iter != hypertrie_iter_end; ++hypertrie_iter) {
//        os << ((hypertrie_iter != iter_begin) ? ", " : "") << *hypertrie_iter;
//    }
//    os << "}";
//    return os;
//}

template<typename Value>
std::ostream &operator<<(std::ostream &os, const std::set<Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "{";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << "}";
    return os;
}

template<typename Value>
std::ostream &operator<<(std::ostream &os, const std::unordered_set<Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "{";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << "}";
    return os;
}


//template<template<typename, typename> class SetTyp, typename Key, typename Value>
//ostream &operator<<(ostream &os, const SetTyp<Key, Value> &elements) {
//    auto iter_begin = begin(elements);
//    auto hypertrie_iter_end = end(elements);
//    os << "{";
//    for (auto hypertrie_iter = iter_begin; hypertrie_iter != hypertrie_iter_end; ++hypertrie_iter) {
//        os << ((hypertrie_iter != iter_begin) ? ", " : "") << (*hypertrie_iter);
//    }
//    os << "}";
//    return os;
//}

template<typename Key, typename Value>
std::ostream &operator<<(std::ostream &os, const std::map<Key, Value> &elements) {
    auto iter_begin = begin(elements);
    auto iter_end = end(elements);
    os << "{";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        os << ((iter != iter_begin) ? ", " : "") << (*iter);
    }
    os << "}";
    return os;
}

template<typename Key, typename Value>
std::ostream &operator<<(std::ostream &os, const std::unordered_map<Key, Value> &elements) {
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
