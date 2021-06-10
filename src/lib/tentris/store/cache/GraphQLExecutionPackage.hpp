#ifndef TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
#define TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP

#include <algorithm>
#include <any>
#include <exception>
#include <ostream>

#include <Dice/graphql-parser/GraphQLParser.hpp>

#include "tentris/store/AtomicGraphqlSchema.hpp"
#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store {
	class TripleStore;
};

namespace tentris::store::cache {

	/**
     * A QueryExecutionPackage contains everything that is necessary to execute a given graphql query for a state of the
     * RDF graph.
     */
	struct GraphQLExecutionPackage {

	private:
		using const_BoolHypertrie = ::tentris::tensor::const_BoolHypertrie;
		using time_point_t = logging::time_point_t;
		using Subscript = ::tentris::tensor::Subscript;
		using Einsum = ::tentris::tensor::Einsum<::tentris::tensor::COUNTED_t>;

	private:
		std::string query_name;
		// a graphql query can generate multiple subscripts (multiple root fields)
		std::vector<std::shared_ptr<Subscript>> subscripts;
		// a vector containing the operands of each subscript
		std::vector<std::vector<const_BoolHypertrie>> all_operands{};
		// a vector containing the paths of the query (one list per root field)
		std::vector<std::vector<std::vector<std::pair<Subscript::Label, std::string>>>> all_paths{};
		// a vector containing the fragment labels (one set per root field)
		std::vector<std::set<Subscript::Label>> all_fragment_labels{};
		std::vector<Subscript::Label> opt_begin{'['};
		std::vector<Subscript::Label> opt_end{']'};

	public:
		GraphQLExecutionPackage() = delete;

		/**
         * @param graphql_string graphql query to be parsed
         */
		explicit GraphQLExecutionPackage(const std::string &document,
										 const std::string &query_name = "") : query_name(query_name) {
			using namespace logging;
			logDebug(fmt::format("Parsing document {}", document));
			auto parsed_graphql = Dice::graphql_parser::GraphQLParser::parseQuery(document, query_name);
			all_paths = std::move(parsed_graphql->all_paths);
			all_fragment_labels = std::move(parsed_graphql->all_fragment_labels);
			auto &triple_store = AtomicTripleStore::getInstance();
			auto &schema = AtomicGraphqlSchema::getInstance();
			logDebug(fmt::format("Preparing Subscripts"));
			// one subscript for each root field of the query
			for (auto i : iter::range(parsed_graphql->all_operands_labels.size())) {
				std::vector<std::vector<Subscript::Label>> operands_labels{};
				std::vector<Subscript::Label> result_labels = parsed_graphql->all_result_labels[i];
				std::vector<const_BoolHypertrie> operands{};
				uint32_t field_arg_pos = 0;
				uint32_t field_pos = 0;
				// stack that keeps track of the parent types
				std::deque<std::string> parent_types{};
				// resolve the fields -- make use of the operands labels to find the fields in the schema
				// makes use of the fact that graphql queries form a tree
				// the first label of an operand label refers to the parent of the current field
				for (auto &operand_labels : parsed_graphql->all_operands_labels[i]) {
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
						const auto &field_name = std::get<0>(parsed_graphql->all_fields_name_arguments[i][field_arg_pos]);
						const auto &field_type = schema.getFieldType(field_name);
						operands_labels.push_back(operand_labels);
						parent_types.push_back(field_type);
						operands.emplace_back(triple_store.resolveGQLObjectType(schema.getObjectUri(field_type)));
					} else {
						// field
						if (operand_labels.size() == 2) {
							field_pos = field_arg_pos;
							const auto &field_name = std::get<0>(parsed_graphql->all_fields_name_arguments[i][field_arg_pos]);
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
								parent_types.push_back(std::get<0>(parsed_graphql->all_fields_name_arguments[i][field_arg_pos]));
								operands.emplace_back(triple_store.resolveGQLObjectType(schema.getObjectUri(parent_types.back())));
								operands_labels.push_back(operand_labels);
							}
							// argument
							else {
								const auto &arg_name = std::get<1>(parsed_graphql->all_fields_name_arguments[i][field_arg_pos]).first;
								const auto &value = std::get<1>(parsed_graphql->all_fields_name_arguments[i][field_arg_pos]).second;
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

	public:
		[[nodiscard]] std::vector<std::shared_ptr<Einsum>> generateEinsums(const time_point_t &timeout) {
			std::vector<std::shared_ptr<Einsum>> einsums{};
			for (auto i : iter::range(subscripts.size())) {
				auto einsum = std::make_shared<Einsum>(subscripts[i], all_operands[i], timeout);
				einsums.push_back(std::move(einsum));
			}
			return einsums;
		}

		[[nodiscard]] const std::vector<std::vector<std::pair<Subscript::Label, std::string>>> &getPath(std::size_t pos) {
			return all_paths[pos];
		}

		[[nodiscard]] const std::set<Subscript::Label> &getFragmentLabels(std::size_t pos) {
			return all_fragment_labels[pos];
		}

		[[nodiscard]] const std::vector<std::vector<const_BoolHypertrie>> &getOperands() const {
			return all_operands;
		}

		friend struct ::fmt::formatter<GraphQLExecutionPackage>;
	};

}// namespace tentris::store::cache

template<>
struct fmt::formatter<tentris::store::cache::GraphQLExecutionPackage> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::cache::GraphQLExecutionPackage &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
						 " GraphQL Query Name:     {}\n"
						 " subscripts:  {}\n",
						 p.query_name, p.subscripts);
	}
};
#endif//TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP