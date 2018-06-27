#ifndef SPARSETENSOR_CONTAINER_VECMAP
#define SPARSETENSOR_CONTAINER_VECMAP

#include <vector>
#include <algorithm>
#include <tuple>
#include "BinarySearch.hpp"

namespace sparsetensor::container {

    /**
     * A Map that is based on two sorted vectors storing the entries.
     * @tparam KEY_t type of the key
     * @tparam VALUE_t type of the value
     */
    template<typename KEY_t, typename VALUE_t>
    class VecMap {
        /**
         * Minimum value of KEY_t.
         */
        constexpr const static KEY_t MIN_KEY = std::numeric_limits<KEY_t>::max();
        /**
         * Maximum value of KEY_t.
         */
        constexpr const static KEY_t MAX_KEY = std::numeric_limits<KEY_t>::max();
        /**
         * Minimum value of VALUE_t.
         */
        constexpr const static VALUE_t MAX_VAL = std::numeric_limits<VALUE_t>::max();
        /**
         * Maximum value of VALUE_t.
         */
        constexpr const static VALUE_t MIN_VAL = std::numeric_limits<VALUE_t>::max();

        /**
         * Vector string the keys.
         */
        std::vector<KEY_t> _keys{};
        /**
         * Vector storing the values.
         */
        std::vector<VALUE_t> _values{};
    public:

        /**
         * Instance of an empty set.
         */
        VecMap() {}

        /**
         * Get the minimum key currently stored.
         * @return the minimum key currently stored.
         */
        const KEY_t &min() const {
            if (_keys.size())
                return *_keys.cbegin();
            else
                return MAX_KEY;
        }

        /**
         * Get the maximum key currently stored.
         * @return the maximum key currently stored.
         */
        const KEY_t &max() const {
            if (_keys.size())
                return *_keys.crbegin();
            else
                return MIN_KEY;
        }

        /**
         * Set a value for a key. If a value is already set for that key it is replaced with the new one.
         * @param key key where to set the value
         * @param value value to be set
         */
        void setItem(const KEY_t &key, const VALUE_t &value) {
            size_t pos = insert_pos<KEY_t>(_keys, key);
            if (pos != _keys.size() and _keys[pos] == key) {
                _values[pos] = value;
            } else {
                _keys.insert(_keys.begin() + pos, key);
                _values.insert(_values.begin() + pos, value);
            }
        }

        /**
         * Deletes the entry for the given key. If there is no value for that key nothing happens.
         * @param key the key to the entry to be deleted.
         */
        void delItem(const KEY_t &key) {
            size_t pos = search<KEY_t>(_keys, key);
            if (pos != NOT_FOUND) {
                _keys.erase(_keys.begin() + pos);
                _values.erase(_values.begin() + pos);
            }
        }

        /**
         * Returns the values stored for key. Throws an exception if there is none.
         * @param key key to the value
         * @return the stored value
         * @throws std::out_of_range if the key is not contained
         */
        VALUE_t &at(const KEY_t &key) {
            size_t pos = search<KEY_t>(_keys, key);
            return _values.at(pos); // throws if result is out of range
        }

        /**
         * Returns the values stored for key. Returns NOT_FOUND if there is none.
         * @param key key to the value
         * @return the stored value or MAX_VAL
         */
        const VALUE_t &get(const KEY_t &key) const {
            size_t pos = search<KEY_t>(_keys, key);
            if (pos != NOT_FOUND) {
                return _values.at(pos);
            } else {
                return MAX_VAL;
            }
        }

        /**
         * Returns if an entry by the key is contained in the set.
         * @param key key to check
         * @return if there is an entry for that key or not.
         */
        inline bool contains(const KEY_t &key) const {
            return search<KEY_t>(_keys, key) != NOT_FOUND;
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
            if (ind == this->_keys.size() or key != this->_keys[ind]) {
                return std::make_tuple(false, ind);
            } else {
                return std::make_tuple(true, ind);
            }
        }

        inline std::tuple<bool, size_t> containsAndInd(const KEY_t &key) const {
            const size_t ind = insert_pos<KEY_t>(_keys, key);
            if (ind == this->_keys.size() or key != this->_keys[ind]) {
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
            if (ind == this->_keys.size() or key != this->_keys[ind]) {
                if (ind == 0) {
                    return std::make_tuple(false, SIZE_MAX);
                } else {
                    return std::make_tuple(false, ind - 1);
                }
            } else {
                return std::make_tuple(true, ind);
            }
        }

        inline std::tuple<bool, size_t> containsAndIndLower(const KEY_t &key) {
            const size_t ind = insert_pos<KEY_t>(_keys, key);
            if (ind == this->_keys.size() or key != this->_keys[ind]) {
                if (ind == 0) {
                    return std::make_tuple(false, SIZE_MAX);
                } else {
                    return std::make_tuple(false, ind - 1);
                }
            } else {
                return std::make_tuple(true, ind);
            }
        }

        /**
         * Returns a key by its internal index in the keys vector.
         * @param index index of the key.
         * @return the key
         * @throws std::out_of_range there is no key for that index, i.e. it is not 0 <= index < size()
         */
        inline const KEY_t &keyByInd(size_t index) const {
            return _keys.at(index);
        }

        /**
         * Returns a value by its internal index in the values vector.
         * @param index index of the value.
         * @return the value
         * @throws std::out_of_range there is no value for that index, i.e. it is not 0 <= index < size()
         */
        inline const VALUE_t &valByInd(size_t index) const {
            return _values.at(index);
        }

        /**
         * Number of entries.
         * @return number of entries.
         */
        inline size_t size() const {
            return _values.size();
        }


        inline const std::vector<KEY_t> &keys() const {
            return _keys;
        }

        inline const std::vector<VALUE_t> &values() const {
            return _values;
        }


    protected:
        /**
         * This is an non-public parent class for views on Keys and Entries. It allows to access only a range of entries.
         */
        class View {
        protected:
            /**
             * The map that is viewed.
             */
            const VecMap &map;
            /**
             * The minimum key that may be accessed.
             */
            KEY_t _min;
            /**
             * The maximum key that may be accessed.
             */
            KEY_t _max;
            /**
             * The index of the minimum key that may be accessed.
             */
            size_t _min_ind = MIN_SIZE_T;
            /**
             * The index of the maximum key that may be accessed.
             */
            size_t _max_ind = NOT_FOUND;
            /**
             * The number of elements in the view.
             */
            size_t _size;
        public:
            /**
             * Constructor without restricting the range.
             * @param map the VecMap to be viewed.
             */
            explicit View(const VecMap &map) : map{map}, _min{map.min()}, _max{map.max()}, _size{map.size()} {
                if (auto size = map.size(); size > 0) {
                    _min_ind = 0;
                    _max_ind = size - 1;
                }
            }

            /**
             * Constructor restricting the keys requested to min <= key <= max.
             * @param map the VecMap to be viewed.
             * @param min minimum key
             * @param max maximum key
             */
            View(const VecMap &map, KEY_t min, KEY_t max) :
                    map{map}, _min{min}, _max{max}, _size{map.size()} {
                if (_size != 0 and _min <= _max) { // check if view is empty
                    // get min value index
                    _min_ind = insert_pos<KEY_t>(map._keys, _min);
                    if (min != _size) {
                        // get max value index
                        _max_ind = insert_pos<KEY_t>(map._keys, _max, _min_ind);
                        // check if a higher value was found.
                        if (_max_ind != map.size()) {
                            if (auto actual_max = map.keyByInd(_max_ind); actual_max != _max) {
                                --_max_ind;
                            }
                        }
                        if (_min_ind <= _max_ind) {
                            // get actual min and max values
                            _min = map.keyByInd(_min_ind);
                            _max = map.keyByInd(_max_ind);
                            _size = _max_ind - _min_ind + 1;
                            return;
                        }
                    }
                }
                _min = MAX_SIZE_T;
                _max = MIN_SIZE_T;
                _min_ind = NOT_FOUND;
                _max_ind = NOT_FOUND;
                _size = 0;
            }

            /**
             * Get the minimum key currently visible.
             * @return the minimum key currently visible.
             */
            inline const KEY_t &min() const {
                return _min;
            }

            /**
             * Get the maximum key currently visible.
             * @return the maximum key currently visible.
             */
            inline const KEY_t &max() const {
                return _max;
            }

            /**
             * Get the index of the maximum key currently visible.
             * @return the index of the maximum key currently visible.
             */
            inline const size_t &minInd() const {
                return _min_ind;
            }

            /**
             * Get the index of the maximum key currently visible.
             * @return the index of the maximum key currently visible.
             */
            inline const size_t &maxInd() const {
                return _max_ind;
            }

            /**
             * Returns the values stored for key. Throws an exception if there is none.
             * @param key key to the value
             * @return the stored value
             * @throws std::out_of_range if the key is not contained
             */
            const VALUE_t &at(const KEY_t &key) const {
                size_t pos = search<KEY_t>(map._keys, key, _min_ind, _max_ind);
                // throws if result is out of range
                return map._values.at(pos);

            }

            /**
             * Returns the values stored for key. Returns NOT_FOUND if there is none.
             * @param key key to the value
             * @return the stored value or MAX_VAL
             */
            const VALUE_t &get(const KEY_t &key) const {
                size_t pos = search<KEY_t>(map._keys, key, _min_ind, _max_ind);
                if (pos != NOT_FOUND)
                    return map._keys.at(pos);
                else
                    return MAX_VAL;
            }

            /**
             * Returns a value by its internal index in the values vector.
             * @param index index of the value.
             * @return the value
             * @throws std::out_of_range there is no value for that index, i.e. it is not 0 <= index < size()
             */
            inline const KEY_t &keyByInd(size_t index) const {
                if (_min_ind <= index and index <= _max_ind)
                    return map._keys.at(index);
                else
                    throw std::out_of_range{"not in view."};
            }

            /**
             * Returns a value by its internal index in the values vector.
             * @param index index of the value.
             * @return the value
             * @throws std::out_of_range there is no value for that index, i.e. it is not 0 <= index < size()
             */
            inline const VALUE_t &valByInd(size_t index) const {
                if (_min_ind <= index and index <= _max_ind)
                    return map._values.at(index);
                else
                    throw std::out_of_range{"not in view."};
            }

            /**
             * Number of entries in the view.
             * @return number of entries.
             */
            inline const size_t &size() const {
                return _size;
            }


            /**
             * Returns if an entry by the key is contained in the set.
             * @param key key to check
             * @return if there is an entry for that key or not.
             */
            bool contains(const KEY_t &key) {
                return search<KEY_t>(map._keys, key, _min_ind, _max_ind) != NOT_FOUND;
            }
        };

    public:
        /**
         * Provides an iterable view of the items (key-value-pairs) of a VecMap. The viewed key range can be restricted.
         */
        class ItemView : public View {
        public:
            explicit ItemView(const VecMap &map) : View{map} {}

            ItemView(const VecMap &map, KEY_t min, KEY_t max) : View{map, min, max} {
            }

            class iterator {
                ItemView &view;
                size_t pos;
            public:
                explicit iterator(ItemView &itemView, size_t pos = 0) : view{itemView}, pos{pos} {}

                iterator &operator++() {
                    if (pos < view.size())
                        ++pos;
                    return *this;
                }

                iterator operator++(int) {
                    operator++();
                    return *this;
                }

                std::tuple<KEY_t, VALUE_t> operator*() {
                    return std::make_tuple(view.map.keyByInd(view._min_ind + pos),
                                           view.map.valByInd(view._min_ind + pos));
                }

                bool operator==(const iterator &rhs) const {
                    return ((*rhs.view == *view) and
                            (rhs.pos == pos or
                             (rhs.pos > view._min_ind and pos < view._min_ind)));
                }

                bool operator!=(const iterator &rhs) const {
                    return not this->operator==(rhs);
                }

            };

            iterator begin() {
                return iterator{*this};
            }

            iterator end() {
                return iterator{*this, size()};
            }

        };

        ItemView itemView(KEY_t min, KEY_t max) {
            return ItemView{*this, min, max};
        }

        class KeyView : public View {
        public:
            explicit KeyView(const VecMap &map) : View{map} {}

            KeyView(const VecMap &map, KEY_t min, KEY_t max) : View{map, min, max} {
            }

            class iterator {
                KeyView &view;
                size_t pos;
            public:

                explicit iterator(KeyView &itemView, size_t pos = 0) : view{itemView}, pos{pos} {}

                iterator &operator++() {
                    if (pos < view.size())
                        ++pos;
                    return *this;
                }

                iterator operator++(int) {
                    operator++();
                    return *this;
                }

                KEY_t operator*() {
                    return view.keyByInd(view._min_ind + pos);
                }

                bool operator==(const iterator &rhs) const {
                    return ((*rhs.view == *view) and
                            (rhs.pos == pos or
                             (rhs.pos > view._min_ind and pos < view._min_ind)));
                }

                bool operator!=(const iterator &rhs) const {
                    return not this->operator==(rhs);
                }

            };

            iterator begin() {
                return iterator{*this};
            }

            iterator end() {
                return iterator{*this, size()};
            }

        };

        KeyView keyView(KEY_t min, KEY_t max) {
            return KeyView{*this, min, max};
        }

        typename KeyView::iterator lower_bound(KEY_t min_) {
            return KeyView{*this, min_, max()}.begin();
        }

        typename KeyView::iterator upper_bound(KEY_t max_) {
            return KeyView{*this, min(), max_}.begin();
        }

        typename KeyView::iterator begin() {
            return KeyView{*this}.begin();
        }

        typename KeyView::iterator end() {
            return KeyView{*this}.end();
        }


    };
}

#endif //SPARSETENSOR_CONTAINER_VECMAP
