#ifndef TNT_SPARQLREQUESTTHREAD_HPP
#define TNT_SPARQLREQUESTTHREAD_HPP

#include "pistache/http.h"
#include "tnt/store/TripleStore.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "JsonSerializer.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"

#include <chrono>

namespace {
    using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
    using ResponseWriter = Pistache::Http::ResponseWriter;
    using Variable = tnt::store::sparql::Variable;
    using SelectModifier =  tnt::store::sparql::SelectModifier;
    using namespace tnt::store::cache;
    using namespace tnt::tensor::einsum::operators;
    using Code = Pistache::Http::Code;
};
namespace tnt::http {
    void runQuery(ResponseWriter &response, const std::shared_ptr<QueryExecutionPackage> &query_package,
                  tnt::store::TripleStore &store,
                  const std::chrono::time_point<std::chrono::high_resolution_clock> &time_out) {

        const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
        const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();
        switch (sparqlQuery.getSelectModifier()) {
            case SelectModifier::NONE: {
                auto stream = response.stream(Code::Ok);
                stream_out<counted_binding>(vars, query_package->getRegularGenerator(), stream, store, time_out);
                break;
            }
            case SelectModifier::DISTINCT: {
                auto stream = response.stream(Code::Ok);
                stream_out<distinct_binding>(vars, query_package->getDistinctGenerator(), stream, store, time_out);
                break;
            }
            default:
                response.send(Code::Not_Implemented);
        }
    }
}

#endif //TNT_SPARQLREQUESTTHREAD_HPP


