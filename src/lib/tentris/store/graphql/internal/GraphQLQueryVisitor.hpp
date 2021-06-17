#ifndef TENTRIS_GRAPHQLQUERYVISITOR_HPP
#define TENTRIS_GRAPHQLQUERYVISITOR_HPP

#include "tentris/store/graphql/GraphQLSchema.hpp"
#include "tentris/store/graphql/internal/GraphQLDataStructures.hpp"
#include <GraphQL/GraphQLBaseVisitor.h>

#include <utility>

namespace tentris::store::graphql::internal {

	using namespace Dice::graphql_parser;

	class GraphQLQueryVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

	private:
		ParsedGraphQL* parsed_query;
		const GraphQLSchema *schema;
		// active subscript label
		char next_label = 'a';
		// subscript label of last visited field
		char field_label = 'a';
		// stack - subscript label of last visited selection set
		std::vector<char> selection_set_label{};
		// stack - type of parent field
		std::vector<std::string> parent_type{};
		// active field name
		std::string field_name{};
		// position of last field operand - used for argument ids
		uint32_t operand_pos;
		// stack - keeps track of type conditions
		std::vector<std::string> type_conditions{};
		// the active path
		std::vector<char> active_path{};

	public:
		GraphQLQueryVisitor() = delete;

		explicit GraphQLQueryVisitor(ParsedGraphQL* pq,
									 const GraphQLSchema *schema) : parsed_query(pq),
																	schema(schema) {}

		antlrcpp::Any visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) override;

		antlrcpp::Any visitSelectionSet(base::GraphQLParser::SelectionSetContext *ctx) override;

		antlrcpp::Any visitField(base::GraphQLParser::FieldContext *ctx) override;

		antlrcpp::Any visitInlineFragment(base::GraphQLParser::InlineFragmentContext *ctx) override;

		antlrcpp::Any visitArguments(base::GraphQLParser::ArgumentsContext *ctx) override;
	};

}// namespace tentris::store::graphql::internal

#endif//TENTRIS_GRAPHQLQUERYVISITOR_HPP
