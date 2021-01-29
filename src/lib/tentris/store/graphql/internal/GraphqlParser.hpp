#ifndef TENTRIS_GRAPHQLPARSER_HPP
#define TENTRIS_GRAPHQLPARSER_HPP

#include <libgraphqlparser/GraphQLParser.h>
#include <libgraphqlparser/AstNode.h>

namespace tentris::graphql::internal {

    using GQLAstNode = facebook::graphql::ast::Node;

    // Makes use of the libgraphqlparser library
    // https://github.com/graphql/libgraphqlparser
    class GraphQLParser {

    public:

        static std::unique_ptr<GQLAstNode> parseRequestFile(FILE* file) {
            const char* error;
            auto root = facebook::graphql::parseFile(file, &error);
            if(root == nullptr) {
                std::stringstream err_msg {"GraphQL parse error at "};
                err_msg << error;
                throw std::runtime_error{err_msg.str()};
            }
            return root;
        }

        static std::unique_ptr<GQLAstNode> parseRequestString(const std::string& input_str) {
            const char* error;
            auto root = facebook::graphql::parseString(input_str.c_str(), &error);
            if(root == nullptr) {
                std::stringstream err_msg {"GraphQL parse error at "};
                err_msg << error;
                throw std::runtime_error{err_msg.str()};
            }
            return root;
        }

    };

} // tentris::graphql::internal

#endif//TENTRIS_GRAPHQLPARSER_HPP
