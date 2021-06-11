#ifndef TENTRIS_GRAPHQLSCHEMAVISITOR_HPP
#define TENTRIS_GRAPHQLSCHEMAVISITOR_HPP

#include <GraphQL/GraphQLBaseVisitor.h>

#include "tentris/store/graphql/GraphQLSchema.hpp"

namespace tentris::store::graphql::internal {

	using namespace Dice::graphql_parser;

	class GraphQLSchemaVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

	private:
		GraphQLSchema *schema;

	public:
		GraphQLSchemaVisitor() = delete;

		explicit GraphQLSchemaVisitor(GraphQLSchema *schema) : schema(schema) {}

		antlrcpp::Any visitDocument(base::GraphQLParser::DocumentContext *ctx) override;

		antlrcpp::Any visitTypeSystemDefinition(base::GraphQLParser::TypeSystemDefinitionContext *ctx) override;

		antlrcpp::Any visitTypeDefinition(base::GraphQLParser::TypeDefinitionContext *ctx) override;

		antlrcpp::Any visitObjectTypeDefinition(base::GraphQLParser::ObjectTypeDefinitionContext *ctx) override;

		antlrcpp::Any visitInterfaceTypeDefinition(base::GraphQLParser::InterfaceTypeDefinitionContext *ctx) override;

		antlrcpp::Any visitFieldDefinition(base::GraphQLParser::FieldDefinitionContext *ctx) override;

		antlrcpp::Any visitDirective(base::GraphQLParser::DirectiveContext *ctx) override;

		antlrcpp::Any visitValue(base::GraphQLParser::ValueContext *ctx) override;
	};

}// namespace tentris::store::graphql::internal

#endif//TENTRIS_GRAPHQLSCHEMAVISITOR_HPP
