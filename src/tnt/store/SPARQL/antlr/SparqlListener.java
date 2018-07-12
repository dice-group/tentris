// Generated from Sparql.g4 by ANTLR 4.5.3
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link SparqlParser}.
 */
public interface SparqlListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link SparqlParser#query}.
	 * @param ctx the parse tree
	 */
	void enterQuery(SparqlParser.QueryContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#query}.
	 * @param ctx the parse tree
	 */
	void exitQuery(SparqlParser.QueryContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#prologue}.
	 * @param ctx the parse tree
	 */
	void enterPrologue(SparqlParser.PrologueContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#prologue}.
	 * @param ctx the parse tree
	 */
	void exitPrologue(SparqlParser.PrologueContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#baseDecl}.
	 * @param ctx the parse tree
	 */
	void enterBaseDecl(SparqlParser.BaseDeclContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#baseDecl}.
	 * @param ctx the parse tree
	 */
	void exitBaseDecl(SparqlParser.BaseDeclContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#prefixDecl}.
	 * @param ctx the parse tree
	 */
	void enterPrefixDecl(SparqlParser.PrefixDeclContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#prefixDecl}.
	 * @param ctx the parse tree
	 */
	void exitPrefixDecl(SparqlParser.PrefixDeclContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#selectQuery}.
	 * @param ctx the parse tree
	 */
	void enterSelectQuery(SparqlParser.SelectQueryContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#selectQuery}.
	 * @param ctx the parse tree
	 */
	void exitSelectQuery(SparqlParser.SelectQueryContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#constructQuery}.
	 * @param ctx the parse tree
	 */
	void enterConstructQuery(SparqlParser.ConstructQueryContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#constructQuery}.
	 * @param ctx the parse tree
	 */
	void exitConstructQuery(SparqlParser.ConstructQueryContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#describeQuery}.
	 * @param ctx the parse tree
	 */
	void enterDescribeQuery(SparqlParser.DescribeQueryContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#describeQuery}.
	 * @param ctx the parse tree
	 */
	void exitDescribeQuery(SparqlParser.DescribeQueryContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#askQuery}.
	 * @param ctx the parse tree
	 */
	void enterAskQuery(SparqlParser.AskQueryContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#askQuery}.
	 * @param ctx the parse tree
	 */
	void exitAskQuery(SparqlParser.AskQueryContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#datasetClause}.
	 * @param ctx the parse tree
	 */
	void enterDatasetClause(SparqlParser.DatasetClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#datasetClause}.
	 * @param ctx the parse tree
	 */
	void exitDatasetClause(SparqlParser.DatasetClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#defaultGraphClause}.
	 * @param ctx the parse tree
	 */
	void enterDefaultGraphClause(SparqlParser.DefaultGraphClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#defaultGraphClause}.
	 * @param ctx the parse tree
	 */
	void exitDefaultGraphClause(SparqlParser.DefaultGraphClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#namedGraphClause}.
	 * @param ctx the parse tree
	 */
	void enterNamedGraphClause(SparqlParser.NamedGraphClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#namedGraphClause}.
	 * @param ctx the parse tree
	 */
	void exitNamedGraphClause(SparqlParser.NamedGraphClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#sourceSelector}.
	 * @param ctx the parse tree
	 */
	void enterSourceSelector(SparqlParser.SourceSelectorContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#sourceSelector}.
	 * @param ctx the parse tree
	 */
	void exitSourceSelector(SparqlParser.SourceSelectorContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#whereClause}.
	 * @param ctx the parse tree
	 */
	void enterWhereClause(SparqlParser.WhereClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#whereClause}.
	 * @param ctx the parse tree
	 */
	void exitWhereClause(SparqlParser.WhereClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#solutionModifier}.
	 * @param ctx the parse tree
	 */
	void enterSolutionModifier(SparqlParser.SolutionModifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#solutionModifier}.
	 * @param ctx the parse tree
	 */
	void exitSolutionModifier(SparqlParser.SolutionModifierContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#limitOffsetClauses}.
	 * @param ctx the parse tree
	 */
	void enterLimitOffsetClauses(SparqlParser.LimitOffsetClausesContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#limitOffsetClauses}.
	 * @param ctx the parse tree
	 */
	void exitLimitOffsetClauses(SparqlParser.LimitOffsetClausesContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#orderClause}.
	 * @param ctx the parse tree
	 */
	void enterOrderClause(SparqlParser.OrderClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#orderClause}.
	 * @param ctx the parse tree
	 */
	void exitOrderClause(SparqlParser.OrderClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#orderCondition}.
	 * @param ctx the parse tree
	 */
	void enterOrderCondition(SparqlParser.OrderConditionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#orderCondition}.
	 * @param ctx the parse tree
	 */
	void exitOrderCondition(SparqlParser.OrderConditionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#limitClause}.
	 * @param ctx the parse tree
	 */
	void enterLimitClause(SparqlParser.LimitClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#limitClause}.
	 * @param ctx the parse tree
	 */
	void exitLimitClause(SparqlParser.LimitClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#offsetClause}.
	 * @param ctx the parse tree
	 */
	void enterOffsetClause(SparqlParser.OffsetClauseContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#offsetClause}.
	 * @param ctx the parse tree
	 */
	void exitOffsetClause(SparqlParser.OffsetClauseContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#groupGraphPattern}.
	 * @param ctx the parse tree
	 */
	void enterGroupGraphPattern(SparqlParser.GroupGraphPatternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#groupGraphPattern}.
	 * @param ctx the parse tree
	 */
	void exitGroupGraphPattern(SparqlParser.GroupGraphPatternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#triplesBlock}.
	 * @param ctx the parse tree
	 */
	void enterTriplesBlock(SparqlParser.TriplesBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#triplesBlock}.
	 * @param ctx the parse tree
	 */
	void exitTriplesBlock(SparqlParser.TriplesBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#graphPatternNotTriples}.
	 * @param ctx the parse tree
	 */
	void enterGraphPatternNotTriples(SparqlParser.GraphPatternNotTriplesContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#graphPatternNotTriples}.
	 * @param ctx the parse tree
	 */
	void exitGraphPatternNotTriples(SparqlParser.GraphPatternNotTriplesContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#optionalGraphPattern}.
	 * @param ctx the parse tree
	 */
	void enterOptionalGraphPattern(SparqlParser.OptionalGraphPatternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#optionalGraphPattern}.
	 * @param ctx the parse tree
	 */
	void exitOptionalGraphPattern(SparqlParser.OptionalGraphPatternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#graphGraphPattern}.
	 * @param ctx the parse tree
	 */
	void enterGraphGraphPattern(SparqlParser.GraphGraphPatternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#graphGraphPattern}.
	 * @param ctx the parse tree
	 */
	void exitGraphGraphPattern(SparqlParser.GraphGraphPatternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#groupOrUnionGraphPattern}.
	 * @param ctx the parse tree
	 */
	void enterGroupOrUnionGraphPattern(SparqlParser.GroupOrUnionGraphPatternContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#groupOrUnionGraphPattern}.
	 * @param ctx the parse tree
	 */
	void exitGroupOrUnionGraphPattern(SparqlParser.GroupOrUnionGraphPatternContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#filter}.
	 * @param ctx the parse tree
	 */
	void enterFilter(SparqlParser.FilterContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#filter}.
	 * @param ctx the parse tree
	 */
	void exitFilter(SparqlParser.FilterContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#constraint}.
	 * @param ctx the parse tree
	 */
	void enterConstraint(SparqlParser.ConstraintContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#constraint}.
	 * @param ctx the parse tree
	 */
	void exitConstraint(SparqlParser.ConstraintContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#functionCall}.
	 * @param ctx the parse tree
	 */
	void enterFunctionCall(SparqlParser.FunctionCallContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#functionCall}.
	 * @param ctx the parse tree
	 */
	void exitFunctionCall(SparqlParser.FunctionCallContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#argList}.
	 * @param ctx the parse tree
	 */
	void enterArgList(SparqlParser.ArgListContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#argList}.
	 * @param ctx the parse tree
	 */
	void exitArgList(SparqlParser.ArgListContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#constructTemplate}.
	 * @param ctx the parse tree
	 */
	void enterConstructTemplate(SparqlParser.ConstructTemplateContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#constructTemplate}.
	 * @param ctx the parse tree
	 */
	void exitConstructTemplate(SparqlParser.ConstructTemplateContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#constructTriples}.
	 * @param ctx the parse tree
	 */
	void enterConstructTriples(SparqlParser.ConstructTriplesContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#constructTriples}.
	 * @param ctx the parse tree
	 */
	void exitConstructTriples(SparqlParser.ConstructTriplesContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#triplesSameSubject}.
	 * @param ctx the parse tree
	 */
	void enterTriplesSameSubject(SparqlParser.TriplesSameSubjectContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#triplesSameSubject}.
	 * @param ctx the parse tree
	 */
	void exitTriplesSameSubject(SparqlParser.TriplesSameSubjectContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#propertyListNotEmpty}.
	 * @param ctx the parse tree
	 */
	void enterPropertyListNotEmpty(SparqlParser.PropertyListNotEmptyContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#propertyListNotEmpty}.
	 * @param ctx the parse tree
	 */
	void exitPropertyListNotEmpty(SparqlParser.PropertyListNotEmptyContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#propertyList}.
	 * @param ctx the parse tree
	 */
	void enterPropertyList(SparqlParser.PropertyListContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#propertyList}.
	 * @param ctx the parse tree
	 */
	void exitPropertyList(SparqlParser.PropertyListContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#objectList}.
	 * @param ctx the parse tree
	 */
	void enterObjectList(SparqlParser.ObjectListContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#objectList}.
	 * @param ctx the parse tree
	 */
	void exitObjectList(SparqlParser.ObjectListContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#object}.
	 * @param ctx the parse tree
	 */
	void enterObject(SparqlParser.ObjectContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#object}.
	 * @param ctx the parse tree
	 */
	void exitObject(SparqlParser.ObjectContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#verb}.
	 * @param ctx the parse tree
	 */
	void enterVerb(SparqlParser.VerbContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#verb}.
	 * @param ctx the parse tree
	 */
	void exitVerb(SparqlParser.VerbContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#triplesNode}.
	 * @param ctx the parse tree
	 */
	void enterTriplesNode(SparqlParser.TriplesNodeContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#triplesNode}.
	 * @param ctx the parse tree
	 */
	void exitTriplesNode(SparqlParser.TriplesNodeContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#blankNodePropertyList}.
	 * @param ctx the parse tree
	 */
	void enterBlankNodePropertyList(SparqlParser.BlankNodePropertyListContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#blankNodePropertyList}.
	 * @param ctx the parse tree
	 */
	void exitBlankNodePropertyList(SparqlParser.BlankNodePropertyListContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#collection}.
	 * @param ctx the parse tree
	 */
	void enterCollection(SparqlParser.CollectionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#collection}.
	 * @param ctx the parse tree
	 */
	void exitCollection(SparqlParser.CollectionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#graphNode}.
	 * @param ctx the parse tree
	 */
	void enterGraphNode(SparqlParser.GraphNodeContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#graphNode}.
	 * @param ctx the parse tree
	 */
	void exitGraphNode(SparqlParser.GraphNodeContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#varOrTerm}.
	 * @param ctx the parse tree
	 */
	void enterVarOrTerm(SparqlParser.VarOrTermContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#varOrTerm}.
	 * @param ctx the parse tree
	 */
	void exitVarOrTerm(SparqlParser.VarOrTermContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#varOrIRIref}.
	 * @param ctx the parse tree
	 */
	void enterVarOrIRIref(SparqlParser.VarOrIRIrefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#varOrIRIref}.
	 * @param ctx the parse tree
	 */
	void exitVarOrIRIref(SparqlParser.VarOrIRIrefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#var}.
	 * @param ctx the parse tree
	 */
	void enterVar(SparqlParser.VarContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#var}.
	 * @param ctx the parse tree
	 */
	void exitVar(SparqlParser.VarContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#graphTerm}.
	 * @param ctx the parse tree
	 */
	void enterGraphTerm(SparqlParser.GraphTermContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#graphTerm}.
	 * @param ctx the parse tree
	 */
	void exitGraphTerm(SparqlParser.GraphTermContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterExpression(SparqlParser.ExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitExpression(SparqlParser.ExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#conditionalOrExpression}.
	 * @param ctx the parse tree
	 */
	void enterConditionalOrExpression(SparqlParser.ConditionalOrExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#conditionalOrExpression}.
	 * @param ctx the parse tree
	 */
	void exitConditionalOrExpression(SparqlParser.ConditionalOrExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#conditionalAndExpression}.
	 * @param ctx the parse tree
	 */
	void enterConditionalAndExpression(SparqlParser.ConditionalAndExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#conditionalAndExpression}.
	 * @param ctx the parse tree
	 */
	void exitConditionalAndExpression(SparqlParser.ConditionalAndExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#valueLogical}.
	 * @param ctx the parse tree
	 */
	void enterValueLogical(SparqlParser.ValueLogicalContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#valueLogical}.
	 * @param ctx the parse tree
	 */
	void exitValueLogical(SparqlParser.ValueLogicalContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#relationalExpression}.
	 * @param ctx the parse tree
	 */
	void enterRelationalExpression(SparqlParser.RelationalExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#relationalExpression}.
	 * @param ctx the parse tree
	 */
	void exitRelationalExpression(SparqlParser.RelationalExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#numericExpression}.
	 * @param ctx the parse tree
	 */
	void enterNumericExpression(SparqlParser.NumericExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#numericExpression}.
	 * @param ctx the parse tree
	 */
	void exitNumericExpression(SparqlParser.NumericExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#additiveExpression}.
	 * @param ctx the parse tree
	 */
	void enterAdditiveExpression(SparqlParser.AdditiveExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#additiveExpression}.
	 * @param ctx the parse tree
	 */
	void exitAdditiveExpression(SparqlParser.AdditiveExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#multiplicativeExpression}.
	 * @param ctx the parse tree
	 */
	void enterMultiplicativeExpression(SparqlParser.MultiplicativeExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#multiplicativeExpression}.
	 * @param ctx the parse tree
	 */
	void exitMultiplicativeExpression(SparqlParser.MultiplicativeExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#unaryExpression}.
	 * @param ctx the parse tree
	 */
	void enterUnaryExpression(SparqlParser.UnaryExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#unaryExpression}.
	 * @param ctx the parse tree
	 */
	void exitUnaryExpression(SparqlParser.UnaryExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#primaryExpression}.
	 * @param ctx the parse tree
	 */
	void enterPrimaryExpression(SparqlParser.PrimaryExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#primaryExpression}.
	 * @param ctx the parse tree
	 */
	void exitPrimaryExpression(SparqlParser.PrimaryExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#brackettedExpression}.
	 * @param ctx the parse tree
	 */
	void enterBrackettedExpression(SparqlParser.BrackettedExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#brackettedExpression}.
	 * @param ctx the parse tree
	 */
	void exitBrackettedExpression(SparqlParser.BrackettedExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#builtInCall}.
	 * @param ctx the parse tree
	 */
	void enterBuiltInCall(SparqlParser.BuiltInCallContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#builtInCall}.
	 * @param ctx the parse tree
	 */
	void exitBuiltInCall(SparqlParser.BuiltInCallContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#regexExpression}.
	 * @param ctx the parse tree
	 */
	void enterRegexExpression(SparqlParser.RegexExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#regexExpression}.
	 * @param ctx the parse tree
	 */
	void exitRegexExpression(SparqlParser.RegexExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#iriRefOrFunction}.
	 * @param ctx the parse tree
	 */
	void enterIriRefOrFunction(SparqlParser.IriRefOrFunctionContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#iriRefOrFunction}.
	 * @param ctx the parse tree
	 */
	void exitIriRefOrFunction(SparqlParser.IriRefOrFunctionContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#rdfLiteral}.
	 * @param ctx the parse tree
	 */
	void enterRdfLiteral(SparqlParser.RdfLiteralContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#rdfLiteral}.
	 * @param ctx the parse tree
	 */
	void exitRdfLiteral(SparqlParser.RdfLiteralContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#numericLiteral}.
	 * @param ctx the parse tree
	 */
	void enterNumericLiteral(SparqlParser.NumericLiteralContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#numericLiteral}.
	 * @param ctx the parse tree
	 */
	void exitNumericLiteral(SparqlParser.NumericLiteralContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#numericLiteralUnsigned}.
	 * @param ctx the parse tree
	 */
	void enterNumericLiteralUnsigned(SparqlParser.NumericLiteralUnsignedContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#numericLiteralUnsigned}.
	 * @param ctx the parse tree
	 */
	void exitNumericLiteralUnsigned(SparqlParser.NumericLiteralUnsignedContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#numericLiteralPositive}.
	 * @param ctx the parse tree
	 */
	void enterNumericLiteralPositive(SparqlParser.NumericLiteralPositiveContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#numericLiteralPositive}.
	 * @param ctx the parse tree
	 */
	void exitNumericLiteralPositive(SparqlParser.NumericLiteralPositiveContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#numericLiteralNegative}.
	 * @param ctx the parse tree
	 */
	void enterNumericLiteralNegative(SparqlParser.NumericLiteralNegativeContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#numericLiteralNegative}.
	 * @param ctx the parse tree
	 */
	void exitNumericLiteralNegative(SparqlParser.NumericLiteralNegativeContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#booleanLiteral}.
	 * @param ctx the parse tree
	 */
	void enterBooleanLiteral(SparqlParser.BooleanLiteralContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#booleanLiteral}.
	 * @param ctx the parse tree
	 */
	void exitBooleanLiteral(SparqlParser.BooleanLiteralContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#string}.
	 * @param ctx the parse tree
	 */
	void enterString(SparqlParser.StringContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#string}.
	 * @param ctx the parse tree
	 */
	void exitString(SparqlParser.StringContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#iriRef}.
	 * @param ctx the parse tree
	 */
	void enterIriRef(SparqlParser.IriRefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#iriRef}.
	 * @param ctx the parse tree
	 */
	void exitIriRef(SparqlParser.IriRefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#prefixedName}.
	 * @param ctx the parse tree
	 */
	void enterPrefixedName(SparqlParser.PrefixedNameContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#prefixedName}.
	 * @param ctx the parse tree
	 */
	void exitPrefixedName(SparqlParser.PrefixedNameContext ctx);
	/**
	 * Enter a parse tree produced by {@link SparqlParser#blankNode}.
	 * @param ctx the parse tree
	 */
	void enterBlankNode(SparqlParser.BlankNodeContext ctx);
	/**
	 * Exit a parse tree produced by {@link SparqlParser#blankNode}.
	 * @param ctx the parse tree
	 */
	void exitBlankNode(SparqlParser.BlankNodeContext ctx);
}