#ifndef SPARSETENSOR_UTIL_ENUMERATE_HPP
#define SPARSETENSOR_UTIL_ENUMERATE_HPP

#include <cstdio>
#include <iterator>
#include <utility>
#include <tuple>

template<class Iterator>
class enumerate_iterator {
    using iterator = Iterator;
    using index_type = typename std::iterator_traits<iterator>::difference_type;
    using reference = typename std::iterator_traits<iterator>::reference;

    index_type index;
    iterator iter;
public:
    enumerate_iterator(iterator iterator) : index{}, iter(iterator) {}

    enumerate_iterator &operator++() {
        ++index;
        ++iter;
        return *this;
    }

    bool operator!=(const enumerate_iterator &other) const {
        return iter != other.iter;
    }

    std::tuple<index_type &, reference &> operator*() {
        return std::forward_as_tuple(index, *iter);
    }
};

template<class Iterator>
class enumerate_range {
    // using index_type = typename std::iterator_traits<Iterator>::difference_type;
    using index_type = size_t;
    using iterator = enumerate_iterator<Iterator>;
    Iterator first;
    Iterator last;
    index_type initial;
public:

    enumerate_range(Iterator first, Iterator last, index_type initial)
            : first(first), last(last), initial(initial) {}

    iterator begin() const { return iterator(first); }

    iterator end() const { return iterator(last); }

};
template<class Container>
decltype(auto) enumerate(Container &content) {
    return enumerate_range(std::begin(content), std::end(content), 0);
}


#endif //SPARSETENSOR_UTIL_ENUMERATE_HPP
