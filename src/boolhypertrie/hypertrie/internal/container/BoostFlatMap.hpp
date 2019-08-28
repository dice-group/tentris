#ifndef HYPERTRIE_BOOSTFLATMAP_HPP
#define HYPERTRIE_BOOSTFLATMAP_HPP

#include <boost/container/flat_map.hpp>

namespace hypertrie::internal::container {
    template<typename key_type, typename value>
    using boost_flat_map = boost::container::flat_map
            <
                    key_type,
                    value,
                    std::less<key_type>,
                    boost::container::new_allocator<std::pair<key_type, value>>
            >;
}

#endif //HYPERTRIE_BOOSTFLATMAP_HPP
