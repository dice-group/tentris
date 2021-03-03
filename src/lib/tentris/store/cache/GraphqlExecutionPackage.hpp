#ifndef TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
#define TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP

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
	struct GraphqlExecutionPackage {

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

	public:

		GraphqlExecutionPackage() = delete;

		/**
         * @param graphql_string graphql query to be parsed
         */
		explicit GraphqlExecutionPackage(const std::string &document,
										 const std::string &query_name = "") : query_name(query_name) {
            using namespace logging;
            logDebug(fmt::format("Parsing document {}", document));
            auto parsed_graphql = Dice::graphql_parser::GraphQLParser::parseQuery(document, query_name);
            auto &triple_store = AtomicTripleStore::getInstance();
			auto &schema = AtomicGraphqlSchema::getInstance();
            logDebug(fmt::format("Iterating over Einstein Summations"));
            // one einsum for each root field of the query
			for(auto i : iter::range(parsed_graphql->all_operands_labels.size())) {
				subscripts.emplace_back(std::make_shared<Subscript>(parsed_graphql->all_operands_labels[i],
																	parsed_graphql->all_result_labels[i]));
				all_paths.emplace_back(parsed_graphql->all_paths[i]);
				std::vector<const_BoolHypertrie> operands{};
				logDebug(fmt::format("Slicing TPs"));
				uint32_t field_pos = 0;
				// map from subscript labels to type names
                std::map<Subscript::Label, std::string> field_types{};
                // resolve the fields -- make use of the operands labels to find the fields in the schema
				// makes use of the fact that graphql queries form a tree
				// the first label of an operand label refers to the parent of the current field
                for(const auto &operand_labels : subscripts.back()->getRawSubscript().operands) {
                    const auto &field_name = parsed_graphql->all_fields_names[i][field_pos];
                    // root field
                    if(field_types.empty()) {
						field_types[operand_labels[0]] = schema.getFieldType(field_name);
						operands.emplace_back(triple_store.resolveGQLRootField(schema.getFieldUri(field_name)));
					}
                    else {
						auto parent_type = field_types[operand_labels[0]];
						field_types[operand_labels[1]] = schema.getFieldType(field_name, parent_type);
                        operands.emplace_back(triple_store.resolveGQLField(schema.getFieldUri(field_name, parent_type)));
					}
                    field_pos++;
				}
                all_operands.emplace_back(std::move(operands));
			}
		}

		[[nodiscard]] std::vector<std::shared_ptr<Einsum>> generateEinsums(const time_point_t &timeout) {
            std::vector<std::shared_ptr<Einsum>> einsums{};
			for(auto i : iter::range(subscripts.size())) {
				auto einsum = std::make_shared<Einsum>(subscripts[i], all_operands[i], timeout);
				einsums.push_back(std::move(einsum));
			}
			return einsums;
		}

		[[nodiscard]] const std::vector<std::vector<std::pair<Subscript::Label, std::string>>> &getPath(std::size_t pos) {
			return all_paths[pos];
		}

		[[nodiscard]] const std::vector<std::shared_ptr<Subscript>> &getSubscripts() const {
			return subscripts;
		}

		[[nodiscard]] const std::vector<std::vector<const_BoolHypertrie>> &getOperands() const {
			return all_operands;
		}

		friend struct ::fmt::formatter<GraphqlExecutionPackage>;

	};

}

template<>
struct fmt::formatter<tentris::store::cache::GraphqlExecutionPackage> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const tentris::store::cache::GraphqlExecutionPackage &p, FormatContext &ctx) {
        return format_to(ctx.begin(),
                         " GraphQL Query Name:     {}\n"
                         " subscripts:  {}\n",
                         p.query_name, p.subscripts);
    }
};
#endif//TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
