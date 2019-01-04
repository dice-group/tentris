#ifndef TNT_PARSEDSPARQLCACHES_HPP
#define TNT_PARSEDSPARQLCACHES_HPP

#include "tnt/http/TimeoutException.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"
#include "tnt/util/SynchronizedCachedFactory.hpp"

namespace {
	using tnt::http::TimeoutException;
	using tnt::util::sync::SynchronizedCachedFactory;
} // namespace

namespace tnt::store::cache {

	class QueryExecutionPackage_cache : public SynchronizedCachedFactory<std::string, QueryExecutionPackage> {
		std::timed_mutex set_timer_mutex;
		std::chrono::system_clock::duration max_processing_time;

	protected:
		QueryExecutionPackage *construct(const std::string &key) override {
			try {
				auto *qep = new QueryExecutionPackage{key, trie, termIndex};
				return qep;
			} catch ([[maybe_unused]] std::invalid_argument &exc) {
			}
			return nullptr;
		}

		BoolHyperTrie &trie;
		TermStore &termIndex;

	public: // TODO: use atomic config to initialize ot
		QueryExecutionPackage_cache(BoolHyperTrie &trie, TermStore &termIndex, uint capacity = 500,
		                            std::chrono::system_clock::duration max_processing_time = std::chrono::seconds(180))
				: SynchronizedCachedFactory{capacity}, max_processing_time{max_processing_time}, trie{trie}, termIndex{
				termIndex} {}

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

			auto qep = this->SynchronizedCachedFactory::get(query);
			if (qep->queuing.try_lock_until(timeout)) {

				if (qep->processing.try_lock()) {
					qep->queuing.unlock();
					qep->timeout = timeout;

					return qep;
				} else {
					qep->keep_result_timeout = timeout;

					if (qep->processing.try_lock_until(timeout)) {
						qep->queuing.unlock();
						qep->timeout = timeout;

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
	}; // namespace tnt::store::cache

} // namespace tnt::store::cache

#endif // TNT_PARSEDSPARQLCACHES_HPP
