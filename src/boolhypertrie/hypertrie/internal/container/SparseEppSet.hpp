#ifndef HYPERTRIE_EPPSET_HPP
#define HYPERTRIE_EPPSET_HPP

#include <sparsepp/spp.h>



namespace hypertrie::internal::container {

    template<typename Key>
    using epp_sparse_set = spp::sparse_hash_set<
            Key,
            absl::Hash<Key>
            >;
}
#endif //HYPERTRIE_EPPSET_HPP
