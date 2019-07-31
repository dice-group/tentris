#ifndef HYPERTRIE_COUNTDOWNNTUPLE_HPP
#define HYPERTRIE_COUNTDOWNNTUPLE_HPP

#include <tuple>
#include "hypertrie/internal/util/NTuple.hpp"

namespace hypertrie::internal::util {
	namespace {

		template<template<auto C> typename T, unsigned int N,
				template<typename...> class TT>
		struct repeatC {
			using type = typename
			append_to_type_seq<
					T<N>,
					typename repeatC<T, N - 1, TT>::type
			>::type;
		};

		template<template<auto C> typename T,
				template<typename...> class TT>
		struct repeatC<T, 0, TT> {
			using type = TT<>;
		};
	}

	// TODO: implement an iterator for NTuple
	template<template<auto C> typename T, unsigned int N = 0>
	using CountDownNTuple = typename repeatC<T, N, std::tuple>::type;
}


#endif //HYPERTRIE_COUNTDOWNNTUPLE_HPP
