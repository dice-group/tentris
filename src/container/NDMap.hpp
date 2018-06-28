#ifndef TEST_NDMAP_HPP
#define TEST_NDMAP_HPP


#include <map>

#include "../util/All.hpp"

namespace sparsetensor::container {
    using Key_t = sparsetensor::util::types::Key_t;

    template<typename VALUE_t>
    using NDMap = std::map<Key_t, VALUE_t>;

};
#endif //TEST_NDMAP_HPP
