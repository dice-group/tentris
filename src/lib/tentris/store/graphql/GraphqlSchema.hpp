#ifndef TENTRIS_GRAPHQLSCHEMA_HPP
#define TENTRIS_GRAPHQLSCHEMA_HPP

#include <utility>

#include "tentris/store/graphql/internal/GraphqlDocumentVisitor.hpp"
#include "tentris/store/graphql/internal/GraphqlObjectTypeVisitor.hpp"
#include "tentris/store/graphql/internal/GraphqlParser.hpp"

namespace tentris::store::graphql {

    using namespace tentris::store::graphql::internal;
	using FieldData = GraphqlObjectTypeVisitor::FieldData;

    class GraphqlSchema {

	private:

		struct ObjectData {
			std::string uri;
			std::map<std::string, FieldData> fields_data{};
		};

        std::string query_type_name = "Query";
		std::map<std::string, ObjectData> objects_data{};

    public:

        explicit GraphqlSchema(const std::string& path_to_schema_file) {
            GraphqlDocumentVisitor doc_visitor{};
			auto schema_doc = GraphqlParser::parseSchema(path_to_schema_file);
			schema_doc->accept(&doc_visitor);
			// iterate through and parse the object definitions of the the provided schema
			for(const auto& obj_def : doc_visitor.getObjectDefs()) {
				GraphqlObjectTypeVisitor obj_visitor{};
				obj_def->accept(&obj_visitor);
                ObjectData cur_obj_data{};
                cur_obj_data.uri = obj_visitor.getUri();
                cur_obj_data.fields_data = obj_visitor.getFieldsData();
                objects_data[obj_visitor.getObjName()] = std::move(cur_obj_data);
			}
		}

		[[nodiscard]] const std::string& getEntryPointUri(const std::string& field_name) const {
			auto type_name = objects_data.at(query_type_name).fields_data.at(field_name).type;
			return objects_data.at(type_name).uri;
		}

        [[nodiscard]] const std::string& getEntryPointType(const std::string& field_name) const {
            return objects_data.at(query_type_name).fields_data.at(field_name).type;
        }

        [[nodiscard]] const std::string& getFieldUri(const std::string& parent_type, const std::string& field_name) const {
            return objects_data.at(parent_type).fields_data.at(field_name).uri;
        }

        [[nodiscard]] const std::string& getFieldType(const std::string& parent_type, const std::string& field_name) const {
            return objects_data.at(parent_type).fields_data.at(field_name).type;
        }

	};

}

#endif//TENTRIS_GRAPHQLSCHEMA_HPP
