#ifndef TNT_SINGLETONFACTORY_HPP
#define TNT_SINGLETONFACTORY_HPP

#include <atomic>
#include <iostream>
#include <future>
#include <mutex>

namespace tnt::util::sync {
    template<typename Class>
    class SingletonFactory {
    public:
        static Class &getInstance() {
            Class *inst = instance.load(std::memory_order_acquire);
            if (!inst) {
                std::lock_guard<std::mutex> myLock(mutex);
                inst = instance.load(std::memory_order_relaxed);
                if (!inst) {
                    inst = new Class();
                    instance.store(inst, std::memory_order_release);
                }
            }
            return *inst;
        }

    protected:
        SingletonFactory() = default;

        ~SingletonFactory() = default;

        SingletonFactory(const SingletonFactory &) = delete;

        SingletonFactory &operator=(const SingletonFactory &) = delete;

        static std::atomic<Class *> instance;
        static std::mutex mutex;
    };

    template<typename Class>
    std::atomic<Class *> SingletonFactory<Class>::instance;

    template<typename Class>
    std::mutex  SingletonFactory<Class>::mutex;
}
#endif //TNT_SINGLETONFACTORY_HPP
