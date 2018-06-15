#ifndef SPARSETENSOR_CONTAINER_VECSET
#define SPARSETENSOR_CONTAINER_VECSET

#include <vector>
#include <algorithm>
#include "BinarySearch.hpp"

namespace sparsetensor::container {

    template<typename KEY_t>
    class VecSet {
        std::vector<KEY_t> keys{};
    public:
        using iterator = std::vector<KEY_t>::iterator;

        VecSet() {};

        const KEY_t &min() {
            if (keys.size())
                return *keys.cbegin();
            else
                return std::numeric_limits<KEY_t>::max();
        }

        const KEY_t &max() {
            if (keys.size())
                return *keys.crbegin();
            else
                return std::numeric_limits<KEY_t>::min();
        }

        void add(const KEY_t &key) {
            size_t pos = bin_search_insert_pos<KEY_t>(keys, key);
            if (pos == keys.size() or keys[pos] != key) {
                keys.insert(keys.begin() + pos, key);
            }
        }


        void del(const KEY_t &key) {
            size_t pos = bin_search_insert_pos<KEY_t>(keys, key);
            if (pos != keys.size() and keys[pos] == key) {
                keys.erase(keys.begin() + pos);
            }
        }

        bool contains(const KEY_t &key) const {
            return search<KEY_t>(keys, key) != std::numeric_limits<size_t>::max();
        }

        const KEY_t &byInd(size_t index) const {
            return keys.at(index);
        }

        inline size_t size() const {
            return keys.size();
        }

        /**
         *
         * @param key
         * @return The position or if the value is not contained the size of the Set.
         */
        size_t getInd(const KEY_t &key) {
            return bin_search_insert_pos<KEY_t>(keys, key);
        }

        inline iterator begin() {
            return keys.begin();
        }

        inline iterator cbegin() {
            return keys.cbegin();
        }

        inline iterator crbegin() {
            return keys.crbegin();
        }

        inline iterator end() {
            return keys.end();
        }

        inline iterator cend() {
            return keys.cend();
        }

        inline iterator crend() {
            return keys.crend();
        }
    };
}

#endif //SPARSETENSOR_CONTAINER_VECSET
