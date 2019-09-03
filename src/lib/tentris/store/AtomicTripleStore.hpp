#ifndef TENTRIS_ATOMIC_TRIPLE_STORE
#define TENTRIS_ATOMIC_TRIPLE_STORE


#include "tentris/util/SingletonFactory.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/store/config/AtomicTripleStoreConfig.cpp"

namespace {
	using namespace tentris::store;
	using namespace tentris::util::sync;
	using namespace tentris::store::config;
}

namespace tentris::util::sync {
	template<>
	inline TripleStore *SingletonFactory<TripleStore>::make_instance() {
		return new TripleStore{};
	}
};

namespace tentris::store {

	/**
	 * A SingletonFactory that allows to share a single TripleStore instance between multiple threads.
	 */


	using AtomicTripleStore = SingletonFactory<TripleStore>;
};
#endif //TENTRIS_ATOMIC_TRIPLE_STORE
