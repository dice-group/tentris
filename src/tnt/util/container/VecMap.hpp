#ifndef SPARSETENSOR_CONTAINER_VECMAP
#define SPARSETENSOR_CONTAINER_VECMAP

#include <vector>
#include <algorithm>
#include <tuple>

#include "BinarySearch.hpp"

namespace tnt::util::container {

    /**
     * A Map that is based on two sorted vectors storing the entries.
     * @tparam KEY_t type of the key
     * @tparam VALUE_t type of the value
     */
    template<typename KEY_t, typename VALUE_t>
    class VecMap {
    public:
        /**
         * Minimum value of KEY_t.
         */
        constexpr const static KEY_t MIN_KEY = std::numeric_limits<KEY_t>::min();
        /**
         * Maximum value of KEY_t.
         */
        constexpr const static KEY_t MAX_KEY = std::numeric_limits<KEY_t>::max();
        /**
         * Maximum value of VALUE_t.
         */
        constexpr const static VALUE_t MIN_VAL = std::numeric_limits<VALUE_t>::min();
        /**
         * Minimum value of VALUE_t.
         */
        constexpr const static VALUE_t MAX_VAL = std::numeric_limits<VALUE_t>::max();


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
         * Get the minimum key currently stored. If there is no element return MAX_KEY, empty set is (MAX_KEY, MIN_KEY).
         * @return the minimum key currently stored.
         */
        const KEY_t &min() const noexcept {
            if (_keys.size())
                return *_keys.cbegin();
            else
                return MAX_KEY;
        }

        /**
         * Get the maximum key currently stored. If there is no element return MIN_KEY, empty set is (MAX_KEY, MIN_KEY).
         * @return the maximum key currently stored.
         */
        const KEY_t &max() const noexcept {
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
        void setItem(const KEY_t &key, const VALUE_t &value) noexcept {
            size_t pos = insert_pos<KEY_t>(_keys, key);
            if (pos != _keys.size() and _keys.at(pos) == key) {
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
        void delItem(const KEY_t &key) noexcept {
            size_t pos = search<KEY_t>(_keys, key);
            if (pos != NOT_FOUND) {
                _keys.erase(_keys.begin() + pos);
                _values.erase(_values.begin() + pos);
            }
        }


        /**
         * Returns the values stored for key. Throws an exception if there is none.
         * @param key key to the value
         * @param low_ind minimum index (inclusive) to look for the value
         * @param high_ind maximum index (exclusive) to look for the value
         * @return the stored value
         * @throws std::out_of_range if the key is not contained in min_ind , max_ind
         */
        const VALUE_t &at(const KEY_t &key, const size_t low_ind, const size_t high_ind) const {
            size_t pos = search<KEY_t>(_keys, low_ind, high_ind, key); //+ 1 is moved to the usages
            // throws if result is out of range
            return _values.at(pos);
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
         * Returns the values stored for key in min_ind to max_ind domain. Returns NOT_FOUND if there is none.
         * @param key key to the value
         * @param low_ind minimum index (inclusive) to start search for the given key
         * @param high_ind maximum index (exclusive) to start search for the given key
         * @return the stored value or MAX_VAL
         */
        const VALUE_t &get(const KEY_t &key, const size_t low_ind, const size_t high_ind) const noexcept {
            size_t pos = search<KEY_t>(_keys, low_ind, high_ind, key); //+ 1 is moved to the usages
            if (pos != NOT_FOUND)
                return _values.at(pos);
            else
                return MAX_VAL;
        }


        /**
         * Returns the values stored for key. Returns NOT_FOUND if there is none.
         * @param key key to the value
         * @return the stored value or MAX_VAL
         */
        const VALUE_t &get(const KEY_t &key) const noexcept {
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
        inline bool contains(const KEY_t &key) const noexcept {
            return search<KEY_t>(_keys, key) != NOT_FOUND;
        }

        /**
         * Returns a tuple. The first position tells if the key is contained and second tells the position where it is.
         * If it is not present the position where to insert the key is returned.
         * @param key key to check
         * @param min_ind the minimum index where to look
         * @param max_ind the maximum index where to look todo take care of it usages
         * @return if there is an entry for that key or not.
         */
         //todo it can be replaced by search function
        inline std::tuple<bool, size_t>
        containsAndInd(const KEY_t &key, size_t min_ind, size_t max_ind) const noexcept {
            const size_t ind = insert_pos<KEY_t>(_keys, min_ind, max_ind, key); //todo +1 should moved to the usages
            if (ind == (max_ind + 1) or key != this->_keys.at(ind)) {
                return std::make_tuple(false, ind);
            } else {
                return std::make_tuple(true, ind);
            }
        }

        //todo it can be replaced by search function
        inline std::tuple<bool, size_t> containsAndInd(const KEY_t &key) const noexcept {
            const size_t ind = insert_pos<KEY_t>(_keys, key);
            if (ind == this->_keys.size() or key != this->_keys.at(ind)) {
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
         * @param maxInd the maximum index where to look todo take care of it usages
         * @return if there is an entry for that key or not.
         */
        //todo it can be replaced by search function
        std::tuple<bool, size_t> containsAndIndLower(const KEY_t &key, size_t min, size_t max) const noexcept {
            const size_t ind = insert_pos<KEY_t>(_keys, min, max, key);//todo +1 should moved to the useages
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
        inline size_t size() const noexcept {
            return _values.size();
        }


        inline const std::vector<KEY_t> &keys() const noexcept {
            return _keys;
        }

        inline const std::vector<VALUE_t> &values() const noexcept {
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
            size_t _min_ind = SIZE_MAX;
            /**
             * The index of the maximum key that may be accessed.
             */
            size_t _max_ind = 0;
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
                if (_size > 0) {
                    _min_ind = 0;
                    _max_ind = _size - 1;
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
                    if (min != map._keys.size()) {
                        // get max value index
                        _max_ind = insert_pos<KEY_t>(map._keys, _min_ind, _max);
                        // check if a higher value was found.
                        if (_max_ind != map._keys.size()) {
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
                _min = MAX_KEY;
                _max = MIN_KEY;
                _min_ind = SIZE_MAX;
                _max_ind = 0;
                _size = 0;
            }

            bool keyInRange(const KEY_t &key) const noexcept {
                return (_min <= key and key <= _max);
            }

            bool indexInRange(const size_t &ind) const noexcept {
                return (_min_ind <= ind and ind <= _max_ind);
            }

            /**
             * Get the minimum key currently visible.
             * @return the minimum key currently visible.
             */
            inline const KEY_t &min() const noexcept {
                return _min;
            }

            /**
             * Get the maximum key currently visible.
             * @return the maximum key currently visible.
             */
            inline const KEY_t &max() const noexcept {
                return _max;
            }

            /**
             * Get the index of the maximum key currently visible.
             * @return the index of the maximum key currently visible.
             */
            inline const size_t &minInd() const noexcept {
                return _min_ind;
            }

            /**
             * Get the index of the maximum key currently visible.
             * @return the index of the maximum key currently visible.
             */
            inline const size_t &maxInd() const noexcept {
                return _max_ind;
            }


            /**
             * Returns the values stored for key. Throws an exception if there is none.
             * @param key key to the value
             * @return the stored value
             * @throws std::out_of_range if the key is not contained
             */
            const VALUE_t &at(const KEY_t &key) const {
                if (keyInRange(key))
                    return map.at(key, _min_ind, _max_ind + 1);
                else
                    // throws if result is out of range
                    throw std::out_of_range("key is not in range of view.");


            }


            /**
             * Returns the values stored for key. Returns NOT_FOUND if there is none.
             * @param key key to the value
             * @return the stored value or MAX_VAL
             */
            const VALUE_t &get(const KEY_t &key) const noexcept {
                return map.get(key, _min_ind, _max_ind + 1);
            }

            /**
             * Returns a value by its internal index in the values vector.
             * @param index index of the value.
             * @return the value
             * @throws std::out_of_range there is no value for that index, i.e. it is not 0 <= index < size()
             */
            const KEY_t &keyByInd(size_t index) const {
                if (indexInRange(index))
                    return map._keys.at(index);
                else
                    throw std::out_of_range{"index is not in range of view."};
            }

            /**
             * Returns a value by its internal index in the values vector.
             * @param index index of the value.
             * @return the value
             * @throws std::out_of_range there is no value for that index, i.e. it is not 0 <= index < size()
             */
            const VALUE_t &valByInd(size_t index) const {
                if (indexInRange(index))
                    return map._values.at(index);
                else
                    throw std::out_of_range{"index is not in range of view."};
            }

            /**
             * Number of entries in the view.
             * @return number of entries.
             */
            inline const size_t &size() const noexcept {
                return _size;
            }


            /**
             * Returns if an entry by the key is contained in the set.
             * @param key key to check
             * @return if there is an entry for that key or not.
             */
            bool contains(const KEY_t &key) noexcept {
                return search<KEY_t>(map._keys, _min_ind, _max_ind + 1, key) != NOT_FOUND;
            }

            bool operator==(const View &rhs) const {
                return std::tie(map, _min, _max) == std::tie(rhs.map, rhs._min, rhs._max);
            }

            bool operator!=(const View &rhs) const {
                return !(rhs == *this);
            }
        };

    public:
        /**
         * Provides an iterable view of the items (key-value-pairs) of a VecMap. The viewed key range can be restricted.
         */
         // TODO: do not test as it is currently not used
        class ItemView : public View {
        public:
            explicit ItemView(const VecMap &map) : View{map} {}

            ItemView(const VecMap &map, KEY_t min, KEY_t max) : View{map, min, max} {}

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

                std::tuple<KEY_t &, VALUE_t &> operator*() {
                    return std::make_tuple(view.map.keyByInd(view._min_ind + pos),
                                           view.map.valByInd(view._min_ind + pos));
                }

                bool operator==(const iterator &rhs) const {
                    return (rhs.pos == pos or
                            (rhs.pos > rhs.view._min_ind and pos > view._min_ind));
                }

                bool operator!=(const iterator &rhs) const {
                    return not this->operator==(rhs);
                }

            };

            iterator begin() noexcept {
                return iterator{*this};
            }

            iterator end() noexcept {
                return iterator{*this, size()};
            }


        };

        // TODO: do not test as it is currently not used
        ItemView itemView() {
            return ItemView{*this};
        }

        // TODO: do not test as it is currently not used
        ItemView itemView(KEY_t min) {
            min = (min >= this->min()) ? min : this->min();
            return ItemView{*this, min, max()};
        }

        // TODO: do not test as it is currently not used
        ItemView itemView_max(KEY_t max) {
            max = (max <= this->max()) ? max : this->max();
            return ItemView{*this, min(), max};
        }

        // TODO: do not test as it is currently not used
        ItemView itemView(KEY_t min, KEY_t max) {
            min = (min >= this->min()) ? min : this->min();
            max = (max <= this->max()) ? max : this->max();
            return ItemView{*this, min, max};
        }

        // TODO: do not test as it is currently not used
        class KeyView : public View {
        public:
            explicit KeyView(const VecMap &map) : View{map} {}

            KeyView(const VecMap &map, KEY_t min, KEY_t max) : View{map, min, max} {}

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

                KEY_t &operator*() {
                    return view.keyByInd(view._min_ind + pos);
                }

                bool operator==(const iterator &rhs) const {
                    return (rhs.pos == pos or
                            (rhs.pos > rhs.view._min_ind and pos > view._min_ind));
                }

                bool operator!=(const iterator &rhs) const {
                    return not this->operator==(rhs);
                }

            };

            iterator begin() noexcept {
                return iterator{*this};
            }

            iterator end() noexcept {
                return iterator{*this, size()};
            }

        };

        // TODO: do not test as it is currently not used
        KeyView keyView() {
            return KeyView{*this};
        }

        // TODO: do not test as it is currently not used
        KeyView keyView(KEY_t min) {
            min = (min >= this->min()) ? min : this->min();
            return KeyView{*this, min, max()};
        }

        // TODO: do not test as it is currently not used
        KeyView keyView_max(KEY_t max) {
            max = (max <= this->max()) ? max : this->max();
            return KeyView{*this, min(), max};
        }

        // TODO: do not test as it is currently not used
        KeyView keyView(KEY_t min, KEY_t max) {
            min = (min >= this->min()) ? min : this->min();
            max = (max <= this->max()) ? max : this->max();
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
