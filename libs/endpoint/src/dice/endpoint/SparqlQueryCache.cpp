#include "SparqlQueryCache.hpp"

#include <dice/sparql2tensor/SPARQLQuery.hpp>

namespace dice::endpoint {
	template class SyncedLRUCache<std::string, dice::sparql2tensor::SPARQLQuery>;
}// namespace dice::endpoint