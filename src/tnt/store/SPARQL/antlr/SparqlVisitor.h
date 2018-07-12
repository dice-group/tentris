
// Generated from Sparql.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "SparqlParser.h"


namespace tnt::store::sparql::detail {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by SparqlParser.
 */
class  SparqlVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SparqlParser.
   */
    virtual antlrcpp::Any visitQuery(SparqlParser::QueryContext *context) = 0;

    virtual antlrcpp::Any visitPrologue(SparqlParser::PrologueContext *context) = 0;

    virtual antlrcpp::Any visitBaseDecl(SparqlParser::BaseDeclContext *context) = 0;

    virtual antlrcpp::Any visitPrefixDecl(SparqlParser::PrefixDeclContext *context) = 0;

    virtual antlrcpp::Any visitSelectQuery(SparqlParser::SelectQueryContext *context) = 0;

    virtual antlrcpp::Any visitSelectModifier(SparqlParser::SelectModifierContext *context) = 0;

    virtual antlrcpp::Any visitConstructQuery(SparqlParser::ConstructQueryContext *context) = 0;

    virtual antlrcpp::Any visitDescribeQuery(SparqlParser::DescribeQueryContext *context) = 0;

    virtual antlrcpp::Any visitAskQuery(SparqlParser::AskQueryContext *context) = 0;

    virtual antlrcpp::Any visitDatasetClause(SparqlParser::DatasetClauseContext *context) = 0;

    virtual antlrcpp::Any visitDefaultGraphClause(SparqlParser::DefaultGraphClauseContext *context) = 0;

    virtual antlrcpp::Any visitNamedGraphClause(SparqlParser::NamedGraphClauseContext *context) = 0;

    virtual antlrcpp::Any visitSourceSelector(SparqlParser::SourceSelectorContext *context) = 0;

    virtual antlrcpp::Any visitWhereClause(SparqlParser::WhereClauseContext *context) = 0;

    virtual antlrcpp::Any visitSolutionModifier(SparqlParser::SolutionModifierContext *context) = 0;

    virtual antlrcpp::Any visitLimitOffsetClauses(SparqlParser::LimitOffsetClausesContext *context) = 0;

    virtual antlrcpp::Any visitOrderClause(SparqlParser::OrderClauseContext *context) = 0;

    virtual antlrcpp::Any visitOrderCondition(SparqlParser::OrderConditionContext *context) = 0;

    virtual antlrcpp::Any visitLimitClause(SparqlParser::LimitClauseContext *context) = 0;

    virtual antlrcpp::Any visitOffsetClause(SparqlParser::OffsetClauseContext *context) = 0;

    virtual antlrcpp::Any visitGroupGraphPattern(SparqlParser::GroupGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitTriplesBlock(SparqlParser::TriplesBlockContext *context) = 0;

    virtual antlrcpp::Any visitGraphPatternNotTriples(SparqlParser::GraphPatternNotTriplesContext *context) = 0;

    virtual antlrcpp::Any visitOptionalGraphPattern(SparqlParser::OptionalGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitGraphGraphPattern(SparqlParser::GraphGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitGroupOrUnionGraphPattern(SparqlParser::GroupOrUnionGraphPatternContext *context) = 0;

    virtual antlrcpp::Any visitFilter(SparqlParser::FilterContext *context) = 0;

    virtual antlrcpp::Any visitConstraint(SparqlParser::ConstraintContext *context) = 0;

    virtual antlrcpp::Any visitFunctionCall(SparqlParser::FunctionCallContext *context) = 0;

    virtual antlrcpp::Any visitArgList(SparqlParser::ArgListContext *context) = 0;

    virtual antlrcpp::Any visitConstructTemplate(SparqlParser::ConstructTemplateContext *context) = 0;

    virtual antlrcpp::Any visitConstructTriples(SparqlParser::ConstructTriplesContext *context) = 0;

    virtual antlrcpp::Any visitTriplesSameSubject(SparqlParser::TriplesSameSubjectContext *context) = 0;

    virtual antlrcpp::Any visitPropertyListNotEmpty(SparqlParser::PropertyListNotEmptyContext *context) = 0;

    virtual antlrcpp::Any visitPropertyList(SparqlParser::PropertyListContext *context) = 0;

    virtual antlrcpp::Any visitObjectList(SparqlParser::ObjectListContext *context) = 0;

    virtual antlrcpp::Any visitObject(SparqlParser::ObjectContext *context) = 0;

    virtual antlrcpp::Any visitVerb(SparqlParser::VerbContext *context) = 0;

    virtual antlrcpp::Any visitTriplesNode(SparqlParser::TriplesNodeContext *context) = 0;

    virtual antlrcpp::Any visitBlankNodePropertyList(SparqlParser::BlankNodePropertyListContext *context) = 0;

    virtual antlrcpp::Any visitCollection(SparqlParser::CollectionContext *context) = 0;

    virtual antlrcpp::Any visitGraphNode(SparqlParser::GraphNodeContext *context) = 0;

    virtual antlrcpp::Any visitVarOrTerm(SparqlParser::VarOrTermContext *context) = 0;

    virtual antlrcpp::Any visitVarOrIRIref(SparqlParser::VarOrIRIrefContext *context) = 0;

    virtual antlrcpp::Any visitVar(SparqlParser::VarContext *context) = 0;

    virtual antlrcpp::Any visitGraphTerm(SparqlParser::GraphTermContext *context) = 0;

    virtual antlrcpp::Any visitExpression(SparqlParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitConditionalOrExpression(SparqlParser::ConditionalOrExpressionContext *context) = 0;

    virtual antlrcpp::Any visitConditionalAndExpression(SparqlParser::ConditionalAndExpressionContext *context) = 0;

    virtual antlrcpp::Any visitValueLogical(SparqlParser::ValueLogicalContext *context) = 0;

    virtual antlrcpp::Any visitRelationalExpression(SparqlParser::RelationalExpressionContext *context) = 0;

    virtual antlrcpp::Any visitNumericExpression(SparqlParser::NumericExpressionContext *context) = 0;

    virtual antlrcpp::Any visitAdditiveExpression(SparqlParser::AdditiveExpressionContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicativeExpression(SparqlParser::MultiplicativeExpressionContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpression(SparqlParser::UnaryExpressionContext *context) = 0;

    virtual antlrcpp::Any visitPrimaryExpression(SparqlParser::PrimaryExpressionContext *context) = 0;

    virtual antlrcpp::Any visitBrackettedExpression(SparqlParser::BrackettedExpressionContext *context) = 0;

    virtual antlrcpp::Any visitBuiltInCall(SparqlParser::BuiltInCallContext *context) = 0;

    virtual antlrcpp::Any visitRegexExpression(SparqlParser::RegexExpressionContext *context) = 0;

    virtual antlrcpp::Any visitIriRefOrFunction(SparqlParser::IriRefOrFunctionContext *context) = 0;

    virtual antlrcpp::Any visitRdfLiteral(SparqlParser::RdfLiteralContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteral(SparqlParser::NumericLiteralContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteralUnsigned(SparqlParser::NumericLiteralUnsignedContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteralPositive(SparqlParser::NumericLiteralPositiveContext *context) = 0;

    virtual antlrcpp::Any visitNumericLiteralNegative(SparqlParser::NumericLiteralNegativeContext *context) = 0;

    virtual antlrcpp::Any visitBooleanLiteral(SparqlParser::BooleanLiteralContext *context) = 0;

    virtual antlrcpp::Any visitString(SparqlParser::StringContext *context) = 0;

    virtual antlrcpp::Any visitIriRef(SparqlParser::IriRefContext *context) = 0;

    virtual antlrcpp::Any visitPrefixedName(SparqlParser::PrefixedNameContext *context) = 0;

    virtual antlrcpp::Any visitBlankNode(SparqlParser::BlankNodeContext *context) = 0;


};

}  // namespace tnt::store::sparql::detail
