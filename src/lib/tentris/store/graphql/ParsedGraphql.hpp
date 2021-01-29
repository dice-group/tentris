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

#include "tentris/store/graphql/internal/GraphqlParser.hpp"
#include "tentris/store/graphql/internal/GraphqlDocumentVisitor.hpp"
#include "tentris/store/graphql/internal/GraphqlQueryVisitor.hpp"

namespace tentris::store::graphql {

	namespace {
		using Subscript = einsum::internal::Subscript;
		using namespace fmt::literals;
        using namespace tentris::graphql::internal;
		using namespace facebook::graphql::ast;
    }

    class ParsedGraphql {

	private:

        std::unique_ptr<GQLAstNode> root_node;
		GraphqlDocumentVisitor gql_doc_visitor;
        std::vector<std::shared_ptr<Subscript>> subscripts;
		std::vector<std::vector<std::vector<std::string>>> slice_keys{};
        std::string graphql_str;

	public:

        ParsedGraphql() = default;

        explicit ParsedGraphql(const std::string& graphql_str) : graphql_str(graphql_str) {
			root_node = GraphQLParser::parseRequestString(graphql_str);
			generate_subscript();
		}

        [[nodiscard]] const std::vector<std::shared_ptr<Subscript>> &getSubscripts() const {
            return subscripts;
        }

        [[nodiscard]] const std::vector<std::vector<std::vector<std::string>>> &getSliceKeys() const {
			return slice_keys;
		}

        [[nodiscard]] const std::string &getGraphqlStr() const {
			return graphql_str;
		}


	private:

		void generate_subscript() {
			root_node->accept(&gql_doc_visitor);
			if(gql_doc_visitor.getOpType() == "query") {
				GraphqlQueryVisitor gql_query_visitor{};
				gql_doc_visitor.getOperation()->accept(&gql_query_visitor);
				subscripts = std::move(gql_query_visitor.getSubscripts());
				slice_keys = std::move(gql_query_visitor.getSlicingKeys());
			}
		}

	};

}

#endif//TENTRIS_PARSEDGRAPHQL_HPP
