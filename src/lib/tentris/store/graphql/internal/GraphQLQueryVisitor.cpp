#include "GraphQLQueryVisitor.hpp"

namespace tentris::store::graphql::internal {
	antlrcpp::Any
	GraphQLQueryVisitor::visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) {
		// iterate over all root fields
		for (const auto &root_field : ctx->selectionSet()->selection()) {
			assert(root_field->field());
			ParsedSubGraphQL sub_query{};
			sub_query.fields_name_arguments.emplace_back(FieldName(root_field->field()->name()->getText()));
			sub_query.result_labels.push_back(field_label);
			active_path.push_back({field_label, root_field->field()->name()->getText()});
			// root field that is a leaf field
			if (not root_field->field()->selectionSet()) {
				sub_query.operands_labels.emplace_back(OperandLabels{field_label, ++field_label});
				sub_query.result_labels.push_back(field_label);
				parsed_query->push_back(std::move(sub_query));
			} else {
				sub_query.operands_labels.emplace_back(OperandLabels{field_label});
                parsed_query->push_back(std::move(sub_query));
				// parse arguments of root field
				if (root_field->field()->arguments())
					visitArguments(root_field->field()->arguments());
				selection_set_label.push_back(field_label);
				if (root_field->field()->selectionSet())
					visitSelectionSet(root_field->field()->selectionSet());
				selection_set_label.pop_back();
			}
			// reset
			next_label = 'a';
			field_label = 'a';
			active_path.clear();
			in_fragment = false;
		}
		return nullptr;
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
			parsed_query->back().fragment_labels.insert(field_label);
		const auto &field_name = ctx->name()->getText();
		active_path.push_back({field_label, field_name});
		// beginning of optional part
		parsed_query->back().operands_labels.emplace_back(OperandLabels{'['});
		parsed_query->back().operands_labels.emplace_back(OperandLabels{selection_set_label.back(), field_label});
		// the labels of all fields will go into the result labels
		parsed_query->back().result_labels.push_back(field_label);
		parsed_query->back().fields_name_arguments.emplace_back(FieldName(field_name));
		// leaf field - we reached the end of the path
		if (not ctx->selectionSet())
			parsed_query->back().paths.emplace_back(active_path);
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
		parsed_query->back().operands_labels.emplace_back(OperandLabels{']'});
		// remove field name from path
		active_path.pop_back();
		return nullptr;
	}

	antlrcpp::Any
	GraphQLQueryVisitor::visitInlineFragment(base::GraphQLParser::InlineFragmentContext *ctx) {
		if (ctx->typeCondition()) {
			parsed_query->back().fields_name_arguments.push_back(ctx->typeCondition()->namedType()->name()->getText());
			// the inline fragment adds an additional optional layer
			parsed_query->back().operands_labels.emplace_back(OperandLabels{'['});
			parsed_query->back().operands_labels.emplace_back(OperandLabels{selection_set_label.back()});
			in_fragment = true;
			visitSelectionSet(ctx->selectionSet());
			// close the optional layer of the inline fragment
			parsed_query->back().operands_labels.emplace_back(OperandLabels{']'});
		}
		return nullptr;
	}

	antlrcpp::Any
	GraphQLQueryVisitor::visitArguments(base::GraphQLParser::ArgumentsContext *ctx) {
		for (const auto &arg : ctx->argument()) {
			// add operand labels
			parsed_query->back().operands_labels.emplace_back(OperandLabels{field_label});
			auto name = arg->name()->getText();
			std::string value;
			// argument value coercion
			if (arg->value()->stringValue()) {
				value = arg->value()->stringValue()->getText();
				value = value.substr(1, value.size() - 2);
			} else if (arg->value()->intValue())
				value = arg->value()->intValue()->getText();
			else if (arg->value()->floatValue())
				value = arg->value()->floatValue()->getText();
			else if (arg->value()->booleanValue())
				value = (arg->value()->booleanValue()->getText() == "true" ? "true" : "false");
			else
				throw std::invalid_argument("Non scalar values for arguments are not supported");
			parsed_query->back().fields_name_arguments.emplace_back(Argument(name, value));
		}
		return nullptr;
	}
}// namespace tentris::store::graphql::internal