#ifndef TENTRIS_ATOMIC_CLEANUP_TASKGROUP_HPP
#define TENTRIS_ATOMIC_CLEANUP_TASKGROUP_HPP
#include "tentris/util/SingletonFactory.hpp"
#include <tbb/task_group.h>

namespace {
}

namespace tentris::http {

    /**
     * A SingletonFactory that allows to share a single TripleStore instance between multiple threads.
     */
    class AtomicCleanupTaskGroup : public util::sync::SingletonFactory<tbb::task_group> {
    public:
        AtomicCleanupTaskGroup() : util::sync::SingletonFactory<tbb::task_group>{} {}
    };

};
#endif //TENTRIS_ATOMIC_CLEANUP_TASKGROUP_HPP
