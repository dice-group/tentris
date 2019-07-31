#ifndef HYPERTRIE_TSLMAP_HPP
#define HYPERTRIE_TSLMAP_HPP

#include <tsl/sparse_map.h>

namespace hypertrie::internal::container {

    template<typename Key, typename T>
    using tsl_sparse_map  =  tsl::sparse_map<Key,
            T,
            std::hash<Key>,
            std::equal_to<Key>,
            std::allocator<std::pair<Key, T>>,
            tsl::sh::power_of_two_growth_policy<2>,
            tsl::sh::exception_safety::basic,
            tsl::sh::sparsity::high
            >;
}
#endif //HYPERTRIE_TSLMAP_HPP
