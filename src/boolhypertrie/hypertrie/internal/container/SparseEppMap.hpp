#ifndef HYPERTRIE_EPPMAP_HPP
#define HYPERTRIE_EPPMAP_HPP

#include <sparsepp/spp.h>
#include <absl/hash/hash.h>

namespace hypertrie::internal::container {

    template<typename Key, typename T>
    using epp_sparse_map  =  spp::sparse_hash_map<Key,
            T,
		    absl::Hash<Key>
            >;
}
#endif //HYPERTRIE_EPPMAP_HPP
