#ifndef TENTRIS_GRAPHQLOBJECTTYPEVISITOR_HPP
#define TENTRIS_GRAPHQLOBJECTTYPEVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

#include "tentris/store/graphql/internal/GraphqlFieldDefinitionVisitor.hpp"
#include "tentris/store/graphql/internal/GraphqlValueVisitor.hpp"

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;


    class GraphqlObjectTypeVisitor : public AstVisitor {

	public:

        struct FieldData {
            std::string type;
            std::string uri;
        };

    private:

        GraphqlValueVisitor value_visitor{};

        std::string obj_name;
		std::string uri;
		std::map<std::string, FieldData> fields_data{};

    public:

		// getters

		[[nodiscard]] const std::string &getObjName() const {
			return obj_name;
		}

        [[nodiscard]] const std::string &getUri() const {
			return uri;
		}

        [[nodiscard]] const std::map<std::string, FieldData> &getFieldsData() const {
			return fields_data;
		}

		// visit functions

		void endVisitObjectTypeDefinition(const facebook::graphql::ast::ObjectTypeDefinition &objectTypeDefinition) override {
			obj_name = objectTypeDefinition.getName().getValue();
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
				throw std::runtime_error("Only the directive `@uri(value: String!)` is currently supported on type definitions");
            return false;
        }

        bool visitFieldDefinition(const facebook::graphql::ast::FieldDefinition &fieldDefinition) override {
            GraphqlFieldDefinitionVisitor field_def_visitor{};
			FieldData cur_field_data{};
			fieldDefinition.accept(&field_def_visitor);
			cur_field_data.type = field_def_visitor.getTypeName();
			cur_field_data.uri = field_def_visitor.getUri();
			fields_data[field_def_visitor.getFieldName()] = std::move(cur_field_data);
            return false;
        }
    };

}

#endif//TENTRIS_GRAPHQLOBJECTTYPEVISITOR_HPP
