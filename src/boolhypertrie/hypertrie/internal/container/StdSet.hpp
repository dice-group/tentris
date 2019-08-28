#ifndef HYPERTRIE_STDSET_HPP
#define HYPERTRIE_STDSET_HPP

#include <set>

namespace hypertrie::internal::container {
    template<typename key_type>
    using std_set = std::set
            <
                    key_type
            >;
}

#endif //HYPERTRIE_STDSET_HPP
