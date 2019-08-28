#ifndef HYPERTRIE_FRONTSKIPITERATOR_HPP
#define HYPERTRIE_FRONTSKIPITERATOR_HPP

namespace hypertrie::internal::util {
	namespace {


		template<typename T, size_t n>
		struct SkipWrapper {
			T &t;

			explicit SkipWrapper(T &v) : t(v) {}

			auto begin() -> decltype(auto) {
				return std::next(std::begin(t), n);
			}

			auto end() -> decltype(auto) {
				return std::end(t);
			}
		};

	}

	template<size_t n, typename T>
	auto skip(T &iterable) -> decltype(auto) {
		assert(size(iterable) >= n);
		return SkipWrapper<T, n>(iterable);
	}

}


#endif //HYPERTRIE_FRONTSKIPITERATOR_HPP
