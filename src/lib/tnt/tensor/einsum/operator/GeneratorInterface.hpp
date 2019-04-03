#ifndef TNT_GENERATORINTERFACE_HPP
#define TNT_GENERATORINTERFACE_HPP

#include <functional>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include <boost/container_hash/hash.hpp>
#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>

#include <boost/bind.hpp>
#include <boost/coroutine2/all.hpp>

#include "tnt/util/Types.hpp"

namespace {
	using namespace tnt::util::types;
}

namespace tnt::tensor::einsum::operators {

	template<typename K>
	struct binding_type {
		using key_t = K;
		using count_t = K;
		using binding_t = K;
		using collection_t = K;
		using unordered_collection_t = K;

		static key_t getKey(binding_t binding);

		static count_t getCount(binding_t binding);

		static binding_t makeBinding(key_t key, count_t count);

		static void multiply(count_t &a, const count_t &b);
	};


	template<>
	struct binding_type<int> {
		using key_t = const Key_t &;
		using count_t = size_t;
		using binding_t = std::pair<const Key_t &, size_t>;
		using collection_t = std::map<std::decay_t<key_t>, count_t>;
		using unordered_collection_t = tsl::hopscotch_map<std::decay_t<key_t>, count_t, boost::hash<std::decay_t<key_t>>>;

		constexpr static key_t getKey(binding_t binding) { return binding.first; }

		constexpr static count_t getCount(binding_t binding) { return binding.second; }

		constexpr static binding_t makeBinding(key_t key, count_t count) { return {key, count}; }


		constexpr static void multiply(count_t &a, const count_t &b) { a *= b; }
	};

	using counted_binding = binding_type<int>;

	template<>
	struct binding_type<bool> {
		using key_t = const Key_t &;
		using count_t = bool;
		using binding_t = const Key_t &;
		using collection_t = std::set<std::decay_t<key_t>>;
		using unordered_collection_t = tsl::hopscotch_set<std::decay_t<key_t>, boost::hash<std::decay_t<key_t>>>;

		constexpr static key_t getKey(binding_t binding) { return binding; }

		constexpr static count_t getCount([[maybe_unused]] binding_t binding) { return true; }

		constexpr static binding_t makeBinding(key_t key, count_t count) {
			if (count)
				return key;
			else
				throw std::invalid_argument("distinct_binding::makeBinding: count must not be false.");
		}


		constexpr static void multiply(count_t &a, const count_t &b) { a &= b; }
	};

	using distinct_binding = binding_type<bool>;

	/**
	 * Class to determine if T is counted_binding or distinct_binding. Use as:
	 *
	 * \code{.cpp}
	 *     template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	 * \endcode
	 *
	 * @tparam T
	 */
	template<typename T>
	struct is_binding {
		enum {
			value = std::is_same<T, counted_binding>::value || std::is_same<T, distinct_binding>::value
		};
	};

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	using yield_push = typename boost::coroutines2::coroutine<typename RESULT_TYPE::binding_t>::push_type;

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	using yield_pull = typename boost::coroutines2::coroutine<typename RESULT_TYPE::binding_t>::pull_type;

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	static void addToCollection(typename RESULT_TYPE::collection_t &items, typename RESULT_TYPE::binding_t binding);

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	static void
	addToCollection(typename RESULT_TYPE::unordered_collection_t &items, typename RESULT_TYPE::binding_t binding);

	template<>
	inline void
	addToCollection<counted_binding>(counted_binding::collection_t &items, counted_binding::binding_t binding) {
		if (auto found = items.find(counted_binding::getKey(binding)); found != items.end()) {
			found->second += counted_binding::getCount(binding);
		} else {
			items.insert_or_assign(counted_binding::getKey(binding), counted_binding::getCount(binding));
		}
	}

	template<>
	inline void
	addToCollection<distinct_binding>(distinct_binding::collection_t &items, distinct_binding::binding_t binding) {
		items.insert(distinct_binding::getKey(binding));
	}

	template<>
	inline void
	addToCollection<counted_binding>(counted_binding::unordered_collection_t &items,
									 counted_binding::binding_t binding) {
		if (auto found = items.find(counted_binding::getKey(binding)); found != items.end()) {
			found.value() += counted_binding::getCount(binding);
		} else {
			items.insert_or_assign(counted_binding::getKey(binding), counted_binding::getCount(binding));
		}
	}

	template<>
	inline void
	addToCollection<distinct_binding>(distinct_binding::unordered_collection_t &items,
									  distinct_binding::binding_t binding) {
		items.insert(distinct_binding::getKey(binding));
	}


} // namespace tnt::tensor::einsum::operators

#endif // TNT_GENERATORINTERFACE_HPP
