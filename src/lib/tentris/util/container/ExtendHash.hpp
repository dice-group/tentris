#ifndef SPARSETENSOR_UTIL_EXTENDHASH_HPP
#define SPARSETENSOR_UTIL_EXTENDHASH_HPP

#include <cinttypes>
#include <unordered_set>
#include <tuple>

namespace std {
    template<typename Tt>
    struct hash<std::unordered_set<Tt>> {
        size_t operator()(const unordered_set <Tt> &v) const {
            std::hash<Tt> hasher;
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
            seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
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
    struct hash<std::tuple<TT...>> {
        size_t operator()(std::tuple<TT...> const &tt) const {
            size_t seed = 0;
            HashValueImpl<tuple<TT...> >::apply(seed, tt);
            return seed;
        }

    };
}

#endif //SPARSETENSOR_UTIL_EXTENDHASH_HPP
