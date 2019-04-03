#ifndef TNT_TRIPLESTORECONFIG_HPP
#define TNT_TRIPLESTORECONFIG_HPP

#include <string>

#include <cxxopts.hpp>
#include <chrono>

#include <tnt/util/LogHelper.hpp>
#include <tnt/util/SingletonFactory.hpp>


namespace {
	using namespace ::tnt::logging;
}

namespace tnt::store::config {
	/**
	 * The configuration of a Tentris TripleStore instance.
	 */
	struct TripleStoreConfig;

	/**
	 * A Factory that assures that only one instance of TripleStoreConfig is created.
	 */
	class AtomicTripleStoreConfig : public util::sync::SingletonFactory<TripleStoreConfig> {
	public:
		AtomicTripleStoreConfig() : SingletonFactory<TripleStoreConfig>{} {}
	};

	struct TripleStoreConfig {
		/**
		 * The relative or absolute path to the RDF file that Tentris loads on startup.
		 */
		std::string rdf_file{};
		/**
		 * The timeout for query processing of single queries.
		 */
		std::chrono::system_clock::duration timeout = std::chrono::seconds(180);
		/**
		 * Max number queries that may be cached.
		 */
		size_t cache_size = 500;
		/**
		 * Max number of bindings per query result that is cached.
		 */
		size_t cache_bucket_capacity = 500;

	};


};

#endif //TNT_TRIPLESTORECONFIG_HPP
