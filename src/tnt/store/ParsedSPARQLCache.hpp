#ifndef TNT_PARSEDSPARQLCACHES_HPP
#define TNT_PARSEDSPARQLCACHES_HPP

#include "tnt/util/SycronizedFactory.hpp"

#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"

#include "tnt/util/SycronizedFactory.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"

namespace tnt::store::cache {

    class QueryCache : public SycronizedFactory<std::string, QueryExecutionPackage> {
    protected:
        QueryExecutionPackage *construct(const std::string &key) {
            try {
                return new QueryExecutionPackage{key, _trie, _termIndex};
            } catch (std::invalid_argument exc) {}
            return nullptr;
        }

        BoolHyperTrie &_trie;
        TermStore &_termIndex;

    public:
        QueryCache(BoolHyperTrie &trie, TermStore &termIndex, uint capacity = 500) : _trie{trie},
                                                                                     _termIndex{termIndex},
                                                                                     SycronizedFactory{capacity} {}
    };

}

#endif //TNT_PARSEDSPARQLCACHES_HPP
