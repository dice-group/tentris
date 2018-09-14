#ifndef SPARSETENSOR_CONTAINER_VECSET
#define SPARSETENSOR_CONTAINER_VECSET

#include <vector>
#include <algorithm>
#include <tuple>

#include "BinarySearch.hpp"

namespace tnt::util::container {


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

        inline const KEY_t &keyByInd(size_t index) const {
            return _keys.at(index);
        }

        inline size_t size() const {
            return _keys.size();
        }

        inline const std::vector<KEY_t> keys() const{
            return _keys;
        }

        /**
         *
         * @param key
         * @return The position or if the value is NOT_FOUND
         */
        inline size_t getInd(const KEY_t &key) {
            return search<KEY_t>(_keys, key);
        }

        /**
  * Returns a tuple. The first position tells if the key is contained and second tells the position where it is.
  * If it is not present the position where to insert the key is returned.
  * @param key key to check
  * @param minInd the minimum index where to look
  * @param maxInd the maximum index where to look
  * @return if there is an entry for that key or not.
  */
        inline std::tuple<bool, size_t> containsAndInd(const KEY_t &key, size_t min, size_t max) const {
            const size_t ind = insert_pos<KEY_t>(_keys, key, min, max);
            if (ind == (max + 1) or key != this->_keys[ind]) {
                return std::make_tuple(false, ind);
            } else {
                return std::make_tuple(true, ind);
            }
        }

        /**
         * Returns a tuple. The first position tells if the key is contained and second tells the position where it is.
         * If it is not present the position of the next smaller value is returned. If it would be before the first element,
         * SIZE_MAX is returned.
         * @param key key to check
         * @param minInd the minimum index where to look
         * @param maxInd the maximum index where to look
         * @return if there is an entry for that key or not.
         */
        inline std::tuple<bool, size_t> containsAndIndLower(const KEY_t &key, size_t min, size_t max) const {
            const size_t ind = insert_pos<KEY_t>(_keys, key, min, max);
            if (ind == (max + 1) or key != this->_keys[ind]) {
                if (ind == 0) {
                    return std::make_tuple(false, SIZE_MAX);
                } else {
                    return std::make_tuple(false, ind - 1);
                }
            } else {
                return std::make_tuple(true, ind);
            }
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
