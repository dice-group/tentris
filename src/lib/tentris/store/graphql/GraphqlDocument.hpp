#ifndef TENTRIS_GRAPHQLDOCUMENT_HPP
#define TENTRIS_GRAPHQLDOCUMENT_HPP

#include <AstVisitor.h>

#include "tentris/store/graphql/internal/GraphqlParser.hpp"

namespace tentris::store::graphql {

	using namespace facebook::graphql::ast;
	
	class GraphqlDocument : public facebook::graphql::ast::visitor::AstVisitor {

	private:

		using GraphqlParser = ::tentris::store::graphql::internal::GraphqlParser;

		// the root node of the document
		std::unique_ptr<internal::GQLAstNode> root_node;
        // the queries of the provided document
        std::map<std::string, const OperationDefinition*> queries{};
        // the object type definitions of the provided document
        std::vector<const ObjectTypeDefinition*> object_defs{};
        // the interface type definitions of the provided document
        std::vector<const InterfaceTypeDefinition*> interface_defs{};
        // the input types definitions of the provided document
        std::vector<const InputObjectTypeDefinition*> input_defs{};

	public:

		// constructors

		GraphqlDocument() = delete;

		explicit GraphqlDocument(const std::string& request, bool schema_support = false) {
			if(not schema_support)
			    root_node = GraphqlParser::parseRequestDocument(request);
			else
                root_node = GraphqlParser::parseSchema(request);
			root_node->accept(this);
		}

		//getters
		
        // returns the requested query operation
        [[nodiscard]] const OperationDefinition* getQuery(const std::string& q_name) const {
            if(queries.empty())
                throw std::runtime_error("No query operations were provided");
            if(not queries.contains(q_name))
                throw std::runtime_error("Requested query does not exist");
            return queries.at(q_name);
        }

        [[nodiscard]] const std::vector<const ObjectTypeDefinition *> &getObjectDefs() const {
            return object_defs;
        }

        [[nodiscard]] const std::vector<const InterfaceTypeDefinition *> &getInterfaceDefs() const {
            return interface_defs;
        }

        [[nodiscard]] const std::vector<const InputObjectTypeDefinition *> &getInputDefs() const {
            return input_defs;
        }

	private:
        // visit functions

        bool visitOperationDefinition(const OperationDefinition &operationDefinition) override {
            // we are only interested in query operations
            if(strcmp(operationDefinition.getOperation(), "query") == 0) {
                if(operationDefinition.getName())
                    queries[operationDefinition.getName()->getValue()] = &operationDefinition;
                else
                    queries[""] = &operationDefinition;
            }
            return false;
        }

        bool visitObjectTypeDefinition(const ObjectTypeDefinition &objectTypeDefinition) override {
            object_defs.push_back(&objectTypeDefinition);
            return false;
        }

        bool visitInterfaceTypeDefinition(const InterfaceTypeDefinition &interfaceTypeDefinition) override {
            interface_defs.push_back(&interfaceTypeDefinition);
            return false;
        }

        bool visitInputObjectTypeDefinition(const InputObjectTypeDefinition &inputObjectTypeDefinition) override {
            input_defs.push_back(&inputObjectTypeDefinition);
            return false;
        }

        // union types are currently not supported
        bool visitUnionTypeDefinition([[maybe_unused]] const UnionTypeDefinition &unionTypeDefinition) override {
            return false;
        }

        // enum types are currently not supported
        bool visitEnumTypeDefinition([[maybe_unused]] const EnumTypeDefinition &enumTypeDefinition) override {
            return false;
        }

        // new scalar type definitions are currently not supported
        bool visitScalarTypeDefinition([[maybe_unused]] const ScalarTypeDefinition &scalarTypeDefinition) override {
            return false;
        }
		
	};
	
}

#endif//TENTRIS_GRAPHQLDOCUMENT_HPP
