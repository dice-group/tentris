#include "GraphQLExecutionPackage.hpp"

namespace tentris::store::cache {
	GraphQLExecutionPackage::GraphQLExecutionPackage(const std::pair<std::string, std::string> &request) {
		using namespace logging;
		logDebug(fmt::format("Parsing document {}", std::get<0>(request)));
		auto parsed_graphql = graphql::GraphQLParser::parseQuery(std::get<0>(request), std::get<1>(request));
		auto &triple_store = AtomicTripleStore::getInstance();
		auto &schema = AtomicGraphqlSchema::getInstance();
		logDebug(fmt::format("Preparing Subscripts"));
		// one subscript for each root field of the query
		for (auto& sub_query : *parsed_graphql) {
			all_paths.push_back(std::move(sub_query.paths));
			all_fragment_labels.push_back(std::move(sub_query.fragment_labels));
			std::vector<std::vector<Subscript::Label>> operands_labels{};
			std::vector<Subscript::Label> result_labels = sub_query.result_labels;
			std::vector<const_BoolHypertrie> operands{};
			uint32_t field_arg_pos = 0;
			uint32_t field_pos = 0;
			// stack that keeps track of the parent types
			std::deque<std::string> parent_types{};
			// resolve the fields -- make use of the operands labels to find the fields in the schema
			// makes use of the fact that graphql queries form a tree
			// the first label of an operand label refers to the parent of the current field
			for (auto &operand_labels : sub_query.operands_labels) {
				if (operand_labels == opt_begin) {
					operands_labels.push_back(operand_labels);
					continue;
				}
				if (operand_labels == opt_end) {
					if (operands_labels.back() == opt_begin)// remove [ ] patterns created by ID fields
						operands_labels.pop_back();
					else
						operands_labels.push_back(operand_labels);
					parent_types.pop_back();
					continue;
				}
				// root field
				if (parent_types.empty()) {
					const auto &field_name = std::get<0>(sub_query.fields_name_arguments[field_arg_pos]);
					const auto &field_type = schema.getFieldType(field_name);
					operands_labels.push_back(operand_labels);
					parent_types.push_back(field_type);
					operands.emplace_back(triple_store.resolveGQLObjectType(schema.getObjectUri(field_type)));
				} else {
					// field
					if (operand_labels.size() == 2) {
						field_pos = field_arg_pos;
						const auto &field_name = std::get<0>(sub_query.fields_name_arguments[field_arg_pos]);
						const auto &parent_type = parent_types.back();
						const auto &field_type = schema.getFieldType(field_name, parent_type);
						const auto &field_uri = schema.getFieldUri(field_name, parent_type);
						auto is_inverse = schema.fieldIsInverse(field_name, parent_type);
						parent_types.push_back(field_type);
						// inner field
						if (not schema.fieldIsScalar(field_name, parent_type)) {
							operands.emplace_back(triple_store.resolveGQLField(field_uri));
							// check the direction of the edge -- @inverse directive
							if (is_inverse) {
								std::swap(operand_labels[0], operand_labels[1]);
								operands_labels.push_back(operand_labels);
							} else {
								operands_labels.push_back(operand_labels);
							}
							if (schema.fieldIsList(field_name, parent_type) and
								schema.typeFilter(field_uri, field_type, is_inverse)) {
								// operand to filter based on type
								operands.emplace_back(triple_store.resolveGQLObjectType(schema.getObjectUri(field_type)));
								// create operand label for the filter operand
								if (is_inverse)
									operands_labels.emplace_back(std::vector<Subscript::Label>({operand_labels[0]}));
								else
									operands_labels.emplace_back(std::vector<Subscript::Label>({operand_labels[1]}));
							}
						}
						// leaf field
						else {
							// field whose type is ID -- change result label and remove operand_labels
							// replace result label with the label of the parent -> entity URI
							if (field_type == "ID")
								result_labels.erase(
										std::remove(result_labels.begin(), result_labels.end(), operand_labels[1]), result_labels.end());
							else {
								operands.emplace_back(triple_store.resolveGQLField(field_uri));
								operands_labels.push_back(operand_labels);
							}
						}
					}
					// argument or inline fragment
					else {
						// inline fragment
						if (operands_labels.back() == opt_begin) {
							parent_types.push_back(std::get<0>(sub_query.fields_name_arguments[field_arg_pos]));
							operands.emplace_back(triple_store.resolveGQLObjectType(schema.getObjectUri(parent_types.back())));
							operands_labels.push_back(operand_labels);
						}
						// argument
						else {
							const auto &arg_name = std::get<1>(sub_query.fields_name_arguments[field_arg_pos]).first;
							const auto &value = std::get<1>(sub_query.fields_name_arguments[field_arg_pos]).second;
							const auto &parent_type = parent_types.back();
							// filter field operand
							if (schema.getFieldType(arg_name, parent_type) == "ID")
								operands[field_pos] = triple_store.resolveGQLArgumentID(std::any_cast<std::string>(value), operands[field_pos]);
							else {
								operands.emplace_back(triple_store.resolveGQLArgument(schema.getFieldUri(arg_name, parent_type),
																					  schema.getFieldType(arg_name, parent_type),
																					  value));
								operands_labels.push_back(operand_labels);
							}
						}
					}
				}
				field_arg_pos++;
			}
			subscripts.emplace_back(std::make_shared<Subscript>(operands_labels, result_labels));
			all_operands.emplace_back(std::move(operands));
		}
	}

}// namespace tentris::store::cache