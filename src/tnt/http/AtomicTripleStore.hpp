#ifndef TNT_ATOMIC_TRIPLE_STORE
#define TNT_ATOMIC_TRIPLE_STORE


#include "tnt/util/SingletonFactory.hpp"
#include "tnt/store/TripleStore.hpp"

namespace {
    using namespace tnt::store;
    using namespace tnt::util::sync;
}

namespace tnt::http {
    class AtomicTripleStore : public SingletonFactory<TripleStore> {
    public:
        AtomicTripleStore() : SingletonFactory<TripleStore>{} {}
    };
};
#endif //TNT_ATOMIC_TRIPLE_STORE
