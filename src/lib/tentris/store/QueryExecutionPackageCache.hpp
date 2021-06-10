#ifndef TENTRIS_PARSEDSPARQLCACHES_HPP

#define TENTRIS_PARSEDSPARQLCACHES_HPP

#include "tentris/store/cache/GraphQLExecutionPackage.hpp"
#include "tentris/store/cache/SPARQLExecutionPackage.hpp"
#include "tentris/util/SyncedLRUCache.hpp"
#include <memory>

namespace tentris::store::cache {
    using GraphQLExecutionPackage_cache = util::sync::SyncedLRUCache<std::string, GraphQLExecutionPackage>;
    using SPARQLExecutionPackage_cache = util::sync::SyncedLRUCache<std::string, SPARQLExecutionPackage>;
} // namespace tentris::store::cache

#endif // TENTRIS_PARSEDSPARQLCACHES_HPP
