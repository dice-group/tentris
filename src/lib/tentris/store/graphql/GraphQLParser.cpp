#include "GraphQLParser.hpp"

namespace tentris::store::graphql {

	void GraphQLParser::parseSchema(const std::string &document,
									GraphQLSchema &schema) {
		antlr4::ANTLRInputStream input(document);
		Dice::graphql_parser::base::GraphQLLexer lexer(&input);
		antlr4::CommonTokenStream tokens(&lexer);
		Dice::graphql_parser::base::GraphQLParser parser(&tokens);
		auto root = parser.document();
		GraphQLSchemaVisitor schema_visitor{&schema};
		schema_visitor.visitDocument(root);
	}

	std::shared_ptr<ParsedGraphQL> GraphQLParser::parseQuery(const std::string &document,
															 const std::string &query_name = "") {
		antlr4::ANTLRInputStream input(document);
		Dice::graphql_parser::base::GraphQLLexer lexer(&input);
		antlr4::CommonTokenStream tokens(&lexer);
		Dice::graphql_parser::base::GraphQLParser parser(&tokens);
		auto root = parser.document();
		Dice::graphql_parser::base::GraphQLParser::OperationDefinitionContext *query;
		if (root->definition().empty())
			throw std::runtime_error("Document does not contain any definitions");
		if (query_name.empty()) {
			if (root->definition().size() > 1)
				throw std::runtime_error("Provide a name for the query operation to be executed");
			if (not root->definition()[0]->executableDefinition() and
				not root->definition()[0]->executableDefinition()->operationDefinition())
				throw std::runtime_error("Executable request does not contain executable definition");
			if (root->definition()[0]->executableDefinition()->operationDefinition()->operationType() and
				root->definition()[0]->executableDefinition()->operationDefinition()->operationType()->getText() != "query")
				throw std::runtime_error("Only queries are currently supported");
			query = root->definition()[0]->executableDefinition()->operationDefinition();
		} else {
			// make sure that the are no type definitions and no type system extensions in the document
			for (const auto &definition : root->definition()) {
				if (definition->typeSystemDefinition() or definition->typeSystemExtension())
					throw std::runtime_error("Executable request contains type system definition");
				if (definition->executableDefinition()->operationDefinition()->name()->getText() == query_name) {
					if (definition->executableDefinition()->operationDefinition()->operationType()->getText() != "query")
						throw std::runtime_error("Only queries are currently supported");
					query = definition->executableDefinition()->operationDefinition();
				}
			}
		}
        auto parsed_query = std::make_shared<ParsedGraphQL>();
        GraphQLQueryVisitor query_visitor{parsed_query};
		query_visitor.visitOperationDefinition(query);
		return parsed_query;
	}


}// namespace tentris::store::graphql