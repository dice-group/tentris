#ifndef TENTRIS_ATOMICGRAPHQLSCHEMA_HPP
#define TENTRIS_ATOMICGRAPHQLSCHEMA_HPP

#include "tentris/util/SingletonFactory.hpp"
#include "tentris/store/graphql/GraphqlSchema.hpp"


//namespace tentris::util::sync {
//    template<>
//    inline ::tentris::store::TripleStore *SingletonFactory<::tentris::store::TripleStore>::make_instance() {
//        return new ::tentris::store::TripleStore{};
//    }
//};

namespace tentris::store {

    /**
     * A SingletonFactory that allows to share a single GraphqlSchema instance between multiple threads.
     */
    using AtomicGraphqlSchema = ::tentris::util::sync::SingletonFactory<graphql::GraphqlSchema>;
};

#endif//TENTRIS_ATOMICGRAPHQLSCHEMA_HPP
