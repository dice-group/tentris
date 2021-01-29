#ifndef TENTRIS_GRAPHQLQUERYVISITOR_HPP
#define TENTRIS_GRAPHQLQUERYVISITOR_HPP

#include <libgraphqlparser/AstVisitor.h>

#include "Dice/einsum/internal/Subscript.hpp"

#include "GraphqlValueVisitor.hpp"

namespace tentris::graphql::internal {

	using namespace facebook::graphql::ast::visitor;
	using namespace einsum::internal;

	class GraphqlQueryVisitor : public AstVisitor {

	private:

		std::vector<std::shared_ptr<Subscript>> subscripts{};
		std::vector<std::vector<std::vector<std::string>>> slicing_keys{};

		// this visitor will generate one subscript for each root field
		class RootSelectionVisitor : public AstVisitor {

		private:

            struct FieldData {
				std::string name;
				std::map<std::string, std::string> arguments{};
				std::map<std::string, std::map<std::string, std::string>> directives{};
            };

            GraphqlValueVisitor value_visitor{};
            std::vector<FieldData> fields_data{};
            Subscript::Label next_label = 'a';
            Subscript::Label selection_set_label{};
            Subscript::Label field_label{};
            uint8_t bracket_counter = 0;

		public:

            std::vector<Subscript::Label> result{};
            std::vector<std::vector<Subscript::Label>> operands_labels{};
            std::vector<std::vector<std::string>> fields{}; // will be used for slicing

			bool visitSelectionSet(const facebook::graphql::ast::SelectionSet &selectionSet) override {
                selection_set_label = field_label;
				return true;
			}

			bool visitField(const facebook::graphql::ast::Field &field) override {
				fields_data.push_back(FieldData{});
                field_label = next_label++;
                std::vector<Subscript::Label> operand_labels{};
                // inner field
                if(not operands_labels.empty()) {
                    operands_labels.push_back({'['});
                    bracket_counter++;
                    operand_labels.emplace_back(selection_set_label);
                    operand_labels.emplace_back(field_label);
                    // leaf field
                    if (not field.getSelectionSet())
                        result.emplace_back(field_label);
                }
                    // root field
                else {
                    operand_labels.emplace_back(field_label);
                    // root & leaf field
                    if (not field.getSelectionSet()) {
                        operand_labels.emplace_back(++next_label);
                        result.emplace_back(next_label);
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

            bool visitName(const facebook::graphql::ast::Name &name) override {
				fields.push_back({name.getValue()});
                return false;
            }

			bool visitArgument(const facebook::graphql::ast::Argument &argument) override {
                operands_labels.push_back({field_label});
				std::vector<std::string> arg_slice_key{};
                arg_slice_key.push_back(argument.getName().getValue());
				argument.getValue().accept(&value_visitor);
                arg_slice_key.push_back(value_visitor.getValue());
				fields.emplace_back(std::move(arg_slice_key));
				return false;
			}

			bool visitDirective(const facebook::graphql::ast::Directive &directive) override {
				// currently supports only the @rdf directive
                if(strcmp(directive.getName().getValue(), "rdf") != 0)
					return false;
				for(auto& argument : *directive.getArguments()) {
					// get the namespace of the field
					if(strcmp(argument->getName().getValue(), "base") == 0) {
						argument->getValue().accept(&value_visitor);
                        (*fields.rbegin())[0].insert(0, value_visitor.getValue());
					}
				}
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

	public:

		bool visitOperationDefinition(const facebook::graphql::ast::OperationDefinition &operationDefinition) override {
			return true;
		}

		void endVisitOperationDefinition(const facebook::graphql::ast::OperationDefinition &operationDefinition) override {
			auto& root_selection_set = operationDefinition.getSelectionSet();
			for(auto& selection : root_selection_set.getSelections()) {
                RootSelectionVisitor root_selection_visitor{};
                selection->accept(&root_selection_visitor);
                subscripts.emplace_back(std::move(std::make_shared<Subscript>(root_selection_visitor.operands_labels,
                                                                              root_selection_visitor.result)));
                slicing_keys.emplace_back(std::move(root_selection_visitor.fields));
			}
		}

		// do not visit selection sets with this visitor
		bool visitSelectionSet(const facebook::graphql::ast::SelectionSet &selectionSet) override {
			return false;
		}

		[[nodiscard]] std::vector<std::shared_ptr<Subscript>> &getSubscripts() {
			return subscripts;
		}

        [[nodiscard]] std::vector<std::vector<std::vector<std::string>>> &getSlicingKeys() {
			return slicing_keys;
		}
	};

}

#endif//TENTRIS_GRAPHQLQUERYVISITOR_HPP
