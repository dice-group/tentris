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
                                    age: Int
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

TEST_F(TestGraphQLQueryParsing, FieldCollectionSameSelectionSet) {
    std::string base_query = "{ articles { title pages } } ";
	std::vector<std::string> test_queries {
            "{ articles { title pages title } } ",
            "{ articles { title title pages title title } } ",
            "{ articles { title pages pages title } } ",
            "{ articles { title pages title pages title pages } } "
	};
    auto parsed_base = GraphQLParser::parseQuery(&schema, base_query, "");
    for (const auto& test_query : test_queries) {
        auto parsed_test = GraphQLParser::parseQuery(&schema, test_query, "");
		assert(parsed_base.back().paths == parsed_test.back().paths);
		assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
		assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
		assert(parsed_base.back().result_labels == parsed_test.back().result_labels);
        assert(parsed_base.back().features == parsed_test.back().features);
    }
	std::string test_query = "{ articles { pages title pages } } ";
    auto parsed_test = GraphQLParser::parseQuery(&schema, test_query, "");
    assert(parsed_base.back().paths == parsed_test.back().paths);
    assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
    assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
    assert(parsed_base.back().result_labels == parsed_test.back().result_labels);
    ASSERT_FALSE(parsed_base.back().features == parsed_test.back().features);
}

TEST_F(TestGraphQLQueryParsing, FieldCollectionDifferentSelectionSet) {
    std::string base_query = "{ articles { pages journal { number volume issued } } } ";
    std::vector<std::string> test_queries {
            "{ articles { pages journal { number } journal { volume } journal { issued } } } ",
            "{ articles { pages journal { number } journal { volume issued } } } ",
            "{ articles { pages journal { number volume issued } journal { issued volume } } } ",
            "{ articles { pages journal { number volume issued } journal { issued } } } "
    };
    auto parsed_base = GraphQLParser::parseQuery(&schema, base_query, "");
    for (const auto& test_query : test_queries) {
        auto parsed_test = GraphQLParser::parseQuery(&schema, test_query, "");
        assert(parsed_base.back().paths == parsed_test.back().paths);
        assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
        assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
        assert(parsed_base.back().result_labels == parsed_test.back().result_labels);
        assert(parsed_base.back().features == parsed_test.back().features);
    }
}

TEST_F(TestGraphQLQueryParsing, FieldCollectionDifferentSelectionSetNested) {
    std::string base_query = "{ articles { pages journal { number editor { name age } volume } } } ";
    std::vector<std::string> test_queries {
            "{ articles { pages journal { number editor { name } } journal { volume editor { age } } } } ",
            "{ articles { pages journal { number editor { name } volume } journal { volume editor { name age } } } } "
    };
    auto parsed_base = GraphQLParser::parseQuery(&schema, base_query, "");
    for (const auto& test_query : test_queries) {
        auto parsed_test = GraphQLParser::parseQuery(&schema, test_query, "");
        assert(parsed_base.back().paths == parsed_test.back().paths);
        assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
        assert(parsed_base.back().operands_labels == parsed_test.back().operands_labels);
        assert(parsed_base.back().result_labels == parsed_test.back().result_labels);
        assert(parsed_base.back().features == parsed_test.back().features);
    }
}

TEST_F(TestGraphQLQueryParsing, RootFieldAlias) {
	std::string query = "{ alias:people { name } }";
	auto parsed_query = GraphQLParser::parseQuery(&schema, query, "");
	assert(parsed_query.back().field_names.size() == 2);
	assert(parsed_query.back().field_names['a'] == "alias");
	assert(parsed_query.back().field_names['b'] == "name");
	assert(parsed_query.back().paths.size() == 1);
	assert(parsed_query.back().operands_labels.size() == 4);
}

TEST_F(TestGraphQLQueryParsing, InnerAndLeafFieldAlias) {
    std::string query = "{ people { name alias2:age } }";
    auto parsed_query = GraphQLParser::parseQuery(&schema, query, "");
    assert(parsed_query.back().field_names.size() == 3);
    assert(parsed_query.back().field_names['a'] == "people");
    assert(parsed_query.back().field_names['b'] == "name");
    assert(parsed_query.back().field_names['c'] == "alias2");
    assert(parsed_query.back().paths.size() == 2);
    assert(parsed_query.back().operands_labels.size() == 7);
}

TEST_F(TestGraphQLQueryParsing, AliasAndFieldCollection) {
    std::string query = "{ people { name name:age } }";
    auto parsed_query = GraphQLParser::parseQuery(&schema, query, "");
    assert(parsed_query.back().field_names.size() == 2);
    assert(parsed_query.back().field_names['a'] == "people");
    assert(parsed_query.back().field_names['b'] == "name");
    assert(parsed_query.back().paths.size() == 1);
    assert(parsed_query.back().operands_labels.size() == 4);
    std::string query1 = "{ people { n:name name:age } }";
    auto parsed_query1 = GraphQLParser::parseQuery(&schema, query1, "");
    assert(parsed_query1.back().field_names.size() == 3);
    assert(parsed_query1.back().field_names['a'] == "people");
    assert(parsed_query1.back().field_names['b'] == "n");
    assert(parsed_query1.back().field_names['c'] == "name");
    assert(parsed_query1.back().paths.size() == 2);
    assert(parsed_query1.back().operands_labels.size() == 7);
    std::string query2 = "{ articles { p:pages people:creator { name } } }";
    auto parsed_query2 = GraphQLParser::parseQuery(&schema, query2, "");
    assert(parsed_query2.back().field_names.size() == 4);
    assert(parsed_query2.back().field_names['a'] == "articles");
    assert(parsed_query2.back().field_names['b'] == "p");
    assert(parsed_query2.back().field_names['c'] == "people");
    assert(parsed_query2.back().field_names['d'] == "name");
    assert(parsed_query2.back().paths.size() == 2);
    assert(parsed_query2.back().operands_labels.size() == 10);
    std::string query3 = "{ articles { p:pages p:creator { name } } }";
    ASSERT_THROW(GraphQLParser::parseQuery(&schema, query3, ""), SchemaException);
}