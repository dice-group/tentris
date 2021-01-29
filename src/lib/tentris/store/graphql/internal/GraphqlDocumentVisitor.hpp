#ifndef TENTRIS_GRAPHQLDOCUMENTVISITOR_HPP
#define TENTRIS_GRAPHQLDOCUMENTVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

namespace tentris::graphql::internal {

	using namespace facebook::graphql::ast::visitor;

	class GraphqlDocumentVisitor : public AstVisitor {

	private:

        std::string op_type{};
		const facebook::graphql::ast::OperationDefinition* operation;
		std::map<std::string, std::string> vars_map{};
		std::string last_def_value;

	public:

		bool visitDocument(const facebook::graphql::ast::Document &document) override {
			return true;
		}

		void endVisitDocument(const facebook::graphql::ast::Document &document) override {

		}

		bool visitOperationDefinition(const facebook::graphql::ast::OperationDefinition &operationDefinition) override {
			return true;
		}

		void endVisitOperationDefinition(const facebook::graphql::ast::OperationDefinition &operationDefinition) override {
			op_type = operationDefinition.getOperation();
			operation = dynamic_cast<const facebook::graphql::ast::OperationDefinition*>(&operationDefinition);
		}

        // do not visit names
		bool visitName(const facebook::graphql::ast::Name &name) override {
			return false;
		}

		bool visitVariableDefinition(const facebook::graphql::ast::VariableDefinition &variableDefinition) override {
			return true;
		}

        void endVisitVariableDefinition(const facebook::graphql::ast::VariableDefinition &variableDefinition) override {
			std::string var_name = variableDefinition.getVariable().getName().getValue();
			if(not last_def_value.empty()) {
				vars_map[var_name] = last_def_value;
				last_def_value.clear();
			}
			else
				vars_map[variableDefinition.getVariable().getName().getValue()];
        }

		bool visitVariable(const facebook::graphql::ast::Variable &variable) override {
			return false;
		}

		bool visitIntValue(const facebook::graphql::ast::IntValue &intValue) override {
			last_def_value = intValue.getValue();
			return false;
		}

		bool visitStringValue(const facebook::graphql::ast::StringValue &stringValue) override {
            last_def_value = stringValue.getValue();
            return false;
		}

		// do not visit directives
		bool visitDirective(const facebook::graphql::ast::Directive &directive) override {
			return false;
		}

        // do not visit selection sets
		bool visitSelectionSet(const facebook::graphql::ast::SelectionSet &selectionSet) override {
			return false;
		}

		[[nodiscard]] const std::string &getOpType() const {
			return op_type;
		}

		[[nodiscard]] const facebook::graphql::ast::OperationDefinition *getOperation() const {
			return operation;
		}
	};
}
#endif//TENTRIS_GRAPHQLDOCUMENTVISITOR_HPP
