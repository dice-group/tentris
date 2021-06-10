#ifndef TENTRIS_GRAPHQLSCHEMA_HPP
#define TENTRIS_GRAPHQLSCHEMA_HPP

#include "tentris/store/graphql/exception/SchemaException.hpp"
#include "tentris/store/graphql/internal/GraphQLDataStructures.hpp"

namespace tentris::store::graphql {

	using namespace tentris::store::graphql::exception;
	using namespace tentris::store::graphql::internal;

	class GraphQLSchema {

	private:
		using InterfaceTypeDefinitions = std::map<std::string, InterfaceData>;
		using ObjectTypeDefinitions = std::map<std::string, ObjectData>;

	private:
		std::string query_type_name = "Query";
		InterfaceTypeDefinitions interface_type_defs{};
		ObjectTypeDefinitions object_type_defs{};

	public:
		GraphQLSchema() = default;

		// setters

		void addObjectType(const std::string &name, ObjectData object_data) {
			object_type_defs[name] = std::move(object_data);
		}

		void addInterfaceType(const std::string &name, InterfaceData interface_data) {
			interface_type_defs[name] = std::move(interface_data);
		}

		void setQueryRoot(const std::string &query_name) {
			query_type_name = query_name;
		}

		// getters

		[[nodiscard]] const ObjectTypeDefinitions &getObjectTypeDefinitions() {
			return object_type_defs;
		}

		[[nodiscard]] const InterfaceTypeDefinitions &getInterfaceTypeDefinitions() {
			return interface_type_defs;
		}

		[[nodiscard]] const std::string &getFieldUri(const std::string &field_name,
													 const std::string &parent_type) const {
			try {
				return object_type_defs.at(parent_type).fields_data.at(field_name).uri;
			} catch (std::out_of_range &e) {
				try {
					return interface_type_defs.at(parent_type).fields_data.at(field_name).uri;
				} catch (std::out_of_range &e) {
					throw FieldNotFoundException(field_name, parent_type);
				}
			}
		}

		[[nodiscard]] const std::string &getFieldType(const std::string &field_name,
													  const std::string &parent_type = "") const {
			if (parent_type.empty()) {
				try {
					return object_type_defs.at(query_type_name).fields_data.at(field_name).type_name;
				} catch (std::out_of_range &e) {
					throw FieldNotFoundException(field_name, query_type_name);
				}
			} else {
				try {
					return object_type_defs.at(parent_type).fields_data.at(field_name).type_name;
				} catch (std::out_of_range &e) {
					try {
						return interface_type_defs.at(parent_type).fields_data.at(field_name).type_name;
					} catch (std::out_of_range &e) {
						throw FieldNotFoundException(field_name, parent_type);
					}
				}
			}
		}

		[[nodiscard]] const std::string &getObjectUri(const std::string &type_name) {
			try {
				return object_type_defs.at(type_name).uri;
			} catch (std::out_of_range &e) {
				try {
					return interface_type_defs.at(type_name).uri;
				} catch (std::out_of_range &e) {
					throw TypeNotFoundException(type_name);
				}
			}
		}

		[[nodiscard]] bool fieldIsList(const std::string &field_name,
									   const std::string &parent_type = "") const {
			if (parent_type.empty())
				return object_type_defs.at(query_type_name).fields_data.at(field_name).is_list;
			else {
				try {
					return object_type_defs.at(parent_type).fields_data.at(field_name).is_list;
				} catch (std::out_of_range &e) {
					try {
						return interface_type_defs.at(parent_type).fields_data.at(field_name).is_list;
					} catch (std::out_of_range &e) {
						throw FieldNotFoundException(field_name, parent_type);
					}
				}
			}
		}

		[[nodiscard]] bool fieldIsScalar(const std::string &field_name,
										 const std::string &parent_type) const {
			if (parent_type.empty())
				return false;
			std::string type_name{};
			try {
				type_name = object_type_defs.at(parent_type).fields_data.at(field_name).type_name;
			} catch (std::out_of_range &e) {
				try {
					type_name = interface_type_defs.at(parent_type).fields_data.at(field_name).type_name;
				} catch (std::out_of_range &e) {
					throw FieldNotFoundException(field_name, parent_type);
				}
			}
			if (type_name == "ID" or type_name == "String" or type_name == "Int" or type_name == "Float" or type_name == "Boolean")
				return true;
			return false;
		}

		[[nodiscard]] bool fieldIsNonNull(const std::string &field_name,
										  const std::string &parent_type = "") const {
			if (parent_type.empty())
				return object_type_defs.at(query_type_name).fields_data.at(field_name).non_null;
			else {
				try {
					return object_type_defs.at(parent_type).fields_data.at(field_name).non_null;
				} catch (std::out_of_range &e) {
					try {
						return interface_type_defs.at(parent_type).fields_data.at(field_name).non_null;
					} catch (std::out_of_range &e) {
						throw FieldNotFoundException(field_name, parent_type);
					}
				}
			}
		}

		[[nodiscard]] bool fieldIsInverse(const std::string &field_name,
										  const std::string &parent_type) const {
			try {
				return object_type_defs.at(parent_type).fields_data.at(field_name).is_inverse;
			} catch (std::out_of_range &e) {
				try {
					return interface_type_defs.at(parent_type).fields_data.at(field_name).is_inverse;
				} catch (std::out_of_range &e) {
					throw FieldNotFoundException(field_name, parent_type);
				}
			}
		}

		[[nodiscard]] bool typeFilter(const std::string &uri,
									  const std::string &type,
									  bool inverse) const {
			if (interface_type_defs.contains(type))
				return false;
			for (const auto &obj : object_type_defs) {
				for (const auto &field : obj.second.fields_data) {
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

}// namespace tentris::store::graphql

#endif//TENTRIS_GRAPHQLSCHEMA_HPP
