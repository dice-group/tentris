#include "GraphQLSchemaVisitor.hpp"

namespace tentris::store::graphql::internal {
	antlrcpp::Any GraphQLSchemaVisitor::visitDocument(base::GraphQLParser::DocumentContext *ctx) {
		for (const auto &definition : ctx->definition())
			if (definition->typeSystemDefinition())
				visitTypeSystemDefinition(definition->typeSystemDefinition());
		return nullptr;
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitTypeSystemDefinition(base::GraphQLParser::TypeSystemDefinitionContext *ctx) {
		if (ctx->typeDefinition())
			visitTypeDefinition(ctx->typeDefinition());
		return nullptr;
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitTypeDefinition(base::GraphQLParser::TypeDefinitionContext *ctx) {
		if (ctx->objectTypeDefinition())
			visitObjectTypeDefinition(ctx->objectTypeDefinition());
		else if (ctx->interfaceTypeDefinition())
			visitInterfaceTypeDefinition(ctx->interfaceTypeDefinition());
		return nullptr;
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitObjectTypeDefinition(base::GraphQLParser::ObjectTypeDefinitionContext *ctx) {
		// create new object definition
		ObjectData obj_def{};
		// store the name of the type
		std::string name = ctx->name()->getText();
		// iterate over the directives of the object definition
		if (ctx->directives()) {
			for (const auto &dir_ctx : ctx->directives()->directive()) {
				auto [name, args] = visitDirective(dir_ctx).as<std::pair<std::string, std::map<std::string, std::string>>>();
				if (name != "uri")
					continue;
				try {
					obj_def.uri = args.at("value");
				} catch (std::out_of_range &e) {
					throw ArgumentNotFoundException("value", "directive", name);
				}
			}
		}
		// iterate over the field definitions of the object definition
		if (ctx->fieldsDefinition()) {
			for (const auto &fd_ctx : ctx->fieldsDefinition()->fieldDefinition()) {
				auto [name, field_def] = visitFieldDefinition(fd_ctx).as<std::pair<std::string, FieldData>>();
				obj_def.fields_data[name] = std::move(field_def);
			}
		}
		// iterate over the implemented interfaces
		auto current_ctx = ctx->implementsInterfaces();
		while (current_ctx) {
			obj_def.implementations.insert(current_ctx->namedType()->name()->getText());
			current_ctx = (current_ctx->implementsInterfaces());
		}
		schema->addObjectType(name, std::move(obj_def));
		return nullptr;
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitInterfaceTypeDefinition(base::GraphQLParser::InterfaceTypeDefinitionContext *ctx) {
		// create new interface definition
		InterfaceData interface_def{};
		// store the name of the interface
		std::string name = ctx->name()->getText();
		// iterate over the directives of the interface definition
		if (ctx->directives()) {
			for (const auto &dir_ctx : ctx->directives()->directive()) {
				auto [name, args] = visitDirective(dir_ctx).as<std::pair<std::string, std::map<std::string, std::string>>>();
				if (name != "uri")
					continue;
				try {
					interface_def.uri = args.at("value");
				} catch (std::out_of_range &e) {
					throw ArgumentNotFoundException("value", "directive", name);
				}
			}
		}
		// iterate over the field definitions of the interface definition
		if (ctx->fieldsDefinition()) {
			for (const auto &fd_ctx : ctx->fieldsDefinition()->fieldDefinition()) {
				auto [name, field_def] = visitFieldDefinition(fd_ctx).as<std::pair<std::string, FieldData>>();
				interface_def.fields_data[name] = std::move(field_def);
			}
		}
		schema->addInterfaceType(name, std::move(interface_def));
		return nullptr;
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitFieldDefinition(base::GraphQLParser::FieldDefinitionContext *ctx) {
		// create new field definition
		FieldData field_def{};
		// store the name of the field
		std::string name = ctx->name()->getText();
		// iterate over the arguments
		if (ctx->argumentsDefinition()) {
			for (const auto &input_ctx : ctx->argumentsDefinition()->inputValueDefinition()) {
				const auto &arg_name = input_ctx->name()->getText();
				const auto &arg_type = input_ctx->type_()->namedType()->name()->getText();
				field_def.arguments[arg_name] = arg_type;
			}
		}
		// iterate over the directives of the field definition
		if (ctx->directives()) {
			for (const auto &dir_ctx : ctx->directives()->directive()) {
				auto [name, args] = visitDirective(dir_ctx).as<std::pair<std::string, std::map<std::string, std::string>>>();
				if (name == "uri") {
					try {
						field_def.uri = args.at("value");
					} catch (std::out_of_range &e) {
						throw ArgumentNotFoundException("value", "directive", name);
					}
				} else if (name == "inverse") {
					field_def.is_inverse = true;
				}
			}
		}
		// find the type of the field
		if (ctx->type_()) {
			// check if it is a non-null type
			if (ctx->type_()->getText().ends_with('!'))
				field_def.non_null = true;
			// check if it is a list type
			if (ctx->type_()->listType()) {
				field_def.is_list = true;
				// check if list is allowed to have null values
				if (ctx->type_()->listType()->type_()->getText().ends_with('!'))
					field_def.non_null_list_values = true;
				// get the name of the type
				field_def.type_name = ctx->type_()->listType()->type_()->namedType()->getText();
			}
			// get the name of the type
			else
				field_def.type_name = ctx->type_()->namedType()->getText();
		}
		return std::make_pair(name, field_def);
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitDirective(base::GraphQLParser::DirectiveContext *ctx) {
		std::string name = ctx->name()->getText();
		std::map<std::string, std::string> arguments{};
		if (ctx->arguments()) {
			for (const auto &argument : ctx->arguments()->argument()) {
				auto arg_name = argument->name()->getText();
				auto value = visitValue(argument->value());
				arguments[arg_name] = value.as<std::string>();
			}
		}
		return std::make_pair(name, arguments);
	}

	antlrcpp::Any GraphQLSchemaVisitor::visitValue(base::GraphQLParser::ValueContext *ctx) {
		if (ctx->objectValue())
			throw std::runtime_error("ObjectValues in arguments are not supported");
		if (ctx->listValue())
			throw std::runtime_error("Lists of values in arguments are not supported");
		if (ctx->stringValue())
			return ctx->stringValue()->getText().substr(1, ctx->stringValue()->getText().size() - 2);
		else if (ctx->intValue())
			return ctx->intValue()->getText();
		else if (ctx->floatValue())
			return ctx->floatValue()->getText();
		else if (ctx->booleanValue())
			return ctx->booleanValue()->getText();
		return nullptr;
	}
}// namespace tentris::store::graphql::internal