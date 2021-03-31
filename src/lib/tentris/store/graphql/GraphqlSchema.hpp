#ifndef TENTRIS_GRAPHQLSCHEMA_HPP
#define TENTRIS_GRAPHQLSCHEMA_HPP

#include <Dice/graphql-parser/GraphQLParser.hpp>

#include "tentris/store/graphql/exception/SchemaException.hpp"
#include "tentris/store/graphql/internal/GraphqlDataStructures.hpp"

namespace tentris::store::graphql {

	using namespace tentris::store::graphql::exception;
    using ObjectData = ::tentris::store::graphql::internal::ObjectData;

    class GraphqlSchema {

    private:

        std::string query_type_name = "Query";
        // for each object type stores its information
        std::map<std::string , ObjectData> objects_data{};

    public:

        GraphqlSchema() = default;

        void load(const std::string& document) {
            // parse the schema
			auto type_definitions = Dice::graphql_parser::GraphQLParser::parseSchema(document);
            // iterate over the object defintions of the schema
            for(const auto &obj_def : type_definitions->object_definitions) {
                auto &obj_data = objects_data[obj_def->name];
                // iterate over the directives of the object definition
                for(const auto &directive : obj_def->directives) {
                    // we are currently interested only in the @uri directive
                    if(directive->name != "uri")
                        continue;
                    // get the uri of the object and remove quotation marks
                    obj_data.uri = directive->arguments["value"].substr(1, directive->arguments["value"].size()-2);
                }
                // iterate over the field definitions of the object definition
                for(const auto &field_def : obj_def->field_definitions) {
                    auto &field_data = obj_data.fields_data[field_def->name];
                    // iterate over the directives of the field defintions
                    for(const auto &directive : field_def->directives) {
                        // we are currently interested only in the @uri and @inverse directives
                        if(directive->name == "uri")
                            field_data.uri = directive->arguments["value"].substr(1, directive->arguments["value"].size()-2);
						else if(directive->name == "inverse")
							field_data.is_inverse = true;
                    }
                    field_data.non_null = field_def->non_null;
                    field_data.type_name = field_def->type_name;
                    field_data.is_list = field_def->is_list;
                    field_data.non_null_list_values = not field_def->null_list_values;
                }
            }
			// iterate over interface definitions
            for(const auto &obj_def : type_definitions->interface_definitions) {
                auto &obj_data = objects_data[obj_def->name];
                // iterate over the directives of the object definition
                for(const auto &directive : obj_def->directives) {
                    // we are currently interested only in the @uri directive
                    if(directive->name != "uri")
                        continue;
                    // get the uri of the object and remove quotation marks
                    obj_data.uri = directive->arguments["value"].substr(1, directive->arguments["value"].size()-2);
                }
                // iterate over the field definitions of the object definition
                for(const auto &field_def : obj_def->field_definitions) {
                    auto &field_data = obj_data.fields_data[field_def->name];
                    // iterate over the directives of the field defintions
                    for(const auto &directive : field_def->directives) {
                        // we are currently interested only in the @uri and @inverse directives
                        if(directive->name == "uri")
                            field_data.uri = directive->arguments["value"].substr(1, directive->arguments["value"].size()-2);
                        else if(directive->name == "inverse")
                            field_data.is_inverse = true;
                    }
                    field_data.non_null = field_def->non_null;
                    field_data.type_name = field_def->type_name;
                    field_data.is_list = field_def->is_list;
                    field_data.non_null_list_values = not field_def->null_list_values;
                }
            }
        }

        // getters

        [[nodiscard]] const std::string& getFieldUri(const std::string& field_name,
													 const std::string& parent_type) const {
            return objects_data.at(parent_type).fields_data.at(field_name).uri;
        }

        [[nodiscard]] const std::string& getFieldType(const std::string& field_name,
													  const std::string& parent_type = "") const {
            if(parent_type.empty())
				try {
					return objects_data.at(query_type_name).fields_data.at(field_name).type_name;
				} catch (std::out_of_range &e) {
					throw FieldNotFoundException(field_name, query_type_name);
				}
            else
				try {
					return objects_data.at(parent_type).fields_data.at(field_name).type_name;
				} catch (std::out_of_range &e) {
                    throw FieldNotFoundException(field_name, parent_type);
				}
        }

		[[nodiscard]] const std::string& getObjectUri(const std::string& field_name) {
			try {
				return objects_data.at(field_name).uri;
			} catch (std::out_of_range &e) {
                throw TypeNotFoundException(field_name);
			}
		}

        [[nodiscard]] bool fieldIsList(const std::string& field_name,
									   const std::string& parent_type = "") const {
			if(parent_type.empty())
                return objects_data.at(query_type_name).fields_data.at(field_name).is_list;
			else
			    return objects_data.at(parent_type).fields_data.at(field_name).is_list;
        }

		[[nodiscard]] bool fieldIsScalar(const std::string& field_name,
                                         const std::string& parent_type) const {
			if (parent_type.empty())
				return false;
            auto type_name = objects_data.at(parent_type).fields_data.at(field_name).type_name;
            if(type_name == "ID" or type_name == "String" or type_name == "Int" or type_name == "Float" or type_name == "Boolean")
                return true;
            return false;
		}

        [[nodiscard]] bool fieldIsNonNull(const std::string& field_name,
										  const std::string& parent_type = "") const {
			if(parent_type.empty())
                return objects_data.at(query_type_name).fields_data.at(field_name).non_null;
			else
                return objects_data.at(parent_type).fields_data.at(field_name).non_null;
        }

        [[nodiscard]] bool fieldIsInverse(const std::string& field_name,
                                          const std::string& parent_type) const {
			return objects_data.at(parent_type).fields_data.at(field_name).is_inverse;
        }

		[[nodiscard]] bool typeFilter(const std::string& uri,
									  const std::string& type,
									  bool inverse) const {
			for(const auto &obj : objects_data) {
				for(const auto &field : obj.second.fields_data) {
					if (uri != field.second.uri)
						continue;
					if ((inverse and type != obj.first) or
                        (not inverse and type != field.second.type_name))
						return true;
				}
			}
			return false;
		}

    };

}

#endif//TENTRIS_GRAPHQLSCHEMA_HPP
