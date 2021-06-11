#ifndef TENTRIS_GRAPHQLQUERYVISITOR_HPP
#define TENTRIS_GRAPHQLQUERYVISITOR_HPP

#include "tentris/store/graphql/internal/GraphQLDataStructures.hpp"
#include <GraphQL/GraphQLBaseVisitor.h>

#include <utility>

namespace tentris::store::graphql::internal {

	using namespace Dice::graphql_parser;

	class GraphQLQueryVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

	private:
		std::shared_ptr<ParsedGraphQL> parsed_query;
		// active path
		Path active_path{};
		// active subscript label
		char next_label = 'a';
		// subscript label of last visited field
		char field_label = 'a';
		// stack - subscript label of last visited selection set
		std::vector<char> selection_set_label{};
		// used to store fragment labels
		bool in_fragment = false;

	public:
		GraphQLQueryVisitor() = delete;

		explicit GraphQLQueryVisitor(std::shared_ptr<ParsedGraphQL> pq) : parsed_query(std::move(pq)) {}

		antlrcpp::Any visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) override;

		antlrcpp::Any visitSelectionSet(base::GraphQLParser::SelectionSetContext *ctx) override;

		antlrcpp::Any visitField(base::GraphQLParser::FieldContext *ctx) override;

		antlrcpp::Any visitInlineFragment(base::GraphQLParser::InlineFragmentContext *ctx) override;

		antlrcpp::Any visitArguments(base::GraphQLParser::ArgumentsContext *ctx) override;
	};

}// namespace tentris::store::graphql::internal

#endif//TENTRIS_GRAPHQLQUERYVISITOR_HPP
