#include "GraphQLSchema.hpp"

namespace tentris::store::graphql {

	[[nodiscard]] const std::string &GraphQLSchema::getFieldUri(const std::string &field_name,
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

	[[nodiscard]] const std::string &GraphQLSchema::getFieldType(const std::string &field_name,
																 const std::string &parent_type) const {
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

	[[nodiscard]] const std::string &GraphQLSchema::getObjectUri(const std::string &type_name) const {
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

	[[nodiscard]] bool GraphQLSchema::fieldIsList(const std::string &field_name,
												  const std::string &parent_type) const {
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

	[[nodiscard]] bool GraphQLSchema::fieldIsScalar(const std::string &field_name,
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

	[[nodiscard]] bool GraphQLSchema::fieldIsNonNull(const std::string &field_name,
													 const std::string &parent_type) const {
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

	[[nodiscard]] bool GraphQLSchema::fieldIsInverse(const std::string &field_name,
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

	[[nodiscard]] const std::string &GraphQLSchema::getArguemntType(const std::string &argument_name,
																	const std::string &field_name,
																	const std::string &parent_type) const {
        try {
            return object_type_defs.at(parent_type).fields_data.at(field_name).arguments.at(argument_name);
        } catch (std::out_of_range &e) {
            try {
                return interface_type_defs.at(parent_type).fields_data.at(field_name).arguments.at(argument_name);
            } catch (std::out_of_range &e) {
                throw ArgumentNotFoundException(argument_name, field_name, parent_type);
            }
        }
	}

	[[nodiscard]] bool GraphQLSchema::typeFilter(const std::string &uri,
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

	[[nodiscard]] bool GraphQLSchema::implementsInterface(const std::string &object_type,
														  const std::string &parent_type) const {
		if (object_type == parent_type)
			return true;
		return object_type_defs.at(object_type).implementations.contains(parent_type);
	}

}// namespace tentris::store::graphql