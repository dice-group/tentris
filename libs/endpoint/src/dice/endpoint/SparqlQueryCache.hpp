#ifndef TENTRIS_SPARQLQUERYCACHE_HPP
#define TENTRIS_SPARQLQUERYCACHE_HPP

#include <dice/sparql2tensor/SPARQLQuery.hpp>

#include <dice/endpoint/SyncedLRUCache.hpp>

namespace dice::endpoint {

	using SparqlQueryCache = SyncedLRUCache<std::string, sparql2tensor::SPARQLQuery>;

}
#endif//TENTRIS_SPARQLQUERYCACHE_HPP
