#ifndef TEST_NDMAP_HPP
#define TEST_NDMAP_HPP

#include "../tensor/Types.hpp"

#include <map>

namespace sparsetensor::container {
    using Key_t = sparsetensor::tensor::Key_t;

    template<typename VALUE_t>
    using NDMap = std::map<Key_t, VALUE_t>;

};
#endif //TEST_NDMAP_HPP
