#ifndef DICE_SPARQL_SELECTASKQUERYVISITOR_HPP
#define DICE_SPARQL_SELECTASKQUERYVISITOR_HPP

#include <rdf4cpp/rdf.hpp>

#include <SparqlParser/SparqlParserBaseVisitor.h>

#include "dice/sparql2tensor/SPARQLQuery.hpp"

#include <robin_hood.h>

namespace dice::sparql2tensor::parser::visitors {

	using namespace dice::sparql_parser::base;

	class SelectAskQueryVisitor : public SparqlParserBaseVisitor {

	private:
		SPARQLQuery *const query;
		rdf4cpp::rdf::Node active_subject;
		rdf4cpp::rdf::Node active_predicate;
		char var_id = 'a';
		/* for the construction of the operand dependency graph */
		// stack of group graph patterns
		std::vector<std::vector<uint8_t>> group_patterns;
		// stack of operands appearing in optional patterns; one vector per graph pattern
		std::vector<std::vector<uint8_t>> opt_operands;
		// stack of operands appearing in union patterns found in optional patterns; one vector per graph pattern
		// it is used to avoid creating cartesian connections between optional operands of the same union pattern
		// once an optional sub graph pattern is visited, this vector needs to be cleared
		std::vector<std::vector<uint8_t>> union_operands;
		/* for the "query rewriting" */
		std::vector<std::vector<SparqlParser::TriplesBlockContext *>> triples_blocks;
		std::vector<std::vector<SparqlParser::OptionalGraphPatternContext *>> optional_blocks;

	public:
		SelectAskQueryVisitor() = delete;

		explicit SelectAskQueryVisitor(SPARQLQuery *q) : query{q} {}

		std::any visitAskQuery(SparqlParser::AskQueryContext *ctx) override;

		std::any visitSelectQuery(SparqlParser::SelectQueryContext *) override;

		std::any visitSelectClause(SparqlParser::SelectClauseContext *) override;

		std::any visitWhereClause(SparqlParser::WhereClauseContext *) override;

		std::any visitGroupGraphPattern(SparqlParser::GroupGraphPatternContext *) override;

		std::any visitGroupGraphPatternSub(SparqlParser::GroupGraphPatternSubContext *) override;

		std::any visitTriplesBlock(SparqlParser::TriplesBlockContext *) override;

		std::any visitTriplesSameSubjectPath(SparqlParser::TriplesSameSubjectPathContext *) override;

		std::any visitPropertyListPathNotEmpty(SparqlParser::PropertyListPathNotEmptyContext *) override;

		std::any visitVarOrTerm(SparqlParser::VarOrTermContext *) override;

		std::any visitIri(SparqlParser::IriContext *) override;

		std::any visitBlankNode(SparqlParser::BlankNodeContext *) override;

		std::any visitVar(SparqlParser::VarContext *) override;

		std::any visitObjectListPath(SparqlParser::ObjectListPathContext *) override;

		std::any visitObjectList(SparqlParser::ObjectListContext *) override;

		std::any visitObjectPath(SparqlParser::ObjectPathContext *) override;

		std::any visitObject(SparqlParser::ObjectContext *) override;

		std::any visitPath(SparqlParser::PathContext *) override;

		std::any visitPathAlternative(SparqlParser::PathAlternativeContext *) override;

		std::any visitPathSequence(SparqlParser::PathSequenceContext *) override;

		std::any visitPathEltOrInverse(SparqlParser::PathEltOrInverseContext *) override;

		std::any visitPathElt(SparqlParser::PathEltContext *) override;

		std::any visitRdfLiteral(SparqlParser::RdfLiteralContext *) override;

		std::any visitNumericLiteral(SparqlParser::NumericLiteralContext *) override;

		std::any visitBooleanLiteral(SparqlParser::BooleanLiteralContext *) override;

		std::any visitString(SparqlParser::StringContext *) override;

	private:
		void register_var(rdf4cpp::rdf::query::Variable const &var);

		/**
		 * @brief: Creates a new node in the operand dependency graph and the dependencies between
		 * the new node and the nodes corresponding to triple patterns of the same group graph pattern.
		 * @param tp A triple pattern
		 */
		void add_tp(rdf4cpp::rdf::query::TriplePattern const &tp);

		/**
		 * @brief: Creates dependencies between the nodes (i.e., triple patterns) of differnt group graph patterns.
		 * @param prev_group The previous group graph pattern.
		 * @param cur_group The current group graph pattern.
		 * @param bidirectional Whether the edges should be bidirectional (e.g., in OPTIONAL they are unidirectional).
		 */
		void group_dependencies(std::vector<uint8_t> const &prev_group, std::vector<uint8_t> const &cur_group, bool bidirectional = false);

		/**
		 * @brief: Creates connections between group graph patterns.
		 * Used to capture cartesian products between different optional group graph patterns.
		 * @param prev_group The previous group graph pattern.
		 * @param cur_group The current group graph pattern.
		 */
		void group_connections(std::vector<uint8_t> const &prev_group, std::vector<uint8_t> const &cur_group);

		/**
		 * @brief: A visitor for well-designed SPARQL patterns only.
		 * @param ctx A GroupGraphPatternSub context.
		 * @param gou_ctxs A vector of GroupOrUnionGraphPattern contexts.
		 */
		void visitWellDesignedPattern(SparqlParser::GroupGraphPatternSubContext *ctx,
									  std::vector<SparqlParser::GroupOrUnionGraphPatternContext *> gou_ctxs);
	};

}// namespace dice::sparql2tensor::parser::visitors

#endif//DICE_SPARQL_SELECTASKQUERYVISITOR_HPP