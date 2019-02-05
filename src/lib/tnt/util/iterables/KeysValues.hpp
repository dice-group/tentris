#ifndef SPARSETENSOR_UTIL_KEYSVALUE
#define SPARSETENSOR_UTIL_KEYSVALUE

#include <iterator>

namespace tnt::util::iterable::keys {
    template<class Associative>
    class Keys {
        Associative &associative;
    public:
        Keys(Associative &associative) : associative(associative) {}

        decltype(auto) begin() const { return iterator<typename Associative::const_iterator>(associative.begin()); }

        decltype(auto) end() const { return iterator<typename Associative::const_iterator>(associative.end()); }

        template<class Iterator>
        class iterator {
            using iter_key_t = std::remove_const_t<typename std::iterator_traits<Iterator>::value_type::first_type>;
            Iterator iter;
        public:
            iterator(Iterator iter) : iter(iter) {}

            iterator &operator++() {
                ++iter;
                return *this;
            }

            bool operator!=(const iterator &other) const {
                return iter != other.iter;
            }

            const iter_key_t &operator*() {
                return iter->first;
            }
        };
    };
};

namespace tnt::util::iterable::values {
    template<class Associative>
    class Values {
        Associative &associative;
    public:
        Values(Associative &associative) : associative(associative) {}

        decltype(auto) begin() const { return iterator<typename Associative::const_iterator>{associative.begin()}; }

        decltype(auto) end() const { return iterator<typename Associative::const_iterator>{associative.end()}; }

        template<class Iterator>
        class iterator {
            using iter_val_t =  std::decay_t<typename  std::iterator_traits<Iterator>::value_type::second_type>;
            Iterator iter;
        public:
            iterator(Iterator iter) : iter{iter} {}

            iterator &operator++() {
                ++iter;
                return *this;
            }

            bool operator!=(const iterator &other) const {
                return iter != other.iter;
            }

            const iter_val_t &operator*() {
                return iter->second;
            }
        };
    };
};

template<class Associative>
decltype(auto) keys(Associative &associative) {
    return tnt::util::iterable::keys::Keys(associative);
}

template<class Associative>
decltype(auto) values(Associative &associative) {
    return tnt::util::iterable::values::Values(associative);
}


#endif //SPARSETENSOR_UTIL_KEYSVALUE
