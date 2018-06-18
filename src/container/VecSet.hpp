#ifndef SPARSETENSOR_CONTAINER_VECSET
#define SPARSETENSOR_CONTAINER_VECSET

#include <vector>
#include <algorithm>
#include "BinarySearch.hpp"

namespace sparsetensor::container {


    template<typename KEY_t>
    class VecSet {

        constexpr const static KEY_t MIN_KEY = std::numeric_limits<KEY_t>::max();
        constexpr const static KEY_t MAX_KEY = std::numeric_limits<KEY_t>::max();

        std::vector<KEY_t> keys{};
    public:
        using iterator = typename std::vector<KEY_t>::iterator;

        VecSet() {};

        inline const KEY_t &min() const {
            if (keys.size())
                return keys.at(0);
            else
                return MAX_KEY;
        }

        inline const KEY_t &max() const {
            if (keys.size())
                return *keys.crbegin();
            else
                return MIN_KEY;
        }

        inline void add(const KEY_t &key) {
            size_t pos = insert_pos<KEY_t>(keys, key);
            if (pos == keys.size() or keys[pos] != key) {
                keys.insert(keys.begin() + pos, key);
            }
        }


        inline void del(const KEY_t &key) {
            size_t pos = search<KEY_t>(keys, key);
            if (pos != NOT_FOUND) {
                keys.erase(keys.begin() + pos);
            }
        }

        inline bool contains(const KEY_t &key) const {
            return search<KEY_t>(keys, key) != NOT_FOUND;
        }

        inline const KEY_t &byInd(size_t index) const {
            return keys.at(index);
        }

        inline size_t size() const {
            return keys.size();
        }

        /**
         *
         * @param key
         * @return The position or if the value is NOT_FOUND
         */
        inline size_t getInd(const KEY_t &key) {
            return search<KEY_t>(keys, key);
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

        inline iterator lower_bound(KEY_t min) {
            const size_t pos = insert_pos(keys, min);
            return keys.begin() + pos;
        }

        inline iterator upper_bound(KEY_t max) {
            size_t pos = insert_pos(keys, max);
            if (pos != keys.size() and keys.at(pos) != max) {
                --pos;
            }
            return keys.begin() + pos;
        }
    };
}

#endif //SPARSETENSOR_CONTAINER_VECSET
