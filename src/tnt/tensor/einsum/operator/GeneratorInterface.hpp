#ifndef TNT_GENERATORINTERFACE_HPP
#define TNT_GENERATORINTERFACE_HPP

#include <boost/coroutine2/all.hpp>
#include <boost/bind.hpp>

#include "tnt/util/Types.hpp"

namespace {
    using namespace tnt::util::types;
}

namespace tnt::tensor::einsum::operators {

    template<typename RESULT_TYPE>
    using yield_push = typename boost::coroutines2::coroutine<RESULT_TYPE>::push_type;
    template<typename RESULT_TYPE>
    using yield_pull = typename boost::coroutines2::coroutine<RESULT_TYPE>::pull_type;
    using INT_VALUES = const std::tuple<const Key_t &, size_t>;
    using BOOL_VALUES = const Key_t &;


    template<typename VALUE_TYPE>
    const Key_t &getKey(const VALUE_TYPE &result);

    template<>
    inline const Key_t &getKey<INT_VALUES>(const INT_VALUES &result) {
        return std::get<0>(result);
    }

    template<>
    inline const Key_t &getKey<BOOL_VALUES>(const BOOL_VALUES &result) {
        return result;
    }

    template<typename VALUE_TYPE>
    size_t getCount(const VALUE_TYPE &result);

    template<>
    inline size_t getCount<INT_VALUES>(const INT_VALUES &result) {
        return std::get<1>(result);
    }

    template<>
    inline size_t getCount<BOOL_VALUES>(const BOOL_VALUES &result) {
        return 1;
    }

}

#endif //TNT_GENERATORINTERFACE_HPP
