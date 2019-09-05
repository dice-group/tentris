#ifndef TENTRIS_PARSEDSPARQLCACHES_HPP

#define TENTRIS_PARSEDSPARQLCACHES_HPP

#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/util/SyncedLRUCache.hpp"
#include <memory>

namespace tentris::store::cache {
	using QueryExecutionPackage_cache = util::sync::SyncedLRUCache<std::string, QueryExecutionPackage>;

} // namespace tentris::store::cache

#endif // TENTRIS_PARSEDSPARQLCACHES_HPP
