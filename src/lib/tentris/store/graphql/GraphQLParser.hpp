#ifndef TENTRIS_GRAPHQLPARSER_HPP
#define TENTRIS_GRAPHQLPARSER_HPP

#include <GraphQL/GraphQLLexer.h>

#include "tentris/store/graphql/GraphQLSchema.hpp"
#include "tentris/store/graphql/internal/GraphQLSchemaVisitor.hpp"

namespace tentris::store::graphql {
	class GraphQLParser {

	public:
		static void parseSchema(const std::string &document, GraphQLSchema &schema) {
			// parse document
			antlr4::ANTLRInputStream input(document);
			Dice::graphql_parser::base::GraphQLLexer lexer(&input);
			antlr4::CommonTokenStream tokens(&lexer);
			Dice::graphql_parser::base::GraphQLParser parser(&tokens);
			auto root = parser.document();
			GraphQLSchemaVisitor schema_visitor{&schema};
			schema_visitor.visitDocument(root);
		}
	};
}// namespace tentris::store::graphql

#endif//TENTRIS_GRAPHQLPARSER_HPP
