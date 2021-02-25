#ifndef TENTRIS_GRAPHQLDIRECTIVEVISITOR_HPP
#define TENTRIS_GRAPHQLDIRECTIVEVISITOR_HPP

#include "tentris/store/graphql/internal/GraphqlValueVisitor.hpp"

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;


	class GraphqlDirectiveVisitor : public AstVisitor {

	private:

        GraphqlValueVisitor value_visitor{};

	public:

		std::string directive_name{};
		std::map<std::string, std::string> args_vals{};

		bool visitDirective([[maybe_unused]] const Directive &directive) override {
			return true;
		}

		bool visitName(const Name &name) override {
			directive_name = name.getValue();
			return false;
		}

		bool visitArgument(const Argument &argument) override {
			auto &arg_value = args_vals[argument.getName().getValue()];
			argument.getValue().accept(&value_visitor);
			arg_value = value_visitor.getValue();
			return false;
		}

	};

}

#endif//TENTRIS_GRAPHQLDIRECTIVEVISITOR_HPP
