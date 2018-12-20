#ifndef TNT_GENERATORINTERFACE_HPP
#define TNT_GENERATORINTERFACE_HPP

#include <set>
#include <map>
#include <unordered_set>
#include <functional>

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

        static bidning_t makeBinding(key_t key, count_t count);

        static void multiply(count_t &a, const count_t &b);
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

        constexpr static bidning_t makeBinding(key_t key, count_t count) {
            return {key, count};
        }

        constexpr static void multiply(count_t &a, const count_t &b){
            a *= b;
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

        constexpr static bidning_t makeBinding(key_t key, count_t count) {
            if (count)
                return key;
            else
                throw std::invalid_argument("distinct_binding::makeBinding: count must not be false.");
        }

        constexpr static void multiply(count_t &a, const count_t &b){
            a &= b;
        }
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
    using yield_push = typename boost::coroutines2::coroutine<typename RESULT_TYPE::bidning_t>::push_type;

    template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
    using yield_pull = typename boost::coroutines2::coroutine<typename RESULT_TYPE::bidning_t>::pull_type;

}

#endif //TNT_GENERATORINTERFACE_HPP
