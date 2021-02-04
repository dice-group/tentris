#ifndef TENTRIS_GRAPHQLROOTSELECTIONVISITOR_HPP
#define TENTRIS_GRAPHQLROOTSELECTIONVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

#include "tentris/store/graphql/internal/GraphqlValueVisitor.hpp"

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;

	class GraphqlRootSelectionVisitor : public AstVisitor {

    private:

        GraphqlValueVisitor value_visitor{};
        uint8_t bracket_counter = 0;

    public:

        std::vector<std::variant<char,std::vector<std::string>>> fields{};

        bool visitSelectionSet(const facebook::graphql::ast::SelectionSet &selectionSet) override {
            return true;
        }

        bool visitField(const facebook::graphql::ast::Field &field) override {
            // inner field
            if(not fields.empty()) {
                fields.emplace_back('[');
                bracket_counter++;
            }
            return true;
        }

        void endVisitField(const facebook::graphql::ast::Field &field) override {
            if(bracket_counter > 0) {
                fields.emplace_back(']');
                bracket_counter--;
            }
        }

        bool visitName(const facebook::graphql::ast::Name &name) override {
			std::vector<std::string> field_name{name.getValue()};
            fields.emplace_back(std::move(field_name));
            return false;
        }

        bool visitArgument(const facebook::graphql::ast::Argument &argument) override {
            return false;
        }

		// TODO: support @include and @skip
        bool visitDirective(const facebook::graphql::ast::Directive &directive) override {
            return false;
        }

        // TODO: support fragment spreads
        bool visitFragmentSpread(const facebook::graphql::ast::FragmentSpread &fragmentSpread) override {
            return false;
        }

        // TODO: support inline fragments
        bool visitInlineFragment(const facebook::graphql::ast::InlineFragment &inlineFragment) override {
            return false;
        }

    };

}

#endif//TENTRIS_GRAPHQLROOTSELECTIONVISITOR_HPP
