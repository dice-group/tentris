#ifndef TEST_NDMAP_HPP
#define TEST_NDMAP_HPP


#include <map>

#include "tnt/util/Types.hpp"

namespace tnt::util::container {
    using Key_t = tnt::util::types::Key_t;

    template<typename VALUE_t>
    using NDMap = std::map<Key_t, VALUE_t>;

};
#endif //TEST_NDMAP_HPP
