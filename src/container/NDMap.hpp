#ifndef TEST_NDMAP_HPP
#define TEST_NDMAP_HPP

#import "../tensor/Types.hpp"
#import <tuple>
#import <map>

namespace sparsetensor::container {
    using Key_t = sparsetensor::tensor::Key_t;

    template<typename VALUE_t>
    using NDMap<VALUE_t> = typedef std::map<Key_t, VALUE_t>;

};
#endif //TEST_NDMAP_HPP
