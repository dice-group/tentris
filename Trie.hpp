//
// Created by me on 1/9/18.
//

#ifndef LIBSPARSETENSOR_TRIE_HPP
#define LIBSPARSETENSOR_TRIE_HPP


#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include "boost/variant.hpp"
#include <tuple>
#include <memory>
#include <iostream>

template<typename T>
class Trie {
public:
    Trie(uint8_t depth);

private:
    uint8_t depth;
    std::map<uint64_t, std::variant<Trie *, T>> edges;
    T leafsum;
    int64_t leafcount;
public:

    void add(std::vector<uint64_t> coords, T value) {
        if (coords.size() != this->depth) {
            throw "coords size() must be equal to depth.";
        }
        Trie *current_trie = this;

        for (uint8_t i = 0; i < coords.size(); i++) {

            uint64_t &coord = coords.at(i);

            auto child_it = current_trie->edges.find(coord);

            if (i != coords.size() - 1) {

                if (child_it != current_trie->edges.end()) {
                    current_trie = std::get<Trie *>(child_it->second);
                } else {
                    Trie *next_trie = new Trie(this->depth - i - uint8_t(1));
                    std::variant<Trie *, T> var{next_trie};
                    current_trie->edges.insert_or_assign(coord, var);
                    current_trie = next_trie;
                }
            } else {
                std::variant<Trie *, T> var{value};
                current_trie->edges.insert_or_assign(coord, var);
            }

        }
    }

    T get(std::vector<uint64_t> coords) {
        if (coords.size() != this->depth) {
            throw "coords size() must be equal to depth.";
        }
        Trie *current_trie = this;

        for (unsigned int i = 0; i < coords.size(); i++) {

            uint64_t &coord = coords.at(i);

            auto child_it = current_trie->edges.find(coord);
            std::cout << current_trie->edges.size() << std::endl;

            if (child_it != current_trie->edges.end()) {

                if (i != coords.size() - 1) {
                    current_trie = std::get<Trie *>(child_it->second);

                } else {
                    return std::get<T>(child_it->second);
                }
            } else {
                return (T) -1;
            }

        }
    }
};

template<typename T>
Trie<T>::Trie(uint8_t depth) : depth(depth) {}


#endif //LIBSPARSETENSOR_TRIE_HPP
