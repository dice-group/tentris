#ifndef TENTRIS_GRAPHQLPARSER_HPP
#define TENTRIS_GRAPHQLPARSER_HPP

#include <GraphQL/GraphQLLexer.h>

#include "tentris/store/graphql/GraphQLSchema.hpp"
#include "tentris/store/graphql/internal/GraphQLQueryVisitor.hpp"
#include "tentris/store/graphql/internal/GraphQLSchemaVisitor.hpp"

namespace tentris::store::graphql {
	class GraphQLParser {

	public:
		static void parseSchema(const std::string &document, GraphQLSchema &schema);

		static std::shared_ptr<ParsedGraphQL> parseQuery(const std::string &document,
														 const std::string &query_name);
	};
}// namespace tentris::store::graphql

#endif//TENTRIS_GRAPHQLPARSER_HPP
