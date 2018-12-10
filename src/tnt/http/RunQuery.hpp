#ifndef TNT_SPARQLREQUESTTHREAD_HPP
#define TNT_SPARQLREQUESTTHREAD_HPP

#include "pistache/http.h"
#include "tnt/store/TripleStore.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "JsonSerializer.hpp"
#include "tnt/store/QueryExecutionPackage.hpp"

#include <chrono>

#include <tnt/util/LogHelper.hpp>

namespace {
    using ParsedSPARQL = tnt::store::sparql::ParsedSPARQL;
    using ResponseWriter = Pistache::Http::ResponseWriter;
    using Variable = tnt::store::sparql::Variable;
    using SelectModifier =  tnt::store::sparql::SelectModifier;
    using TripleStore = tnt::store::TripleStore;
    using namespace tnt::store::cache;
    using namespace tnt::tensor::einsum::operators;
    using Code = Pistache::Http::Code;
    using namespace std::chrono;
};
namespace tnt::http {
    /**
     * Executes a QueryExecutionPackage and writes its as JSON to a Pistache ResponseWriter. The execution is stopped
     * when time_out is reached.
     * @param response the writer to write the result of the query to.
     * @param query_package a query package that is executed to produce the result
     * @param store a instance of a triple store
     * @param time_out a time stamp after that the execution must be canceled
     */
    void runQuery(ResponseWriter &response, const std::shared_ptr<QueryExecutionPackage> &query_package,
                  TripleStore &store, const time_point<high_resolution_clock> &time_out) {

        const ParsedSPARQL &sparqlQuery = query_package->getParsedSPARQL();
        const std::vector<Variable> &vars = sparqlQuery.getQueryVariables();
        switch (sparqlQuery.getSelectModifier()) {
            case SelectModifier::NONE: {
                logDebug("Running select query.");
                auto stream = response.stream(Code::Ok);
                stream_out<counted_binding>(vars, query_package->getRegularGenerator(), stream, store, time_out);
                break;
            }
            case SelectModifier::DISTINCT: {
                logDebug("Running select distinct query.");
                auto stream = response.stream(Code::Ok);
                stream_out<distinct_binding>(vars, query_package->getDistinctGenerator(), stream, store, time_out);
                break;
            }
            default:
                logDebug("Query type is not yet supported.");
                response.send(Code::Not_Implemented);
        }
    }
}

#endif //TNT_SPARQLREQUESTTHREAD_HPP


