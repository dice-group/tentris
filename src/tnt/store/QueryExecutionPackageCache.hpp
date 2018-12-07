#ifndef TNT_PARSEDSPARQLCACHES_HPP
#define TNT_PARSEDSPARQLCACHES_HPP

#include "tnt/util/SycronizedFactory.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"

namespace {
    using tnt::util::sync::SycronizedFactory;
}

namespace tnt::store::cache {

    class QueryExecutionPackage_cache : public SycronizedFactory<std::string, QueryExecutionPackage> {
    protected:
        virtual QueryExecutionPackage *construct(const std::string &key) {
            try {
                return new QueryExecutionPackage{key, _trie, _termIndex};
            } catch (std::invalid_argument &exc) {}
            return nullptr;
        }

        BoolHyperTrie &_trie;
        TermStore &_termIndex;

    public:
        QueryExecutionPackage_cache(BoolHyperTrie &trie, TermStore &termIndex, uint capacity = 500) :
                SycronizedFactory{capacity}, _trie{trie}, _termIndex{termIndex} {}
    };

}

#endif //TNT_PARSEDSPARQLCACHES_HPP
