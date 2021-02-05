#ifndef TENTRIS_GRAPHQLROOTSELECTIONVISITOR_HPP
#define TENTRIS_GRAPHQLROOTSELECTIONVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

#include <Dice/einsum/internal/Subscript.hpp>

#include "tentris/store/graphql/internal/GraphqlValueVisitor.hpp"

namespace tentris::store::graphql::internal {

	using namespace facebook::graphql::ast::visitor;
	using Subscript = einsum::internal::Subscript;

	class GraphqlRootSelectionVisitor : public AstVisitor {

    private:

        uint8_t bracket_counter = 0;
        Subscript::Label next_label = 'a';
        std::vector<Subscript::Label> selection_set_label{}; // stack
        std::vector<std::string> selection_set_root{};
		std::string field_name{};
        Subscript::Label field_label{};
        std::vector<Subscript::Label> result_labels{};
        std::vector<std::vector<Subscript::Label>> operands_labels{};
        std::vector<std::vector<std::pair<std::string, std::string>>> fields{};

    public:

        bool visitSelectionSet(const facebook::graphql::ast::SelectionSet &selectionSet) override {
            selection_set_label.push_back(field_label);
            selection_set_root.push_back(field_name);
            return true;
        }

		void endVisitSelectionSet(const facebook::graphql::ast::SelectionSet &selectionSet) override {
			selection_set_root.pop_back();
			selection_set_label.pop_back();
		}

		bool visitField(const facebook::graphql::ast::Field &field) override {
			field_name = field.getName().getValue();
            std::vector<std::pair<std::string, std::string>> new_field{};
			if(fields.empty())// root field
                new_field.emplace_back(std::string(),field_name);
			else
                new_field.emplace_back(selection_set_root.back(),field_name);
            fields.emplace_back(std::move(new_field));
            field_label = next_label++;
            std::vector<Subscript::Label> operand_labels{};
            // inner field
            if(not operands_labels.empty()) {
                operands_labels.push_back({'['});
                bracket_counter++;
                operand_labels.emplace_back(selection_set_label.back());
                operand_labels.emplace_back(field_label);
                // leaf field
                if (not field.getSelectionSet())
                    result_labels.emplace_back(field_label);
            }
            // root field
            else {
                operand_labels.emplace_back(field_label);
                // root & leaf field
                if (not field.getSelectionSet()) {
                    operand_labels.emplace_back(++next_label);
                    result_labels.emplace_back(next_label);
                }
            }
            operands_labels.emplace_back(std::move(operand_labels));
            return true;
        }

        void endVisitField(const facebook::graphql::ast::Field &field) override {
            if(bracket_counter > 0) {
                operands_labels.push_back({']'});
                bracket_counter--;
            }
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

		[[nodiscard]] const std::vector<std::vector<std::pair<std::string, std::string>>> &getFields() const {
			return fields;
		}

		std::shared_ptr<Subscript> getSubscript() {
			return std::make_shared<Subscript>(operands_labels, result_labels);
		}

	};

}

#endif//TENTRIS_GRAPHQLROOTSELECTIONVISITOR_HPP
