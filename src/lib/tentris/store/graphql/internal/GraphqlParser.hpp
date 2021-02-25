#ifndef TENTRIS_GRAPHQLPARSER_HPP
#define TENTRIS_GRAPHQLPARSER_HPP

#include <GraphQLParser.h>
#include <AstNode.h>

namespace tentris::store::graphql::internal {

    using GQLAstNode = facebook::graphql::ast::Node;

    // Makes use of the libgraphqlparser library
    // https://github.com/graphql/libgraphqlparser
    class GraphqlParser {

    public:

        static std::unique_ptr<GQLAstNode> parseSchema(const std::string& path_to_schema_file) {
            const char* error;
			FILE* file = std::fopen(path_to_schema_file.c_str(), "r");
            auto root = facebook::graphql::parseFileWithExperimentalSchemaSupport(file, &error);
            if(root == nullptr) {
                std::stringstream err_msg {"GraphQL parse error at "};
                err_msg << error;
                throw std::runtime_error{err_msg.str()};
            }
			fclose(file);
            return root;
        }

        static std::unique_ptr<GQLAstNode> parseRequestDocument(const std::string& path_to_request_document) {
            const char* error;
            std::unique_ptr<GQLAstNode> root;
            FILE* file = std::fopen(path_to_request_document.c_str(), "r");
			if(file != nullptr)
                root = facebook::graphql::parseFile(file, &error);
			else
                root = facebook::graphql::parseString(path_to_request_document.c_str(), &error);
            if(root == nullptr) {
                std::stringstream err_msg {"GraphQL parse error at "};
                err_msg << error;
                throw std::runtime_error{err_msg.str()};
            }
			if(file != nullptr)
                fclose(file);
            return root;
        }

    };

} // tentris::graphql::internal

#endif//TENTRIS_GRAPHQLPARSER_HPP
