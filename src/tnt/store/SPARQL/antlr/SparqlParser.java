// Generated from Sparql.g4 by ANTLR 4.5.3
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class SparqlParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.5.3", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, T__27=28, T__28=29, T__29=30, T__30=31, 
		T__31=32, T__32=33, T__33=34, T__34=35, T__35=36, T__36=37, T__37=38, 
		T__38=39, T__39=40, T__40=41, T__41=42, T__42=43, T__43=44, T__44=45, 
		T__45=46, T__46=47, T__47=48, T__48=49, T__49=50, T__50=51, T__51=52, 
		T__52=53, T__53=54, T__54=55, T__55=56, T__56=57, T__57=58, IRI_REF=59, 
		PNAME_NS=60, PNAME_LN=61, BLANK_NODE_LABEL=62, VAR1=63, VAR2=64, LANGTAG=65, 
		INTEGER=66, DECIMAL=67, DOUBLE=68, INTEGER_POSITIVE=69, DECIMAL_POSITIVE=70, 
		DOUBLE_POSITIVE=71, INTEGER_NEGATIVE=72, DECIMAL_NEGATIVE=73, DOUBLE_NEGATIVE=74, 
		EXPONENT=75, STRING_LITERAL1=76, STRING_LITERAL2=77, STRING_LITERAL_LONG1=78, 
		STRING_LITERAL_LONG2=79, ECHAR=80, NIL=81, ANON=82, PN_CHARS_U=83, VARNAME=84, 
		PN_PREFIX=85, PN_LOCAL=86, WS=87;
	public static final int
		RULE_query = 0, RULE_prologue = 1, RULE_baseDecl = 2, RULE_prefixDecl = 3, 
		RULE_selectQuery = 4, RULE_constructQuery = 5, RULE_describeQuery = 6, 
		RULE_askQuery = 7, RULE_datasetClause = 8, RULE_defaultGraphClause = 9, 
		RULE_namedGraphClause = 10, RULE_sourceSelector = 11, RULE_whereClause = 12, 
		RULE_solutionModifier = 13, RULE_limitOffsetClauses = 14, RULE_orderClause = 15, 
		RULE_orderCondition = 16, RULE_limitClause = 17, RULE_offsetClause = 18, 
		RULE_groupGraphPattern = 19, RULE_triplesBlock = 20, RULE_graphPatternNotTriples = 21, 
		RULE_optionalGraphPattern = 22, RULE_graphGraphPattern = 23, RULE_groupOrUnionGraphPattern = 24, 
		RULE_filter = 25, RULE_constraint = 26, RULE_functionCall = 27, RULE_argList = 28, 
		RULE_constructTemplate = 29, RULE_constructTriples = 30, RULE_triplesSameSubject = 31, 
		RULE_propertyListNotEmpty = 32, RULE_propertyList = 33, RULE_objectList = 34, 
		RULE_object = 35, RULE_verb = 36, RULE_triplesNode = 37, RULE_blankNodePropertyList = 38, 
		RULE_collection = 39, RULE_graphNode = 40, RULE_varOrTerm = 41, RULE_varOrIRIref = 42, 
		RULE_var = 43, RULE_graphTerm = 44, RULE_expression = 45, RULE_conditionalOrExpression = 46, 
		RULE_conditionalAndExpression = 47, RULE_valueLogical = 48, RULE_relationalExpression = 49, 
		RULE_numericExpression = 50, RULE_additiveExpression = 51, RULE_multiplicativeExpression = 52, 
		RULE_unaryExpression = 53, RULE_primaryExpression = 54, RULE_brackettedExpression = 55, 
		RULE_builtInCall = 56, RULE_regexExpression = 57, RULE_iriRefOrFunction = 58, 
		RULE_rdfLiteral = 59, RULE_numericLiteral = 60, RULE_numericLiteralUnsigned = 61, 
		RULE_numericLiteralPositive = 62, RULE_numericLiteralNegative = 63, RULE_booleanLiteral = 64, 
		RULE_string = 65, RULE_iriRef = 66, RULE_prefixedName = 67, RULE_blankNode = 68;
	public static final String[] ruleNames = {
		"query", "prologue", "baseDecl", "prefixDecl", "selectQuery", "constructQuery", 
		"describeQuery", "askQuery", "datasetClause", "defaultGraphClause", "namedGraphClause", 
		"sourceSelector", "whereClause", "solutionModifier", "limitOffsetClauses", 
		"orderClause", "orderCondition", "limitClause", "offsetClause", "groupGraphPattern", 
		"triplesBlock", "graphPatternNotTriples", "optionalGraphPattern", "graphGraphPattern", 
		"groupOrUnionGraphPattern", "filter", "constraint", "functionCall", "argList", 
		"constructTemplate", "constructTriples", "triplesSameSubject", "propertyListNotEmpty", 
		"propertyList", "objectList", "object", "verb", "triplesNode", "blankNodePropertyList", 
		"collection", "graphNode", "varOrTerm", "varOrIRIref", "var", "graphTerm", 
		"expression", "conditionalOrExpression", "conditionalAndExpression", "valueLogical", 
		"relationalExpression", "numericExpression", "additiveExpression", "multiplicativeExpression", 
		"unaryExpression", "primaryExpression", "brackettedExpression", "builtInCall", 
		"regexExpression", "iriRefOrFunction", "rdfLiteral", "numericLiteral", 
		"numericLiteralUnsigned", "numericLiteralPositive", "numericLiteralNegative", 
		"booleanLiteral", "string", "iriRef", "prefixedName", "blankNode"
	};

	private static final String[] _LITERAL_NAMES = {
		null, "'BASE'", "'PREFIX'", "'SELECT'", "'DISTINCT'", "'REDUCED'", "'*'", 
		"'CONSTRUCT'", "'DESCRIBE'", "'ASK'", "'FROM'", "'NAMED'", "'WHERE'", 
		"'ORDER'", "'BY'", "'ASC'", "'DESC'", "'LIMIT'", "'OFFSET'", "'{'", "'.'", 
		"'}'", "'OPTIONAL'", "'GRAPH'", "'UNION'", "'FILTER'", "'('", "','", "')'", 
		"';'", "'a'", "'['", "']'", "'||'", "'&&'", "'='", "'!='", "'<'", "'>'", 
		"'<='", "'>='", "'+'", "'-'", "'/'", "'!'", "'STR'", "'LANG'", "'LANGMATCHES'", 
		"'DATATYPE'", "'BOUND'", "'sameTerm'", "'isIRI'", "'isURI'", "'isBLANK'", 
		"'isLITERAL'", "'REGEX'", "'^^'", "'true'", "'false'"
	};
	private static final String[] _SYMBOLIC_NAMES = {
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, null, null, null, null, null, null, null, null, null, "IRI_REF", 
		"PNAME_NS", "PNAME_LN", "BLANK_NODE_LABEL", "VAR1", "VAR2", "LANGTAG", 
		"INTEGER", "DECIMAL", "DOUBLE", "INTEGER_POSITIVE", "DECIMAL_POSITIVE", 
		"DOUBLE_POSITIVE", "INTEGER_NEGATIVE", "DECIMAL_NEGATIVE", "DOUBLE_NEGATIVE", 
		"EXPONENT", "STRING_LITERAL1", "STRING_LITERAL2", "STRING_LITERAL_LONG1", 
		"STRING_LITERAL_LONG2", "ECHAR", "NIL", "ANON", "PN_CHARS_U", "VARNAME", 
		"PN_PREFIX", "PN_LOCAL", "WS"
	};
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Sparql.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public SparqlParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}
	public static class QueryContext extends ParserRuleContext {
		public PrologueContext prologue() {
			return getRuleContext(PrologueContext.class,0);
		}
		public TerminalNode EOF() { return getToken(SparqlParser.EOF, 0); }
		public SelectQueryContext selectQuery() {
			return getRuleContext(SelectQueryContext.class,0);
		}
		public ConstructQueryContext constructQuery() {
			return getRuleContext(ConstructQueryContext.class,0);
		}
		public DescribeQueryContext describeQuery() {
			return getRuleContext(DescribeQueryContext.class,0);
		}
		public AskQueryContext askQuery() {
			return getRuleContext(AskQueryContext.class,0);
		}
		public QueryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_query; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterQuery(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitQuery(this);
		}
	}

	public final QueryContext query() throws RecognitionException {
		QueryContext _localctx = new QueryContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_query);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(138);
			prologue();
			setState(143);
			switch (_input.LA(1)) {
			case T__2:
				{
				setState(139);
				selectQuery();
				}
				break;
			case T__6:
				{
				setState(140);
				constructQuery();
				}
				break;
			case T__7:
				{
				setState(141);
				describeQuery();
				}
				break;
			case T__8:
				{
				setState(142);
				askQuery();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(145);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrologueContext extends ParserRuleContext {
		public BaseDeclContext baseDecl() {
			return getRuleContext(BaseDeclContext.class,0);
		}
		public List<PrefixDeclContext> prefixDecl() {
			return getRuleContexts(PrefixDeclContext.class);
		}
		public PrefixDeclContext prefixDecl(int i) {
			return getRuleContext(PrefixDeclContext.class,i);
		}
		public PrologueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prologue; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterPrologue(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitPrologue(this);
		}
	}

	public final PrologueContext prologue() throws RecognitionException {
		PrologueContext _localctx = new PrologueContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_prologue);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(148);
			_la = _input.LA(1);
			if (_la==T__0) {
				{
				setState(147);
				baseDecl();
				}
			}

			setState(153);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__1) {
				{
				{
				setState(150);
				prefixDecl();
				}
				}
				setState(155);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BaseDeclContext extends ParserRuleContext {
		public TerminalNode IRI_REF() { return getToken(SparqlParser.IRI_REF, 0); }
		public BaseDeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_baseDecl; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterBaseDecl(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitBaseDecl(this);
		}
	}

	public final BaseDeclContext baseDecl() throws RecognitionException {
		BaseDeclContext _localctx = new BaseDeclContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_baseDecl);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(156);
			match(T__0);
			setState(157);
			match(IRI_REF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrefixDeclContext extends ParserRuleContext {
		public TerminalNode PNAME_NS() { return getToken(SparqlParser.PNAME_NS, 0); }
		public TerminalNode IRI_REF() { return getToken(SparqlParser.IRI_REF, 0); }
		public PrefixDeclContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prefixDecl; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterPrefixDecl(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitPrefixDecl(this);
		}
	}

	public final PrefixDeclContext prefixDecl() throws RecognitionException {
		PrefixDeclContext _localctx = new PrefixDeclContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_prefixDecl);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(159);
			match(T__1);
			setState(160);
			match(PNAME_NS);
			setState(161);
			match(IRI_REF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SelectQueryContext extends ParserRuleContext {
		public WhereClauseContext whereClause() {
			return getRuleContext(WhereClauseContext.class,0);
		}
		public SolutionModifierContext solutionModifier() {
			return getRuleContext(SolutionModifierContext.class,0);
		}
		public List<DatasetClauseContext> datasetClause() {
			return getRuleContexts(DatasetClauseContext.class);
		}
		public DatasetClauseContext datasetClause(int i) {
			return getRuleContext(DatasetClauseContext.class,i);
		}
		public List<VarContext> var() {
			return getRuleContexts(VarContext.class);
		}
		public VarContext var(int i) {
			return getRuleContext(VarContext.class,i);
		}
		public SelectQueryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_selectQuery; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterSelectQuery(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitSelectQuery(this);
		}
	}

	public final SelectQueryContext selectQuery() throws RecognitionException {
		SelectQueryContext _localctx = new SelectQueryContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_selectQuery);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(163);
			match(T__2);
			setState(165);
			_la = _input.LA(1);
			if (_la==T__3 || _la==T__4) {
				{
				setState(164);
				_la = _input.LA(1);
				if ( !(_la==T__3 || _la==T__4) ) {
				_errHandler.recoverInline(this);
				} else {
					consume();
				}
				}
			}

			setState(173);
			switch (_input.LA(1)) {
			case VAR1:
			case VAR2:
				{
				setState(168); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(167);
					var();
					}
					}
					setState(170); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==VAR1 || _la==VAR2 );
				}
				break;
			case T__5:
				{
				setState(172);
				match(T__5);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(178);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__9) {
				{
				{
				setState(175);
				datasetClause();
				}
				}
				setState(180);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(181);
			whereClause();
			setState(182);
			solutionModifier();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstructQueryContext extends ParserRuleContext {
		public ConstructTemplateContext constructTemplate() {
			return getRuleContext(ConstructTemplateContext.class,0);
		}
		public WhereClauseContext whereClause() {
			return getRuleContext(WhereClauseContext.class,0);
		}
		public SolutionModifierContext solutionModifier() {
			return getRuleContext(SolutionModifierContext.class,0);
		}
		public List<DatasetClauseContext> datasetClause() {
			return getRuleContexts(DatasetClauseContext.class);
		}
		public DatasetClauseContext datasetClause(int i) {
			return getRuleContext(DatasetClauseContext.class,i);
		}
		public ConstructQueryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constructQuery; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterConstructQuery(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitConstructQuery(this);
		}
	}

	public final ConstructQueryContext constructQuery() throws RecognitionException {
		ConstructQueryContext _localctx = new ConstructQueryContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_constructQuery);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(184);
			match(T__6);
			setState(185);
			constructTemplate();
			setState(189);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__9) {
				{
				{
				setState(186);
				datasetClause();
				}
				}
				setState(191);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(192);
			whereClause();
			setState(193);
			solutionModifier();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DescribeQueryContext extends ParserRuleContext {
		public SolutionModifierContext solutionModifier() {
			return getRuleContext(SolutionModifierContext.class,0);
		}
		public List<DatasetClauseContext> datasetClause() {
			return getRuleContexts(DatasetClauseContext.class);
		}
		public DatasetClauseContext datasetClause(int i) {
			return getRuleContext(DatasetClauseContext.class,i);
		}
		public WhereClauseContext whereClause() {
			return getRuleContext(WhereClauseContext.class,0);
		}
		public List<VarOrIRIrefContext> varOrIRIref() {
			return getRuleContexts(VarOrIRIrefContext.class);
		}
		public VarOrIRIrefContext varOrIRIref(int i) {
			return getRuleContext(VarOrIRIrefContext.class,i);
		}
		public DescribeQueryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_describeQuery; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterDescribeQuery(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitDescribeQuery(this);
		}
	}

	public final DescribeQueryContext describeQuery() throws RecognitionException {
		DescribeQueryContext _localctx = new DescribeQueryContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_describeQuery);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(195);
			match(T__7);
			setState(202);
			switch (_input.LA(1)) {
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case VAR1:
			case VAR2:
				{
				setState(197); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(196);
					varOrIRIref();
					}
					}
					setState(199); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( ((((_la - 59)) & ~0x3f) == 0 && ((1L << (_la - 59)) & ((1L << (IRI_REF - 59)) | (1L << (PNAME_NS - 59)) | (1L << (PNAME_LN - 59)) | (1L << (VAR1 - 59)) | (1L << (VAR2 - 59)))) != 0) );
				}
				break;
			case T__5:
				{
				setState(201);
				match(T__5);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(207);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__9) {
				{
				{
				setState(204);
				datasetClause();
				}
				}
				setState(209);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(211);
			_la = _input.LA(1);
			if (_la==T__11 || _la==T__18) {
				{
				setState(210);
				whereClause();
				}
			}

			setState(213);
			solutionModifier();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AskQueryContext extends ParserRuleContext {
		public WhereClauseContext whereClause() {
			return getRuleContext(WhereClauseContext.class,0);
		}
		public List<DatasetClauseContext> datasetClause() {
			return getRuleContexts(DatasetClauseContext.class);
		}
		public DatasetClauseContext datasetClause(int i) {
			return getRuleContext(DatasetClauseContext.class,i);
		}
		public AskQueryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_askQuery; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterAskQuery(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitAskQuery(this);
		}
	}

	public final AskQueryContext askQuery() throws RecognitionException {
		AskQueryContext _localctx = new AskQueryContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_askQuery);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(215);
			match(T__8);
			setState(219);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__9) {
				{
				{
				setState(216);
				datasetClause();
				}
				}
				setState(221);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(222);
			whereClause();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DatasetClauseContext extends ParserRuleContext {
		public DefaultGraphClauseContext defaultGraphClause() {
			return getRuleContext(DefaultGraphClauseContext.class,0);
		}
		public NamedGraphClauseContext namedGraphClause() {
			return getRuleContext(NamedGraphClauseContext.class,0);
		}
		public DatasetClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_datasetClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterDatasetClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitDatasetClause(this);
		}
	}

	public final DatasetClauseContext datasetClause() throws RecognitionException {
		DatasetClauseContext _localctx = new DatasetClauseContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_datasetClause);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(224);
			match(T__9);
			setState(227);
			switch (_input.LA(1)) {
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
				{
				setState(225);
				defaultGraphClause();
				}
				break;
			case T__10:
				{
				setState(226);
				namedGraphClause();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DefaultGraphClauseContext extends ParserRuleContext {
		public SourceSelectorContext sourceSelector() {
			return getRuleContext(SourceSelectorContext.class,0);
		}
		public DefaultGraphClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_defaultGraphClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterDefaultGraphClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitDefaultGraphClause(this);
		}
	}

	public final DefaultGraphClauseContext defaultGraphClause() throws RecognitionException {
		DefaultGraphClauseContext _localctx = new DefaultGraphClauseContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_defaultGraphClause);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(229);
			sourceSelector();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NamedGraphClauseContext extends ParserRuleContext {
		public SourceSelectorContext sourceSelector() {
			return getRuleContext(SourceSelectorContext.class,0);
		}
		public NamedGraphClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_namedGraphClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterNamedGraphClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitNamedGraphClause(this);
		}
	}

	public final NamedGraphClauseContext namedGraphClause() throws RecognitionException {
		NamedGraphClauseContext _localctx = new NamedGraphClauseContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_namedGraphClause);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(231);
			match(T__10);
			setState(232);
			sourceSelector();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SourceSelectorContext extends ParserRuleContext {
		public IriRefContext iriRef() {
			return getRuleContext(IriRefContext.class,0);
		}
		public SourceSelectorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_sourceSelector; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterSourceSelector(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitSourceSelector(this);
		}
	}

	public final SourceSelectorContext sourceSelector() throws RecognitionException {
		SourceSelectorContext _localctx = new SourceSelectorContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_sourceSelector);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(234);
			iriRef();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class WhereClauseContext extends ParserRuleContext {
		public GroupGraphPatternContext groupGraphPattern() {
			return getRuleContext(GroupGraphPatternContext.class,0);
		}
		public WhereClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_whereClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterWhereClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitWhereClause(this);
		}
	}

	public final WhereClauseContext whereClause() throws RecognitionException {
		WhereClauseContext _localctx = new WhereClauseContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_whereClause);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(237);
			_la = _input.LA(1);
			if (_la==T__11) {
				{
				setState(236);
				match(T__11);
				}
			}

			setState(239);
			groupGraphPattern();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SolutionModifierContext extends ParserRuleContext {
		public OrderClauseContext orderClause() {
			return getRuleContext(OrderClauseContext.class,0);
		}
		public LimitOffsetClausesContext limitOffsetClauses() {
			return getRuleContext(LimitOffsetClausesContext.class,0);
		}
		public SolutionModifierContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_solutionModifier; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterSolutionModifier(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitSolutionModifier(this);
		}
	}

	public final SolutionModifierContext solutionModifier() throws RecognitionException {
		SolutionModifierContext _localctx = new SolutionModifierContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_solutionModifier);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(242);
			_la = _input.LA(1);
			if (_la==T__12) {
				{
				setState(241);
				orderClause();
				}
			}

			setState(245);
			_la = _input.LA(1);
			if (_la==T__16 || _la==T__17) {
				{
				setState(244);
				limitOffsetClauses();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LimitOffsetClausesContext extends ParserRuleContext {
		public LimitClauseContext limitClause() {
			return getRuleContext(LimitClauseContext.class,0);
		}
		public OffsetClauseContext offsetClause() {
			return getRuleContext(OffsetClauseContext.class,0);
		}
		public LimitOffsetClausesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_limitOffsetClauses; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterLimitOffsetClauses(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitLimitOffsetClauses(this);
		}
	}

	public final LimitOffsetClausesContext limitOffsetClauses() throws RecognitionException {
		LimitOffsetClausesContext _localctx = new LimitOffsetClausesContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_limitOffsetClauses);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(255);
			switch (_input.LA(1)) {
			case T__16:
				{
				setState(247);
				limitClause();
				setState(249);
				_la = _input.LA(1);
				if (_la==T__17) {
					{
					setState(248);
					offsetClause();
					}
				}

				}
				break;
			case T__17:
				{
				setState(251);
				offsetClause();
				setState(253);
				_la = _input.LA(1);
				if (_la==T__16) {
					{
					setState(252);
					limitClause();
					}
				}

				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OrderClauseContext extends ParserRuleContext {
		public List<OrderConditionContext> orderCondition() {
			return getRuleContexts(OrderConditionContext.class);
		}
		public OrderConditionContext orderCondition(int i) {
			return getRuleContext(OrderConditionContext.class,i);
		}
		public OrderClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_orderClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterOrderClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitOrderClause(this);
		}
	}

	public final OrderClauseContext orderClause() throws RecognitionException {
		OrderClauseContext _localctx = new OrderClauseContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_orderClause);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(257);
			match(T__12);
			setState(258);
			match(T__13);
			setState(260); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(259);
				orderCondition();
				}
				}
				setState(262); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( ((((_la - 15)) & ~0x3f) == 0 && ((1L << (_la - 15)) & ((1L << (T__14 - 15)) | (1L << (T__15 - 15)) | (1L << (T__25 - 15)) | (1L << (T__44 - 15)) | (1L << (T__45 - 15)) | (1L << (T__46 - 15)) | (1L << (T__47 - 15)) | (1L << (T__48 - 15)) | (1L << (T__49 - 15)) | (1L << (T__50 - 15)) | (1L << (T__51 - 15)) | (1L << (T__52 - 15)) | (1L << (T__53 - 15)) | (1L << (T__54 - 15)) | (1L << (IRI_REF - 15)) | (1L << (PNAME_NS - 15)) | (1L << (PNAME_LN - 15)) | (1L << (VAR1 - 15)) | (1L << (VAR2 - 15)))) != 0) );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OrderConditionContext extends ParserRuleContext {
		public BrackettedExpressionContext brackettedExpression() {
			return getRuleContext(BrackettedExpressionContext.class,0);
		}
		public ConstraintContext constraint() {
			return getRuleContext(ConstraintContext.class,0);
		}
		public VarContext var() {
			return getRuleContext(VarContext.class,0);
		}
		public OrderConditionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_orderCondition; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterOrderCondition(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitOrderCondition(this);
		}
	}

	public final OrderConditionContext orderCondition() throws RecognitionException {
		OrderConditionContext _localctx = new OrderConditionContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_orderCondition);
		int _la;
		try {
			setState(270);
			switch (_input.LA(1)) {
			case T__14:
			case T__15:
				enterOuterAlt(_localctx, 1);
				{
				{
				setState(264);
				_la = _input.LA(1);
				if ( !(_la==T__14 || _la==T__15) ) {
				_errHandler.recoverInline(this);
				} else {
					consume();
				}
				setState(265);
				brackettedExpression();
				}
				}
				break;
			case T__25:
			case T__44:
			case T__45:
			case T__46:
			case T__47:
			case T__48:
			case T__49:
			case T__50:
			case T__51:
			case T__52:
			case T__53:
			case T__54:
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case VAR1:
			case VAR2:
				enterOuterAlt(_localctx, 2);
				{
				setState(268);
				switch (_input.LA(1)) {
				case T__25:
				case T__44:
				case T__45:
				case T__46:
				case T__47:
				case T__48:
				case T__49:
				case T__50:
				case T__51:
				case T__52:
				case T__53:
				case T__54:
				case IRI_REF:
				case PNAME_NS:
				case PNAME_LN:
					{
					setState(266);
					constraint();
					}
					break;
				case VAR1:
				case VAR2:
					{
					setState(267);
					var();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LimitClauseContext extends ParserRuleContext {
		public TerminalNode INTEGER() { return getToken(SparqlParser.INTEGER, 0); }
		public LimitClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_limitClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterLimitClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitLimitClause(this);
		}
	}

	public final LimitClauseContext limitClause() throws RecognitionException {
		LimitClauseContext _localctx = new LimitClauseContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_limitClause);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(272);
			match(T__16);
			setState(273);
			match(INTEGER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OffsetClauseContext extends ParserRuleContext {
		public TerminalNode INTEGER() { return getToken(SparqlParser.INTEGER, 0); }
		public OffsetClauseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_offsetClause; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterOffsetClause(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitOffsetClause(this);
		}
	}

	public final OffsetClauseContext offsetClause() throws RecognitionException {
		OffsetClauseContext _localctx = new OffsetClauseContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_offsetClause);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(275);
			match(T__17);
			setState(276);
			match(INTEGER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GroupGraphPatternContext extends ParserRuleContext {
		public List<TriplesBlockContext> triplesBlock() {
			return getRuleContexts(TriplesBlockContext.class);
		}
		public TriplesBlockContext triplesBlock(int i) {
			return getRuleContext(TriplesBlockContext.class,i);
		}
		public List<GraphPatternNotTriplesContext> graphPatternNotTriples() {
			return getRuleContexts(GraphPatternNotTriplesContext.class);
		}
		public GraphPatternNotTriplesContext graphPatternNotTriples(int i) {
			return getRuleContext(GraphPatternNotTriplesContext.class,i);
		}
		public List<FilterContext> filter() {
			return getRuleContexts(FilterContext.class);
		}
		public FilterContext filter(int i) {
			return getRuleContext(FilterContext.class,i);
		}
		public GroupGraphPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_groupGraphPattern; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterGroupGraphPattern(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitGroupGraphPattern(this);
		}
	}

	public final GroupGraphPatternContext groupGraphPattern() throws RecognitionException {
		GroupGraphPatternContext _localctx = new GroupGraphPatternContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_groupGraphPattern);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(278);
			match(T__18);
			setState(280);
			_la = _input.LA(1);
			if (((((_la - 26)) & ~0x3f) == 0 && ((1L << (_la - 26)) & ((1L << (T__25 - 26)) | (1L << (T__30 - 26)) | (1L << (T__56 - 26)) | (1L << (T__57 - 26)) | (1L << (IRI_REF - 26)) | (1L << (PNAME_NS - 26)) | (1L << (PNAME_LN - 26)) | (1L << (BLANK_NODE_LABEL - 26)) | (1L << (VAR1 - 26)) | (1L << (VAR2 - 26)) | (1L << (INTEGER - 26)) | (1L << (DECIMAL - 26)) | (1L << (DOUBLE - 26)) | (1L << (INTEGER_POSITIVE - 26)) | (1L << (DECIMAL_POSITIVE - 26)) | (1L << (DOUBLE_POSITIVE - 26)) | (1L << (INTEGER_NEGATIVE - 26)) | (1L << (DECIMAL_NEGATIVE - 26)) | (1L << (DOUBLE_NEGATIVE - 26)) | (1L << (STRING_LITERAL1 - 26)) | (1L << (STRING_LITERAL2 - 26)) | (1L << (NIL - 26)) | (1L << (ANON - 26)))) != 0)) {
				{
				setState(279);
				triplesBlock();
				}
			}

			setState(294);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__18) | (1L << T__21) | (1L << T__22) | (1L << T__24))) != 0)) {
				{
				{
				setState(284);
				switch (_input.LA(1)) {
				case T__18:
				case T__21:
				case T__22:
					{
					setState(282);
					graphPatternNotTriples();
					}
					break;
				case T__24:
					{
					setState(283);
					filter();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(287);
				_la = _input.LA(1);
				if (_la==T__19) {
					{
					setState(286);
					match(T__19);
					}
				}

				setState(290);
				_la = _input.LA(1);
				if (((((_la - 26)) & ~0x3f) == 0 && ((1L << (_la - 26)) & ((1L << (T__25 - 26)) | (1L << (T__30 - 26)) | (1L << (T__56 - 26)) | (1L << (T__57 - 26)) | (1L << (IRI_REF - 26)) | (1L << (PNAME_NS - 26)) | (1L << (PNAME_LN - 26)) | (1L << (BLANK_NODE_LABEL - 26)) | (1L << (VAR1 - 26)) | (1L << (VAR2 - 26)) | (1L << (INTEGER - 26)) | (1L << (DECIMAL - 26)) | (1L << (DOUBLE - 26)) | (1L << (INTEGER_POSITIVE - 26)) | (1L << (DECIMAL_POSITIVE - 26)) | (1L << (DOUBLE_POSITIVE - 26)) | (1L << (INTEGER_NEGATIVE - 26)) | (1L << (DECIMAL_NEGATIVE - 26)) | (1L << (DOUBLE_NEGATIVE - 26)) | (1L << (STRING_LITERAL1 - 26)) | (1L << (STRING_LITERAL2 - 26)) | (1L << (NIL - 26)) | (1L << (ANON - 26)))) != 0)) {
					{
					setState(289);
					triplesBlock();
					}
				}

				}
				}
				setState(296);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(297);
			match(T__20);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TriplesBlockContext extends ParserRuleContext {
		public TriplesSameSubjectContext triplesSameSubject() {
			return getRuleContext(TriplesSameSubjectContext.class,0);
		}
		public TriplesBlockContext triplesBlock() {
			return getRuleContext(TriplesBlockContext.class,0);
		}
		public TriplesBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_triplesBlock; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterTriplesBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitTriplesBlock(this);
		}
	}

	public final TriplesBlockContext triplesBlock() throws RecognitionException {
		TriplesBlockContext _localctx = new TriplesBlockContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_triplesBlock);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(299);
			triplesSameSubject();
			setState(304);
			_la = _input.LA(1);
			if (_la==T__19) {
				{
				setState(300);
				match(T__19);
				setState(302);
				_la = _input.LA(1);
				if (((((_la - 26)) & ~0x3f) == 0 && ((1L << (_la - 26)) & ((1L << (T__25 - 26)) | (1L << (T__30 - 26)) | (1L << (T__56 - 26)) | (1L << (T__57 - 26)) | (1L << (IRI_REF - 26)) | (1L << (PNAME_NS - 26)) | (1L << (PNAME_LN - 26)) | (1L << (BLANK_NODE_LABEL - 26)) | (1L << (VAR1 - 26)) | (1L << (VAR2 - 26)) | (1L << (INTEGER - 26)) | (1L << (DECIMAL - 26)) | (1L << (DOUBLE - 26)) | (1L << (INTEGER_POSITIVE - 26)) | (1L << (DECIMAL_POSITIVE - 26)) | (1L << (DOUBLE_POSITIVE - 26)) | (1L << (INTEGER_NEGATIVE - 26)) | (1L << (DECIMAL_NEGATIVE - 26)) | (1L << (DOUBLE_NEGATIVE - 26)) | (1L << (STRING_LITERAL1 - 26)) | (1L << (STRING_LITERAL2 - 26)) | (1L << (NIL - 26)) | (1L << (ANON - 26)))) != 0)) {
					{
					setState(301);
					triplesBlock();
					}
				}

				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GraphPatternNotTriplesContext extends ParserRuleContext {
		public OptionalGraphPatternContext optionalGraphPattern() {
			return getRuleContext(OptionalGraphPatternContext.class,0);
		}
		public GroupOrUnionGraphPatternContext groupOrUnionGraphPattern() {
			return getRuleContext(GroupOrUnionGraphPatternContext.class,0);
		}
		public GraphGraphPatternContext graphGraphPattern() {
			return getRuleContext(GraphGraphPatternContext.class,0);
		}
		public GraphPatternNotTriplesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_graphPatternNotTriples; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterGraphPatternNotTriples(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitGraphPatternNotTriples(this);
		}
	}

	public final GraphPatternNotTriplesContext graphPatternNotTriples() throws RecognitionException {
		GraphPatternNotTriplesContext _localctx = new GraphPatternNotTriplesContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_graphPatternNotTriples);
		try {
			setState(309);
			switch (_input.LA(1)) {
			case T__21:
				enterOuterAlt(_localctx, 1);
				{
				setState(306);
				optionalGraphPattern();
				}
				break;
			case T__18:
				enterOuterAlt(_localctx, 2);
				{
				setState(307);
				groupOrUnionGraphPattern();
				}
				break;
			case T__22:
				enterOuterAlt(_localctx, 3);
				{
				setState(308);
				graphGraphPattern();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OptionalGraphPatternContext extends ParserRuleContext {
		public GroupGraphPatternContext groupGraphPattern() {
			return getRuleContext(GroupGraphPatternContext.class,0);
		}
		public OptionalGraphPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_optionalGraphPattern; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterOptionalGraphPattern(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitOptionalGraphPattern(this);
		}
	}

	public final OptionalGraphPatternContext optionalGraphPattern() throws RecognitionException {
		OptionalGraphPatternContext _localctx = new OptionalGraphPatternContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_optionalGraphPattern);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(311);
			match(T__21);
			setState(312);
			groupGraphPattern();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GraphGraphPatternContext extends ParserRuleContext {
		public VarOrIRIrefContext varOrIRIref() {
			return getRuleContext(VarOrIRIrefContext.class,0);
		}
		public GroupGraphPatternContext groupGraphPattern() {
			return getRuleContext(GroupGraphPatternContext.class,0);
		}
		public GraphGraphPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_graphGraphPattern; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterGraphGraphPattern(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitGraphGraphPattern(this);
		}
	}

	public final GraphGraphPatternContext graphGraphPattern() throws RecognitionException {
		GraphGraphPatternContext _localctx = new GraphGraphPatternContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_graphGraphPattern);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(314);
			match(T__22);
			setState(315);
			varOrIRIref();
			setState(316);
			groupGraphPattern();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GroupOrUnionGraphPatternContext extends ParserRuleContext {
		public List<GroupGraphPatternContext> groupGraphPattern() {
			return getRuleContexts(GroupGraphPatternContext.class);
		}
		public GroupGraphPatternContext groupGraphPattern(int i) {
			return getRuleContext(GroupGraphPatternContext.class,i);
		}
		public GroupOrUnionGraphPatternContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_groupOrUnionGraphPattern; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterGroupOrUnionGraphPattern(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitGroupOrUnionGraphPattern(this);
		}
	}

	public final GroupOrUnionGraphPatternContext groupOrUnionGraphPattern() throws RecognitionException {
		GroupOrUnionGraphPatternContext _localctx = new GroupOrUnionGraphPatternContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_groupOrUnionGraphPattern);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(318);
			groupGraphPattern();
			setState(323);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__23) {
				{
				{
				setState(319);
				match(T__23);
				setState(320);
				groupGraphPattern();
				}
				}
				setState(325);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FilterContext extends ParserRuleContext {
		public ConstraintContext constraint() {
			return getRuleContext(ConstraintContext.class,0);
		}
		public FilterContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_filter; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterFilter(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitFilter(this);
		}
	}

	public final FilterContext filter() throws RecognitionException {
		FilterContext _localctx = new FilterContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_filter);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(326);
			match(T__24);
			setState(327);
			constraint();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstraintContext extends ParserRuleContext {
		public BrackettedExpressionContext brackettedExpression() {
			return getRuleContext(BrackettedExpressionContext.class,0);
		}
		public BuiltInCallContext builtInCall() {
			return getRuleContext(BuiltInCallContext.class,0);
		}
		public FunctionCallContext functionCall() {
			return getRuleContext(FunctionCallContext.class,0);
		}
		public ConstraintContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constraint; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterConstraint(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitConstraint(this);
		}
	}

	public final ConstraintContext constraint() throws RecognitionException {
		ConstraintContext _localctx = new ConstraintContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_constraint);
		try {
			setState(332);
			switch (_input.LA(1)) {
			case T__25:
				enterOuterAlt(_localctx, 1);
				{
				setState(329);
				brackettedExpression();
				}
				break;
			case T__44:
			case T__45:
			case T__46:
			case T__47:
			case T__48:
			case T__49:
			case T__50:
			case T__51:
			case T__52:
			case T__53:
			case T__54:
				enterOuterAlt(_localctx, 2);
				{
				setState(330);
				builtInCall();
				}
				break;
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
				enterOuterAlt(_localctx, 3);
				{
				setState(331);
				functionCall();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FunctionCallContext extends ParserRuleContext {
		public IriRefContext iriRef() {
			return getRuleContext(IriRefContext.class,0);
		}
		public ArgListContext argList() {
			return getRuleContext(ArgListContext.class,0);
		}
		public FunctionCallContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionCall; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterFunctionCall(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitFunctionCall(this);
		}
	}

	public final FunctionCallContext functionCall() throws RecognitionException {
		FunctionCallContext _localctx = new FunctionCallContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_functionCall);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(334);
			iriRef();
			setState(335);
			argList();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ArgListContext extends ParserRuleContext {
		public TerminalNode NIL() { return getToken(SparqlParser.NIL, 0); }
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public ArgListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_argList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterArgList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitArgList(this);
		}
	}

	public final ArgListContext argList() throws RecognitionException {
		ArgListContext _localctx = new ArgListContext(_ctx, getState());
		enterRule(_localctx, 56, RULE_argList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(349);
			switch (_input.LA(1)) {
			case NIL:
				{
				setState(337);
				match(NIL);
				}
				break;
			case T__25:
				{
				setState(338);
				match(T__25);
				setState(339);
				expression();
				setState(344);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__26) {
					{
					{
					setState(340);
					match(T__26);
					setState(341);
					expression();
					}
					}
					setState(346);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(347);
				match(T__27);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstructTemplateContext extends ParserRuleContext {
		public ConstructTriplesContext constructTriples() {
			return getRuleContext(ConstructTriplesContext.class,0);
		}
		public ConstructTemplateContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constructTemplate; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterConstructTemplate(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitConstructTemplate(this);
		}
	}

	public final ConstructTemplateContext constructTemplate() throws RecognitionException {
		ConstructTemplateContext _localctx = new ConstructTemplateContext(_ctx, getState());
		enterRule(_localctx, 58, RULE_constructTemplate);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(351);
			match(T__18);
			setState(353);
			_la = _input.LA(1);
			if (((((_la - 26)) & ~0x3f) == 0 && ((1L << (_la - 26)) & ((1L << (T__25 - 26)) | (1L << (T__30 - 26)) | (1L << (T__56 - 26)) | (1L << (T__57 - 26)) | (1L << (IRI_REF - 26)) | (1L << (PNAME_NS - 26)) | (1L << (PNAME_LN - 26)) | (1L << (BLANK_NODE_LABEL - 26)) | (1L << (VAR1 - 26)) | (1L << (VAR2 - 26)) | (1L << (INTEGER - 26)) | (1L << (DECIMAL - 26)) | (1L << (DOUBLE - 26)) | (1L << (INTEGER_POSITIVE - 26)) | (1L << (DECIMAL_POSITIVE - 26)) | (1L << (DOUBLE_POSITIVE - 26)) | (1L << (INTEGER_NEGATIVE - 26)) | (1L << (DECIMAL_NEGATIVE - 26)) | (1L << (DOUBLE_NEGATIVE - 26)) | (1L << (STRING_LITERAL1 - 26)) | (1L << (STRING_LITERAL2 - 26)) | (1L << (NIL - 26)) | (1L << (ANON - 26)))) != 0)) {
				{
				setState(352);
				constructTriples();
				}
			}

			setState(355);
			match(T__20);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstructTriplesContext extends ParserRuleContext {
		public TriplesSameSubjectContext triplesSameSubject() {
			return getRuleContext(TriplesSameSubjectContext.class,0);
		}
		public ConstructTriplesContext constructTriples() {
			return getRuleContext(ConstructTriplesContext.class,0);
		}
		public ConstructTriplesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constructTriples; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterConstructTriples(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitConstructTriples(this);
		}
	}

	public final ConstructTriplesContext constructTriples() throws RecognitionException {
		ConstructTriplesContext _localctx = new ConstructTriplesContext(_ctx, getState());
		enterRule(_localctx, 60, RULE_constructTriples);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(357);
			triplesSameSubject();
			setState(362);
			_la = _input.LA(1);
			if (_la==T__19) {
				{
				setState(358);
				match(T__19);
				setState(360);
				_la = _input.LA(1);
				if (((((_la - 26)) & ~0x3f) == 0 && ((1L << (_la - 26)) & ((1L << (T__25 - 26)) | (1L << (T__30 - 26)) | (1L << (T__56 - 26)) | (1L << (T__57 - 26)) | (1L << (IRI_REF - 26)) | (1L << (PNAME_NS - 26)) | (1L << (PNAME_LN - 26)) | (1L << (BLANK_NODE_LABEL - 26)) | (1L << (VAR1 - 26)) | (1L << (VAR2 - 26)) | (1L << (INTEGER - 26)) | (1L << (DECIMAL - 26)) | (1L << (DOUBLE - 26)) | (1L << (INTEGER_POSITIVE - 26)) | (1L << (DECIMAL_POSITIVE - 26)) | (1L << (DOUBLE_POSITIVE - 26)) | (1L << (INTEGER_NEGATIVE - 26)) | (1L << (DECIMAL_NEGATIVE - 26)) | (1L << (DOUBLE_NEGATIVE - 26)) | (1L << (STRING_LITERAL1 - 26)) | (1L << (STRING_LITERAL2 - 26)) | (1L << (NIL - 26)) | (1L << (ANON - 26)))) != 0)) {
					{
					setState(359);
					constructTriples();
					}
				}

				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TriplesSameSubjectContext extends ParserRuleContext {
		public VarOrTermContext varOrTerm() {
			return getRuleContext(VarOrTermContext.class,0);
		}
		public PropertyListNotEmptyContext propertyListNotEmpty() {
			return getRuleContext(PropertyListNotEmptyContext.class,0);
		}
		public TriplesNodeContext triplesNode() {
			return getRuleContext(TriplesNodeContext.class,0);
		}
		public PropertyListContext propertyList() {
			return getRuleContext(PropertyListContext.class,0);
		}
		public TriplesSameSubjectContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_triplesSameSubject; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterTriplesSameSubject(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitTriplesSameSubject(this);
		}
	}

	public final TriplesSameSubjectContext triplesSameSubject() throws RecognitionException {
		TriplesSameSubjectContext _localctx = new TriplesSameSubjectContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_triplesSameSubject);
		try {
			setState(370);
			switch (_input.LA(1)) {
			case T__56:
			case T__57:
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case BLANK_NODE_LABEL:
			case VAR1:
			case VAR2:
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
			case STRING_LITERAL1:
			case STRING_LITERAL2:
			case NIL:
			case ANON:
				enterOuterAlt(_localctx, 1);
				{
				setState(364);
				varOrTerm();
				setState(365);
				propertyListNotEmpty();
				}
				break;
			case T__25:
			case T__30:
				enterOuterAlt(_localctx, 2);
				{
				setState(367);
				triplesNode();
				setState(368);
				propertyList();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PropertyListNotEmptyContext extends ParserRuleContext {
		public List<VerbContext> verb() {
			return getRuleContexts(VerbContext.class);
		}
		public VerbContext verb(int i) {
			return getRuleContext(VerbContext.class,i);
		}
		public List<ObjectListContext> objectList() {
			return getRuleContexts(ObjectListContext.class);
		}
		public ObjectListContext objectList(int i) {
			return getRuleContext(ObjectListContext.class,i);
		}
		public PropertyListNotEmptyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_propertyListNotEmpty; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterPropertyListNotEmpty(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitPropertyListNotEmpty(this);
		}
	}

	public final PropertyListNotEmptyContext propertyListNotEmpty() throws RecognitionException {
		PropertyListNotEmptyContext _localctx = new PropertyListNotEmptyContext(_ctx, getState());
		enterRule(_localctx, 64, RULE_propertyListNotEmpty);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(372);
			verb();
			setState(373);
			objectList();
			setState(382);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__28) {
				{
				{
				setState(374);
				match(T__28);
				setState(378);
				_la = _input.LA(1);
				if (((((_la - 30)) & ~0x3f) == 0 && ((1L << (_la - 30)) & ((1L << (T__29 - 30)) | (1L << (IRI_REF - 30)) | (1L << (PNAME_NS - 30)) | (1L << (PNAME_LN - 30)) | (1L << (VAR1 - 30)) | (1L << (VAR2 - 30)))) != 0)) {
					{
					setState(375);
					verb();
					setState(376);
					objectList();
					}
				}

				}
				}
				setState(384);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PropertyListContext extends ParserRuleContext {
		public PropertyListNotEmptyContext propertyListNotEmpty() {
			return getRuleContext(PropertyListNotEmptyContext.class,0);
		}
		public PropertyListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_propertyList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterPropertyList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitPropertyList(this);
		}
	}

	public final PropertyListContext propertyList() throws RecognitionException {
		PropertyListContext _localctx = new PropertyListContext(_ctx, getState());
		enterRule(_localctx, 66, RULE_propertyList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(386);
			_la = _input.LA(1);
			if (((((_la - 30)) & ~0x3f) == 0 && ((1L << (_la - 30)) & ((1L << (T__29 - 30)) | (1L << (IRI_REF - 30)) | (1L << (PNAME_NS - 30)) | (1L << (PNAME_LN - 30)) | (1L << (VAR1 - 30)) | (1L << (VAR2 - 30)))) != 0)) {
				{
				setState(385);
				propertyListNotEmpty();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ObjectListContext extends ParserRuleContext {
		public List<ObjectContext> object() {
			return getRuleContexts(ObjectContext.class);
		}
		public ObjectContext object(int i) {
			return getRuleContext(ObjectContext.class,i);
		}
		public ObjectListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_objectList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterObjectList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitObjectList(this);
		}
	}

	public final ObjectListContext objectList() throws RecognitionException {
		ObjectListContext _localctx = new ObjectListContext(_ctx, getState());
		enterRule(_localctx, 68, RULE_objectList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(388);
			object();
			setState(393);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__26) {
				{
				{
				setState(389);
				match(T__26);
				setState(390);
				object();
				}
				}
				setState(395);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ObjectContext extends ParserRuleContext {
		public GraphNodeContext graphNode() {
			return getRuleContext(GraphNodeContext.class,0);
		}
		public ObjectContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_object; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterObject(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitObject(this);
		}
	}

	public final ObjectContext object() throws RecognitionException {
		ObjectContext _localctx = new ObjectContext(_ctx, getState());
		enterRule(_localctx, 70, RULE_object);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(396);
			graphNode();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VerbContext extends ParserRuleContext {
		public VarOrIRIrefContext varOrIRIref() {
			return getRuleContext(VarOrIRIrefContext.class,0);
		}
		public VerbContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_verb; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterVerb(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitVerb(this);
		}
	}

	public final VerbContext verb() throws RecognitionException {
		VerbContext _localctx = new VerbContext(_ctx, getState());
		enterRule(_localctx, 72, RULE_verb);
		try {
			setState(400);
			switch (_input.LA(1)) {
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case VAR1:
			case VAR2:
				enterOuterAlt(_localctx, 1);
				{
				setState(398);
				varOrIRIref();
				}
				break;
			case T__29:
				enterOuterAlt(_localctx, 2);
				{
				setState(399);
				match(T__29);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TriplesNodeContext extends ParserRuleContext {
		public CollectionContext collection() {
			return getRuleContext(CollectionContext.class,0);
		}
		public BlankNodePropertyListContext blankNodePropertyList() {
			return getRuleContext(BlankNodePropertyListContext.class,0);
		}
		public TriplesNodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_triplesNode; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterTriplesNode(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitTriplesNode(this);
		}
	}

	public final TriplesNodeContext triplesNode() throws RecognitionException {
		TriplesNodeContext _localctx = new TriplesNodeContext(_ctx, getState());
		enterRule(_localctx, 74, RULE_triplesNode);
		try {
			setState(404);
			switch (_input.LA(1)) {
			case T__25:
				enterOuterAlt(_localctx, 1);
				{
				setState(402);
				collection();
				}
				break;
			case T__30:
				enterOuterAlt(_localctx, 2);
				{
				setState(403);
				blankNodePropertyList();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlankNodePropertyListContext extends ParserRuleContext {
		public PropertyListNotEmptyContext propertyListNotEmpty() {
			return getRuleContext(PropertyListNotEmptyContext.class,0);
		}
		public BlankNodePropertyListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_blankNodePropertyList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterBlankNodePropertyList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitBlankNodePropertyList(this);
		}
	}

	public final BlankNodePropertyListContext blankNodePropertyList() throws RecognitionException {
		BlankNodePropertyListContext _localctx = new BlankNodePropertyListContext(_ctx, getState());
		enterRule(_localctx, 76, RULE_blankNodePropertyList);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(406);
			match(T__30);
			setState(407);
			propertyListNotEmpty();
			setState(408);
			match(T__31);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class CollectionContext extends ParserRuleContext {
		public List<GraphNodeContext> graphNode() {
			return getRuleContexts(GraphNodeContext.class);
		}
		public GraphNodeContext graphNode(int i) {
			return getRuleContext(GraphNodeContext.class,i);
		}
		public CollectionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_collection; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterCollection(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitCollection(this);
		}
	}

	public final CollectionContext collection() throws RecognitionException {
		CollectionContext _localctx = new CollectionContext(_ctx, getState());
		enterRule(_localctx, 78, RULE_collection);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(410);
			match(T__25);
			setState(412); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(411);
				graphNode();
				}
				}
				setState(414); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( ((((_la - 26)) & ~0x3f) == 0 && ((1L << (_la - 26)) & ((1L << (T__25 - 26)) | (1L << (T__30 - 26)) | (1L << (T__56 - 26)) | (1L << (T__57 - 26)) | (1L << (IRI_REF - 26)) | (1L << (PNAME_NS - 26)) | (1L << (PNAME_LN - 26)) | (1L << (BLANK_NODE_LABEL - 26)) | (1L << (VAR1 - 26)) | (1L << (VAR2 - 26)) | (1L << (INTEGER - 26)) | (1L << (DECIMAL - 26)) | (1L << (DOUBLE - 26)) | (1L << (INTEGER_POSITIVE - 26)) | (1L << (DECIMAL_POSITIVE - 26)) | (1L << (DOUBLE_POSITIVE - 26)) | (1L << (INTEGER_NEGATIVE - 26)) | (1L << (DECIMAL_NEGATIVE - 26)) | (1L << (DOUBLE_NEGATIVE - 26)) | (1L << (STRING_LITERAL1 - 26)) | (1L << (STRING_LITERAL2 - 26)) | (1L << (NIL - 26)) | (1L << (ANON - 26)))) != 0) );
			setState(416);
			match(T__27);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GraphNodeContext extends ParserRuleContext {
		public VarOrTermContext varOrTerm() {
			return getRuleContext(VarOrTermContext.class,0);
		}
		public TriplesNodeContext triplesNode() {
			return getRuleContext(TriplesNodeContext.class,0);
		}
		public GraphNodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_graphNode; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterGraphNode(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitGraphNode(this);
		}
	}

	public final GraphNodeContext graphNode() throws RecognitionException {
		GraphNodeContext _localctx = new GraphNodeContext(_ctx, getState());
		enterRule(_localctx, 80, RULE_graphNode);
		try {
			setState(420);
			switch (_input.LA(1)) {
			case T__56:
			case T__57:
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case BLANK_NODE_LABEL:
			case VAR1:
			case VAR2:
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
			case STRING_LITERAL1:
			case STRING_LITERAL2:
			case NIL:
			case ANON:
				enterOuterAlt(_localctx, 1);
				{
				setState(418);
				varOrTerm();
				}
				break;
			case T__25:
			case T__30:
				enterOuterAlt(_localctx, 2);
				{
				setState(419);
				triplesNode();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VarOrTermContext extends ParserRuleContext {
		public VarContext var() {
			return getRuleContext(VarContext.class,0);
		}
		public GraphTermContext graphTerm() {
			return getRuleContext(GraphTermContext.class,0);
		}
		public VarOrTermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varOrTerm; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterVarOrTerm(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitVarOrTerm(this);
		}
	}

	public final VarOrTermContext varOrTerm() throws RecognitionException {
		VarOrTermContext _localctx = new VarOrTermContext(_ctx, getState());
		enterRule(_localctx, 82, RULE_varOrTerm);
		try {
			setState(424);
			switch (_input.LA(1)) {
			case VAR1:
			case VAR2:
				enterOuterAlt(_localctx, 1);
				{
				setState(422);
				var();
				}
				break;
			case T__56:
			case T__57:
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case BLANK_NODE_LABEL:
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
			case STRING_LITERAL1:
			case STRING_LITERAL2:
			case NIL:
			case ANON:
				enterOuterAlt(_localctx, 2);
				{
				setState(423);
				graphTerm();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VarOrIRIrefContext extends ParserRuleContext {
		public VarContext var() {
			return getRuleContext(VarContext.class,0);
		}
		public IriRefContext iriRef() {
			return getRuleContext(IriRefContext.class,0);
		}
		public VarOrIRIrefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varOrIRIref; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterVarOrIRIref(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitVarOrIRIref(this);
		}
	}

	public final VarOrIRIrefContext varOrIRIref() throws RecognitionException {
		VarOrIRIrefContext _localctx = new VarOrIRIrefContext(_ctx, getState());
		enterRule(_localctx, 84, RULE_varOrIRIref);
		try {
			setState(428);
			switch (_input.LA(1)) {
			case VAR1:
			case VAR2:
				enterOuterAlt(_localctx, 1);
				{
				setState(426);
				var();
				}
				break;
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
				enterOuterAlt(_localctx, 2);
				{
				setState(427);
				iriRef();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VarContext extends ParserRuleContext {
		public TerminalNode VAR1() { return getToken(SparqlParser.VAR1, 0); }
		public TerminalNode VAR2() { return getToken(SparqlParser.VAR2, 0); }
		public VarContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_var; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterVar(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitVar(this);
		}
	}

	public final VarContext var() throws RecognitionException {
		VarContext _localctx = new VarContext(_ctx, getState());
		enterRule(_localctx, 86, RULE_var);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(430);
			_la = _input.LA(1);
			if ( !(_la==VAR1 || _la==VAR2) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class GraphTermContext extends ParserRuleContext {
		public IriRefContext iriRef() {
			return getRuleContext(IriRefContext.class,0);
		}
		public RdfLiteralContext rdfLiteral() {
			return getRuleContext(RdfLiteralContext.class,0);
		}
		public NumericLiteralContext numericLiteral() {
			return getRuleContext(NumericLiteralContext.class,0);
		}
		public BooleanLiteralContext booleanLiteral() {
			return getRuleContext(BooleanLiteralContext.class,0);
		}
		public BlankNodeContext blankNode() {
			return getRuleContext(BlankNodeContext.class,0);
		}
		public TerminalNode NIL() { return getToken(SparqlParser.NIL, 0); }
		public GraphTermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_graphTerm; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterGraphTerm(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitGraphTerm(this);
		}
	}

	public final GraphTermContext graphTerm() throws RecognitionException {
		GraphTermContext _localctx = new GraphTermContext(_ctx, getState());
		enterRule(_localctx, 88, RULE_graphTerm);
		try {
			setState(438);
			switch (_input.LA(1)) {
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
				enterOuterAlt(_localctx, 1);
				{
				setState(432);
				iriRef();
				}
				break;
			case STRING_LITERAL1:
			case STRING_LITERAL2:
				enterOuterAlt(_localctx, 2);
				{
				setState(433);
				rdfLiteral();
				}
				break;
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
				enterOuterAlt(_localctx, 3);
				{
				setState(434);
				numericLiteral();
				}
				break;
			case T__56:
			case T__57:
				enterOuterAlt(_localctx, 4);
				{
				setState(435);
				booleanLiteral();
				}
				break;
			case BLANK_NODE_LABEL:
			case ANON:
				enterOuterAlt(_localctx, 5);
				{
				setState(436);
				blankNode();
				}
				break;
			case NIL:
				enterOuterAlt(_localctx, 6);
				{
				setState(437);
				match(NIL);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExpressionContext extends ParserRuleContext {
		public ConditionalOrExpressionContext conditionalOrExpression() {
			return getRuleContext(ConditionalOrExpressionContext.class,0);
		}
		public ExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitExpression(this);
		}
	}

	public final ExpressionContext expression() throws RecognitionException {
		ExpressionContext _localctx = new ExpressionContext(_ctx, getState());
		enterRule(_localctx, 90, RULE_expression);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(440);
			conditionalOrExpression();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConditionalOrExpressionContext extends ParserRuleContext {
		public List<ConditionalAndExpressionContext> conditionalAndExpression() {
			return getRuleContexts(ConditionalAndExpressionContext.class);
		}
		public ConditionalAndExpressionContext conditionalAndExpression(int i) {
			return getRuleContext(ConditionalAndExpressionContext.class,i);
		}
		public ConditionalOrExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditionalOrExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterConditionalOrExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitConditionalOrExpression(this);
		}
	}

	public final ConditionalOrExpressionContext conditionalOrExpression() throws RecognitionException {
		ConditionalOrExpressionContext _localctx = new ConditionalOrExpressionContext(_ctx, getState());
		enterRule(_localctx, 92, RULE_conditionalOrExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(442);
			conditionalAndExpression();
			setState(447);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__32) {
				{
				{
				setState(443);
				match(T__32);
				setState(444);
				conditionalAndExpression();
				}
				}
				setState(449);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConditionalAndExpressionContext extends ParserRuleContext {
		public List<ValueLogicalContext> valueLogical() {
			return getRuleContexts(ValueLogicalContext.class);
		}
		public ValueLogicalContext valueLogical(int i) {
			return getRuleContext(ValueLogicalContext.class,i);
		}
		public ConditionalAndExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditionalAndExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterConditionalAndExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitConditionalAndExpression(this);
		}
	}

	public final ConditionalAndExpressionContext conditionalAndExpression() throws RecognitionException {
		ConditionalAndExpressionContext _localctx = new ConditionalAndExpressionContext(_ctx, getState());
		enterRule(_localctx, 94, RULE_conditionalAndExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(450);
			valueLogical();
			setState(455);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__33) {
				{
				{
				setState(451);
				match(T__33);
				setState(452);
				valueLogical();
				}
				}
				setState(457);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ValueLogicalContext extends ParserRuleContext {
		public RelationalExpressionContext relationalExpression() {
			return getRuleContext(RelationalExpressionContext.class,0);
		}
		public ValueLogicalContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_valueLogical; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterValueLogical(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitValueLogical(this);
		}
	}

	public final ValueLogicalContext valueLogical() throws RecognitionException {
		ValueLogicalContext _localctx = new ValueLogicalContext(_ctx, getState());
		enterRule(_localctx, 96, RULE_valueLogical);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(458);
			relationalExpression();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class RelationalExpressionContext extends ParserRuleContext {
		public List<NumericExpressionContext> numericExpression() {
			return getRuleContexts(NumericExpressionContext.class);
		}
		public NumericExpressionContext numericExpression(int i) {
			return getRuleContext(NumericExpressionContext.class,i);
		}
		public RelationalExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_relationalExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterRelationalExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitRelationalExpression(this);
		}
	}

	public final RelationalExpressionContext relationalExpression() throws RecognitionException {
		RelationalExpressionContext _localctx = new RelationalExpressionContext(_ctx, getState());
		enterRule(_localctx, 98, RULE_relationalExpression);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(460);
			numericExpression();
			setState(473);
			switch (_input.LA(1)) {
			case T__34:
				{
				setState(461);
				match(T__34);
				setState(462);
				numericExpression();
				}
				break;
			case T__35:
				{
				setState(463);
				match(T__35);
				setState(464);
				numericExpression();
				}
				break;
			case T__36:
				{
				setState(465);
				match(T__36);
				setState(466);
				numericExpression();
				}
				break;
			case T__37:
				{
				setState(467);
				match(T__37);
				setState(468);
				numericExpression();
				}
				break;
			case T__38:
				{
				setState(469);
				match(T__38);
				setState(470);
				numericExpression();
				}
				break;
			case T__39:
				{
				setState(471);
				match(T__39);
				setState(472);
				numericExpression();
				}
				break;
			case T__26:
			case T__27:
			case T__32:
			case T__33:
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericExpressionContext extends ParserRuleContext {
		public AdditiveExpressionContext additiveExpression() {
			return getRuleContext(AdditiveExpressionContext.class,0);
		}
		public NumericExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterNumericExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitNumericExpression(this);
		}
	}

	public final NumericExpressionContext numericExpression() throws RecognitionException {
		NumericExpressionContext _localctx = new NumericExpressionContext(_ctx, getState());
		enterRule(_localctx, 100, RULE_numericExpression);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(475);
			additiveExpression();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AdditiveExpressionContext extends ParserRuleContext {
		public List<MultiplicativeExpressionContext> multiplicativeExpression() {
			return getRuleContexts(MultiplicativeExpressionContext.class);
		}
		public MultiplicativeExpressionContext multiplicativeExpression(int i) {
			return getRuleContext(MultiplicativeExpressionContext.class,i);
		}
		public List<NumericLiteralPositiveContext> numericLiteralPositive() {
			return getRuleContexts(NumericLiteralPositiveContext.class);
		}
		public NumericLiteralPositiveContext numericLiteralPositive(int i) {
			return getRuleContext(NumericLiteralPositiveContext.class,i);
		}
		public List<NumericLiteralNegativeContext> numericLiteralNegative() {
			return getRuleContexts(NumericLiteralNegativeContext.class);
		}
		public NumericLiteralNegativeContext numericLiteralNegative(int i) {
			return getRuleContext(NumericLiteralNegativeContext.class,i);
		}
		public AdditiveExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_additiveExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterAdditiveExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitAdditiveExpression(this);
		}
	}

	public final AdditiveExpressionContext additiveExpression() throws RecognitionException {
		AdditiveExpressionContext _localctx = new AdditiveExpressionContext(_ctx, getState());
		enterRule(_localctx, 102, RULE_additiveExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(477);
			multiplicativeExpression();
			setState(486);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (((((_la - 41)) & ~0x3f) == 0 && ((1L << (_la - 41)) & ((1L << (T__40 - 41)) | (1L << (T__41 - 41)) | (1L << (INTEGER_POSITIVE - 41)) | (1L << (DECIMAL_POSITIVE - 41)) | (1L << (DOUBLE_POSITIVE - 41)) | (1L << (INTEGER_NEGATIVE - 41)) | (1L << (DECIMAL_NEGATIVE - 41)) | (1L << (DOUBLE_NEGATIVE - 41)))) != 0)) {
				{
				setState(484);
				switch (_input.LA(1)) {
				case T__40:
					{
					setState(478);
					match(T__40);
					setState(479);
					multiplicativeExpression();
					}
					break;
				case T__41:
					{
					setState(480);
					match(T__41);
					setState(481);
					multiplicativeExpression();
					}
					break;
				case INTEGER_POSITIVE:
				case DECIMAL_POSITIVE:
				case DOUBLE_POSITIVE:
					{
					setState(482);
					numericLiteralPositive();
					}
					break;
				case INTEGER_NEGATIVE:
				case DECIMAL_NEGATIVE:
				case DOUBLE_NEGATIVE:
					{
					setState(483);
					numericLiteralNegative();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(488);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MultiplicativeExpressionContext extends ParserRuleContext {
		public List<UnaryExpressionContext> unaryExpression() {
			return getRuleContexts(UnaryExpressionContext.class);
		}
		public UnaryExpressionContext unaryExpression(int i) {
			return getRuleContext(UnaryExpressionContext.class,i);
		}
		public MultiplicativeExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_multiplicativeExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterMultiplicativeExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitMultiplicativeExpression(this);
		}
	}

	public final MultiplicativeExpressionContext multiplicativeExpression() throws RecognitionException {
		MultiplicativeExpressionContext _localctx = new MultiplicativeExpressionContext(_ctx, getState());
		enterRule(_localctx, 104, RULE_multiplicativeExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(489);
			unaryExpression();
			setState(496);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__5 || _la==T__42) {
				{
				setState(494);
				switch (_input.LA(1)) {
				case T__5:
					{
					setState(490);
					match(T__5);
					setState(491);
					unaryExpression();
					}
					break;
				case T__42:
					{
					setState(492);
					match(T__42);
					setState(493);
					unaryExpression();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(498);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class UnaryExpressionContext extends ParserRuleContext {
		public PrimaryExpressionContext primaryExpression() {
			return getRuleContext(PrimaryExpressionContext.class,0);
		}
		public UnaryExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unaryExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterUnaryExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitUnaryExpression(this);
		}
	}

	public final UnaryExpressionContext unaryExpression() throws RecognitionException {
		UnaryExpressionContext _localctx = new UnaryExpressionContext(_ctx, getState());
		enterRule(_localctx, 106, RULE_unaryExpression);
		try {
			setState(506);
			switch (_input.LA(1)) {
			case T__43:
				enterOuterAlt(_localctx, 1);
				{
				setState(499);
				match(T__43);
				setState(500);
				primaryExpression();
				}
				break;
			case T__40:
				enterOuterAlt(_localctx, 2);
				{
				setState(501);
				match(T__40);
				setState(502);
				primaryExpression();
				}
				break;
			case T__41:
				enterOuterAlt(_localctx, 3);
				{
				setState(503);
				match(T__41);
				setState(504);
				primaryExpression();
				}
				break;
			case T__25:
			case T__44:
			case T__45:
			case T__46:
			case T__47:
			case T__48:
			case T__49:
			case T__50:
			case T__51:
			case T__52:
			case T__53:
			case T__54:
			case T__56:
			case T__57:
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case VAR1:
			case VAR2:
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
			case STRING_LITERAL1:
			case STRING_LITERAL2:
				enterOuterAlt(_localctx, 4);
				{
				setState(505);
				primaryExpression();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrimaryExpressionContext extends ParserRuleContext {
		public BrackettedExpressionContext brackettedExpression() {
			return getRuleContext(BrackettedExpressionContext.class,0);
		}
		public BuiltInCallContext builtInCall() {
			return getRuleContext(BuiltInCallContext.class,0);
		}
		public IriRefOrFunctionContext iriRefOrFunction() {
			return getRuleContext(IriRefOrFunctionContext.class,0);
		}
		public RdfLiteralContext rdfLiteral() {
			return getRuleContext(RdfLiteralContext.class,0);
		}
		public NumericLiteralContext numericLiteral() {
			return getRuleContext(NumericLiteralContext.class,0);
		}
		public BooleanLiteralContext booleanLiteral() {
			return getRuleContext(BooleanLiteralContext.class,0);
		}
		public VarContext var() {
			return getRuleContext(VarContext.class,0);
		}
		public PrimaryExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_primaryExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterPrimaryExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitPrimaryExpression(this);
		}
	}

	public final PrimaryExpressionContext primaryExpression() throws RecognitionException {
		PrimaryExpressionContext _localctx = new PrimaryExpressionContext(_ctx, getState());
		enterRule(_localctx, 108, RULE_primaryExpression);
		try {
			setState(515);
			switch (_input.LA(1)) {
			case T__25:
				enterOuterAlt(_localctx, 1);
				{
				setState(508);
				brackettedExpression();
				}
				break;
			case T__44:
			case T__45:
			case T__46:
			case T__47:
			case T__48:
			case T__49:
			case T__50:
			case T__51:
			case T__52:
			case T__53:
			case T__54:
				enterOuterAlt(_localctx, 2);
				{
				setState(509);
				builtInCall();
				}
				break;
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
				enterOuterAlt(_localctx, 3);
				{
				setState(510);
				iriRefOrFunction();
				}
				break;
			case STRING_LITERAL1:
			case STRING_LITERAL2:
				enterOuterAlt(_localctx, 4);
				{
				setState(511);
				rdfLiteral();
				}
				break;
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
				enterOuterAlt(_localctx, 5);
				{
				setState(512);
				numericLiteral();
				}
				break;
			case T__56:
			case T__57:
				enterOuterAlt(_localctx, 6);
				{
				setState(513);
				booleanLiteral();
				}
				break;
			case VAR1:
			case VAR2:
				enterOuterAlt(_localctx, 7);
				{
				setState(514);
				var();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BrackettedExpressionContext extends ParserRuleContext {
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public BrackettedExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_brackettedExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterBrackettedExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitBrackettedExpression(this);
		}
	}

	public final BrackettedExpressionContext brackettedExpression() throws RecognitionException {
		BrackettedExpressionContext _localctx = new BrackettedExpressionContext(_ctx, getState());
		enterRule(_localctx, 110, RULE_brackettedExpression);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(517);
			match(T__25);
			setState(518);
			expression();
			setState(519);
			match(T__27);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BuiltInCallContext extends ParserRuleContext {
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public VarContext var() {
			return getRuleContext(VarContext.class,0);
		}
		public RegexExpressionContext regexExpression() {
			return getRuleContext(RegexExpressionContext.class,0);
		}
		public BuiltInCallContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_builtInCall; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterBuiltInCall(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitBuiltInCall(this);
		}
	}

	public final BuiltInCallContext builtInCall() throws RecognitionException {
		BuiltInCallContext _localctx = new BuiltInCallContext(_ctx, getState());
		enterRule(_localctx, 112, RULE_builtInCall);
		try {
			setState(576);
			switch (_input.LA(1)) {
			case T__44:
				enterOuterAlt(_localctx, 1);
				{
				setState(521);
				match(T__44);
				setState(522);
				match(T__25);
				setState(523);
				expression();
				setState(524);
				match(T__27);
				}
				break;
			case T__45:
				enterOuterAlt(_localctx, 2);
				{
				setState(526);
				match(T__45);
				setState(527);
				match(T__25);
				setState(528);
				expression();
				setState(529);
				match(T__27);
				}
				break;
			case T__46:
				enterOuterAlt(_localctx, 3);
				{
				setState(531);
				match(T__46);
				setState(532);
				match(T__25);
				setState(533);
				expression();
				setState(534);
				match(T__26);
				setState(535);
				expression();
				setState(536);
				match(T__27);
				}
				break;
			case T__47:
				enterOuterAlt(_localctx, 4);
				{
				setState(538);
				match(T__47);
				setState(539);
				match(T__25);
				setState(540);
				expression();
				setState(541);
				match(T__27);
				}
				break;
			case T__48:
				enterOuterAlt(_localctx, 5);
				{
				setState(543);
				match(T__48);
				setState(544);
				match(T__25);
				setState(545);
				var();
				setState(546);
				match(T__27);
				}
				break;
			case T__49:
				enterOuterAlt(_localctx, 6);
				{
				setState(548);
				match(T__49);
				setState(549);
				match(T__25);
				setState(550);
				expression();
				setState(551);
				match(T__26);
				setState(552);
				expression();
				setState(553);
				match(T__27);
				}
				break;
			case T__50:
				enterOuterAlt(_localctx, 7);
				{
				setState(555);
				match(T__50);
				setState(556);
				match(T__25);
				setState(557);
				expression();
				setState(558);
				match(T__27);
				}
				break;
			case T__51:
				enterOuterAlt(_localctx, 8);
				{
				setState(560);
				match(T__51);
				setState(561);
				match(T__25);
				setState(562);
				expression();
				setState(563);
				match(T__27);
				}
				break;
			case T__52:
				enterOuterAlt(_localctx, 9);
				{
				setState(565);
				match(T__52);
				setState(566);
				match(T__25);
				setState(567);
				expression();
				setState(568);
				match(T__27);
				}
				break;
			case T__53:
				enterOuterAlt(_localctx, 10);
				{
				setState(570);
				match(T__53);
				setState(571);
				match(T__25);
				setState(572);
				expression();
				setState(573);
				match(T__27);
				}
				break;
			case T__54:
				enterOuterAlt(_localctx, 11);
				{
				setState(575);
				regexExpression();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class RegexExpressionContext extends ParserRuleContext {
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public RegexExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_regexExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterRegexExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitRegexExpression(this);
		}
	}

	public final RegexExpressionContext regexExpression() throws RecognitionException {
		RegexExpressionContext _localctx = new RegexExpressionContext(_ctx, getState());
		enterRule(_localctx, 114, RULE_regexExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(578);
			match(T__54);
			setState(579);
			match(T__25);
			setState(580);
			expression();
			setState(581);
			match(T__26);
			setState(582);
			expression();
			setState(585);
			_la = _input.LA(1);
			if (_la==T__26) {
				{
				setState(583);
				match(T__26);
				setState(584);
				expression();
				}
			}

			setState(587);
			match(T__27);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IriRefOrFunctionContext extends ParserRuleContext {
		public IriRefContext iriRef() {
			return getRuleContext(IriRefContext.class,0);
		}
		public ArgListContext argList() {
			return getRuleContext(ArgListContext.class,0);
		}
		public IriRefOrFunctionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_iriRefOrFunction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterIriRefOrFunction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitIriRefOrFunction(this);
		}
	}

	public final IriRefOrFunctionContext iriRefOrFunction() throws RecognitionException {
		IriRefOrFunctionContext _localctx = new IriRefOrFunctionContext(_ctx, getState());
		enterRule(_localctx, 116, RULE_iriRefOrFunction);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(589);
			iriRef();
			setState(591);
			_la = _input.LA(1);
			if (_la==T__25 || _la==NIL) {
				{
				setState(590);
				argList();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class RdfLiteralContext extends ParserRuleContext {
		public StringContext string() {
			return getRuleContext(StringContext.class,0);
		}
		public TerminalNode LANGTAG() { return getToken(SparqlParser.LANGTAG, 0); }
		public IriRefContext iriRef() {
			return getRuleContext(IriRefContext.class,0);
		}
		public RdfLiteralContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_rdfLiteral; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterRdfLiteral(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitRdfLiteral(this);
		}
	}

	public final RdfLiteralContext rdfLiteral() throws RecognitionException {
		RdfLiteralContext _localctx = new RdfLiteralContext(_ctx, getState());
		enterRule(_localctx, 118, RULE_rdfLiteral);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(593);
			string();
			setState(597);
			switch (_input.LA(1)) {
			case LANGTAG:
				{
				setState(594);
				match(LANGTAG);
				}
				break;
			case T__55:
				{
				{
				setState(595);
				match(T__55);
				setState(596);
				iriRef();
				}
				}
				break;
			case T__5:
			case T__18:
			case T__19:
			case T__20:
			case T__21:
			case T__22:
			case T__24:
			case T__25:
			case T__26:
			case T__27:
			case T__28:
			case T__29:
			case T__30:
			case T__31:
			case T__32:
			case T__33:
			case T__34:
			case T__35:
			case T__36:
			case T__37:
			case T__38:
			case T__39:
			case T__40:
			case T__41:
			case T__42:
			case T__56:
			case T__57:
			case IRI_REF:
			case PNAME_NS:
			case PNAME_LN:
			case BLANK_NODE_LABEL:
			case VAR1:
			case VAR2:
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
			case STRING_LITERAL1:
			case STRING_LITERAL2:
			case NIL:
			case ANON:
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericLiteralContext extends ParserRuleContext {
		public NumericLiteralUnsignedContext numericLiteralUnsigned() {
			return getRuleContext(NumericLiteralUnsignedContext.class,0);
		}
		public NumericLiteralPositiveContext numericLiteralPositive() {
			return getRuleContext(NumericLiteralPositiveContext.class,0);
		}
		public NumericLiteralNegativeContext numericLiteralNegative() {
			return getRuleContext(NumericLiteralNegativeContext.class,0);
		}
		public NumericLiteralContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericLiteral; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterNumericLiteral(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitNumericLiteral(this);
		}
	}

	public final NumericLiteralContext numericLiteral() throws RecognitionException {
		NumericLiteralContext _localctx = new NumericLiteralContext(_ctx, getState());
		enterRule(_localctx, 120, RULE_numericLiteral);
		try {
			setState(602);
			switch (_input.LA(1)) {
			case INTEGER:
			case DECIMAL:
			case DOUBLE:
				enterOuterAlt(_localctx, 1);
				{
				setState(599);
				numericLiteralUnsigned();
				}
				break;
			case INTEGER_POSITIVE:
			case DECIMAL_POSITIVE:
			case DOUBLE_POSITIVE:
				enterOuterAlt(_localctx, 2);
				{
				setState(600);
				numericLiteralPositive();
				}
				break;
			case INTEGER_NEGATIVE:
			case DECIMAL_NEGATIVE:
			case DOUBLE_NEGATIVE:
				enterOuterAlt(_localctx, 3);
				{
				setState(601);
				numericLiteralNegative();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericLiteralUnsignedContext extends ParserRuleContext {
		public TerminalNode INTEGER() { return getToken(SparqlParser.INTEGER, 0); }
		public TerminalNode DECIMAL() { return getToken(SparqlParser.DECIMAL, 0); }
		public TerminalNode DOUBLE() { return getToken(SparqlParser.DOUBLE, 0); }
		public NumericLiteralUnsignedContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericLiteralUnsigned; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterNumericLiteralUnsigned(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitNumericLiteralUnsigned(this);
		}
	}

	public final NumericLiteralUnsignedContext numericLiteralUnsigned() throws RecognitionException {
		NumericLiteralUnsignedContext _localctx = new NumericLiteralUnsignedContext(_ctx, getState());
		enterRule(_localctx, 122, RULE_numericLiteralUnsigned);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(604);
			_la = _input.LA(1);
			if ( !(((((_la - 66)) & ~0x3f) == 0 && ((1L << (_la - 66)) & ((1L << (INTEGER - 66)) | (1L << (DECIMAL - 66)) | (1L << (DOUBLE - 66)))) != 0)) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericLiteralPositiveContext extends ParserRuleContext {
		public TerminalNode INTEGER_POSITIVE() { return getToken(SparqlParser.INTEGER_POSITIVE, 0); }
		public TerminalNode DECIMAL_POSITIVE() { return getToken(SparqlParser.DECIMAL_POSITIVE, 0); }
		public TerminalNode DOUBLE_POSITIVE() { return getToken(SparqlParser.DOUBLE_POSITIVE, 0); }
		public NumericLiteralPositiveContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericLiteralPositive; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterNumericLiteralPositive(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitNumericLiteralPositive(this);
		}
	}

	public final NumericLiteralPositiveContext numericLiteralPositive() throws RecognitionException {
		NumericLiteralPositiveContext _localctx = new NumericLiteralPositiveContext(_ctx, getState());
		enterRule(_localctx, 124, RULE_numericLiteralPositive);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(606);
			_la = _input.LA(1);
			if ( !(((((_la - 69)) & ~0x3f) == 0 && ((1L << (_la - 69)) & ((1L << (INTEGER_POSITIVE - 69)) | (1L << (DECIMAL_POSITIVE - 69)) | (1L << (DOUBLE_POSITIVE - 69)))) != 0)) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericLiteralNegativeContext extends ParserRuleContext {
		public TerminalNode INTEGER_NEGATIVE() { return getToken(SparqlParser.INTEGER_NEGATIVE, 0); }
		public TerminalNode DECIMAL_NEGATIVE() { return getToken(SparqlParser.DECIMAL_NEGATIVE, 0); }
		public TerminalNode DOUBLE_NEGATIVE() { return getToken(SparqlParser.DOUBLE_NEGATIVE, 0); }
		public NumericLiteralNegativeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericLiteralNegative; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterNumericLiteralNegative(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitNumericLiteralNegative(this);
		}
	}

	public final NumericLiteralNegativeContext numericLiteralNegative() throws RecognitionException {
		NumericLiteralNegativeContext _localctx = new NumericLiteralNegativeContext(_ctx, getState());
		enterRule(_localctx, 126, RULE_numericLiteralNegative);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(608);
			_la = _input.LA(1);
			if ( !(((((_la - 72)) & ~0x3f) == 0 && ((1L << (_la - 72)) & ((1L << (INTEGER_NEGATIVE - 72)) | (1L << (DECIMAL_NEGATIVE - 72)) | (1L << (DOUBLE_NEGATIVE - 72)))) != 0)) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BooleanLiteralContext extends ParserRuleContext {
		public BooleanLiteralContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_booleanLiteral; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterBooleanLiteral(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitBooleanLiteral(this);
		}
	}

	public final BooleanLiteralContext booleanLiteral() throws RecognitionException {
		BooleanLiteralContext _localctx = new BooleanLiteralContext(_ctx, getState());
		enterRule(_localctx, 128, RULE_booleanLiteral);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(610);
			_la = _input.LA(1);
			if ( !(_la==T__56 || _la==T__57) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StringContext extends ParserRuleContext {
		public TerminalNode STRING_LITERAL1() { return getToken(SparqlParser.STRING_LITERAL1, 0); }
		public TerminalNode STRING_LITERAL2() { return getToken(SparqlParser.STRING_LITERAL2, 0); }
		public StringContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_string; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterString(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitString(this);
		}
	}

	public final StringContext string() throws RecognitionException {
		StringContext _localctx = new StringContext(_ctx, getState());
		enterRule(_localctx, 130, RULE_string);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(612);
			_la = _input.LA(1);
			if ( !(_la==STRING_LITERAL1 || _la==STRING_LITERAL2) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IriRefContext extends ParserRuleContext {
		public TerminalNode IRI_REF() { return getToken(SparqlParser.IRI_REF, 0); }
		public PrefixedNameContext prefixedName() {
			return getRuleContext(PrefixedNameContext.class,0);
		}
		public IriRefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_iriRef; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterIriRef(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitIriRef(this);
		}
	}

	public final IriRefContext iriRef() throws RecognitionException {
		IriRefContext _localctx = new IriRefContext(_ctx, getState());
		enterRule(_localctx, 132, RULE_iriRef);
		try {
			setState(616);
			switch (_input.LA(1)) {
			case IRI_REF:
				enterOuterAlt(_localctx, 1);
				{
				setState(614);
				match(IRI_REF);
				}
				break;
			case PNAME_NS:
			case PNAME_LN:
				enterOuterAlt(_localctx, 2);
				{
				setState(615);
				prefixedName();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrefixedNameContext extends ParserRuleContext {
		public TerminalNode PNAME_LN() { return getToken(SparqlParser.PNAME_LN, 0); }
		public TerminalNode PNAME_NS() { return getToken(SparqlParser.PNAME_NS, 0); }
		public PrefixedNameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prefixedName; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterPrefixedName(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitPrefixedName(this);
		}
	}

	public final PrefixedNameContext prefixedName() throws RecognitionException {
		PrefixedNameContext _localctx = new PrefixedNameContext(_ctx, getState());
		enterRule(_localctx, 134, RULE_prefixedName);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(618);
			_la = _input.LA(1);
			if ( !(_la==PNAME_NS || _la==PNAME_LN) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlankNodeContext extends ParserRuleContext {
		public TerminalNode BLANK_NODE_LABEL() { return getToken(SparqlParser.BLANK_NODE_LABEL, 0); }
		public TerminalNode ANON() { return getToken(SparqlParser.ANON, 0); }
		public BlankNodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_blankNode; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).enterBlankNode(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof SparqlListener ) ((SparqlListener)listener).exitBlankNode(this);
		}
	}

	public final BlankNodeContext blankNode() throws RecognitionException {
		BlankNodeContext _localctx = new BlankNodeContext(_ctx, getState());
		enterRule(_localctx, 136, RULE_blankNode);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(620);
			_la = _input.LA(1);
			if ( !(_la==BLANK_NODE_LABEL || _la==ANON) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3Y\u0271\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t+\4"+
		",\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\4\62\t\62\4\63\t\63\4\64\t"+
		"\64\4\65\t\65\4\66\t\66\4\67\t\67\48\t8\49\t9\4:\t:\4;\t;\4<\t<\4=\t="+
		"\4>\t>\4?\t?\4@\t@\4A\tA\4B\tB\4C\tC\4D\tD\4E\tE\4F\tF\3\2\3\2\3\2\3\2"+
		"\3\2\5\2\u0092\n\2\3\2\3\2\3\3\5\3\u0097\n\3\3\3\7\3\u009a\n\3\f\3\16"+
		"\3\u009d\13\3\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\6\3\6\5\6\u00a8\n\6\3\6\6"+
		"\6\u00ab\n\6\r\6\16\6\u00ac\3\6\5\6\u00b0\n\6\3\6\7\6\u00b3\n\6\f\6\16"+
		"\6\u00b6\13\6\3\6\3\6\3\6\3\7\3\7\3\7\7\7\u00be\n\7\f\7\16\7\u00c1\13"+
		"\7\3\7\3\7\3\7\3\b\3\b\6\b\u00c8\n\b\r\b\16\b\u00c9\3\b\5\b\u00cd\n\b"+
		"\3\b\7\b\u00d0\n\b\f\b\16\b\u00d3\13\b\3\b\5\b\u00d6\n\b\3\b\3\b\3\t\3"+
		"\t\7\t\u00dc\n\t\f\t\16\t\u00df\13\t\3\t\3\t\3\n\3\n\3\n\5\n\u00e6\n\n"+
		"\3\13\3\13\3\f\3\f\3\f\3\r\3\r\3\16\5\16\u00f0\n\16\3\16\3\16\3\17\5\17"+
		"\u00f5\n\17\3\17\5\17\u00f8\n\17\3\20\3\20\5\20\u00fc\n\20\3\20\3\20\5"+
		"\20\u0100\n\20\5\20\u0102\n\20\3\21\3\21\3\21\6\21\u0107\n\21\r\21\16"+
		"\21\u0108\3\22\3\22\3\22\3\22\5\22\u010f\n\22\5\22\u0111\n\22\3\23\3\23"+
		"\3\23\3\24\3\24\3\24\3\25\3\25\5\25\u011b\n\25\3\25\3\25\5\25\u011f\n"+
		"\25\3\25\5\25\u0122\n\25\3\25\5\25\u0125\n\25\7\25\u0127\n\25\f\25\16"+
		"\25\u012a\13\25\3\25\3\25\3\26\3\26\3\26\5\26\u0131\n\26\5\26\u0133\n"+
		"\26\3\27\3\27\3\27\5\27\u0138\n\27\3\30\3\30\3\30\3\31\3\31\3\31\3\31"+
		"\3\32\3\32\3\32\7\32\u0144\n\32\f\32\16\32\u0147\13\32\3\33\3\33\3\33"+
		"\3\34\3\34\3\34\5\34\u014f\n\34\3\35\3\35\3\35\3\36\3\36\3\36\3\36\3\36"+
		"\7\36\u0159\n\36\f\36\16\36\u015c\13\36\3\36\3\36\5\36\u0160\n\36\3\37"+
		"\3\37\5\37\u0164\n\37\3\37\3\37\3 \3 \3 \5 \u016b\n \5 \u016d\n \3!\3"+
		"!\3!\3!\3!\3!\5!\u0175\n!\3\"\3\"\3\"\3\"\3\"\3\"\5\"\u017d\n\"\7\"\u017f"+
		"\n\"\f\"\16\"\u0182\13\"\3#\5#\u0185\n#\3$\3$\3$\7$\u018a\n$\f$\16$\u018d"+
		"\13$\3%\3%\3&\3&\5&\u0193\n&\3\'\3\'\5\'\u0197\n\'\3(\3(\3(\3(\3)\3)\6"+
		")\u019f\n)\r)\16)\u01a0\3)\3)\3*\3*\5*\u01a7\n*\3+\3+\5+\u01ab\n+\3,\3"+
		",\5,\u01af\n,\3-\3-\3.\3.\3.\3.\3.\3.\5.\u01b9\n.\3/\3/\3\60\3\60\3\60"+
		"\7\60\u01c0\n\60\f\60\16\60\u01c3\13\60\3\61\3\61\3\61\7\61\u01c8\n\61"+
		"\f\61\16\61\u01cb\13\61\3\62\3\62\3\63\3\63\3\63\3\63\3\63\3\63\3\63\3"+
		"\63\3\63\3\63\3\63\3\63\3\63\5\63\u01dc\n\63\3\64\3\64\3\65\3\65\3\65"+
		"\3\65\3\65\3\65\3\65\7\65\u01e7\n\65\f\65\16\65\u01ea\13\65\3\66\3\66"+
		"\3\66\3\66\3\66\7\66\u01f1\n\66\f\66\16\66\u01f4\13\66\3\67\3\67\3\67"+
		"\3\67\3\67\3\67\3\67\5\67\u01fd\n\67\38\38\38\38\38\38\38\58\u0206\n8"+
		"\39\39\39\39\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:"+
		"\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:"+
		"\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\3:\5:\u0243\n:\3;\3;\3;\3;\3;\3;"+
		"\3;\5;\u024c\n;\3;\3;\3<\3<\5<\u0252\n<\3=\3=\3=\3=\5=\u0258\n=\3>\3>"+
		"\3>\5>\u025d\n>\3?\3?\3@\3@\3A\3A\3B\3B\3C\3C\3D\3D\5D\u026b\nD\3E\3E"+
		"\3F\3F\3F\2\2G\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36 \"$&(*,.\60\62\64"+
		"\668:<>@BDFHJLNPRTVXZ\\^`bdfhjlnprtvxz|~\u0080\u0082\u0084\u0086\u0088"+
		"\u008a\2\f\3\2\6\7\3\2\21\22\3\2AB\3\2DF\3\2GI\3\2JL\3\2;<\3\2NO\3\2>"+
		"?\4\2@@TT\u028d\2\u008c\3\2\2\2\4\u0096\3\2\2\2\6\u009e\3\2\2\2\b\u00a1"+
		"\3\2\2\2\n\u00a5\3\2\2\2\f\u00ba\3\2\2\2\16\u00c5\3\2\2\2\20\u00d9\3\2"+
		"\2\2\22\u00e2\3\2\2\2\24\u00e7\3\2\2\2\26\u00e9\3\2\2\2\30\u00ec\3\2\2"+
		"\2\32\u00ef\3\2\2\2\34\u00f4\3\2\2\2\36\u0101\3\2\2\2 \u0103\3\2\2\2\""+
		"\u0110\3\2\2\2$\u0112\3\2\2\2&\u0115\3\2\2\2(\u0118\3\2\2\2*\u012d\3\2"+
		"\2\2,\u0137\3\2\2\2.\u0139\3\2\2\2\60\u013c\3\2\2\2\62\u0140\3\2\2\2\64"+
		"\u0148\3\2\2\2\66\u014e\3\2\2\28\u0150\3\2\2\2:\u015f\3\2\2\2<\u0161\3"+
		"\2\2\2>\u0167\3\2\2\2@\u0174\3\2\2\2B\u0176\3\2\2\2D\u0184\3\2\2\2F\u0186"+
		"\3\2\2\2H\u018e\3\2\2\2J\u0192\3\2\2\2L\u0196\3\2\2\2N\u0198\3\2\2\2P"+
		"\u019c\3\2\2\2R\u01a6\3\2\2\2T\u01aa\3\2\2\2V\u01ae\3\2\2\2X\u01b0\3\2"+
		"\2\2Z\u01b8\3\2\2\2\\\u01ba\3\2\2\2^\u01bc\3\2\2\2`\u01c4\3\2\2\2b\u01cc"+
		"\3\2\2\2d\u01ce\3\2\2\2f\u01dd\3\2\2\2h\u01df\3\2\2\2j\u01eb\3\2\2\2l"+
		"\u01fc\3\2\2\2n\u0205\3\2\2\2p\u0207\3\2\2\2r\u0242\3\2\2\2t\u0244\3\2"+
		"\2\2v\u024f\3\2\2\2x\u0253\3\2\2\2z\u025c\3\2\2\2|\u025e\3\2\2\2~\u0260"+
		"\3\2\2\2\u0080\u0262\3\2\2\2\u0082\u0264\3\2\2\2\u0084\u0266\3\2\2\2\u0086"+
		"\u026a\3\2\2\2\u0088\u026c\3\2\2\2\u008a\u026e\3\2\2\2\u008c\u0091\5\4"+
		"\3\2\u008d\u0092\5\n\6\2\u008e\u0092\5\f\7\2\u008f\u0092\5\16\b\2\u0090"+
		"\u0092\5\20\t\2\u0091\u008d\3\2\2\2\u0091\u008e\3\2\2\2\u0091\u008f\3"+
		"\2\2\2\u0091\u0090\3\2\2\2\u0092\u0093\3\2\2\2\u0093\u0094\7\2\2\3\u0094"+
		"\3\3\2\2\2\u0095\u0097\5\6\4\2\u0096\u0095\3\2\2\2\u0096\u0097\3\2\2\2"+
		"\u0097\u009b\3\2\2\2\u0098\u009a\5\b\5\2\u0099\u0098\3\2\2\2\u009a\u009d"+
		"\3\2\2\2\u009b\u0099\3\2\2\2\u009b\u009c\3\2\2\2\u009c\5\3\2\2\2\u009d"+
		"\u009b\3\2\2\2\u009e\u009f\7\3\2\2\u009f\u00a0\7=\2\2\u00a0\7\3\2\2\2"+
		"\u00a1\u00a2\7\4\2\2\u00a2\u00a3\7>\2\2\u00a3\u00a4\7=\2\2\u00a4\t\3\2"+
		"\2\2\u00a5\u00a7\7\5\2\2\u00a6\u00a8\t\2\2\2\u00a7\u00a6\3\2\2\2\u00a7"+
		"\u00a8\3\2\2\2\u00a8\u00af\3\2\2\2\u00a9\u00ab\5X-\2\u00aa\u00a9\3\2\2"+
		"\2\u00ab\u00ac\3\2\2\2\u00ac\u00aa\3\2\2\2\u00ac\u00ad\3\2\2\2\u00ad\u00b0"+
		"\3\2\2\2\u00ae\u00b0\7\b\2\2\u00af\u00aa\3\2\2\2\u00af\u00ae\3\2\2\2\u00b0"+
		"\u00b4\3\2\2\2\u00b1\u00b3\5\22\n\2\u00b2\u00b1\3\2\2\2\u00b3\u00b6\3"+
		"\2\2\2\u00b4\u00b2\3\2\2\2\u00b4\u00b5\3\2\2\2\u00b5\u00b7\3\2\2\2\u00b6"+
		"\u00b4\3\2\2\2\u00b7\u00b8\5\32\16\2\u00b8\u00b9\5\34\17\2\u00b9\13\3"+
		"\2\2\2\u00ba\u00bb\7\t\2\2\u00bb\u00bf\5<\37\2\u00bc\u00be\5\22\n\2\u00bd"+
		"\u00bc\3\2\2\2\u00be\u00c1\3\2\2\2\u00bf\u00bd\3\2\2\2\u00bf\u00c0\3\2"+
		"\2\2\u00c0\u00c2\3\2\2\2\u00c1\u00bf\3\2\2\2\u00c2\u00c3\5\32\16\2\u00c3"+
		"\u00c4\5\34\17\2\u00c4\r\3\2\2\2\u00c5\u00cc\7\n\2\2\u00c6\u00c8\5V,\2"+
		"\u00c7\u00c6\3\2\2\2\u00c8\u00c9\3\2\2\2\u00c9\u00c7\3\2\2\2\u00c9\u00ca"+
		"\3\2\2\2\u00ca\u00cd\3\2\2\2\u00cb\u00cd\7\b\2\2\u00cc\u00c7\3\2\2\2\u00cc"+
		"\u00cb\3\2\2\2\u00cd\u00d1\3\2\2\2\u00ce\u00d0\5\22\n\2\u00cf\u00ce\3"+
		"\2\2\2\u00d0\u00d3\3\2\2\2\u00d1\u00cf\3\2\2\2\u00d1\u00d2\3\2\2\2\u00d2"+
		"\u00d5\3\2\2\2\u00d3\u00d1\3\2\2\2\u00d4\u00d6\5\32\16\2\u00d5\u00d4\3"+
		"\2\2\2\u00d5\u00d6\3\2\2\2\u00d6\u00d7\3\2\2\2\u00d7\u00d8\5\34\17\2\u00d8"+
		"\17\3\2\2\2\u00d9\u00dd\7\13\2\2\u00da\u00dc\5\22\n\2\u00db\u00da\3\2"+
		"\2\2\u00dc\u00df\3\2\2\2\u00dd\u00db\3\2\2\2\u00dd\u00de\3\2\2\2\u00de"+
		"\u00e0\3\2\2\2\u00df\u00dd\3\2\2\2\u00e0\u00e1\5\32\16\2\u00e1\21\3\2"+
		"\2\2\u00e2\u00e5\7\f\2\2\u00e3\u00e6\5\24\13\2\u00e4\u00e6\5\26\f\2\u00e5"+
		"\u00e3\3\2\2\2\u00e5\u00e4\3\2\2\2\u00e6\23\3\2\2\2\u00e7\u00e8\5\30\r"+
		"\2\u00e8\25\3\2\2\2\u00e9\u00ea\7\r\2\2\u00ea\u00eb\5\30\r\2\u00eb\27"+
		"\3\2\2\2\u00ec\u00ed\5\u0086D\2\u00ed\31\3\2\2\2\u00ee\u00f0\7\16\2\2"+
		"\u00ef\u00ee\3\2\2\2\u00ef\u00f0\3\2\2\2\u00f0\u00f1\3\2\2\2\u00f1\u00f2"+
		"\5(\25\2\u00f2\33\3\2\2\2\u00f3\u00f5\5 \21\2\u00f4\u00f3\3\2\2\2\u00f4"+
		"\u00f5\3\2\2\2\u00f5\u00f7\3\2\2\2\u00f6\u00f8\5\36\20\2\u00f7\u00f6\3"+
		"\2\2\2\u00f7\u00f8\3\2\2\2\u00f8\35\3\2\2\2\u00f9\u00fb\5$\23\2\u00fa"+
		"\u00fc\5&\24\2\u00fb\u00fa\3\2\2\2\u00fb\u00fc\3\2\2\2\u00fc\u0102\3\2"+
		"\2\2\u00fd\u00ff\5&\24\2\u00fe\u0100\5$\23\2\u00ff\u00fe\3\2\2\2\u00ff"+
		"\u0100\3\2\2\2\u0100\u0102\3\2\2\2\u0101\u00f9\3\2\2\2\u0101\u00fd\3\2"+
		"\2\2\u0102\37\3\2\2\2\u0103\u0104\7\17\2\2\u0104\u0106\7\20\2\2\u0105"+
		"\u0107\5\"\22\2\u0106\u0105\3\2\2\2\u0107\u0108\3\2\2\2\u0108\u0106\3"+
		"\2\2\2\u0108\u0109\3\2\2\2\u0109!\3\2\2\2\u010a\u010b\t\3\2\2\u010b\u0111"+
		"\5p9\2\u010c\u010f\5\66\34\2\u010d\u010f\5X-\2\u010e\u010c\3\2\2\2\u010e"+
		"\u010d\3\2\2\2\u010f\u0111\3\2\2\2\u0110\u010a\3\2\2\2\u0110\u010e\3\2"+
		"\2\2\u0111#\3\2\2\2\u0112\u0113\7\23\2\2\u0113\u0114\7D\2\2\u0114%\3\2"+
		"\2\2\u0115\u0116\7\24\2\2\u0116\u0117\7D\2\2\u0117\'\3\2\2\2\u0118\u011a"+
		"\7\25\2\2\u0119\u011b\5*\26\2\u011a\u0119\3\2\2\2\u011a\u011b\3\2\2\2"+
		"\u011b\u0128\3\2\2\2\u011c\u011f\5,\27\2\u011d\u011f\5\64\33\2\u011e\u011c"+
		"\3\2\2\2\u011e\u011d\3\2\2\2\u011f\u0121\3\2\2\2\u0120\u0122\7\26\2\2"+
		"\u0121\u0120\3\2\2\2\u0121\u0122\3\2\2\2\u0122\u0124\3\2\2\2\u0123\u0125"+
		"\5*\26\2\u0124\u0123\3\2\2\2\u0124\u0125\3\2\2\2\u0125\u0127\3\2\2\2\u0126"+
		"\u011e\3\2\2\2\u0127\u012a\3\2\2\2\u0128\u0126\3\2\2\2\u0128\u0129\3\2"+
		"\2\2\u0129\u012b\3\2\2\2\u012a\u0128\3\2\2\2\u012b\u012c\7\27\2\2\u012c"+
		")\3\2\2\2\u012d\u0132\5@!\2\u012e\u0130\7\26\2\2\u012f\u0131\5*\26\2\u0130"+
		"\u012f\3\2\2\2\u0130\u0131\3\2\2\2\u0131\u0133\3\2\2\2\u0132\u012e\3\2"+
		"\2\2\u0132\u0133\3\2\2\2\u0133+\3\2\2\2\u0134\u0138\5.\30\2\u0135\u0138"+
		"\5\62\32\2\u0136\u0138\5\60\31\2\u0137\u0134\3\2\2\2\u0137\u0135\3\2\2"+
		"\2\u0137\u0136\3\2\2\2\u0138-\3\2\2\2\u0139\u013a\7\30\2\2\u013a\u013b"+
		"\5(\25\2\u013b/\3\2\2\2\u013c\u013d\7\31\2\2\u013d\u013e\5V,\2\u013e\u013f"+
		"\5(\25\2\u013f\61\3\2\2\2\u0140\u0145\5(\25\2\u0141\u0142\7\32\2\2\u0142"+
		"\u0144\5(\25\2\u0143\u0141\3\2\2\2\u0144\u0147\3\2\2\2\u0145\u0143\3\2"+
		"\2\2\u0145\u0146\3\2\2\2\u0146\63\3\2\2\2\u0147\u0145\3\2\2\2\u0148\u0149"+
		"\7\33\2\2\u0149\u014a\5\66\34\2\u014a\65\3\2\2\2\u014b\u014f\5p9\2\u014c"+
		"\u014f\5r:\2\u014d\u014f\58\35\2\u014e\u014b\3\2\2\2\u014e\u014c\3\2\2"+
		"\2\u014e\u014d\3\2\2\2\u014f\67\3\2\2\2\u0150\u0151\5\u0086D\2\u0151\u0152"+
		"\5:\36\2\u01529\3\2\2\2\u0153\u0160\7S\2\2\u0154\u0155\7\34\2\2\u0155"+
		"\u015a\5\\/\2\u0156\u0157\7\35\2\2\u0157\u0159\5\\/\2\u0158\u0156\3\2"+
		"\2\2\u0159\u015c\3\2\2\2\u015a\u0158\3\2\2\2\u015a\u015b\3\2\2\2\u015b"+
		"\u015d\3\2\2\2\u015c\u015a\3\2\2\2\u015d\u015e\7\36\2\2\u015e\u0160\3"+
		"\2\2\2\u015f\u0153\3\2\2\2\u015f\u0154\3\2\2\2\u0160;\3\2\2\2\u0161\u0163"+
		"\7\25\2\2\u0162\u0164\5> \2\u0163\u0162\3\2\2\2\u0163\u0164\3\2\2\2\u0164"+
		"\u0165\3\2\2\2\u0165\u0166\7\27\2\2\u0166=\3\2\2\2\u0167\u016c\5@!\2\u0168"+
		"\u016a\7\26\2\2\u0169\u016b\5> \2\u016a\u0169\3\2\2\2\u016a\u016b\3\2"+
		"\2\2\u016b\u016d\3\2\2\2\u016c\u0168\3\2\2\2\u016c\u016d\3\2\2\2\u016d"+
		"?\3\2\2\2\u016e\u016f\5T+\2\u016f\u0170\5B\"\2\u0170\u0175\3\2\2\2\u0171"+
		"\u0172\5L\'\2\u0172\u0173\5D#\2\u0173\u0175\3\2\2\2\u0174\u016e\3\2\2"+
		"\2\u0174\u0171\3\2\2\2\u0175A\3\2\2\2\u0176\u0177\5J&\2\u0177\u0180\5"+
		"F$\2\u0178\u017c\7\37\2\2\u0179\u017a\5J&\2\u017a\u017b\5F$\2\u017b\u017d"+
		"\3\2\2\2\u017c\u0179\3\2\2\2\u017c\u017d\3\2\2\2\u017d\u017f\3\2\2\2\u017e"+
		"\u0178\3\2\2\2\u017f\u0182\3\2\2\2\u0180\u017e\3\2\2\2\u0180\u0181\3\2"+
		"\2\2\u0181C\3\2\2\2\u0182\u0180\3\2\2\2\u0183\u0185\5B\"\2\u0184\u0183"+
		"\3\2\2\2\u0184\u0185\3\2\2\2\u0185E\3\2\2\2\u0186\u018b\5H%\2\u0187\u0188"+
		"\7\35\2\2\u0188\u018a\5H%\2\u0189\u0187\3\2\2\2\u018a\u018d\3\2\2\2\u018b"+
		"\u0189\3\2\2\2\u018b\u018c\3\2\2\2\u018cG\3\2\2\2\u018d\u018b\3\2\2\2"+
		"\u018e\u018f\5R*\2\u018fI\3\2\2\2\u0190\u0193\5V,\2\u0191\u0193\7 \2\2"+
		"\u0192\u0190\3\2\2\2\u0192\u0191\3\2\2\2\u0193K\3\2\2\2\u0194\u0197\5"+
		"P)\2\u0195\u0197\5N(\2\u0196\u0194\3\2\2\2\u0196\u0195\3\2\2\2\u0197M"+
		"\3\2\2\2\u0198\u0199\7!\2\2\u0199\u019a\5B\"\2\u019a\u019b\7\"\2\2\u019b"+
		"O\3\2\2\2\u019c\u019e\7\34\2\2\u019d\u019f\5R*\2\u019e\u019d\3\2\2\2\u019f"+
		"\u01a0\3\2\2\2\u01a0\u019e\3\2\2\2\u01a0\u01a1\3\2\2\2\u01a1\u01a2\3\2"+
		"\2\2\u01a2\u01a3\7\36\2\2\u01a3Q\3\2\2\2\u01a4\u01a7\5T+\2\u01a5\u01a7"+
		"\5L\'\2\u01a6\u01a4\3\2\2\2\u01a6\u01a5\3\2\2\2\u01a7S\3\2\2\2\u01a8\u01ab"+
		"\5X-\2\u01a9\u01ab\5Z.\2\u01aa\u01a8\3\2\2\2\u01aa\u01a9\3\2\2\2\u01ab"+
		"U\3\2\2\2\u01ac\u01af\5X-\2\u01ad\u01af\5\u0086D\2\u01ae\u01ac\3\2\2\2"+
		"\u01ae\u01ad\3\2\2\2\u01afW\3\2\2\2\u01b0\u01b1\t\4\2\2\u01b1Y\3\2\2\2"+
		"\u01b2\u01b9\5\u0086D\2\u01b3\u01b9\5x=\2\u01b4\u01b9\5z>\2\u01b5\u01b9"+
		"\5\u0082B\2\u01b6\u01b9\5\u008aF\2\u01b7\u01b9\7S\2\2\u01b8\u01b2\3\2"+
		"\2\2\u01b8\u01b3\3\2\2\2\u01b8\u01b4\3\2\2\2\u01b8\u01b5\3\2\2\2\u01b8"+
		"\u01b6\3\2\2\2\u01b8\u01b7\3\2\2\2\u01b9[\3\2\2\2\u01ba\u01bb\5^\60\2"+
		"\u01bb]\3\2\2\2\u01bc\u01c1\5`\61\2\u01bd\u01be\7#\2\2\u01be\u01c0\5`"+
		"\61\2\u01bf\u01bd\3\2\2\2\u01c0\u01c3\3\2\2\2\u01c1\u01bf\3\2\2\2\u01c1"+
		"\u01c2\3\2\2\2\u01c2_\3\2\2\2\u01c3\u01c1\3\2\2\2\u01c4\u01c9\5b\62\2"+
		"\u01c5\u01c6\7$\2\2\u01c6\u01c8\5b\62\2\u01c7\u01c5\3\2\2\2\u01c8\u01cb"+
		"\3\2\2\2\u01c9\u01c7\3\2\2\2\u01c9\u01ca\3\2\2\2\u01caa\3\2\2\2\u01cb"+
		"\u01c9\3\2\2\2\u01cc\u01cd\5d\63\2\u01cdc\3\2\2\2\u01ce\u01db\5f\64\2"+
		"\u01cf\u01d0\7%\2\2\u01d0\u01dc\5f\64\2\u01d1\u01d2\7&\2\2\u01d2\u01dc"+
		"\5f\64\2\u01d3\u01d4\7\'\2\2\u01d4\u01dc\5f\64\2\u01d5\u01d6\7(\2\2\u01d6"+
		"\u01dc\5f\64\2\u01d7\u01d8\7)\2\2\u01d8\u01dc\5f\64\2\u01d9\u01da\7*\2"+
		"\2\u01da\u01dc\5f\64\2\u01db\u01cf\3\2\2\2\u01db\u01d1\3\2\2\2\u01db\u01d3"+
		"\3\2\2\2\u01db\u01d5\3\2\2\2\u01db\u01d7\3\2\2\2\u01db\u01d9\3\2\2\2\u01db"+
		"\u01dc\3\2\2\2\u01dce\3\2\2\2\u01dd\u01de\5h\65\2\u01deg\3\2\2\2\u01df"+
		"\u01e8\5j\66\2\u01e0\u01e1\7+\2\2\u01e1\u01e7\5j\66\2\u01e2\u01e3\7,\2"+
		"\2\u01e3\u01e7\5j\66\2\u01e4\u01e7\5~@\2\u01e5\u01e7\5\u0080A\2\u01e6"+
		"\u01e0\3\2\2\2\u01e6\u01e2\3\2\2\2\u01e6\u01e4\3\2\2\2\u01e6\u01e5\3\2"+
		"\2\2\u01e7\u01ea\3\2\2\2\u01e8\u01e6\3\2\2\2\u01e8\u01e9\3\2\2\2\u01e9"+
		"i\3\2\2\2\u01ea\u01e8\3\2\2\2\u01eb\u01f2\5l\67\2\u01ec\u01ed\7\b\2\2"+
		"\u01ed\u01f1\5l\67\2\u01ee\u01ef\7-\2\2\u01ef\u01f1\5l\67\2\u01f0\u01ec"+
		"\3\2\2\2\u01f0\u01ee\3\2\2\2\u01f1\u01f4\3\2\2\2\u01f2\u01f0\3\2\2\2\u01f2"+
		"\u01f3\3\2\2\2\u01f3k\3\2\2\2\u01f4\u01f2\3\2\2\2\u01f5\u01f6\7.\2\2\u01f6"+
		"\u01fd\5n8\2\u01f7\u01f8\7+\2\2\u01f8\u01fd\5n8\2\u01f9\u01fa\7,\2\2\u01fa"+
		"\u01fd\5n8\2\u01fb\u01fd\5n8\2\u01fc\u01f5\3\2\2\2\u01fc\u01f7\3\2\2\2"+
		"\u01fc\u01f9\3\2\2\2\u01fc\u01fb\3\2\2\2\u01fdm\3\2\2\2\u01fe\u0206\5"+
		"p9\2\u01ff\u0206\5r:\2\u0200\u0206\5v<\2\u0201\u0206\5x=\2\u0202\u0206"+
		"\5z>\2\u0203\u0206\5\u0082B\2\u0204\u0206\5X-\2\u0205\u01fe\3\2\2\2\u0205"+
		"\u01ff\3\2\2\2\u0205\u0200\3\2\2\2\u0205\u0201\3\2\2\2\u0205\u0202\3\2"+
		"\2\2\u0205\u0203\3\2\2\2\u0205\u0204\3\2\2\2\u0206o\3\2\2\2\u0207\u0208"+
		"\7\34\2\2\u0208\u0209\5\\/\2\u0209\u020a\7\36\2\2\u020aq\3\2\2\2\u020b"+
		"\u020c\7/\2\2\u020c\u020d\7\34\2\2\u020d\u020e\5\\/\2\u020e\u020f\7\36"+
		"\2\2\u020f\u0243\3\2\2\2\u0210\u0211\7\60\2\2\u0211\u0212\7\34\2\2\u0212"+
		"\u0213\5\\/\2\u0213\u0214\7\36\2\2\u0214\u0243\3\2\2\2\u0215\u0216\7\61"+
		"\2\2\u0216\u0217\7\34\2\2\u0217\u0218\5\\/\2\u0218\u0219\7\35\2\2\u0219"+
		"\u021a\5\\/\2\u021a\u021b\7\36\2\2\u021b\u0243\3\2\2\2\u021c\u021d\7\62"+
		"\2\2\u021d\u021e\7\34\2\2\u021e\u021f\5\\/\2\u021f\u0220\7\36\2\2\u0220"+
		"\u0243\3\2\2\2\u0221\u0222\7\63\2\2\u0222\u0223\7\34\2\2\u0223\u0224\5"+
		"X-\2\u0224\u0225\7\36\2\2\u0225\u0243\3\2\2\2\u0226\u0227\7\64\2\2\u0227"+
		"\u0228\7\34\2\2\u0228\u0229\5\\/\2\u0229\u022a\7\35\2\2\u022a\u022b\5"+
		"\\/\2\u022b\u022c\7\36\2\2\u022c\u0243\3\2\2\2\u022d\u022e\7\65\2\2\u022e"+
		"\u022f\7\34\2\2\u022f\u0230\5\\/\2\u0230\u0231\7\36\2\2\u0231\u0243\3"+
		"\2\2\2\u0232\u0233\7\66\2\2\u0233\u0234\7\34\2\2\u0234\u0235\5\\/\2\u0235"+
		"\u0236\7\36\2\2\u0236\u0243\3\2\2\2\u0237\u0238\7\67\2\2\u0238\u0239\7"+
		"\34\2\2\u0239\u023a\5\\/\2\u023a\u023b\7\36\2\2\u023b\u0243\3\2\2\2\u023c"+
		"\u023d\78\2\2\u023d\u023e\7\34\2\2\u023e\u023f\5\\/\2\u023f\u0240\7\36"+
		"\2\2\u0240\u0243\3\2\2\2\u0241\u0243\5t;\2\u0242\u020b\3\2\2\2\u0242\u0210"+
		"\3\2\2\2\u0242\u0215\3\2\2\2\u0242\u021c\3\2\2\2\u0242\u0221\3\2\2\2\u0242"+
		"\u0226\3\2\2\2\u0242\u022d\3\2\2\2\u0242\u0232\3\2\2\2\u0242\u0237\3\2"+
		"\2\2\u0242\u023c\3\2\2\2\u0242\u0241\3\2\2\2\u0243s\3\2\2\2\u0244\u0245"+
		"\79\2\2\u0245\u0246\7\34\2\2\u0246\u0247\5\\/\2\u0247\u0248\7\35\2\2\u0248"+
		"\u024b\5\\/\2\u0249\u024a\7\35\2\2\u024a\u024c\5\\/\2\u024b\u0249\3\2"+
		"\2\2\u024b\u024c\3\2\2\2\u024c\u024d\3\2\2\2\u024d\u024e\7\36\2\2\u024e"+
		"u\3\2\2\2\u024f\u0251\5\u0086D\2\u0250\u0252\5:\36\2\u0251\u0250\3\2\2"+
		"\2\u0251\u0252\3\2\2\2\u0252w\3\2\2\2\u0253\u0257\5\u0084C\2\u0254\u0258"+
		"\7C\2\2\u0255\u0256\7:\2\2\u0256\u0258\5\u0086D\2\u0257\u0254\3\2\2\2"+
		"\u0257\u0255\3\2\2\2\u0257\u0258\3\2\2\2\u0258y\3\2\2\2\u0259\u025d\5"+
		"|?\2\u025a\u025d\5~@\2\u025b\u025d\5\u0080A\2\u025c\u0259\3\2\2\2\u025c"+
		"\u025a\3\2\2\2\u025c\u025b\3\2\2\2\u025d{\3\2\2\2\u025e\u025f\t\5\2\2"+
		"\u025f}\3\2\2\2\u0260\u0261\t\6\2\2\u0261\177\3\2\2\2\u0262\u0263\t\7"+
		"\2\2\u0263\u0081\3\2\2\2\u0264\u0265\t\b\2\2\u0265\u0083\3\2\2\2\u0266"+
		"\u0267\t\t\2\2\u0267\u0085\3\2\2\2\u0268\u026b\7=\2\2\u0269\u026b\5\u0088"+
		"E\2\u026a\u0268\3\2\2\2\u026a\u0269\3\2\2\2\u026b\u0087\3\2\2\2\u026c"+
		"\u026d\t\n\2\2\u026d\u0089\3\2\2\2\u026e\u026f\t\13\2\2\u026f\u008b\3"+
		"\2\2\2C\u0091\u0096\u009b\u00a7\u00ac\u00af\u00b4\u00bf\u00c9\u00cc\u00d1"+
		"\u00d5\u00dd\u00e5\u00ef\u00f4\u00f7\u00fb\u00ff\u0101\u0108\u010e\u0110"+
		"\u011a\u011e\u0121\u0124\u0128\u0130\u0132\u0137\u0145\u014e\u015a\u015f"+
		"\u0163\u016a\u016c\u0174\u017c\u0180\u0184\u018b\u0192\u0196\u01a0\u01a6"+
		"\u01aa\u01ae\u01b8\u01c1\u01c9\u01db\u01e6\u01e8\u01f0\u01f2\u01fc\u0205"+
		"\u0242\u024b\u0251\u0257\u025c\u026a";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}