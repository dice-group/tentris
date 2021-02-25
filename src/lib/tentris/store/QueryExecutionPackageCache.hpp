#ifndef TENTRIS_PARSEDSPARQLCACHES_HPP

#define TENTRIS_PARSEDSPARQLCACHES_HPP

#include "tentris/store/cache/GraphqlExecutionPackage.hpp"
#include "tentris/store/cache/SPARQLExecutionPackage.hpp"
#include "tentris/store/graphql/GraphqlDocument.hpp"
#include "tentris/util/SyncedLRUCache.hpp"
#include <memory>

namespace tentris::store::cache {
    using GraphqlExecutionPackage_cache = util::sync::SyncedLRUCache<std::shared_ptr<::tentris::store::graphql::GraphqlDocument>,
																	 GraphqlExecutionPackage>;
    using SPARQLExecutionPackage_cache = util::sync::SyncedLRUCache<std::string, SPARQLExecutionPackage>;
} // namespace tentris::store::cache

#endif // TENTRIS_PARSEDSPARQLCACHES_HPP
