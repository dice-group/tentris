#ifndef TENTRIS_PARSEDSPARQLCACHES_HPP
#define TENTRIS_PARSEDSPARQLCACHES_HPP

#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/util/SynchronizedCachedFactory.hpp"


namespace tentris::store::cache {

	namespace {
		using tentris::util::sync::SynchronizedCachedFactory;
	} // namespace


	class QueryExecutionPackage_cache : public SynchronizedCachedFactory<std::string, QueryExecutionPackage> {
		std::chrono::system_clock::duration max_processing_time;

	protected:
		QueryExecutionPackage *construct(const std::string &key) override {
			try {
				auto *qep = new QueryExecutionPackage{key, trie, termIndex, cache_bucket_size};
				return qep;
			} catch ([[maybe_unused]] std::invalid_argument &exc) {
			}
			return nullptr;
		}

		BoolHypertrie &trie;
		TermStore &termIndex;

		size_t cache_bucket_size;

	public: // TODO: use atomic config to initialize it
		QueryExecutionPackage_cache(BoolHypertrie &trie, TermStore &termIndex, size_t cache_capacity = 500,
									size_t cache_bucket_size = 500,
									std::chrono::system_clock::duration max_processing_time = std::chrono::seconds(180))
				: SynchronizedCachedFactory{cache_capacity}, max_processing_time{max_processing_time}, trie{trie},
				  termIndex{termIndex}, cache_bucket_size{cache_bucket_size} {}

	}; // namespace tentris::store::cache

} // namespace tentris::store::cache

#endif // TENTRIS_PARSEDSPARQLCACHES_HPP
