#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include <tentris/store/graphql/ParsedGraphql.hpp>

namespace tentris::tests::graphql_parser {

	using namespace tentris::store::graphql;

    TEST(TestGraphqlParser, ParseSingleQuery) {

        const std::string query = "query {"
								  "  person {"
								  "    age"
								  "    name"
								  "  }"
								  "}";
        ParsedGraphql q{query};
        assert(q.getSubscripts().size() == 1);
		assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,]->bc");
    }

    TEST(TestGraphqlParser, ParseSingleNestedQuery) {

        const std::string query = "query {"
                                  "  person {"
                                  "    age"
                                  "    name"
								  "    company {"
								  "      location"
								  "      name"
								  "    }"
                                  "  }"
                                  "}";
        ParsedGraphql q{query};
		assert(q.getSubscripts().size() == 1);
		assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,],[,ad,[,de,],[,df,],]->bcef");
    }

    TEST(TestGraphqlParser, ParseSingleQueryWithArgument) {

        const std::string query = "query {"
                                  "  person(age: 24) {"
                                  "    name"
                                  "  }"
                                  "}";
        ParsedGraphql q{query};
        assert(q.getSubscripts().size() == 1);
        assert(q.getSubscripts()[0]->to_string() == "a,a,[,ab,]->b");
    }

    TEST(TestGraphqlParser, ParseSingleNestedQueryWithArguments) {

        const std::string query = "query {"
                                  "  person(age: 26) {"
                                  "    name"
                                  "    company(location: \"germany\") {"
                                  "      name"
                                  "    }"
                                  "  }"
                                  "}";
        ParsedGraphql q{query};
        assert(q.getSubscripts().size() == 1 and q.getSliceKeys().size() == 1);
        assert(q.getSubscripts()[0]->to_string() == "a,a,[,ab,],[,ac,c,[,cd,],]->bd");
        std::vector<std::vector<std::string>> expected_results = {
                {"person"},
                {"age", "26"},
                {"name"},
                {"company"},
                {"location", "germany"},
                {"name"}
        };
        ASSERT_THAT(q.getSliceKeys(), testing::ElementsAre(expected_results));
    }

    TEST(TestGraphqlParser, ParseSingleQueryWithDirectives) {

        const std::string query = "query {"
								  "  person @rdf(base: \"http://example.com/\" ) {"
                                  "    age"
                                  "    name"
                                  "  }"
                                  "}";
        ParsedGraphql q{query};
        assert(q.getSubscripts().size() == 1 and q.getSliceKeys().size() == 1);
        assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,]->bc");
		std::vector<std::vector<std::string>> expected_results = {
                {"http://example.com/person"},
                {"age"},
                {"name"}
		};
		ASSERT_THAT(q.getSliceKeys(), testing::ElementsAre(expected_results));
    }

    TEST(TestGraphqlParser, ParseSingleQueryWithMultipleDirectives) {

        const std::string query = "query {"
                                  "  person @rdf(base: \"http://example.com/\" ) {"
                                  "    age @rdf(base: \"http://example.com/\" )"
                                  "    name @rdf(base: \"http://example.com/\" )"
                                  "  }"
                                  "}";
        ParsedGraphql q{query};
        assert(q.getSubscripts().size() == 1 and q.getSliceKeys().size() == 1);
        assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,]->bc");
        std::vector<std::vector<std::string>> expected_results = {
                {"http://example.com/person"},
                {"http://example.com/age"},
                {"http://example.com/name"}
        };
        ASSERT_THAT(q.getSliceKeys(), testing::ElementsAre(expected_results));
    }

}