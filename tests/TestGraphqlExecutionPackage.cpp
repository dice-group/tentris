#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include <tentris/store/cache/GraphqlExecutionPackage.hpp>
#include <tentris/store/TripleStore.hpp>

namespace tentris::tests::graphql_execution {

    using namespace tentris::store::cache;
    using namespace tentris::store;

    TEST(TestGraphqlExecutionPackage, SingleQuerySlicing) {

        const std::string query = "query {"
                                  "  Article @rdf(base: \"http://localhost/vocabulary/bench/\") {"
                                  "    title @rdf(base: \"http://purl.org/dc/elements/1.1/\")"
                                  "    pages @rdf(base: \"http://swrc.ontoware.org/ontology#\")"
                                  "  }"
                                  "}";

		TripleStore trie{};

        AtomicTripleStore::getInstance().bulkloadRDF("data/test.nt");

        GraphqlExecutionPackage gep{query};
		assert(gep.getSubscripts().size() == 1);
		assert(gep.getOperands().size() == 1);
		assert(gep.getOperands()[0][0].size() == 68);
        assert(gep.getOperands()[0][1].size() == 72);
        assert(gep.getOperands()[0][2].size() == 67);
    }

}