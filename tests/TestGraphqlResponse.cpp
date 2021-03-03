#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include <tentris/store/graphql/GraphqlDocument.hpp>
#include <tentris/store/graphql/GraphqlSchema.hpp>
#include <tentris/store/graphql/GraphqlResponse.hpp>

namespace tentris::tests::graphql_response {

    using namespace tentris::store::graphql;

    class GraphqlResponseTest : public ::testing::Test {

    protected:

        void SetUp() override {
            schema.load(s_document);
        }

        GraphqlSchema schema{};
        GraphqlDocument s_document{"data/schema.graphql", true};
        GraphqlDocument q_document{"data/queries.graphql"};

    };

	TEST_F(GraphqlResponseTest, SingleField) {
        ParsedGraphql q{q_document.getQuery("second"), schema};
		GraphqlResponse response{q_document.getQuery("second"), schema};
	}

}