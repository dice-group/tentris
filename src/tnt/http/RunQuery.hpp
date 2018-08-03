#ifndef TNT_SPARQLREQUESTTHREAD_HPP
#define TNT_SPARQLREQUESTTHREAD_HPP

#include "pistache/http.h"
#include "tnt/store/TripleStore.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "JsonSerializer.hpp"

namespace tnt::http {

    namespace {
        using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
        using ResponseWriter = Pistache::Http::ResponseWriter;
        using Variable = tnt::store::sparql::Variable;
        using SelectModifier =  tnt::store::sparql::SelectModifier;
        using namespace tnt::tensor::einsum::operators;
        using Code = Pistache::Http::Code;
    };

    void runQuery(ResponseWriter &response, const ParsedSPARQL &sparqlQuery, tnt::store::TripleStore &store) {
        const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();
        switch (sparqlQuery.getSelectModifier()) {
            case SelectModifier::NONE: {
                yield_pull<INT_VALUES> results = store.query<INT_VALUES>(sparqlQuery);
                auto stream = response.stream(Code::Ok);
                stream_out<INT_VALUES>(vars, results, stream, store);
                break;
            }
            case SelectModifier::DISTINCT: {
                yield_pull<BOOL_VALUES> results = store.query<BOOL_VALUES>(sparqlQuery);
                auto stream = response.stream(Code::Ok);
                stream_out<BOOL_VALUES>(vars, results, stream, store);
                break;
            }
            default:
                response.send(Code::Not_Implemented);
        }
    }


}

#endif //TNT_SPARQLREQUESTTHREAD_HPP


