#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include <tentris/store/graphql/GraphqlSchema.hpp>
#include <tentris/store/cache/GraphqlExecutionPackage.hpp>

namespace tentris::tests::graphql_execution {

    using namespace tentris::store;
    using namespace tentris::store::cache;
    using namespace tentris::store::graphql;

	class GraphqlExecutionPackageTest : public ::testing::Test {

	protected:

        void SetUp() override {
            AtomicTripleStore::getInstance().bulkloadRDF("data/test.nt");
        }

        TripleStore trie{};
        GraphqlSchema schema{"data/schema.graphql"};
        std::string queries_file = "data/queries.graphql";

	};

    TEST_F(GraphqlExecutionPackageTest, QuerySingleFieldSlicing) {
        GraphqlExecutionPackage gep{queries_file, schema, "first"};
		auto sliced_operands = gep.getOperands()[0];
		assert(sliced_operands.size() == 2);
		assert(sliced_operands[0].size() == 58);
        assert(sliced_operands[1].size() == 58);
    }

    TEST_F(GraphqlExecutionPackageTest, QueryMultipleFieldsSlicing) {
        GraphqlExecutionPackage gep{queries_file, schema, "second"};
        auto sliced_operands = gep.getOperands()[0];
        assert(sliced_operands.size() == 4);
		assert(sliced_operands[0].size() == 68);
        assert(sliced_operands[1].size() == 72);
        assert(sliced_operands[2].size() == 67);
        assert(sliced_operands[3].size() == 69);
    }

}