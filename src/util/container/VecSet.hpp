#ifndef SPARSETENSOR_CONTAINER_VECSET
#define SPARSETENSOR_CONTAINER_VECSET

#include <vector>
#include <algorithm>

#include "BinarySearch.hpp"

namespace tnt::container {


    template<typename KEY_t>
    class VecSet {

    public:
        constexpr const static KEY_t MIN_KEY = std::numeric_limits<KEY_t>::max();
        constexpr const static KEY_t MAX_KEY = std::numeric_limits<KEY_t>::max();
        std::vector<KEY_t> _keys{};
        using iterator = typename std::vector<KEY_t>::iterator;

        VecSet() {}

        inline const KEY_t &min() const {
            if (_keys.size())
                return _keys.at(0);
            else
                return MAX_KEY;
        }

        inline const KEY_t &max() const {
            if (_keys.size())
                return *_keys.crbegin();
            else
                return MIN_KEY;
        }

        inline void add(const KEY_t &key) {
            size_t pos = insert_pos<KEY_t>(_keys, key);
            if (pos == _keys.size() or _keys[pos] != key) {
                _keys.insert(_keys.begin() + pos, key);
            }
        }


        inline void del(const KEY_t &key) {
            size_t pos = search<KEY_t>(_keys, key);
            if (pos != NOT_FOUND) {
                _keys.erase(_keys.begin() + pos);
            }
        }

        inline bool contains(const KEY_t &key) const {
            return search<KEY_t>(_keys, key) != NOT_FOUND;
        }

        inline const KEY_t &byInd(size_t index) const {
            return _keys.at(index);
        }

        inline size_t size() const {
            return _keys.size();
        }

        /**
         *
         * @param key
         * @return The position or if the value is NOT_FOUND
         */
        inline size_t getInd(const KEY_t &key) {
            return search<KEY_t>(_keys, key);
        }

        inline iterator begin() {
            return _keys.begin();
        }

        inline iterator cbegin() {
            return _keys.cbegin();
        }

        inline iterator crbegin() {
            return _keys.crbegin();
        }

        inline iterator end() {
            return _keys.end();
        }

        inline iterator cend() {
            return _keys.cend();
        }

        inline iterator crend() {
            return _keys.crend();
        }

        inline iterator lower_bound(KEY_t min) {
            const size_t pos = insert_pos(_keys, min);
            return _keys.begin() + pos;
        }

        inline iterator upper_bound(KEY_t max) {
            size_t pos = insert_pos(_keys, max);
            if (pos != _keys.size() and _keys.at(pos) != max) {
                --pos;
            }
            return _keys.begin() + pos;
        }
    };
}

#endif //SPARSETENSOR_CONTAINER_VECSET
