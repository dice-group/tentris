#ifndef TENTRIS_TRIPLESTORECONFIG_HPP
#define TENTRIS_TRIPLESTORECONFIG_HPP

#include <string>

#include <chrono>

#include <tentris/util/SingletonFactory.hpp>


namespace tentris::store::config {
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
		std::chrono::steady_clock::duration timeout = std::chrono::seconds(180);
		/**
		 * Max number queries that may be cached.
		 */
		size_t cache_size = 500;
	};


};

#endif //TENTRIS_TRIPLESTORECONFIG_HPP
