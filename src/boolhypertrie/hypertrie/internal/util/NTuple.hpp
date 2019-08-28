#ifndef HYPERTRIE_NTUPLE_HPP
#define HYPERTRIE_NTUPLE_HPP

#include <tuple>

namespace hypertrie::internal::util {
	namespace {

		template<typename, typename>
		struct append_to_type_seq {
		};

		template<typename T, typename... Ts, template<typename...> class TT>
		struct append_to_type_seq<T, TT<Ts...>> {
			using type = TT<Ts..., T>;
		};

		template<typename T, unsigned int N, template<typename...> class TT>
		struct repeat {
			using type = typename
			append_to_type_seq<
					T,
					typename repeat<T, N - 1, TT>::type
			>::type;
		};

		template<typename T, template<typename...> class TT>
		struct repeat<T, 0, TT> {
			using type = TT<>;
		};
	}

	// TODO: implement an iterator for NTuple
	template<typename T, unsigned int N = 0>
	using NTuple = typename repeat<T, N, std::tuple>::type;
}


#endif //HYPERTRIE_NTUPLE_HPP
