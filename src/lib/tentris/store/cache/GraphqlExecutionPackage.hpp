#ifndef TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
#define TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP

#include <any>
#include <exception>
#include <ostream>

#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/store/graphql/ParsedGraphql.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store {
    class TripleStore;
};

namespace tentris::store::cache {

	/**
     * A QueryExecutionPackage contains everything that is necessary to execute a given graphql query for a state of the
     * RDF graph.
     */
	struct GraphqlExecutionPackage {

		using const_BoolHypertrie = ::tentris::tensor::const_BoolHypertrie;
		using time_point_t = logging::time_point_t;
		using ParsedGraphql = graphql::ParsedGraphql;
		using Subscript = ::tentris::tensor::Subscript;

	private:
		std::string graphql_string;
		std::shared_ptr<Subscript> subscript;
        std::vector<const_BoolHypertrie> operands{};

	public:
		GraphqlExecutionPackage() = delete;

		/**
         * @param graphql_string graphql query to be parsed
         */
		explicit GraphqlExecutionPackage(const std::string &graphql_string) : graphql_string{graphql_string} {
		}

        std::shared_ptr<void> getEinsum(const time_point_t &timeout = time_point_t::max()) const {
            using namespace tensor;
            return std::make_shared<Einsum<COUNTED_t>>(subscript, operands, timeout);
        }
	};

}
#endif//TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
