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
		inline static Class &getInstance() {
			Class *inst = instance.load(std::memory_order_acquire);
			if (!inst) {
				std::lock_guard<std::mutex> myLock(mutex);
				inst = instance.load(std::memory_order_relaxed);
				if (!inst) {
					inst = make_instance();
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

		inline static std::atomic<Class *> instance;
		inline static std::mutex mutex;

		inline static Class *make_instance() {
			return new Class();
		}
	};

}
#endif //TNT_SINGLETONFACTORY_HPP
