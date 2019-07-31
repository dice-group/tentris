#ifndef HYPERTRIE_STDMAP_HPP
#define HYPERTRIE_STDMAP_HPP

#include <map>

namespace hypertrie::internal::container {
    template<typename key_type, typename value>
    using std_map = std::map
            <
                    key_type,
                    value
            >;
}

#endif //HYPERTRIE_HYPERTRIE_STDMAP_HPPBOOSTFLATMAP_HPP
