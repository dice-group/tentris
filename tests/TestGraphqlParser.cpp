#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include <tentris/store/graphql/ParsedGraphql.hpp>
#include <tentris/store/graphql/GraphqlSchema.hpp>

namespace tentris::tests::graphql_parser {

	using namespace tentris::store::graphql;

    class ParsedGraphqlTest : public ::testing::Test {

    protected:

        GraphqlSchema schema{"data/schema.graphql"};
        std::string queries_file = "data/queries.graphql";

    };

    TEST_F(ParsedGraphqlTest, ParseQuerySingleField) {
		ParsedGraphql q{queries_file, "first", schema};
        assert(q.getSubscripts().size() == 1);
		assert(q.getSubscripts()[0]->to_string() == "a,[,ab,]->b");
    }

    TEST_F(ParsedGraphqlTest, ParseQueryMultipleFields) {
        ParsedGraphql q{queries_file, "second", schema};
		assert(q.getSubscripts().size() == 1);
		assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,],[,ad,]->bcd");
    }

    TEST_F(ParsedGraphqlTest, ParseQuerySingleNestedSelectionSet) {
        ParsedGraphql q{queries_file, "third", schema};
        assert(q.getSubscripts().size() == 1);
        assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,[,cd,],[,ce,],]->bde");
    }

    TEST_F(ParsedGraphqlTest, ParseQueryMultipleNestedSelectionSets) {
        ParsedGraphql q{queries_file, "fourth", schema};
        assert(q.getSubscripts().size() == 1);
        assert(q.getSubscripts()[0]->to_string() == "a,[,ab,],[,ac,[,cd,],],[,ae,[,ef,],]->bdf");
    }

    TEST_F(ParsedGraphqlTest, ParseQueryMultipleRootFields) {
        ParsedGraphql q{queries_file, "fifth", schema};
        assert(q.getSubscripts().size() == 2);
        assert(q.getSubscripts()[0]->to_string() == "a,[,ab,]->b");
        assert(q.getSubscripts()[1]->to_string() == "a,[,ab,],[,ac,]->bc");
    }

}