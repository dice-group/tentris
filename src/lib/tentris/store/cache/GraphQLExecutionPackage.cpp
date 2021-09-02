#include "GraphQLExecutionPackage.hpp"

namespace tentris::store::cache {
	GraphQLExecutionPackage::GraphQLExecutionPackage(const std::pair<std::string, std::string> &request) {
		using namespace logging;
		logDebug(fmt::format("Parsing document {}", std::get<0>(request)));
		parsed_graphql = graphql::GraphQLParser::parseQuery(&AtomicGraphQLSchema::getInstance(),
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
				if (pos == 0) {
					logDebug(fmt::format("Slicing for root field: `{}`", features[pos]));
					auto field_operand = triple_store.resolveGQLObjectType(features[pos].back());
					// argument id
					try {
						field_operand = triple_store.resolveGQLArgumentID(sub_query.id_arguments.at(pos), field_operand);
					} catch (...) {}
					operands.push_back(std::move(field_operand));
				}
				// arguments, inner/leaf fields and inline fragments
				else {
					// fields
					if (operand_labels.size() == 2) {
						logDebug(fmt::format("Slicing for inner/leaf field: `{}`", features[pos]));
						auto field_operand = triple_store.resolveGQLField(features[pos].back());
						// id field
                        try {
                            field_operand = triple_store.resolveGQLArgumentID(sub_query.id_arguments.at(pos), field_operand);
                        } catch (...) {}
                        operands.push_back(std::move(field_operand));
					} else {
						// arguments
						if (features[pos].size() == 3) {
                            logDebug(fmt::format("Slicing for argument: `{}`. Argument value: `{}`",
												 features[pos][0], features[pos][1]));
							operands.push_back(triple_store.resolveGQLArgument(features[pos][0],
																			   features[pos][1],
																			   features[pos][2]));
						} else { // inline fragment and type filter
                            logDebug(fmt::format("Slicing for type : `{}`", features[pos]));
							operands.push_back(triple_store.resolveGQLObjectType(features[pos].back()));
						}
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