#include "GraphQLExecutionPackage.hpp"

namespace tentris::store::cache {
	GraphQLExecutionPackage::GraphQLExecutionPackage(const std::pair<std::string, std::string> &request) {
		using namespace logging;
		logDebug(fmt::format("Parsing document {}", std::get<0>(request)));
		parsed_graphql = graphql::GraphQLParser::parseQuery(&AtomicGraphqlSchema::getInstance(),
															std::get<0>(request),
															std::get<1>(request));
		auto &triple_store = AtomicTripleStore::getInstance();
		logDebug(fmt::format("Preparing Subscripts"));
		// one subscript for each root field of the query
		for (auto &sub_query : parsed_graphql) {
			std::vector<const_BoolHypertrie> operands{};
			// create on operand for each graphql feature
			uint32_t pos = 0;
			const auto &features = sub_query.features;
			for (const auto &operand_labels : sub_query.operands_labels) {
				if (operand_labels == opt_begin or operand_labels == opt_end)
					continue;
				// root field
				if (pos == 0)
					operands.push_back(triple_store.resolveGQLObjectType(features[pos].back()));
				// arguments, inner/leaf fields and inline fragments
				else {
					// fields
					if (operand_labels.size() == 2)
						operands.push_back(triple_store.resolveGQLField(features[pos].back()));
					else {
						// arguments
						if (features[pos].size() == 3)
							operands.push_back(triple_store.resolveGQLArgument(features[pos][0],
																			   features[pos][1],
																			   features[pos][2]));
						// inline fragment and type filter
						else
							operands.push_back(triple_store.resolveGQLObjectType(features[pos].back()));
					}
				}
				pos++;
			}
			subscripts.push_back(std::make_shared<Subscript>(sub_query.operands_labels, sub_query.result_labels));
			logDebug(fmt::format("Subscript #{}: {}", subscripts.size(), subscripts.back()->to_string()));
			all_operands.push_back(std::move(operands));
		}
	}

}// namespace tentris::store::cache