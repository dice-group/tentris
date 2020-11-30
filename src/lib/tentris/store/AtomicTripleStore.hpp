#ifndef TENTRIS_ATOMIC_TRIPLE_STORE
#define TENTRIS_ATOMIC_TRIPLE_STORE


#include "tentris/util/SingletonFactory.hpp"
#include "tentris/store/TripleStore.hpp"
#include "tentris/store/config/AtomicTripleStoreConfig.cpp"


namespace tentris::util::sync {
	template<>
	inline ::tentris::store::TripleStore *SingletonFactory<::tentris::store::TripleStore>::make_instance() {
		return new ::tentris::store::TripleStore{};
	}
};

namespace tentris::store {

	/**
	 * A SingletonFactory that allows to share a single TripleStore instance between multiple threads.
	 */
	using AtomicTripleStore = ::tentris::util::sync::SingletonFactory<TripleStore>;
};
#endif //TENTRIS_ATOMIC_TRIPLE_STORE
