#include <gtest/gtest.h>

#include <tentris/store/RDF/TermStore.hpp>

namespace tentris::tests::term_store {

	using namespace tentris::store::rdf;

	TEST(TestTermStore, double_write) {
		TermStore store{};
		auto id_ = store[Dice::rdf::parse_term("\"xx\"")];
		auto id2_ = store[Dice::rdf::parse_term("\"xx\"")];
		ASSERT_EQ(id_, id2_);
		ASSERT_EQ(store.size(), 1);
	}

	TEST(TestTermStore, double_write_same_str) {
		TermStore store{};
		auto id_ = store[Dice::rdf::parse_term(R"("Journal 1 (1940) "^^<http://www.w3.org/2001/XMLSchema#string>)")];
		auto id2_ = store[Dice::rdf::parse_term("\"Journal 1 (1940) \"")];
		ASSERT_EQ(id_, id2_);
		ASSERT_EQ(store.size(), 1);
	}


}