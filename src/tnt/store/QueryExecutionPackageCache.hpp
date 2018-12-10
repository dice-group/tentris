#ifndef TNT_PARSEDSPARQLCACHES_HPP
#define TNT_PARSEDSPARQLCACHES_HPP

#include "tnt/util/SynchronizedCachedFactory.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"

namespace {
    using tnt::util::sync::SynchronizedCachedFactory;
}

namespace tnt::store::cache {

    class QueryExecutionPackage_cache : public SynchronizedCachedFactory<std::string, QueryExecutionPackage> {
    protected:
        QueryExecutionPackage *construct(const std::string &key) override{
            try {
                return new QueryExecutionPackage{key, _trie, _termIndex};
            } catch (std::invalid_argument &exc) {}
            return nullptr;
        }

        BoolHyperTrie &_trie;
        TermStore &_termIndex;

    public:
        QueryExecutionPackage_cache(BoolHyperTrie &trie, TermStore &termIndex, uint capacity = 500) :
                SynchronizedCachedFactory{capacity}, _trie{trie}, _termIndex{termIndex} {}
    };

}

#endif //TNT_PARSEDSPARQLCACHES_HPP
