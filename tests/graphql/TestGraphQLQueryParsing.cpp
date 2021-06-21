#include <gtest/gtest.h>

#include "tentris/store/graphql/GraphQLParser.hpp"
#include "tentris/store/graphql/GraphQLSchema.hpp"

class TestGraphQLQueryParsing : public testing::Test {
protected:
	std::string schema_str = R"(type Query {
                                    people: [Person]
                                    articles: [Article]
                                    journals: [Journal]
                                    pets: Pet
                                }
                                type Person @uri(value: "http://xmlns.com/foaf/0.1/Person") {
                                    name: String @uri(value: "http://xmlns.com/foaf/0.1/name")
                                }
                                type Article @uri(value: "http://localhost/vocabulary/bench/Article") {
                                    note: String @uri(value: "http://swrc.ontoware.org/ontology#note")
                                    pages: Int! @uri(value: "http://swrc.ontoware.org/ontology#pages")
                                    title: String @uri(value: "http://purl.org/dc/elements/1.1/title")
                                    homepage: String @uri(value: "http://xmlns.com/foaf/0.1/homepage")
                                    journal: Journal @uri(value: "http://swrc.ontoware.org/ontology#journal")
                                    creator: [Person] @uri(value: "http://purl.org/dc/elements/1.1/creator")
                                }
                                type Journal @uri(value: "http://localhost/vocabulary/bench/Journal") {
                                    number: Int @uri(value: "http://swrc.ontoware.org/ontology#number")
                                    volume: Int @uri(value: "http://swrc.ontoware.org/ontology#volume")
                                    issued: Int @uri(value: "http://purl.org/dc/terms/issued")
                                    editor: [Person] @uri(value: "http://swrc.ontoware.org/ontology#editor")
                                }
                                interface Pet @uri(value: "http://localhost/vocabulary/bench/Pet") {
                                    name: String
                                }
                                type Dog implements Pet @uri(value: "http://localhost/vocabulary/bench/Dog") {
                                    name: String
                                    furColor: String
                                }
                                type Cat implements Pet @uri(value: "http://localhost/vocabulary/bench/Cat") {
                                    name: String
                                    eyeColor: String
                                })";

	GraphQLSchema schema{};

	virtual void SetUp() {
		GraphQLParser::parseSchema(schema_str, schema);
	}
};

TEST_F(TestGraphQLQueryParsing, SimpleQuery) {
	std::string query = "{ articles { pages title } }";
	auto parsed_query = GraphQLParser::parseQuery(&schema, query, "");
	assert(parsed_query.back().features.size() == 3);
	assert(parsed_query.back().operands_labels.size() == 7);
	assert(parsed_query.back().result_labels.size() == 3);
	assert(parsed_query.back().fragment_dependencies.empty());
	assert(parsed_query.back().features[0] == Feature{"http://localhost/vocabulary/bench/Article"});
	assert(parsed_query.back().features[1] == Feature{"http://swrc.ontoware.org/ontology#pages"});
	assert(parsed_query.back().features[2] == Feature{"http://purl.org/dc/elements/1.1/title"});
	assert(parsed_query.back().leaf_types['b'] == "Int");
	assert(parsed_query.back().leaf_types['c'] == "String");
	assert(parsed_query.back().field_names['a'] == "articles");
	assert(parsed_query.back().field_names['b'] == "pages");
	assert(parsed_query.back().field_names['c'] == "title");
	assert(parsed_query.back().paths.size() == 2);
    assert(parsed_query.back().paths[0] == (Path{'a', 'b'}));
    assert(parsed_query.back().paths[1] == (Path{'a', 'c'}));
}

TEST_F(TestGraphQLQueryParsing, FragmentDependencies) {
    std::string query = "{ pets { ... on Dog { furColor } name } }";
    auto parsed_query = GraphQLParser::parseQuery(&schema, query, "");
    assert(parsed_query.back().features.size() == 4);
    assert(parsed_query.back().operands_labels.size() == 10);
    assert(parsed_query.back().result_labels.size() == 3);
    assert(parsed_query.back().fragment_dependencies.size() == 1);
	assert(parsed_query.back().fragment_dependencies['a']["http://localhost/vocabulary/bench/Dog"] == std::vector{'b'});
    assert(parsed_query.back().field_names['a'] == "pets");
    assert(parsed_query.back().field_names['b'] == "furColor");
    assert(parsed_query.back().field_names['c'] == "name");
    assert(parsed_query.back().paths.size() == 2);
    assert(parsed_query.back().paths[0] == (Path{'a', 'b'}));
    assert(parsed_query.back().paths[1] == (Path{'a', 'c'}));
}