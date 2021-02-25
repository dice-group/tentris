#ifndef TENTRIS_GRAPHQLVALUEVISITOR_HPP
#define TENTRIS_GRAPHQLVALUEVISITOR_HPP

#include <AstVisitor.h>

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;

	class GraphqlValueVisitor : public AstVisitor {

	private:

		std::string value;

	public:

		[[nodiscard]] const std::string &getValue() const {
			return value;
		}

		void endVisitIntValue(const facebook::graphql::ast::IntValue &intValue) override {
			value = intValue.getValue();
		}

		void endVisitFloatValue(const facebook::graphql::ast::FloatValue &floatValue) override {
			value = floatValue.getValue();
		}

		void endVisitStringValue(const facebook::graphql::ast::StringValue &stringValue) override {
			value = stringValue.getValue();
		}

		void endVisitBooleanValue(const facebook::graphql::ast::BooleanValue &booleanValue) override {
			value = booleanValue.getValue() ? "true" : "false";
		}
	};

}

#endif//TENTRIS_GRAPHQLVALUEVISITOR_HPP
