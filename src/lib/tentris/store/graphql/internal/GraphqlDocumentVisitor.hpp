#ifndef TENTRIS_GRAPHQLDOCUMENTVISITOR_HPP
#define TENTRIS_GRAPHQLDOCUMENTVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;

	class GraphqlDocumentVisitor : public AstVisitor {

	private:

		// the queries of the provided document
        std::map<std::string, const facebook::graphql::ast::OperationDefinition*> queries{};
		// the object type definitions of the provided document
		std::vector<const facebook::graphql::ast::ObjectTypeDefinition*> object_defs{};
		// the interface type definitions of the provided document
        std::vector<const facebook::graphql::ast::InterfaceTypeDefinition*> interface_defs{};
		// the input types definitions of the provided document
        std::vector<const facebook::graphql::ast::InputObjectTypeDefinition*> input_defs{};

	public:

		// getters

		// returns the requested query operation
		[[nodiscard]] const facebook::graphql::ast::OperationDefinition* getQuery(const std::string& q_name) {
			if(not queries.contains(q_name))
                throw std::runtime_error("Requested query does not exist");
			return queries[q_name];
		}

		// returns the only parsed query operation
        [[nodiscard]] const facebook::graphql::ast::OperationDefinition* getQuery() {
            if(queries.empty())
				throw std::runtime_error("No query operations were provided");
			else if(queries.size() > 1)
				throw std::runtime_error("More than one query operations were provided");
			else
				return queries.begin()->second;
        }

        [[nodiscard]] const std::vector<const facebook::graphql::ast::ObjectTypeDefinition *> &getObjectDefs() const {
			return object_defs;
		}

        [[nodiscard]] const std::vector<const facebook::graphql::ast::InterfaceTypeDefinition *> &getInterfaceDefs() const {
			return interface_defs;
		}

        [[nodiscard]] const std::vector<const facebook::graphql::ast::InputObjectTypeDefinition *> &getInputDefs() const {
			return input_defs;
		}

		// visit functions

		bool visitOperationDefinition(const facebook::graphql::ast::OperationDefinition &operationDefinition) override {
			// we are only interested in query operations
            if(strcmp(operationDefinition.getOperation(), "query") == 0) {
                auto op_name = operationDefinition.getName()->getValue();
                queries[op_name] = &operationDefinition;
            }
			return false;
		}

		bool visitObjectTypeDefinition(const facebook::graphql::ast::ObjectTypeDefinition &objectTypeDefinition) override {
			object_defs.push_back(&objectTypeDefinition);
			return false;
		}

		bool visitInterfaceTypeDefinition(const facebook::graphql::ast::InterfaceTypeDefinition &interfaceTypeDefinition) override {
			interface_defs.push_back(&interfaceTypeDefinition);
			return false;
		}

		bool visitInputObjectTypeDefinition(const facebook::graphql::ast::InputObjectTypeDefinition &inputObjectTypeDefinition) override {
			input_defs.push_back(&inputObjectTypeDefinition);
			return false;
		}

		// union types are currently not supported
		bool visitUnionTypeDefinition(const facebook::graphql::ast::UnionTypeDefinition &unionTypeDefinition) override {
			return false;
		}

		// enum types are currently not supported
		bool visitEnumTypeDefinition(const facebook::graphql::ast::EnumTypeDefinition &enumTypeDefinition) override {
			return false;
		}

		// new scalar type definitions are currently not supported
		bool visitScalarTypeDefinition(const facebook::graphql::ast::ScalarTypeDefinition &scalarTypeDefinition) override {
			return false;
		}

	};
}
#endif//TENTRIS_GRAPHQLDOCUMENTVISITOR_HPP
