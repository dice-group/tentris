#ifndef TENTRIS_GRAPHQLFIELDDEFINITIONVISITOR_H
#define TENTRIS_GRAPHQLFIELDDEFINITIONVISITOR_H

#include <libgraphqlparser/AstVisitor.h>

#include "tentris/store/graphql/internal/GraphqlValueVisitor.hpp"

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;


	class GraphqlFieldDefinitionVisitor : public AstVisitor {

	private:

		GraphqlValueVisitor value_visitor{};

		std::string field_name;
		std::string uri;
		std::string type_name;
		bool non_null = false;
		bool is_list = false;
		bool non_null_list_values = false;

	public:

		// getters

		[[nodiscard]] const std::string &getFieldName() const {
			return field_name;
		}

        [[nodiscard]] const std::string &getUri() const {
			return uri;
		}

        [[nodiscard]] const std::string &getTypeName() const {
			return type_name;
		}

		// visit functions

		bool visitFieldDefinition(const facebook::graphql::ast::FieldDefinition &fieldDefinition) override {
			field_name = fieldDefinition.getName().getValue();
			return true;
		}

		bool visitNamedType(const facebook::graphql::ast::NamedType &namedType) override {
			type_name = namedType.getName().getValue();
			return false;
		}

		bool visitListType(const facebook::graphql::ast::ListType &listType) override {
			is_list = true;
			return true;
		}

		bool visitNonNullType(const facebook::graphql::ast::NonNullType &nonNullType) override {
			if(is_list) // [ NamedType! ] !
				non_null_list_values = true;
			else // NamedType!
				non_null = true;
			return true;
		}

		// skip arguments -- todo
		bool visitArgument(const facebook::graphql::ast::Argument &argument) override {
			return false;
		}

		// we are currently only interested in the @uri(value: <uri>) directive
        bool visitDirective(const facebook::graphql::ast::Directive &directive) override {
            if(strcmp(directive.getName().getValue(), "uri") == 0) {
                if(not directive.getArguments() or directive.getArguments()->size() > 1)
                    throw std::runtime_error("The directive @uri take exactly one argument: `value`");
                auto& value_argument = *(directive.getArguments()->begin());
                value_argument->getValue().accept(&value_visitor);
                uri = value_visitor.getValue();
            }
            else
                throw std::runtime_error("Only the directive `@uri(value: String!)` is currently supported on field definitions");
            return false;
        }

	};

}

#endif//TENTRIS_GRAPHQLFIELDDEFINITIONVISITOR_H
