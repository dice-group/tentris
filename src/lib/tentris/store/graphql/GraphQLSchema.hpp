#ifndef TENTRIS_GRAPHQLSCHEMA_HPP
#define TENTRIS_GRAPHQLSCHEMA_HPP

#include <map>

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

		inline void addObjectType(const std::string &name, ObjectData object_data) {
			object_type_defs[name] = std::move(object_data);
		}

		inline void addInterfaceType(const std::string &name, InterfaceData interface_data) {
			interface_type_defs[name] = std::move(interface_data);
		}

		inline void setQueryRoot(const std::string &query_name) {
			query_type_name = query_name;
		}

		// getters

		[[nodiscard]] inline const ObjectTypeDefinitions &getObjectTypeDefinitions() {
			return object_type_defs;
		}

		[[nodiscard]] inline const InterfaceTypeDefinitions &getInterfaceTypeDefinitions() {
			return interface_type_defs;
		}

		[[nodiscard]] const std::string &getFieldUri(const std::string &field_name,
													 const std::string &parent_type = "") const;

		[[nodiscard]] const std::string &getFieldType(const std::string &field_name,
													  const std::string &parent_type = "") const;

		[[nodiscard]] const std::string &getObjectUri(const std::string &type_name) const;

		[[nodiscard]] bool isRootField(const std::string &field_name) const;

		[[nodiscard]] bool fieldIsList(const std::string &field_name,
									   const std::string &parent_type = "") const;

		[[nodiscard]] bool fieldIsScalar(const std::string &field_name,
										 const std::string &parent_type = "") const;

		[[nodiscard]] bool fieldIsNonNull(const std::string &field_name,
										  const std::string &parent_type = "") const;

		[[nodiscard]] bool fieldIsInverse(const std::string &field_name,
										  const std::string &parent_type = "") const;

		[[nodiscard]] bool typeFilter(const std::string &uri,
									  const std::string &type,
									  bool inverse) const;
	};

}// namespace tentris::store::graphql

#endif//TENTRIS_GRAPHQLSCHEMA_HPP
