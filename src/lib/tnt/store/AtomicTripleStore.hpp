#ifndef TNT_ATOMIC_TRIPLE_STORE
#define TNT_ATOMIC_TRIPLE_STORE


#include "tnt/util/SingletonFactory.hpp"
#include "tnt/store/TripleStore.hpp"
#include "tnt/store/config/AtomicTripleStoreConfig.cpp"

namespace {
    using namespace tnt::store;
    using namespace tnt::util::sync;
	using namespace tnt::store::config;
}

namespace tnt::util::sync {
	template <>
	inline TripleStore * SingletonFactory<TripleStore>::make_instance(){
		const auto & config = AtomicTripleStoreConfig::getInstance();
		return new TripleStore{config.cache_size, config.cache_bucket_capacity, config.timeout};
	}
};

namespace tnt::store {

    /**
     * A SingletonFactory that allows to share a single TripleStore instance between multiple threads.
     */


	using AtomicTripleStore = SingletonFactory<TripleStore>;
};
#endif //TNT_ATOMIC_TRIPLE_STORE
