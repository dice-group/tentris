#include "GraphQLQueryVisitor.hpp"

namespace tentris::store::graphql::internal {
	antlrcpp::Any
	GraphQLQueryVisitor::visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) {
		// iterate over all root fields
		for (const auto &root_field : ctx->selectionSet()->selection()) {
			assert(root_field->field());
			field_label = next_label;
			active_path.push_back({field_label, root_field->field()->name()->getText()});
			parsed_query->all_operands_labels.emplace_back(OperandsLabels());
			parsed_query->all_operands_labels.back().emplace_back(OperandLabels{field_label});
			parsed_query->all_result_labels.emplace_back(ResultsLabels());
			parsed_query->all_result_labels.back().push_back(field_label);
			parsed_query->all_fields_name_arguments.emplace_back(FieldsNameArguments());
			parsed_query->all_fields_name_arguments.back().emplace_back(FieldName(root_field->field()->name()->getText()));
			parsed_query->all_paths.emplace_back(Paths());
			parsed_query->all_fragment_labels.emplace_back(std::set<char>());
			// parse arguments of root field
			if (root_field->field()->arguments())
				visitArguments(root_field->field()->arguments());
			selection_set_label.push_back(field_label);
			if (root_field->field()->selectionSet())
				visitSelectionSet(root_field->field()->selectionSet());
			selection_set_label.pop_back();
			// reset
			next_label = 'a';
			active_path.clear();
			in_fragment = false;
		}
		return parsed_query;
	}

	antlrcpp::Any
    GraphQLQueryVisitor::visitSelectionSet(base::GraphQLParser::SelectionSetContext *ctx) {
		for (const auto &selection : ctx->selection()) {
			if (selection->field())
				visitField(selection->field());
			else if (selection->inlineFragment())
				visitInlineFragment(selection->inlineFragment());
		}
		return nullptr;
	}

	antlrcpp::Any
    GraphQLQueryVisitor::visitField(base::GraphQLParser::FieldContext *ctx) {
		field_label = ++next_label;
		if (in_fragment)
			parsed_query->all_fragment_labels.back().insert(field_label);
		const auto &field_name = ctx->name()->getText();
		active_path.push_back({field_label, field_name});
		// beginning of optional part
		parsed_query->all_operands_labels.back().emplace_back(OperandLabels{'['});
		parsed_query->all_operands_labels.back().emplace_back(OperandLabels{selection_set_label.back(), field_label});
		// the labels of all fields will go into the result labels
		parsed_query->all_result_labels.back().push_back(field_label);
		parsed_query->all_fields_name_arguments.back().emplace_back(FieldName(field_name));
		// leaf field - we reached the end of the path
		if (not ctx->selectionSet())
			parsed_query->all_paths.back().emplace_back(active_path);
		else {
			in_fragment = false;
			// visit arguments
			if (ctx->arguments())
				visitArguments(ctx->arguments());
			// visit nested fields
			selection_set_label.push_back(field_label);
			visitSelectionSet(ctx->selectionSet());
			selection_set_label.pop_back();
		}
		// end of optional part
		parsed_query->all_operands_labels.back().emplace_back(OperandLabels{']'});
		// remove field name from path
		active_path.pop_back();
		return nullptr;
	}

	antlrcpp::Any
    GraphQLQueryVisitor::visitInlineFragment(base::GraphQLParser::InlineFragmentContext *ctx) {
		if (ctx->typeCondition()) {
			parsed_query->all_fields_name_arguments.back().push_back(ctx->typeCondition()->namedType()->name()->getText());
			// the inline fragment adds an additional optional layer
			parsed_query->all_operands_labels.back().emplace_back(OperandLabels{'['});
			parsed_query->all_operands_labels.back().emplace_back(OperandLabels{selection_set_label.back()});
			in_fragment = true;
			visitSelectionSet(ctx->selectionSet());
			// close the optional layer of the inline fragment
			parsed_query->all_operands_labels.back().emplace_back(OperandLabels{']'});
		}
		return nullptr;
	}

	antlrcpp::Any
    GraphQLQueryVisitor::visitArguments(base::GraphQLParser::ArgumentsContext *ctx) {
		for (const auto &arg : ctx->argument()) {
			// add operand labels
			parsed_query->all_operands_labels.back().emplace_back(OperandLabels{field_label});
			auto name = arg->name()->getText();
			std::any value;
			// argument value coercion
			if (arg->value()->stringValue())
				value = arg->value()->stringValue()->getText();
			else if (arg->value()->intValue())
				value = std::stoi(arg->value()->intValue()->getText());
			else if (arg->value()->floatValue())
				value = std::stof(arg->value()->floatValue()->getText());
			else if (arg->value()->booleanValue())
				value = arg->value()->booleanValue()->getText() == "true";
			else
				throw std::invalid_argument("Non scalar values for arguments are not supported");
			parsed_query->all_fields_name_arguments.back().emplace_back(Argument(name, value));
		}
		return nullptr;
	}
}// namespace tentris::store::graphql::internal