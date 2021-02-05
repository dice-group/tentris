#ifndef TENTRIS_GRAPHQLVALUEVISITOR_HPP
#define TENTRIS_GRAPHQLVALUEVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;

	class GraphqlValueVisitor : public AstVisitor {

	private:

		std::string value;

	public:

		const std::string &getValue() const {
			return value;
		}

		bool visitIntValue(const facebook::graphql::ast::IntValue &intValue) override {
			return true;
		}

		void endVisitIntValue(const facebook::graphql::ast::IntValue &intValue) override {
			value = intValue.getValue();
		}
		bool visitFloatValue(const facebook::graphql::ast::FloatValue &floatValue) override {
			return true;
		}
		void endVisitFloatValue(const facebook::graphql::ast::FloatValue &floatValue) override {
			value = floatValue.getValue();
		}
		bool visitStringValue(const facebook::graphql::ast::StringValue &stringValue) override {
			return true;
		}
		void endVisitStringValue(const facebook::graphql::ast::StringValue &stringValue) override {
			value = stringValue.getValue();
		}
		bool visitBooleanValue(const facebook::graphql::ast::BooleanValue &booleanValue) override {
			return false;
		}
		void endVisitBooleanValue(const facebook::graphql::ast::BooleanValue &booleanValue) override {

		}
	};

}

#endif//TENTRIS_GRAPHQLVALUEVISITOR_HPP
