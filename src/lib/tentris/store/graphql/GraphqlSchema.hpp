#ifndef TENTRIS_GRAPHQLSCHEMA_HPP
#define TENTRIS_GRAPHQLSCHEMA_HPP

#include "tentris/store/graphql/GraphqlDocument.hpp"
#include "tentris/store/graphql/internal/GraphqlDataStructures.hpp"
#include "tentris/store/graphql/internal/GraphqlDirectiveVisitor.hpp"

namespace tentris::store::graphql {

	using ObjectData = ::tentris::store::graphql::internal::ObjectData;
	using FieldData = ::tentris::store::graphql::internal::FieldData;

	namespace internal {

		class GraphqlFieldDefinitionVisitor : public facebook::graphql::ast::visitor::AstVisitor {

		private:

			GraphqlDirectiveVisitor directive_visitor{};

        public:

            std::string field_name{};
			FieldData data{};

			// visit functions

            bool visitFieldDefinition(const facebook::graphql::ast::FieldDefinition &fieldDefinition) override {
                field_name = fieldDefinition.getName().getValue();
                return true;
            }

            bool visitNamedType(const facebook::graphql::ast::NamedType &namedType) override {
                data.type_name = namedType.getName().getValue();
                return false;
            }

            bool visitListType([[maybe_unused]] const facebook::graphql::ast::ListType &listType) override {
                data.is_list = true;
                return true;
            }

            bool visitNonNullType([[maybe_unused]] const facebook::graphql::ast::NonNullType &nonNullType) override {
                if(data.is_list) // [ NamedType! ] !
                    data.non_null_list_values = true;
                else // NamedType!
                    data.non_null = true;
                return true;
            }

            // skip arguments -- todo
            bool visitArgument([[maybe_unused]] const facebook::graphql::ast::Argument &argument) override {
                return false;
            }

		    // we are currently only interested in the @uri(value: <uri>) directive
            bool visitDirective(const facebook::graphql::ast::Directive &directive) override {
                if(strcmp(directive.getName().getValue(), "uri") == 0) {
                    directive.accept(&directive_visitor);
					try {
						data.uri = directive_visitor.args_vals.at("value");
					}
					catch (const std::out_of_range& e) {
						throw std::runtime_error("The directive @uri take exactly one argument: `value`");
					}
                }
                else
                    throw std::runtime_error("Only the directive `@uri(value: String!)` is currently supported on field definitions");
                return false;
            }

		};

	} // tentris::store::graphql::internal

    class GraphqlSchema {

	private:

		using TypeName = std::string;

        TypeName query_type_name = "Query";
		// for each object type stores its information
        std::map<TypeName , ObjectData> objects_data{};

    public:

		GraphqlSchema() = default;

		void load(const GraphqlDocument &document) {
            // iterate over and parse the object definitions of the the provided schema
            for(const auto& obj_def : document.getObjectDefs()) {
				auto &obj_data = objects_data[obj_def->getName().getValue()];
				// iterate over the field definitions of the current object definition
				for(auto &field_def : obj_def->getFields()) {
					internal::GraphqlFieldDefinitionVisitor field_def_visitor{};
					field_def->accept(&field_def_visitor);
					obj_data.fields_data[field_def_visitor.field_name] = field_def_visitor.data;
				}
				if(obj_def->getDirectives()) {
					for(auto &directive : *(obj_def->getDirectives())) {
						internal::GraphqlDirectiveVisitor directive_visitor{};
                        if(strcmp(directive->getName().getValue(), "uri") == 0) {
                            directive->accept(&directive_visitor);
                            try {
                                obj_data.uri = directive_visitor.args_vals.at("value");
                            }
                            catch (const std::out_of_range& e) {
                                throw std::runtime_error("The directive @uri take exactly one argument: `value`");
                            }
                        }
                        else
                            throw std::runtime_error("Only the directive `@uri(value: String!)` "
													 "is currently supported on object definitions");
					}
				}
			}
		}

		// getters

        [[nodiscard]] const std::string& getFieldUri(const std::string& field_name) const {
            auto type_name = objects_data.at(query_type_name).fields_data.at(field_name).type_name;
            return objects_data.at(type_name).uri;
        }

        [[nodiscard]] const std::string& getFieldUri(const std::string& field_name, const std::string& parent_type) const {
            return objects_data.at(parent_type).fields_data.at(field_name).uri;
        }

        [[nodiscard]] const std::string& getFieldType(const std::string& field_name) const {
            return objects_data.at(query_type_name).fields_data.at(field_name).type_name;
        }

        [[nodiscard]] const std::string& getFieldType(const std::string& field_name, const std::string& parent_type) const {
            return objects_data.at(parent_type).fields_data.at(field_name).type_name;
        }

        [[nodiscard]] bool fieldIsList(const std::string& field_name) const {
            return objects_data.at(query_type_name).fields_data.at(field_name).is_list;
        }

        [[nodiscard]] bool fieldIsList(const std::string& field_name, const std::string& parent_type) const {
            return objects_data.at(parent_type).fields_data.at(field_name).is_list;
        }

        [[nodiscard]] bool fieldIsNonNull(const std::string& field_name) const {
            return objects_data.at(query_type_name).fields_data.at(field_name).non_null;
        }

        [[nodiscard]] bool fieldIsNonNull(const std::string& field_name, const std::string& parent_type) const {
            return objects_data.at(parent_type).fields_data.at(field_name).non_null;
        }

	};

}

#endif//TENTRIS_GRAPHQLSCHEMA_HPP
