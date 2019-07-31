#ifndef TENTRIS_PARSEDSPARQLCACHES_HPP
#define TENTRIS_PARSEDSPARQLCACHES_HPP

#include "tentris/http/TimeoutException.hpp"
#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/util/SynchronizedCachedFactory.hpp"


namespace tentris::store::cache {

	namespace {
		using tentris::http::TimeoutException;
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
				  termIndex{
						  termIndex}, cache_bucket_size{cache_bucket_size} {}

		/**
		 * Get a query execution package for this query.
		 * @param query the query
		 * @throws TimeoutException If it blocks for too long a TimeoutExecption is thrown. This may be due to another
		 * request for the same query that is currently processed.
		 * @return the query execution package. The std::timed_mutex queuing is set and must be unset after processing is
		 * finished.
		 */
		std::shared_ptr<QueryExecutionPackage> get(const std::string &query) {
			auto timeout = std::chrono::system_clock::now() + max_processing_time;

			auto qep = this->SynchronizedCachedFactory<std::string, QueryExecutionPackage>::get(query);
			if (qep->queuing.try_lock_until(timeout)) {

				if (qep->processing.try_lock()) {
					qep->queuing.unlock();
					qep->setTimeout(timeout);

					return qep;
				} else {
					qep->keep_result_timeout = timeout;

					if (qep->processing.try_lock_until(timeout)) {
						qep->queuing.unlock();
						qep->setTimeout(timeout);

						return qep;
					} else {
						qep->queuing.unlock();
						throw TimeoutException{};
					}
				}
			} else {
				throw TimeoutException{};
			}
		}
	}; // namespace tentris::store::cache

} // namespace tentris::store::cache

#endif // TENTRIS_PARSEDSPARQLCACHES_HPP
