#ifndef HYPERTRIE_TSLSET_HPP
#define HYPERTRIE_TSLSET_HPP

#include <tsl/sparse_set.h>

namespace hypertrie::internal::container {

    template<typename Key>
    using tsl_sparse_set = tsl::sparse_set<
            Key,
            absl::Hash<Key>,
            std::equal_to<Key>,
            std::allocator<Key>,
            tsl::sh::power_of_two_growth_policy<2>,
            tsl::sh::exception_safety::basic,
            tsl::sh::sparsity::high
            >;
}
#endif //HYPERTRIE_TSLSET_HPP
