#ifndef TENTRIS_PARSEDGRAPHQL_HPP
#define TENTRIS_PARSEDGRAPHQL_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <optional>
#include <exception>
#include <memory>
#include <tuple>
#include <regex>
#include <utility>

#include <Dice/einsum/internal/Subscript.hpp>
#include <Ast.h>

#include "tentris/store/graphql/GraphqlSchema.hpp"
#include "tentris/store/graphql/internal/GraphqlValueVisitor.hpp"

namespace tentris::store::graphql {

    using Subscript = einsum::internal::Subscript;
    using namespace fmt::literals;
    using namespace facebook::graphql::ast::visitor;

    class ParsedGraphql {

	private:

		class RootSelectionVisitor;

		std::vector<std::shared_ptr<Subscript>> subscripts{};
		std::vector<std::vector<std::vector<std::string>>> slice_keys{};

	public:

        ParsedGraphql() = delete;

        explicit ParsedGraphql(const OperationDefinition* query, const GraphqlSchema& schema) {
            const auto& root_selection_set = query->getSelectionSet();
            for(const auto& root_selection : root_selection_set.getSelections()) {
                RootSelectionVisitor rsv{&schema};
                std::vector<std::vector<std::string>> root_selection_slice_keys{};
                std::map<std::string, std::string> field_type_map{};
				try {
					root_selection->accept(&rsv);
				} catch(std::exception &e) {
                    throw std::runtime_error(e.what());
				}
                subscripts.emplace_back(std::make_shared<Subscript>(rsv.operands_labels, rsv.result_labels));
                slice_keys.emplace_back(std::move(rsv.fields));
            }
		}

		[[nodiscard]] const std::vector<std::shared_ptr<Subscript>> &getSubscripts() const {
            return subscripts;
        }

        [[nodiscard]] const std::vector<std::vector<std::vector<std::string>>> &getSliceKeys() const {
			return slice_keys;
		}

	private:

		class RootSelectionVisitor : public AstVisitor {

		private:

			const GraphqlSchema *schema;

		public:

			// the result labels for the subscript
            std::vector<Subscript::Label> result_labels{};
			// the operands labels for the subscript
            std::vector<std::vector<Subscript::Label>> operands_labels{};
            // the uris of fields and scalar values arguments
            std::vector<std::vector<std::string>> fields{};

		public:

			RootSelectionVisitor() = delete;

			explicit RootSelectionVisitor(const GraphqlSchema *input_schema) : schema(input_schema) {}

		private:

			// helper fields for the visit functions
            uint8_t bracket_counter = 0;
            Subscript::Label next_label = 'a';
            Subscript::Label field_label{};
            std::vector<Subscript::Label> selection_set_label{}; // stack
            std::vector<std::string> selection_set_root_type{}; // stack
            std::string field_name{};
			std::string field_type{};
			internal::GraphqlValueVisitor value_visitor{};

		public:

			// visit functions

			bool visitSelectionSet([[maybe_unused]] const SelectionSet &selectionSet) override {
				// inner field
				if(not selection_set_root_type.empty())
					selection_set_root_type.push_back(schema->getFieldType(field_name, selection_set_root_type.back()));
				//root field
				else
					selection_set_root_type.push_back(schema->getFieldType(field_name));
                selection_set_label.push_back(field_label);
				return true;
			}

			void endVisitSelectionSet([[maybe_unused]] const SelectionSet &selectionSet) override {
                selection_set_root_type.pop_back();
                selection_set_label.pop_back();
			}

			bool visitField(const Field &field) override {
                field_name = field.getName().getValue();
                std::vector<std::string> new_field{};
                field_label = next_label++;
                std::vector<Subscript::Label> operand_labels{};
                // inner - leaf field
                if(not fields.empty()) {
					field_type = schema->getFieldType(field_name, selection_set_root_type.back());
                    new_field.push_back(schema->getFieldUri(field_name, selection_set_root_type.back()));
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
                    field_type = schema->getFieldType(field_name);
					new_field.push_back(schema->getFieldUri(field_name));
					operand_labels.emplace_back(field_label);
					// root & leaf field
					if (not field.getSelectionSet()) {
						operand_labels.emplace_back(++next_label);
						result_labels.emplace_back(next_label);
					}
				}
                operands_labels.emplace_back(std::move(operand_labels));
                fields.emplace_back(std::move(new_field));
				return true;
			}

			void endVisitField([[maybe_unused]] const Field &field) override {
                if(bracket_counter > 0) {
                    operands_labels.push_back({']'});
                    bracket_counter--;
                }
			}

			bool visitArgument(const Argument &argument) override {
				const auto &arg_name = argument.getName().getValue();
				std::vector<std::string> new_field{};
				operands_labels.emplace_back(std::vector<Subscript::Label>{field_label});
				new_field.push_back(schema->getFieldUri(arg_name, field_type));
				argument.getValue().accept(&value_visitor);
				new_field.push_back(value_visitor.getValue());
				fields.emplace_back(std::move(new_field));
				return false;
			}

			// todo directives, fragments

		};

	};

}

#endif//TENTRIS_PARSEDGRAPHQL_HPP
