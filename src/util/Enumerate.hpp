#ifndef SPARSETENSOR_UTIL_ENUMERATE_HPP
#define SPARSETENSOR_UTIL_ENUMERATE_HPP

#include <iterator>
#include <tuple>

namespace sparsetensor::util::enumerate {
    template<class Container>
    class Enumerate {
        Container &content;
    public:
        Enumerate(Container &content) : content(content) {}

        decltype(auto) begin() const { return iterator(content.begin()); }

        decltype(auto) end() const { return iterator(content.end()); }

        template<class Iterator>
        class iterator {
            using ref_t = typename std::iterator_traits<Iterator>::reference;
            size_t index;
            Iterator iter;
        public:
            iterator(Iterator iter) : index{}, iter(iter) {}

            iterator &operator++() {
                ++index;
                ++iter;
                return *this;
            }

            bool operator!=(const iterator &other) const {
                return iter != other.iter;
            }

            std::tuple<size_t &, ref_t> operator*() {
                return std::forward_as_tuple(index, *iter);
            }
        };
    };
};

template<class Container>
decltype(auto) enumerate(Container &content) {
    return sparsetensor::util::enumerate::Enumerate(content);
}


#endif //SPARSETENSOR_UTIL_ENUMERATE_HPP
