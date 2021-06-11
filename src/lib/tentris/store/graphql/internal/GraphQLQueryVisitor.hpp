#ifndef TENTRIS_GRAPHQLQUERYVISITOR_HPP
#define TENTRIS_GRAPHQLQUERYVISITOR_HPP

#include <any>
#include <variant>

#include <GraphQL/GraphQLBaseVisitor.h>

namespace tentris::store::graphql::internal {

	using namespace Dice::graphql_parser;

	class GraphQLQueryVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

	private:
		using OperandLabels = std::vector<char>;
		using OperandsLabels = std::vector<OperandLabels>;
		using ResultsLabels = std::vector<char>;
		using FieldName = std::string;
		using Argument = std::pair<FieldName, std::any>;
		using FieldsNameArguments = std::vector<std::variant<FieldName, Argument>>;
		using Path = std::vector<std::pair<char, std::string>>;
		using Paths = std::vector<Path>;

	public:
		// contains the information gathered from the input query
		struct ParsedGraphQL {
			// subscript operands
			std::vector<OperandsLabels> all_operands_labels{};
			// subscript result
			std::vector<ResultsLabels> all_result_labels{};
			// list of fields names
			std::vector<FieldsNameArguments> all_fields_name_arguments{};
			// list of paths
			std::vector<Paths> all_paths{};
			// the labels that appear in inline fragments
			std::vector<std::set<char>> all_fragment_labels{};
		};

	private:
		std::shared_ptr<ParsedGraphQL> parsed_query = std::make_shared<ParsedGraphQL>();
		// active path
		Path active_path{};
		// active subscript label
		char next_label = 'a';
		// subscript label of last visited field
		char field_label;
		// stack - subscript label of last visited selection set
		std::vector<char> selection_set_label{};
		// used to store fragment labels
		bool in_fragment = false;

	public:
		antlrcpp::Any visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) override;

		antlrcpp::Any visitSelectionSet(base::GraphQLParser::SelectionSetContext *ctx) override;

		antlrcpp::Any visitField(base::GraphQLParser::FieldContext *ctx) override;

		antlrcpp::Any visitInlineFragment(base::GraphQLParser::InlineFragmentContext *ctx) override;

		antlrcpp::Any visitArguments(base::GraphQLParser::ArgumentsContext *ctx) override;
	};

}// namespace tentris::store::graphql::internal

#endif//TENTRIS_GRAPHQLQUERYVISITOR_HPP
