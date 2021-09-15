#include "GraphQLQueryVisitor.hpp"

namespace tentris::store::graphql::internal {
	antlrcpp::Any
	GraphQLQueryVisitor::visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) {
		parent_type.push_back(schema->getQueryType());
		// iterate over all root fields -- creates a ParsedSubGraphQL for each root field
		for (const auto &root_field : ctx->selectionSet()->selection()) {
			if (not root_field->field())
				throw std::logic_error(fmt::format("Inline fragments are not allowed in query type `{}`",
												   schema->getQueryType()));
			ParsedSubGraphQL sub_query{};
			field_name = root_field->field()->name()->getText();
			sub_query.field_names[field_label] = (not root_field->field()->alias()) ? field_name
																					: root_field->field()->alias()->name()->getText();
			if (schema->fieldIsList(field_name))
				sub_query.list_labels.insert(field_label);
			if (schema->fieldIsNonNull(field_name))
				sub_query.non_null_labels.insert(field_label);
			const auto &field_type = schema->getFieldType(field_name);
			sub_query.features.emplace_back(Feature{schema->getObjectUri(field_type)});
			sub_query.result_labels.push_back(field_label);
			// root field is a leaf field
			if (not root_field->field()->selectionSet()) {
				auto operands_labels = OperandLabels{field_label, ++field_label};
				sub_query.operands_labels.emplace_back(operands_labels);
				sub_query.result_labels.push_back(field_label);
				sub_query.paths.emplace_back(std::move(operands_labels));
				parsed_query->push_back(std::move(sub_query));
			} else {
				sub_query.operands_labels.emplace_back(OperandLabels{field_label});
				parsed_query->push_back(std::move(sub_query));
				// parse arguments of root field
				if (root_field->field()->arguments()) {
					operand_pos = 0;
					visitArguments(root_field->field()->arguments());
				}
				selection_set_label.push_back(field_label);
				parent_type.push_back(field_type);
				active_path.push_back(field_label);
				// not in framgent -- empty type condition
				type_conditions.emplace_back("");
				if (root_field->field()->selectionSet())
					visitSelectionSet(root_field->field()->selectionSet());
				selection_set_label.pop_back();
				parent_type.pop_back();
				type_conditions.pop_back();
				active_path.pop_back();
			}
			// reset
			next_label = 'a';
			field_label = 'a';
		}
		return nullptr;
	}

	antlrcpp::Any
	GraphQLQueryVisitor::visitSelectionSet(base::GraphQLParser::SelectionSetContext *ctx) {
		std::map<std::string, base::GraphQLParser::FieldContext *> field_selections{};
		std::map<std::string, base::GraphQLParser::InlineFragmentContext *> inline_fragment_selections{};
		// collect fields
		for (const auto &selection : ctx->selection()) {
			if (selection->field()) {
				auto f_name = (not selection->field()->alias()) ? selection->field()->name()->getText()
																: selection->field()->alias()->name()->getText();
				if (not field_selections.contains(f_name)) {
					field_selections[f_name] = selection->field();
				}
				else if (selection->field()->selectionSet()) {
					// ensure that in case of field aliases the type of the fields are equal
					if (schema->getFieldType(selection->field()->name()->getText(), parent_type.back()) !=
						schema->getFieldType(f_name, parent_type.back()))
						throw NotCompatibleFieldAliasesException(f_name);
					for (auto &sel : selection->field()->selectionSet()->selection())
						field_selections[f_name]->selectionSet()->addChild(sel);
				}
			} else if (selection->inlineFragment()) {
				auto type_name = selection->inlineFragment()->typeCondition()->namedType()->name()->getText();
				if (not inline_fragment_selections.contains(type_name))
					inline_fragment_selections[type_name] = selection->inlineFragment();
				else
					for (auto &sel : selection->inlineFragment()->selectionSet()->selection())
						inline_fragment_selections[type_name]->selectionSet()->addChild(sel);
			}
		}
		for (const auto &selection : ctx->selection()) {
			if (selection->field()) {
				auto f_name = (not selection->field()->alias()) ? selection->field()->name()->getText()
																: selection->field()->alias()->name()->getText();
				if (not field_selections.contains(f_name))
					continue;
				field_name = selection->field()->name()->getText();
				visitField(field_selections[f_name]);
				field_selections.erase(f_name);
			} else if (selection->inlineFragment())
				visitInlineFragment(selection->inlineFragment());
		}
		return nullptr;
	}

	antlrcpp::Any
	GraphQLQueryVisitor::visitField(base::GraphQLParser::FieldContext *ctx) {
		field_label = ++next_label;
		active_path.push_back(field_label);
		parsed_query->back().field_names[field_label] = (not ctx->alias()) ? field_name
																		   : ctx->alias()->name()->getText();
		const auto &uri = schema->getFieldUri(field_name, parent_type.back());
		const auto &type = schema->getFieldType(field_name, parent_type.back());
		bool is_inverse = schema->fieldIsInverse(field_name, parent_type.back());
		if (schema->fieldIsList(field_name, parent_type.back()))
			parsed_query->back().list_labels.insert(field_label);
		if (schema->fieldIsNonNull(field_name, parent_type.back()))
			parsed_query->back().non_null_labels.insert(field_label);
		if (not type_conditions.back().empty())
			parsed_query->back().fragment_dependencies[selection_set_label.back()][type_conditions.back()].push_back(field_label);
		if (type != "ID") {
			parsed_query->back().operands_labels.emplace_back(OperandLabels{'['});
			if (not is_inverse)
				parsed_query->back().operands_labels.emplace_back(OperandLabels{selection_set_label.back(), field_label});
			else
				parsed_query->back().operands_labels.emplace_back(OperandLabels{field_label, selection_set_label.back()});
			// the labels of all fields will go into the result labels
			parsed_query->back().result_labels.push_back(field_label);
			// the uri of the field -- empty if a uri was not provided
			parsed_query->back().features.emplace_back(Feature{uri});
		}
		// leaf field - we reached the end of the path
		if (not ctx->selectionSet()) {
			parsed_query->back().leaf_types[field_label] = type;
			parsed_query->back().paths.emplace_back(active_path);
			// the label of ID fields do not go into the result -- map it to the original label
		} else {
			// not in fragment -- empty type condition
			type_conditions.emplace_back("");
			// check if type filter is required
			if (schema->typeFilter(field_name, parent_type.back())) {
				parsed_query->back().operands_labels.emplace_back(OperandLabels{field_label});
				parsed_query->back().features.emplace_back(Feature{schema->getObjectUri(type)});
			}
			// visit arguments
			if (ctx->arguments())
				visitArguments(ctx->arguments());
			// visit nested fields
			parent_type.push_back(schema->getFieldType(field_name, parent_type.back()));
			selection_set_label.push_back(field_label);
			visitSelectionSet(ctx->selectionSet());
			selection_set_label.pop_back();
			parent_type.pop_back();
			type_conditions.pop_back();
		}
		if (type != "ID")
			parsed_query->back().operands_labels.emplace_back(OperandLabels{']'});
		// remove field name from path
		active_path.pop_back();
		return nullptr;
	}

	antlrcpp::Any
	GraphQLQueryVisitor::visitInlineFragment(base::GraphQLParser::InlineFragmentContext *ctx) {
		if (ctx->typeCondition()) {
			const auto &type_condition = ctx->typeCondition()->namedType()->name()->getText();
			// check if the type is an implementation of the parent type
			if (not schema->implementsInterface(type_condition, parent_type.back()))
				throw InterfaceNotImplementedExecption(type_condition, parent_type.back());
			parsed_query->back().features.emplace_back(Feature{schema->getObjectUri(type_condition)});
			// the inline fragment adds an additional optional layer
			parsed_query->back().operands_labels.emplace_back(OperandLabels{'['});
			parsed_query->back().operands_labels.emplace_back(OperandLabels{selection_set_label.back()});
			type_conditions.push_back(schema->getObjectUri(type_condition));
			parent_type.push_back(type_condition);
			visitSelectionSet(ctx->selectionSet());
			parent_type.pop_back();
			type_conditions.pop_back();
			// close the optional layer of the inline fragment
			parsed_query->back().operands_labels.emplace_back(OperandLabels{']'});
		}
		return nullptr;
	}

	antlrcpp::Any
	GraphQLQueryVisitor::visitArguments(base::GraphQLParser::ArgumentsContext *ctx) {
		for (const auto &arg : ctx->argument()) {
			const auto &name = arg->name()->getText();
			// if the argument is not part of the field -> exception
			const auto &type = schema->getArgumentType(name, field_name, parent_type.back());
			std::string value;
			if (arg->value()->stringValue()) {
				value = arg->value()->stringValue()->getText();
				value = value.substr(1, value.size() - 2);
			} else if (arg->value()->intValue())
				value = arg->value()->intValue()->getText();
			else if (arg->value()->floatValue())
				value = arg->value()->floatValue()->getText();
			else if (arg->value()->booleanValue())
				value = (arg->value()->booleanValue()->getText() == "true" ? "true" : "false");
			if (type == "ID")
				parsed_query->back().id_arguments[operand_pos] = value;
			else {
				parsed_query->back().operands_labels.emplace_back(OperandLabels{field_label});
				parsed_query->back().features.emplace_back(
						Feature{schema->getFieldUri(name, schema->getFieldType(field_name, parent_type.back())), value, type});
			}
		}
		return nullptr;
	}
}// namespace tentris::store::graphql::internal