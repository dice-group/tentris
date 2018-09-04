#ifndef TNT_GENERATORINTERFACE_HPP
#define TNT_GENERATORINTERFACE_HPP

#include <set>
#include <map>
#include <unordered_set>

#include <boost/coroutine2/all.hpp>
#include <boost/bind.hpp>

#include "tnt/util/Types.hpp"

namespace {
    using namespace tnt::util::types;
}

namespace tnt::tensor::einsum::operators {

    template<typename K>
    struct binding_type {
        using key_t = K;
        using count_t = K;
        using bidning_t = K;
        using collection_t = K;
        using unordered_collection_t = K;

        static key_t getKey(bidning_t binding);

        static count_t getCount(bidning_t binding);
    };


    template<>
    struct binding_type<int> {
        using key_t = const Key_t &;
        using count_t = size_t;
        using bidning_t = std::tuple<const Key_t &, size_t>;
        using collection_t = std::map<std::decay_t<key_t>, count_t>;
        using unordered_collection_t = std::map<std::decay_t<key_t>, count_t>;

        constexpr static key_t getKey(bidning_t binding) {
            return std::get<0>(binding);
        }

        constexpr static count_t getCount(bidning_t binding) {
            return std::get<1>(binding);
        }
    };

    using counted_binding = binding_type<int>;

    template<>
    struct binding_type<bool> {
        using key_t = const Key_t &;
        using count_t = bool;
        using bidning_t = const Key_t &;
        using collection_t = std::set<std::decay_t<key_t>>;
        using unordered_collection_t = std::set<std::decay_t<key_t>>;

        constexpr static key_t getKey(bidning_t binding) {
            return binding;
        }

        constexpr static count_t getCount([[maybe_unused]] bidning_t binding) {
            return true;
        }
    };

    using distinct_binding = binding_type<bool>;

    template<typename T>
    struct is_binding {
        enum {
            value = std::is_same<T, counted_binding>::value || std::is_same<T, distinct_binding>::value
        };
    };


    template<typename RESULT_TYPE>
    using yield_push = typename boost::coroutines2::coroutine<typename RESULT_TYPE::bidning_t>::push_type;
    template<typename RESULT_TYPE>
    using yield_pull = typename boost::coroutines2::coroutine<typename RESULT_TYPE::bidning_t>::pull_type;

}

#endif //TNT_GENERATORINTERFACE_HPP
