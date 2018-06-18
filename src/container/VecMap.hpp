#ifndef SPARSETENSOR_CONTAINER_VECMAP
#define SPARSETENSOR_CONTAINER_VECMAP

#include <vector>
#include <algorithm>
#include "BinarySearch.hpp"

namespace sparsetensor::container {

    template<typename KEY_t, typename VALUE_t>
    class VecMap {
        constexpr const static size_t MAX_SIZE_T = std::numeric_limits<size_t>::max();
        constexpr const static size_t MIN_SIZE_T = std::numeric_limits<size_t>::min();
        constexpr const static KEY_t MIN_KEY = std::numeric_limits<KEY_t>::max();
        constexpr const static KEY_t MAX_KEY = std::numeric_limits<KEY_t>::max();
        constexpr const static VALUE_t MAX_VAL = std::numeric_limits<VALUE_t>::max();
        constexpr const static VALUE_t MIN_VAL = std::numeric_limits<VALUE_t>::max();


    public:
        std::vector<KEY_t> keys{};
        std::vector<VALUE_t> values{};

        VecMap() {};

        const KEY_t &min() const {
            if (keys.size())
                return *keys.cbegin();
            else
                return MAX_KEY;
        }

        const KEY_t &max() const {
            if (keys.size())
                return *keys.crbegin();
            else
                return MIN_KEY;
        }

        void setItem(const KEY_t &key, const VALUE_t &value) {
            size_t pos = insert_pos<KEY_t>(keys, key);
            if (pos != keys.size() and keys[pos] == key) {
                values[pos] = value;
            } else {
                keys.insert(keys.begin() + pos, key);
                values.insert(values.begin() + pos, value);
            }
        }

        void delItem(const KEY_t &key) {
            size_t pos = search<KEY_t>(keys, key);
            if (pos != NOT_FOUND) {
                keys.erase(keys.begin() + pos);
                values.erase(values.begin() + pos);
            }
        }

        /**
         *
         * @param key
         * @return
         * @throws std::out_of_range if the key is not contained
         */
        VALUE_t &at(const KEY_t &key) {
            size_t pos = search<KEY_t>(keys, key);
            return values.at(pos); // throws if result is out of range
        }

        /**
         *
         * @param key
         * @return the value or the max value of VALUE_t if it doesn't exist
         */
        const VALUE_t &get(const KEY_t &key) const {
            size_t pos = search<KEY_t>(keys, key);
            if (pos != NOT_FOUND) {
                return values.at(pos);
            } else {
                return MAX_VAL;
            }
        }

        inline bool contains(const KEY_t &key) {
            return search<KEY_t>(keys, key) != NOT_FOUND;
        }

        inline const KEY_t &keyByInd(size_t index) const {
            return keys.at(index);
        }

        inline const VALUE_t &valByInd(size_t index) const {
            return values.at(index);
        }

        inline size_t size() const {
            return values.size();
        }

    protected:
        class View {
        protected:
            const VecMap &map;
            KEY_t _min;
            KEY_t _max;
            size_t _min_ind = MIN_SIZE_T;
            size_t _max_ind = NOT_FOUND;
            size_t _size;
        public:
            explicit View(const VecMap &map) : map{map}, _min{map.min()}, _max{map.max()}, _size{map.size()} {
                if (auto size = map.size(); size > 0) {
                    _min_ind = 0;
                    _max_ind = size - 1;
                }
            }

            View(const VecMap &map, KEY_t min, KEY_t max) :
                    map{map}, _min{min}, _max{max}, _size{map.size()} {
                if (_size != 0 and _min <= _max) { // check if view is empty
                    // get min value index
                    _min_ind = insert_pos<KEY_t>(map.keys, _min);
                    if (min != _size) {
                        // get max value index
                        _max_ind = insert_pos<KEY_t>(map.keys, _max, _min_ind);
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

            inline const KEY_t &min() const {
                return _min;
            }

            inline const KEY_t &max() const {
                return _max;
            }

            inline const size_t &minInd() const {
                return _min_ind;
            }

            inline const size_t &maxInd() const {
                return _max_ind;
            }

            const VALUE_t &at(const KEY_t &key) const {
                size_t pos = insert_pos<KEY_t>(map.keys, key);
                // throws if result is out of range
                if (_min_ind <= pos <= _max_ind)
                    return map.values.at(pos);
                else
                    throw "Out of Range";

            }

            /**
             *
             * @param key
             * @return the value or the max value of VALUE_t if it doesn't exist
             */
            const VALUE_t &get(const KEY_t &key) const {
                size_t pos = search<KEY_t>(map.keys, key);
                if (_min_ind <= pos <= _max_ind) {
                    return map.values.at(pos);
                } else {
                    return MAX_VAL;
                }
            }

            inline const KEY_t &keyByInd(size_t index) const {
                if (_min_ind <= index <= _max_ind)
                    return map.keys.at(index);
                else
                    throw "Out of Range";
            }

            inline const VALUE_t &valByInd(size_t index) const {
                if (_min_ind <= index <= _max_ind)
                    return map.values.at(index);
                else
                    throw "Out of Range";
            }

            inline const size_t &size() const {
                return _size;
            }

            bool contains(const KEY_t &key) {
                return search<KEY_t>(map.keys, key, _min_ind, _max_ind) != NOT_FOUND;
            }
        };

    public:

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

        typename KeyView::iterator keyView(KEY_t min, KEY_t max) {
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
