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

	ParsedGraphQL GraphQLParser::parseQuery(GraphQLSchema *schema,
											const std::string &document,
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
			// and that all operations have been assgined a name
			std::set<std::string> operation_names{};
			for (const auto &definition : root->definition()) {
				if (definition->typeSystemDefinition() or definition->typeSystemExtension())
					throw std::runtime_error("Executable request contains type system definition");
				if (not definition->executableDefinition()->operationDefinition()->name())
					throw std::runtime_error("Request contains operation without a name");
				auto op_name = definition->executableDefinition()->operationDefinition()->name()->getText();
				if (operation_names.contains(op_name))
					throw std::runtime_error(fmt::format("Duplicate operation name `{}`", op_name));
				operation_names.insert(op_name);
				if (op_name == query_name) {
					if (definition->executableDefinition()->operationDefinition()->operationType()->getText() != "query")
						throw std::runtime_error("Only queries are currently supported");
					query = definition->executableDefinition()->operationDefinition();
				}
			}
		}
		ParsedGraphQL parsed_query;
		GraphQLQueryVisitor query_visitor{&parsed_query, schema};
		query_visitor.visitOperationDefinition(query);
		return parsed_query;
	}


}// namespace tentris::store::graphql