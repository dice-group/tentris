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

#include <boost/algorithm/string.hpp>
#include <Dice/einsum/internal/Subscript.hpp>
#include <libgraphqlparser/Ast.h>
#include <robin_hood.h>

#include "tentris/store/graphql/GraphqlSchema.hpp"
#include "tentris/store/graphql/internal/GraphqlDocumentVisitor.hpp"
#include "tentris/store/graphql/internal/GraphqlRootSelectionVisitor.hpp"

namespace tentris::store::graphql {

	namespace {
		using Subscript = einsum::internal::Subscript;
		using namespace fmt::literals;
		using namespace facebook::graphql::ast;
    }

    class ParsedGraphql {

	private:

        std::vector<std::shared_ptr<Subscript>> subscripts{};
		std::vector<std::vector<std::vector<std::string>>> slice_keys{};

	public:

        ParsedGraphql() = default;

        explicit ParsedGraphql(const std::string& request_doc_path, const GraphqlSchema& schema) {
            auto root_node = GraphqlParser::parseRequestDocument(request_doc_path);
			GraphqlDocumentVisitor doc_visitor{};
			root_node->accept(&doc_visitor);
            auto query = doc_visitor.getQuery();
            parseQuery(query, schema);
		}

		explicit ParsedGraphql(const std::string& request_doc_path, const std::string& q_name, const GraphqlSchema& schema) {
            auto root_node = GraphqlParser::parseRequestDocument(request_doc_path);
            GraphqlDocumentVisitor doc_visitor{};
            root_node->accept(&doc_visitor);
            auto query = doc_visitor.getQuery(q_name);
            parseQuery(query, schema);
		}

        [[nodiscard]] const std::vector<std::shared_ptr<Subscript>> &getSubscripts() const {
            return subscripts;
        }

        [[nodiscard]] const std::vector<std::vector<std::vector<std::string>>> &getSliceKeys() const {
			return slice_keys;
		}

	private:

		void parseQuery(const facebook::graphql::ast::OperationDefinition* query, const GraphqlSchema& schema) {
			const auto& root_selection_set = query->getSelectionSet();
			for(const auto& root_selection : root_selection_set.getSelections()) {
                GraphqlRootSelectionVisitor rsv{};
				std::vector<std::vector<std::string>> root_selection_slice_keys{};
				std::map<std::string, std::string> field_type_map{};
				root_selection->accept(&rsv);
				subscripts.push_back(rsv.getSubscript());
				auto& fields = rsv.getFields();
				auto root_field = fields.begin();
				// get the uri of the type of the root field
				root_selection_slice_keys.push_back({schema.getEntryPointUri((*root_field)[0].second)});
				field_type_map[(*root_field)[0].second] = schema.getEntryPointType((*root_field)[0].second);
				// iterate over the nested fields
				for(auto iter = fields.begin() + 1; iter != fields.end(); iter++) {
                    std::vector<std::string> field_slice_key{};
					for(auto& entry : *iter) {
						auto parent_type = field_type_map[entry.first];
						field_slice_key.push_back(schema.getFieldUri(parent_type, entry.second));
						field_type_map[entry.second] = schema.getFieldType(parent_type, entry.second);
					}
					root_selection_slice_keys.emplace_back(std::move(field_slice_key));
				}
				slice_keys.emplace_back(std::move(root_selection_slice_keys));
			}
		}

	};

}

#endif//TENTRIS_PARSEDGRAPHQL_HPP
