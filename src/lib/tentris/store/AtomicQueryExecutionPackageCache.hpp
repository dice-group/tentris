#ifndef TENTRIS_ATOMIC_QUERY_EXECUTION_CACHE_STORE
#define TENTRIS_ATOMIC_QUERY_EXECUTION_CACHE_STORE


#include "tentris/util/SingletonFactory.hpp"
#include "tentris/store/QueryExecutionPackageCache.hpp"
#include "tentris/store/config/AtomicTripleStoreConfig.cpp"

namespace tentris::util::sync {
	template<>
	inline ::tentris::store::cache::SPARQLExecutionPackage_cache *
	SingletonFactory<::tentris::store::cache::SPARQLExecutionPackage_cache>::make_instance() {
		const auto &config = ::tentris::store::config::AtomicTripleStoreConfig::getInstance();
		return new ::tentris::store::cache::SPARQLExecutionPackage_cache{config.cache_size};
	}

    template<>
    inline ::tentris::store::cache::GraphqlExecutionPackage_cache *
    SingletonFactory<::tentris::store::cache::GraphqlExecutionPackage_cache>::make_instance() {
        const auto &config = ::tentris::store::config::AtomicTripleStoreConfig::getInstance();
        return new ::tentris::store::cache::GraphqlExecutionPackage_cache{config.cache_size};
    }
};

namespace tentris::store {

	/**
	 * A SingletonFactory that allows to share a single TripleStore instance between multiple threads.
	 */
	using AtomicSPARQLExecutionCache = ::tentris::util::sync::SingletonFactory<::tentris::store::cache::SPARQLExecutionPackage_cache>;
    using AtomicGraphqlExecutionCache = ::tentris::util::sync::SingletonFactory<::tentris::store::cache::GraphqlExecutionPackage_cache>;
};
#endif //TENTRIS_ATOMIC_QUERY_EXECUTION_CACHE_STORE
