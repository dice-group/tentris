#ifndef SPARSETENSOR_CONTAINER_VECMAP
#define SPARSETENSOR_CONTAINER_VECMAP

#include <vector>
#include <algorithm>
#include "container/BinarySearch.hpp"

namespace sparsetensor::container {
    constexpr const size_t min_size_t = std::numeric_limits<size_t>::min();
    constexpr const size_t max_size_t = std::numeric_limits<size_t>::max();

    template<typename KEY_t, typename VALUE_t>
    class VecMap {

        std::vector<KEY_t> keys{};
        std::vector<VALUE_t> values{};
    public:
        VecMap() {};

        const KEY_t &minKey() {
            if (keys.size())
                return *keys.cbegin();
            else
                return std::numeric_limits<KEY_t>::max();
        }

        const KEY_t &maxKey() {
            if (keys.size())
                return *keys.crbegin();
            else
                return std::numeric_limits<KEY_t>::min();
        }

        void setItem(const KEY_t &key, const VALUE_t &value) {
            size_t pos = bin_search_insert_pos<KEY_t>(keys, key);
            if (pos != keys.size() and keys[pos] == key) {
                values[pos] = value;
            } else {
                keys.insert(keys.begin() + pos, key);
                values.insert(values.begin() + pos, value);
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
            return values.at(pos);
        }

        void delItem(const KEY_t &key) {
            size_t pos = bin_search_insert_pos<KEY_t>(keys, key);
            if (pos != keys.size() && keys[pos] == key) {
                keys.erase(keys.begin() + pos);
                values.erase(values.begin() + pos);
            }
        }

        /**
         *
         * @param key
         * @return the value or a null-pointer if it doesn't exist
         */
        VALUE_t &get(const KEY_t &key) const {
            size_t pos = search<KEY_t>(keys, key);
            if (pos != std::numeric_limits<size_t>::max()) {
                return values.at(pos);
            } else {
                return nullptr;
            }
        }

        bool contains(const KEY_t &key) {
            return search<KEY_t>(keys, key) != std::numeric_limits<size_t>::max();
        }

        KEY_t keyByInd(size_t index) const {
            return keys.at(index);
        }

        VALUE_t valByInd(size_t index) const {
            return values.at(index);
        }

        inline size_t size() const {
            return values.size();
        }


        class ItemView {
            const VecMap &map;
            KEY_t _min;
            KEY_t _max;
            size_t _min_ind = min_size_t;
            size_t _max_ind = max_size_t;
            size_t _size;
        public:
            explicit ItemView(const VecMap &map, KEY_t min, KEY_t max) :
                    map{map}, _min{min}, _max{max}, _size{map.size()} {
                if (not _size == 0 and not min > max) {
                    this->_min_ind = search<KEY_t>(map.keys, min);
                    if (min != _size) {
                        _max_ind = search_upper<KEY_t>(map.keys, min, _min_ind);
                        if (_min_ind <= _max_ind) {
                            this->_min = map.keyByInd(_min_ind);
                            this->_max = map.keyByInd(_max_ind);
                            this->_size = this->_max_ind - this->_min_ind + 1;
                            return;
                        }
                    }
                }
                this->_min = std::numeric_limits<KEY_t>::max();
                this->_max = std::numeric_limits<KEY_t>::min();
                this->_min_ind = max_size_t;
                this->_max_ind = max_size_t;
                this->_size = 0;
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

            inline const size_t &size() const {
                return _max_ind;
            }

            bool contains(const KEY_t &key) {
                return search<KEY_t>(map.keys, key, _min_ind, _max_ind) != map.size();
            }

            class iterator {
                ItemView &view;
                size_t pos;

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
                             (rhs.pos > view._min_ind and pos < view._min_ind)))
                    return true;
                }

                bool operator!=(const iterator &rhs) const {
                    return not this->operator==(rhs);
                }

            };

        };

    };
}

#endif //SPARSETENSOR_CONTAINER_VECMAP
