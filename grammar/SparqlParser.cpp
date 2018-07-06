
// Generated from grammar/Sparql.g4 by ANTLR 4.7.1


#include "SparqlListener.h"

#include "SparqlParser.h"


using namespace antlrcpp;
using namespace antlr4;

SparqlParser::SparqlParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SparqlParser::~SparqlParser() {
  delete _interpreter;
}

std::string SparqlParser::getGrammarFileName() const {
  return "Sparql.g4";
}

const std::vector<std::string>& SparqlParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& SparqlParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- QueryContext ------------------------------------------------------------------

SparqlParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PrologueContext* SparqlParser::QueryContext::prologue() {
  return getRuleContext<SparqlParser::PrologueContext>(0);
}

tree::TerminalNode* SparqlParser::QueryContext::EOF() {
  return getToken(SparqlParser::EOF, 0);
}

SparqlParser::SelectQueryContext* SparqlParser::QueryContext::selectQuery() {
  return getRuleContext<SparqlParser::SelectQueryContext>(0);
}

SparqlParser::ConstructQueryContext* SparqlParser::QueryContext::constructQuery() {
  return getRuleContext<SparqlParser::ConstructQueryContext>(0);
}

SparqlParser::DescribeQueryContext* SparqlParser::QueryContext::describeQuery() {
  return getRuleContext<SparqlParser::DescribeQueryContext>(0);
}

SparqlParser::AskQueryContext* SparqlParser::QueryContext::askQuery() {
  return getRuleContext<SparqlParser::AskQueryContext>(0);
}


size_t SparqlParser::QueryContext::getRuleIndex() const {
  return SparqlParser::RuleQuery;
}

void SparqlParser::QueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQuery(this);
}

void SparqlParser::QueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQuery(this);
}

SparqlParser::QueryContext* SparqlParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 0, SparqlParser::RuleQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(138);
    prologue();
    setState(143);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__2: {
        setState(139);
        selectQuery();
        break;
      }

      case SparqlParser::T__6: {
        setState(140);
        constructQuery();
        break;
      }

      case SparqlParser::T__7: {
        setState(141);
        describeQuery();
        break;
      }

      case SparqlParser::T__8: {
        setState(142);
        askQuery();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(145);
    match(SparqlParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrologueContext ------------------------------------------------------------------

SparqlParser::PrologueContext::PrologueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::BaseDeclContext* SparqlParser::PrologueContext::baseDecl() {
  return getRuleContext<SparqlParser::BaseDeclContext>(0);
}

std::vector<SparqlParser::PrefixDeclContext *> SparqlParser::PrologueContext::prefixDecl() {
  return getRuleContexts<SparqlParser::PrefixDeclContext>();
}

SparqlParser::PrefixDeclContext* SparqlParser::PrologueContext::prefixDecl(size_t i) {
  return getRuleContext<SparqlParser::PrefixDeclContext>(i);
}


size_t SparqlParser::PrologueContext::getRuleIndex() const {
  return SparqlParser::RulePrologue;
}

void SparqlParser::PrologueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrologue(this);
}

void SparqlParser::PrologueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrologue(this);
}

SparqlParser::PrologueContext* SparqlParser::prologue() {
  PrologueContext *_localctx = _tracker.createInstance<PrologueContext>(_ctx, getState());
  enterRule(_localctx, 2, SparqlParser::RulePrologue);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(148);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__0) {
      setState(147);
      baseDecl();
    }
    setState(153);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__1) {
      setState(150);
      prefixDecl();
      setState(155);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BaseDeclContext ------------------------------------------------------------------

SparqlParser::BaseDeclContext::BaseDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BaseDeclContext::IRI_REF() {
  return getToken(SparqlParser::IRI_REF, 0);
}


size_t SparqlParser::BaseDeclContext::getRuleIndex() const {
  return SparqlParser::RuleBaseDecl;
}

void SparqlParser::BaseDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBaseDecl(this);
}

void SparqlParser::BaseDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBaseDecl(this);
}

SparqlParser::BaseDeclContext* SparqlParser::baseDecl() {
  BaseDeclContext *_localctx = _tracker.createInstance<BaseDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, SparqlParser::RuleBaseDecl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(156);
    match(SparqlParser::T__0);
    setState(157);
    match(SparqlParser::IRI_REF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrefixDeclContext ------------------------------------------------------------------

SparqlParser::PrefixDeclContext::PrefixDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::PrefixDeclContext::PNAME_NS() {
  return getToken(SparqlParser::PNAME_NS, 0);
}

tree::TerminalNode* SparqlParser::PrefixDeclContext::IRI_REF() {
  return getToken(SparqlParser::IRI_REF, 0);
}


size_t SparqlParser::PrefixDeclContext::getRuleIndex() const {
  return SparqlParser::RulePrefixDecl;
}

void SparqlParser::PrefixDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrefixDecl(this);
}

void SparqlParser::PrefixDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrefixDecl(this);
}

SparqlParser::PrefixDeclContext* SparqlParser::prefixDecl() {
  PrefixDeclContext *_localctx = _tracker.createInstance<PrefixDeclContext>(_ctx, getState());
  enterRule(_localctx, 6, SparqlParser::RulePrefixDecl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(159);
    match(SparqlParser::T__1);
    setState(160);
    match(SparqlParser::PNAME_NS);
    setState(161);
    match(SparqlParser::IRI_REF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectQueryContext ------------------------------------------------------------------

SparqlParser::SelectQueryContext::SelectQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::WhereClauseContext* SparqlParser::SelectQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

SparqlParser::SolutionModifierContext* SparqlParser::SelectQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::SelectQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::SelectQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}

std::vector<SparqlParser::VarContext *> SparqlParser::SelectQueryContext::var() {
  return getRuleContexts<SparqlParser::VarContext>();
}

SparqlParser::VarContext* SparqlParser::SelectQueryContext::var(size_t i) {
  return getRuleContext<SparqlParser::VarContext>(i);
}


size_t SparqlParser::SelectQueryContext::getRuleIndex() const {
  return SparqlParser::RuleSelectQuery;
}

void SparqlParser::SelectQueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelectQuery(this);
}

void SparqlParser::SelectQueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelectQuery(this);
}

SparqlParser::SelectQueryContext* SparqlParser::selectQuery() {
  SelectQueryContext *_localctx = _tracker.createInstance<SelectQueryContext>(_ctx, getState());
  enterRule(_localctx, 8, SparqlParser::RuleSelectQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(163);
    match(SparqlParser::T__2);
    setState(165);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__3

    || _la == SparqlParser::T__4) {
      setState(164);
      _la = _input->LA(1);
      if (!(_la == SparqlParser::T__3

      || _la == SparqlParser::T__4)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(173);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        setState(168); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(167);
          var();
          setState(170); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == SparqlParser::VAR1

        || _la == SparqlParser::VAR2);
        break;
      }

      case SparqlParser::T__5: {
        setState(172);
        match(SparqlParser::T__5);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(178);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(175);
      datasetClause();
      setState(180);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(181);
    whereClause();
    setState(182);
    solutionModifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstructQueryContext ------------------------------------------------------------------

SparqlParser::ConstructQueryContext::ConstructQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ConstructTemplateContext* SparqlParser::ConstructQueryContext::constructTemplate() {
  return getRuleContext<SparqlParser::ConstructTemplateContext>(0);
}

SparqlParser::WhereClauseContext* SparqlParser::ConstructQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

SparqlParser::SolutionModifierContext* SparqlParser::ConstructQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::ConstructQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::ConstructQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}


size_t SparqlParser::ConstructQueryContext::getRuleIndex() const {
  return SparqlParser::RuleConstructQuery;
}

void SparqlParser::ConstructQueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstructQuery(this);
}

void SparqlParser::ConstructQueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstructQuery(this);
}

SparqlParser::ConstructQueryContext* SparqlParser::constructQuery() {
  ConstructQueryContext *_localctx = _tracker.createInstance<ConstructQueryContext>(_ctx, getState());
  enterRule(_localctx, 10, SparqlParser::RuleConstructQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(184);
    match(SparqlParser::T__6);
    setState(185);
    constructTemplate();
    setState(189);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(186);
      datasetClause();
      setState(191);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(192);
    whereClause();
    setState(193);
    solutionModifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DescribeQueryContext ------------------------------------------------------------------

SparqlParser::DescribeQueryContext::DescribeQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::SolutionModifierContext* SparqlParser::DescribeQueryContext::solutionModifier() {
  return getRuleContext<SparqlParser::SolutionModifierContext>(0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::DescribeQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::DescribeQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}

SparqlParser::WhereClauseContext* SparqlParser::DescribeQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

std::vector<SparqlParser::VarOrIRIrefContext *> SparqlParser::DescribeQueryContext::varOrIRIref() {
  return getRuleContexts<SparqlParser::VarOrIRIrefContext>();
}

SparqlParser::VarOrIRIrefContext* SparqlParser::DescribeQueryContext::varOrIRIref(size_t i) {
  return getRuleContext<SparqlParser::VarOrIRIrefContext>(i);
}


size_t SparqlParser::DescribeQueryContext::getRuleIndex() const {
  return SparqlParser::RuleDescribeQuery;
}

void SparqlParser::DescribeQueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDescribeQuery(this);
}

void SparqlParser::DescribeQueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDescribeQuery(this);
}

SparqlParser::DescribeQueryContext* SparqlParser::describeQuery() {
  DescribeQueryContext *_localctx = _tracker.createInstance<DescribeQueryContext>(_ctx, getState());
  enterRule(_localctx, 12, SparqlParser::RuleDescribeQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(195);
    match(SparqlParser::T__7);
    setState(202);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        setState(197); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(196);
          varOrIRIref();
          setState(199); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (((((_la - 59) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 59)) & ((1ULL << (SparqlParser::IRI_REF - 59))
          | (1ULL << (SparqlParser::PNAME_NS - 59))
          | (1ULL << (SparqlParser::PNAME_LN - 59))
          | (1ULL << (SparqlParser::VAR1 - 59))
          | (1ULL << (SparqlParser::VAR2 - 59)))) != 0));
        break;
      }

      case SparqlParser::T__5: {
        setState(201);
        match(SparqlParser::T__5);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(207);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(204);
      datasetClause();
      setState(209);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(211);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__11

    || _la == SparqlParser::T__18) {
      setState(210);
      whereClause();
    }
    setState(213);
    solutionModifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AskQueryContext ------------------------------------------------------------------

SparqlParser::AskQueryContext::AskQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::WhereClauseContext* SparqlParser::AskQueryContext::whereClause() {
  return getRuleContext<SparqlParser::WhereClauseContext>(0);
}

std::vector<SparqlParser::DatasetClauseContext *> SparqlParser::AskQueryContext::datasetClause() {
  return getRuleContexts<SparqlParser::DatasetClauseContext>();
}

SparqlParser::DatasetClauseContext* SparqlParser::AskQueryContext::datasetClause(size_t i) {
  return getRuleContext<SparqlParser::DatasetClauseContext>(i);
}


size_t SparqlParser::AskQueryContext::getRuleIndex() const {
  return SparqlParser::RuleAskQuery;
}

void SparqlParser::AskQueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAskQuery(this);
}

void SparqlParser::AskQueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAskQuery(this);
}

SparqlParser::AskQueryContext* SparqlParser::askQuery() {
  AskQueryContext *_localctx = _tracker.createInstance<AskQueryContext>(_ctx, getState());
  enterRule(_localctx, 14, SparqlParser::RuleAskQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(215);
    match(SparqlParser::T__8);
    setState(219);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(216);
      datasetClause();
      setState(221);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(222);
    whereClause();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DatasetClauseContext ------------------------------------------------------------------

SparqlParser::DatasetClauseContext::DatasetClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::DefaultGraphClauseContext* SparqlParser::DatasetClauseContext::defaultGraphClause() {
  return getRuleContext<SparqlParser::DefaultGraphClauseContext>(0);
}

SparqlParser::NamedGraphClauseContext* SparqlParser::DatasetClauseContext::namedGraphClause() {
  return getRuleContext<SparqlParser::NamedGraphClauseContext>(0);
}


size_t SparqlParser::DatasetClauseContext::getRuleIndex() const {
  return SparqlParser::RuleDatasetClause;
}

void SparqlParser::DatasetClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDatasetClause(this);
}

void SparqlParser::DatasetClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDatasetClause(this);
}

SparqlParser::DatasetClauseContext* SparqlParser::datasetClause() {
  DatasetClauseContext *_localctx = _tracker.createInstance<DatasetClauseContext>(_ctx, getState());
  enterRule(_localctx, 16, SparqlParser::RuleDatasetClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(224);
    match(SparqlParser::T__9);
    setState(227);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        setState(225);
        defaultGraphClause();
        break;
      }

      case SparqlParser::T__10: {
        setState(226);
        namedGraphClause();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefaultGraphClauseContext ------------------------------------------------------------------

SparqlParser::DefaultGraphClauseContext::DefaultGraphClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::SourceSelectorContext* SparqlParser::DefaultGraphClauseContext::sourceSelector() {
  return getRuleContext<SparqlParser::SourceSelectorContext>(0);
}


size_t SparqlParser::DefaultGraphClauseContext::getRuleIndex() const {
  return SparqlParser::RuleDefaultGraphClause;
}

void SparqlParser::DefaultGraphClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDefaultGraphClause(this);
}

void SparqlParser::DefaultGraphClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDefaultGraphClause(this);
}

SparqlParser::DefaultGraphClauseContext* SparqlParser::defaultGraphClause() {
  DefaultGraphClauseContext *_localctx = _tracker.createInstance<DefaultGraphClauseContext>(_ctx, getState());
  enterRule(_localctx, 18, SparqlParser::RuleDefaultGraphClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(229);
    sourceSelector();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedGraphClauseContext ------------------------------------------------------------------

SparqlParser::NamedGraphClauseContext::NamedGraphClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::SourceSelectorContext* SparqlParser::NamedGraphClauseContext::sourceSelector() {
  return getRuleContext<SparqlParser::SourceSelectorContext>(0);
}


size_t SparqlParser::NamedGraphClauseContext::getRuleIndex() const {
  return SparqlParser::RuleNamedGraphClause;
}

void SparqlParser::NamedGraphClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNamedGraphClause(this);
}

void SparqlParser::NamedGraphClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNamedGraphClause(this);
}

SparqlParser::NamedGraphClauseContext* SparqlParser::namedGraphClause() {
  NamedGraphClauseContext *_localctx = _tracker.createInstance<NamedGraphClauseContext>(_ctx, getState());
  enterRule(_localctx, 20, SparqlParser::RuleNamedGraphClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(231);
    match(SparqlParser::T__10);
    setState(232);
    sourceSelector();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SourceSelectorContext ------------------------------------------------------------------

SparqlParser::SourceSelectorContext::SourceSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriRefContext* SparqlParser::SourceSelectorContext::iriRef() {
  return getRuleContext<SparqlParser::IriRefContext>(0);
}


size_t SparqlParser::SourceSelectorContext::getRuleIndex() const {
  return SparqlParser::RuleSourceSelector;
}

void SparqlParser::SourceSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSourceSelector(this);
}

void SparqlParser::SourceSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSourceSelector(this);
}

SparqlParser::SourceSelectorContext* SparqlParser::sourceSelector() {
  SourceSelectorContext *_localctx = _tracker.createInstance<SourceSelectorContext>(_ctx, getState());
  enterRule(_localctx, 22, SparqlParser::RuleSourceSelector);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
    iriRef();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhereClauseContext ------------------------------------------------------------------

SparqlParser::WhereClauseContext::WhereClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GroupGraphPatternContext* SparqlParser::WhereClauseContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::WhereClauseContext::getRuleIndex() const {
  return SparqlParser::RuleWhereClause;
}

void SparqlParser::WhereClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhereClause(this);
}

void SparqlParser::WhereClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhereClause(this);
}

SparqlParser::WhereClauseContext* SparqlParser::whereClause() {
  WhereClauseContext *_localctx = _tracker.createInstance<WhereClauseContext>(_ctx, getState());
  enterRule(_localctx, 24, SparqlParser::RuleWhereClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(237);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__11) {
      setState(236);
      match(SparqlParser::T__11);
    }
    setState(239);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SolutionModifierContext ------------------------------------------------------------------

SparqlParser::SolutionModifierContext::SolutionModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::OrderClauseContext* SparqlParser::SolutionModifierContext::orderClause() {
  return getRuleContext<SparqlParser::OrderClauseContext>(0);
}

SparqlParser::LimitOffsetClausesContext* SparqlParser::SolutionModifierContext::limitOffsetClauses() {
  return getRuleContext<SparqlParser::LimitOffsetClausesContext>(0);
}


size_t SparqlParser::SolutionModifierContext::getRuleIndex() const {
  return SparqlParser::RuleSolutionModifier;
}

void SparqlParser::SolutionModifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSolutionModifier(this);
}

void SparqlParser::SolutionModifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSolutionModifier(this);
}

SparqlParser::SolutionModifierContext* SparqlParser::solutionModifier() {
  SolutionModifierContext *_localctx = _tracker.createInstance<SolutionModifierContext>(_ctx, getState());
  enterRule(_localctx, 26, SparqlParser::RuleSolutionModifier);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(242);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__12) {
      setState(241);
      orderClause();
    }
    setState(245);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__16

    || _la == SparqlParser::T__17) {
      setState(244);
      limitOffsetClauses();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LimitOffsetClausesContext ------------------------------------------------------------------

SparqlParser::LimitOffsetClausesContext::LimitOffsetClausesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::LimitClauseContext* SparqlParser::LimitOffsetClausesContext::limitClause() {
  return getRuleContext<SparqlParser::LimitClauseContext>(0);
}

SparqlParser::OffsetClauseContext* SparqlParser::LimitOffsetClausesContext::offsetClause() {
  return getRuleContext<SparqlParser::OffsetClauseContext>(0);
}


size_t SparqlParser::LimitOffsetClausesContext::getRuleIndex() const {
  return SparqlParser::RuleLimitOffsetClauses;
}

void SparqlParser::LimitOffsetClausesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLimitOffsetClauses(this);
}

void SparqlParser::LimitOffsetClausesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLimitOffsetClauses(this);
}

SparqlParser::LimitOffsetClausesContext* SparqlParser::limitOffsetClauses() {
  LimitOffsetClausesContext *_localctx = _tracker.createInstance<LimitOffsetClausesContext>(_ctx, getState());
  enterRule(_localctx, 28, SparqlParser::RuleLimitOffsetClauses);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(255);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__16: {
        setState(247);
        limitClause();
        setState(249);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::T__17) {
          setState(248);
          offsetClause();
        }
        break;
      }

      case SparqlParser::T__17: {
        setState(251);
        offsetClause();
        setState(253);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::T__16) {
          setState(252);
          limitClause();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderClauseContext ------------------------------------------------------------------

SparqlParser::OrderClauseContext::OrderClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::OrderConditionContext *> SparqlParser::OrderClauseContext::orderCondition() {
  return getRuleContexts<SparqlParser::OrderConditionContext>();
}

SparqlParser::OrderConditionContext* SparqlParser::OrderClauseContext::orderCondition(size_t i) {
  return getRuleContext<SparqlParser::OrderConditionContext>(i);
}


size_t SparqlParser::OrderClauseContext::getRuleIndex() const {
  return SparqlParser::RuleOrderClause;
}

void SparqlParser::OrderClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrderClause(this);
}

void SparqlParser::OrderClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrderClause(this);
}

SparqlParser::OrderClauseContext* SparqlParser::orderClause() {
  OrderClauseContext *_localctx = _tracker.createInstance<OrderClauseContext>(_ctx, getState());
  enterRule(_localctx, 30, SparqlParser::RuleOrderClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(257);
    match(SparqlParser::T__12);
    setState(258);
    match(SparqlParser::T__13);
    setState(260); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(259);
      orderCondition();
      setState(262); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 15) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 15)) & ((1ULL << (SparqlParser::T__14 - 15))
      | (1ULL << (SparqlParser::T__15 - 15))
      | (1ULL << (SparqlParser::T__25 - 15))
      | (1ULL << (SparqlParser::T__44 - 15))
      | (1ULL << (SparqlParser::T__45 - 15))
      | (1ULL << (SparqlParser::T__46 - 15))
      | (1ULL << (SparqlParser::T__47 - 15))
      | (1ULL << (SparqlParser::T__48 - 15))
      | (1ULL << (SparqlParser::T__49 - 15))
      | (1ULL << (SparqlParser::T__50 - 15))
      | (1ULL << (SparqlParser::T__51 - 15))
      | (1ULL << (SparqlParser::T__52 - 15))
      | (1ULL << (SparqlParser::T__53 - 15))
      | (1ULL << (SparqlParser::T__54 - 15))
      | (1ULL << (SparqlParser::IRI_REF - 15))
      | (1ULL << (SparqlParser::PNAME_NS - 15))
      | (1ULL << (SparqlParser::PNAME_LN - 15))
      | (1ULL << (SparqlParser::VAR1 - 15))
      | (1ULL << (SparqlParser::VAR2 - 15)))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderConditionContext ------------------------------------------------------------------

SparqlParser::OrderConditionContext::OrderConditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::BrackettedExpressionContext* SparqlParser::OrderConditionContext::brackettedExpression() {
  return getRuleContext<SparqlParser::BrackettedExpressionContext>(0);
}

SparqlParser::ConstraintContext* SparqlParser::OrderConditionContext::constraint() {
  return getRuleContext<SparqlParser::ConstraintContext>(0);
}

SparqlParser::VarContext* SparqlParser::OrderConditionContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}


size_t SparqlParser::OrderConditionContext::getRuleIndex() const {
  return SparqlParser::RuleOrderCondition;
}

void SparqlParser::OrderConditionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrderCondition(this);
}

void SparqlParser::OrderConditionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrderCondition(this);
}

SparqlParser::OrderConditionContext* SparqlParser::orderCondition() {
  OrderConditionContext *_localctx = _tracker.createInstance<OrderConditionContext>(_ctx, getState());
  enterRule(_localctx, 32, SparqlParser::RuleOrderCondition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(270);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__14:
      case SparqlParser::T__15: {
        enterOuterAlt(_localctx, 1);
        setState(264);
        _la = _input->LA(1);
        if (!(_la == SparqlParser::T__14

        || _la == SparqlParser::T__15)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(265);
        brackettedExpression();
        break;
      }

      case SparqlParser::T__25:
      case SparqlParser::T__44:
      case SparqlParser::T__45:
      case SparqlParser::T__46:
      case SparqlParser::T__47:
      case SparqlParser::T__48:
      case SparqlParser::T__49:
      case SparqlParser::T__50:
      case SparqlParser::T__51:
      case SparqlParser::T__52:
      case SparqlParser::T__53:
      case SparqlParser::T__54:
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 2);
        setState(268);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case SparqlParser::T__25:
          case SparqlParser::T__44:
          case SparqlParser::T__45:
          case SparqlParser::T__46:
          case SparqlParser::T__47:
          case SparqlParser::T__48:
          case SparqlParser::T__49:
          case SparqlParser::T__50:
          case SparqlParser::T__51:
          case SparqlParser::T__52:
          case SparqlParser::T__53:
          case SparqlParser::T__54:
          case SparqlParser::IRI_REF:
          case SparqlParser::PNAME_NS:
          case SparqlParser::PNAME_LN: {
            setState(266);
            constraint();
            break;
          }

          case SparqlParser::VAR1:
          case SparqlParser::VAR2: {
            setState(267);
            var();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LimitClauseContext ------------------------------------------------------------------

SparqlParser::LimitClauseContext::LimitClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::LimitClauseContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}


size_t SparqlParser::LimitClauseContext::getRuleIndex() const {
  return SparqlParser::RuleLimitClause;
}

void SparqlParser::LimitClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLimitClause(this);
}

void SparqlParser::LimitClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLimitClause(this);
}

SparqlParser::LimitClauseContext* SparqlParser::limitClause() {
  LimitClauseContext *_localctx = _tracker.createInstance<LimitClauseContext>(_ctx, getState());
  enterRule(_localctx, 34, SparqlParser::RuleLimitClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(272);
    match(SparqlParser::T__16);
    setState(273);
    match(SparqlParser::INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OffsetClauseContext ------------------------------------------------------------------

SparqlParser::OffsetClauseContext::OffsetClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::OffsetClauseContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}


size_t SparqlParser::OffsetClauseContext::getRuleIndex() const {
  return SparqlParser::RuleOffsetClause;
}

void SparqlParser::OffsetClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOffsetClause(this);
}

void SparqlParser::OffsetClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOffsetClause(this);
}

SparqlParser::OffsetClauseContext* SparqlParser::offsetClause() {
  OffsetClauseContext *_localctx = _tracker.createInstance<OffsetClauseContext>(_ctx, getState());
  enterRule(_localctx, 36, SparqlParser::RuleOffsetClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(275);
    match(SparqlParser::T__17);
    setState(276);
    match(SparqlParser::INTEGER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupGraphPatternContext ------------------------------------------------------------------

SparqlParser::GroupGraphPatternContext::GroupGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::TriplesBlockContext *> SparqlParser::GroupGraphPatternContext::triplesBlock() {
  return getRuleContexts<SparqlParser::TriplesBlockContext>();
}

SparqlParser::TriplesBlockContext* SparqlParser::GroupGraphPatternContext::triplesBlock(size_t i) {
  return getRuleContext<SparqlParser::TriplesBlockContext>(i);
}

std::vector<SparqlParser::GraphPatternNotTriplesContext *> SparqlParser::GroupGraphPatternContext::graphPatternNotTriples() {
  return getRuleContexts<SparqlParser::GraphPatternNotTriplesContext>();
}

SparqlParser::GraphPatternNotTriplesContext* SparqlParser::GroupGraphPatternContext::graphPatternNotTriples(size_t i) {
  return getRuleContext<SparqlParser::GraphPatternNotTriplesContext>(i);
}

std::vector<SparqlParser::FilterContext *> SparqlParser::GroupGraphPatternContext::filter() {
  return getRuleContexts<SparqlParser::FilterContext>();
}

SparqlParser::FilterContext* SparqlParser::GroupGraphPatternContext::filter(size_t i) {
  return getRuleContext<SparqlParser::FilterContext>(i);
}


size_t SparqlParser::GroupGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleGroupGraphPattern;
}

void SparqlParser::GroupGraphPatternContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGroupGraphPattern(this);
}

void SparqlParser::GroupGraphPatternContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGroupGraphPattern(this);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::groupGraphPattern() {
  GroupGraphPatternContext *_localctx = _tracker.createInstance<GroupGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 38, SparqlParser::RuleGroupGraphPattern);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(278);
    match(SparqlParser::T__18);
    setState(280);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 26) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 26)) & ((1ULL << (SparqlParser::T__25 - 26))
      | (1ULL << (SparqlParser::T__30 - 26))
      | (1ULL << (SparqlParser::T__56 - 26))
      | (1ULL << (SparqlParser::T__57 - 26))
      | (1ULL << (SparqlParser::IRI_REF - 26))
      | (1ULL << (SparqlParser::PNAME_NS - 26))
      | (1ULL << (SparqlParser::PNAME_LN - 26))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 26))
      | (1ULL << (SparqlParser::VAR1 - 26))
      | (1ULL << (SparqlParser::VAR2 - 26))
      | (1ULL << (SparqlParser::INTEGER - 26))
      | (1ULL << (SparqlParser::DECIMAL - 26))
      | (1ULL << (SparqlParser::DOUBLE - 26))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 26))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 26))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 26))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 26))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 26))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 26))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 26))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 26))
      | (1ULL << (SparqlParser::NIL - 26))
      | (1ULL << (SparqlParser::ANON - 26)))) != 0)) {
      setState(279);
      triplesBlock();
    }
    setState(294);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::T__18)
      | (1ULL << SparqlParser::T__21)
      | (1ULL << SparqlParser::T__22)
      | (1ULL << SparqlParser::T__24))) != 0)) {
      setState(284);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::T__18:
        case SparqlParser::T__21:
        case SparqlParser::T__22: {
          setState(282);
          graphPatternNotTriples();
          break;
        }

        case SparqlParser::T__24: {
          setState(283);
          filter();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(287);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::T__19) {
        setState(286);
        match(SparqlParser::T__19);
      }
      setState(290);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 26) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 26)) & ((1ULL << (SparqlParser::T__25 - 26))
        | (1ULL << (SparqlParser::T__30 - 26))
        | (1ULL << (SparqlParser::T__56 - 26))
        | (1ULL << (SparqlParser::T__57 - 26))
        | (1ULL << (SparqlParser::IRI_REF - 26))
        | (1ULL << (SparqlParser::PNAME_NS - 26))
        | (1ULL << (SparqlParser::PNAME_LN - 26))
        | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 26))
        | (1ULL << (SparqlParser::VAR1 - 26))
        | (1ULL << (SparqlParser::VAR2 - 26))
        | (1ULL << (SparqlParser::INTEGER - 26))
        | (1ULL << (SparqlParser::DECIMAL - 26))
        | (1ULL << (SparqlParser::DOUBLE - 26))
        | (1ULL << (SparqlParser::INTEGER_POSITIVE - 26))
        | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 26))
        | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 26))
        | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 26))
        | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 26))
        | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 26))
        | (1ULL << (SparqlParser::STRING_LITERAL1 - 26))
        | (1ULL << (SparqlParser::STRING_LITERAL2 - 26))
        | (1ULL << (SparqlParser::NIL - 26))
        | (1ULL << (SparqlParser::ANON - 26)))) != 0)) {
        setState(289);
        triplesBlock();
      }
      setState(296);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(297);
    match(SparqlParser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesBlockContext ------------------------------------------------------------------

SparqlParser::TriplesBlockContext::TriplesBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::TriplesBlockContext::triplesSameSubject() {
  return getRuleContext<SparqlParser::TriplesSameSubjectContext>(0);
}

SparqlParser::TriplesBlockContext* SparqlParser::TriplesBlockContext::triplesBlock() {
  return getRuleContext<SparqlParser::TriplesBlockContext>(0);
}


size_t SparqlParser::TriplesBlockContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesBlock;
}

void SparqlParser::TriplesBlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTriplesBlock(this);
}

void SparqlParser::TriplesBlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTriplesBlock(this);
}

SparqlParser::TriplesBlockContext* SparqlParser::triplesBlock() {
  TriplesBlockContext *_localctx = _tracker.createInstance<TriplesBlockContext>(_ctx, getState());
  enterRule(_localctx, 40, SparqlParser::RuleTriplesBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(299);
    triplesSameSubject();
    setState(304);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__19) {
      setState(300);
      match(SparqlParser::T__19);
      setState(302);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 26) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 26)) & ((1ULL << (SparqlParser::T__25 - 26))
        | (1ULL << (SparqlParser::T__30 - 26))
        | (1ULL << (SparqlParser::T__56 - 26))
        | (1ULL << (SparqlParser::T__57 - 26))
        | (1ULL << (SparqlParser::IRI_REF - 26))
        | (1ULL << (SparqlParser::PNAME_NS - 26))
        | (1ULL << (SparqlParser::PNAME_LN - 26))
        | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 26))
        | (1ULL << (SparqlParser::VAR1 - 26))
        | (1ULL << (SparqlParser::VAR2 - 26))
        | (1ULL << (SparqlParser::INTEGER - 26))
        | (1ULL << (SparqlParser::DECIMAL - 26))
        | (1ULL << (SparqlParser::DOUBLE - 26))
        | (1ULL << (SparqlParser::INTEGER_POSITIVE - 26))
        | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 26))
        | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 26))
        | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 26))
        | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 26))
        | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 26))
        | (1ULL << (SparqlParser::STRING_LITERAL1 - 26))
        | (1ULL << (SparqlParser::STRING_LITERAL2 - 26))
        | (1ULL << (SparqlParser::NIL - 26))
        | (1ULL << (SparqlParser::ANON - 26)))) != 0)) {
        setState(301);
        triplesBlock();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphPatternNotTriplesContext ------------------------------------------------------------------

SparqlParser::GraphPatternNotTriplesContext::GraphPatternNotTriplesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::OptionalGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::optionalGraphPattern() {
  return getRuleContext<SparqlParser::OptionalGraphPatternContext>(0);
}

SparqlParser::GroupOrUnionGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::groupOrUnionGraphPattern() {
  return getRuleContext<SparqlParser::GroupOrUnionGraphPatternContext>(0);
}

SparqlParser::GraphGraphPatternContext* SparqlParser::GraphPatternNotTriplesContext::graphGraphPattern() {
  return getRuleContext<SparqlParser::GraphGraphPatternContext>(0);
}


size_t SparqlParser::GraphPatternNotTriplesContext::getRuleIndex() const {
  return SparqlParser::RuleGraphPatternNotTriples;
}

void SparqlParser::GraphPatternNotTriplesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGraphPatternNotTriples(this);
}

void SparqlParser::GraphPatternNotTriplesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGraphPatternNotTriples(this);
}

SparqlParser::GraphPatternNotTriplesContext* SparqlParser::graphPatternNotTriples() {
  GraphPatternNotTriplesContext *_localctx = _tracker.createInstance<GraphPatternNotTriplesContext>(_ctx, getState());
  enterRule(_localctx, 42, SparqlParser::RuleGraphPatternNotTriples);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(309);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__21: {
        enterOuterAlt(_localctx, 1);
        setState(306);
        optionalGraphPattern();
        break;
      }

      case SparqlParser::T__18: {
        enterOuterAlt(_localctx, 2);
        setState(307);
        groupOrUnionGraphPattern();
        break;
      }

      case SparqlParser::T__22: {
        enterOuterAlt(_localctx, 3);
        setState(308);
        graphGraphPattern();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionalGraphPatternContext ------------------------------------------------------------------

SparqlParser::OptionalGraphPatternContext::OptionalGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GroupGraphPatternContext* SparqlParser::OptionalGraphPatternContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::OptionalGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleOptionalGraphPattern;
}

void SparqlParser::OptionalGraphPatternContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptionalGraphPattern(this);
}

void SparqlParser::OptionalGraphPatternContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptionalGraphPattern(this);
}

SparqlParser::OptionalGraphPatternContext* SparqlParser::optionalGraphPattern() {
  OptionalGraphPatternContext *_localctx = _tracker.createInstance<OptionalGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 44, SparqlParser::RuleOptionalGraphPattern);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(311);
    match(SparqlParser::T__21);
    setState(312);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphGraphPatternContext ------------------------------------------------------------------

SparqlParser::GraphGraphPatternContext::GraphGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrIRIrefContext* SparqlParser::GraphGraphPatternContext::varOrIRIref() {
  return getRuleContext<SparqlParser::VarOrIRIrefContext>(0);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::GraphGraphPatternContext::groupGraphPattern() {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(0);
}


size_t SparqlParser::GraphGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleGraphGraphPattern;
}

void SparqlParser::GraphGraphPatternContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGraphGraphPattern(this);
}

void SparqlParser::GraphGraphPatternContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGraphGraphPattern(this);
}

SparqlParser::GraphGraphPatternContext* SparqlParser::graphGraphPattern() {
  GraphGraphPatternContext *_localctx = _tracker.createInstance<GraphGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 46, SparqlParser::RuleGraphGraphPattern);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(314);
    match(SparqlParser::T__22);
    setState(315);
    varOrIRIref();
    setState(316);
    groupGraphPattern();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupOrUnionGraphPatternContext ------------------------------------------------------------------

SparqlParser::GroupOrUnionGraphPatternContext::GroupOrUnionGraphPatternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::GroupGraphPatternContext *> SparqlParser::GroupOrUnionGraphPatternContext::groupGraphPattern() {
  return getRuleContexts<SparqlParser::GroupGraphPatternContext>();
}

SparqlParser::GroupGraphPatternContext* SparqlParser::GroupOrUnionGraphPatternContext::groupGraphPattern(size_t i) {
  return getRuleContext<SparqlParser::GroupGraphPatternContext>(i);
}


size_t SparqlParser::GroupOrUnionGraphPatternContext::getRuleIndex() const {
  return SparqlParser::RuleGroupOrUnionGraphPattern;
}

void SparqlParser::GroupOrUnionGraphPatternContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGroupOrUnionGraphPattern(this);
}

void SparqlParser::GroupOrUnionGraphPatternContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGroupOrUnionGraphPattern(this);
}

SparqlParser::GroupOrUnionGraphPatternContext* SparqlParser::groupOrUnionGraphPattern() {
  GroupOrUnionGraphPatternContext *_localctx = _tracker.createInstance<GroupOrUnionGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 48, SparqlParser::RuleGroupOrUnionGraphPattern);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(318);
    groupGraphPattern();
    setState(323);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__23) {
      setState(319);
      match(SparqlParser::T__23);
      setState(320);
      groupGraphPattern();
      setState(325);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FilterContext ------------------------------------------------------------------

SparqlParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ConstraintContext* SparqlParser::FilterContext::constraint() {
  return getRuleContext<SparqlParser::ConstraintContext>(0);
}


size_t SparqlParser::FilterContext::getRuleIndex() const {
  return SparqlParser::RuleFilter;
}

void SparqlParser::FilterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFilter(this);
}

void SparqlParser::FilterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFilter(this);
}

SparqlParser::FilterContext* SparqlParser::filter() {
  FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, getState());
  enterRule(_localctx, 50, SparqlParser::RuleFilter);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(326);
    match(SparqlParser::T__24);
    setState(327);
    constraint();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintContext ------------------------------------------------------------------

SparqlParser::ConstraintContext::ConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::BrackettedExpressionContext* SparqlParser::ConstraintContext::brackettedExpression() {
  return getRuleContext<SparqlParser::BrackettedExpressionContext>(0);
}

SparqlParser::BuiltInCallContext* SparqlParser::ConstraintContext::builtInCall() {
  return getRuleContext<SparqlParser::BuiltInCallContext>(0);
}

SparqlParser::FunctionCallContext* SparqlParser::ConstraintContext::functionCall() {
  return getRuleContext<SparqlParser::FunctionCallContext>(0);
}


size_t SparqlParser::ConstraintContext::getRuleIndex() const {
  return SparqlParser::RuleConstraint;
}

void SparqlParser::ConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstraint(this);
}

void SparqlParser::ConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstraint(this);
}

SparqlParser::ConstraintContext* SparqlParser::constraint() {
  ConstraintContext *_localctx = _tracker.createInstance<ConstraintContext>(_ctx, getState());
  enterRule(_localctx, 52, SparqlParser::RuleConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(332);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(329);
        brackettedExpression();
        break;
      }

      case SparqlParser::T__44:
      case SparqlParser::T__45:
      case SparqlParser::T__46:
      case SparqlParser::T__47:
      case SparqlParser::T__48:
      case SparqlParser::T__49:
      case SparqlParser::T__50:
      case SparqlParser::T__51:
      case SparqlParser::T__52:
      case SparqlParser::T__53:
      case SparqlParser::T__54: {
        enterOuterAlt(_localctx, 2);
        setState(330);
        builtInCall();
        break;
      }

      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 3);
        setState(331);
        functionCall();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionCallContext ------------------------------------------------------------------

SparqlParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriRefContext* SparqlParser::FunctionCallContext::iriRef() {
  return getRuleContext<SparqlParser::IriRefContext>(0);
}

SparqlParser::ArgListContext* SparqlParser::FunctionCallContext::argList() {
  return getRuleContext<SparqlParser::ArgListContext>(0);
}


size_t SparqlParser::FunctionCallContext::getRuleIndex() const {
  return SparqlParser::RuleFunctionCall;
}

void SparqlParser::FunctionCallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionCall(this);
}

void SparqlParser::FunctionCallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionCall(this);
}

SparqlParser::FunctionCallContext* SparqlParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 54, SparqlParser::RuleFunctionCall);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(334);
    iriRef();
    setState(335);
    argList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgListContext ------------------------------------------------------------------

SparqlParser::ArgListContext::ArgListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::ArgListContext::NIL() {
  return getToken(SparqlParser::NIL, 0);
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::ArgListContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::ArgListContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}


size_t SparqlParser::ArgListContext::getRuleIndex() const {
  return SparqlParser::RuleArgList;
}

void SparqlParser::ArgListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgList(this);
}

void SparqlParser::ArgListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgList(this);
}

SparqlParser::ArgListContext* SparqlParser::argList() {
  ArgListContext *_localctx = _tracker.createInstance<ArgListContext>(_ctx, getState());
  enterRule(_localctx, 56, SparqlParser::RuleArgList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(349);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::NIL: {
        setState(337);
        match(SparqlParser::NIL);
        break;
      }

      case SparqlParser::T__25: {
        setState(338);
        match(SparqlParser::T__25);
        setState(339);
        expression();
        setState(344);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SparqlParser::T__26) {
          setState(340);
          match(SparqlParser::T__26);
          setState(341);
          expression();
          setState(346);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(347);
        match(SparqlParser::T__27);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstructTemplateContext ------------------------------------------------------------------

SparqlParser::ConstructTemplateContext::ConstructTemplateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ConstructTriplesContext* SparqlParser::ConstructTemplateContext::constructTriples() {
  return getRuleContext<SparqlParser::ConstructTriplesContext>(0);
}


size_t SparqlParser::ConstructTemplateContext::getRuleIndex() const {
  return SparqlParser::RuleConstructTemplate;
}

void SparqlParser::ConstructTemplateContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstructTemplate(this);
}

void SparqlParser::ConstructTemplateContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstructTemplate(this);
}

SparqlParser::ConstructTemplateContext* SparqlParser::constructTemplate() {
  ConstructTemplateContext *_localctx = _tracker.createInstance<ConstructTemplateContext>(_ctx, getState());
  enterRule(_localctx, 58, SparqlParser::RuleConstructTemplate);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(351);
    match(SparqlParser::T__18);
    setState(353);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 26) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 26)) & ((1ULL << (SparqlParser::T__25 - 26))
      | (1ULL << (SparqlParser::T__30 - 26))
      | (1ULL << (SparqlParser::T__56 - 26))
      | (1ULL << (SparqlParser::T__57 - 26))
      | (1ULL << (SparqlParser::IRI_REF - 26))
      | (1ULL << (SparqlParser::PNAME_NS - 26))
      | (1ULL << (SparqlParser::PNAME_LN - 26))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 26))
      | (1ULL << (SparqlParser::VAR1 - 26))
      | (1ULL << (SparqlParser::VAR2 - 26))
      | (1ULL << (SparqlParser::INTEGER - 26))
      | (1ULL << (SparqlParser::DECIMAL - 26))
      | (1ULL << (SparqlParser::DOUBLE - 26))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 26))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 26))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 26))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 26))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 26))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 26))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 26))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 26))
      | (1ULL << (SparqlParser::NIL - 26))
      | (1ULL << (SparqlParser::ANON - 26)))) != 0)) {
      setState(352);
      constructTriples();
    }
    setState(355);
    match(SparqlParser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstructTriplesContext ------------------------------------------------------------------

SparqlParser::ConstructTriplesContext::ConstructTriplesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::ConstructTriplesContext::triplesSameSubject() {
  return getRuleContext<SparqlParser::TriplesSameSubjectContext>(0);
}

SparqlParser::ConstructTriplesContext* SparqlParser::ConstructTriplesContext::constructTriples() {
  return getRuleContext<SparqlParser::ConstructTriplesContext>(0);
}


size_t SparqlParser::ConstructTriplesContext::getRuleIndex() const {
  return SparqlParser::RuleConstructTriples;
}

void SparqlParser::ConstructTriplesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstructTriples(this);
}

void SparqlParser::ConstructTriplesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstructTriples(this);
}

SparqlParser::ConstructTriplesContext* SparqlParser::constructTriples() {
  ConstructTriplesContext *_localctx = _tracker.createInstance<ConstructTriplesContext>(_ctx, getState());
  enterRule(_localctx, 60, SparqlParser::RuleConstructTriples);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(357);
    triplesSameSubject();
    setState(362);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__19) {
      setState(358);
      match(SparqlParser::T__19);
      setState(360);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 26) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 26)) & ((1ULL << (SparqlParser::T__25 - 26))
        | (1ULL << (SparqlParser::T__30 - 26))
        | (1ULL << (SparqlParser::T__56 - 26))
        | (1ULL << (SparqlParser::T__57 - 26))
        | (1ULL << (SparqlParser::IRI_REF - 26))
        | (1ULL << (SparqlParser::PNAME_NS - 26))
        | (1ULL << (SparqlParser::PNAME_LN - 26))
        | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 26))
        | (1ULL << (SparqlParser::VAR1 - 26))
        | (1ULL << (SparqlParser::VAR2 - 26))
        | (1ULL << (SparqlParser::INTEGER - 26))
        | (1ULL << (SparqlParser::DECIMAL - 26))
        | (1ULL << (SparqlParser::DOUBLE - 26))
        | (1ULL << (SparqlParser::INTEGER_POSITIVE - 26))
        | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 26))
        | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 26))
        | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 26))
        | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 26))
        | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 26))
        | (1ULL << (SparqlParser::STRING_LITERAL1 - 26))
        | (1ULL << (SparqlParser::STRING_LITERAL2 - 26))
        | (1ULL << (SparqlParser::NIL - 26))
        | (1ULL << (SparqlParser::ANON - 26)))) != 0)) {
        setState(359);
        constructTriples();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesSameSubjectContext ------------------------------------------------------------------

SparqlParser::TriplesSameSubjectContext::TriplesSameSubjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrTermContext* SparqlParser::TriplesSameSubjectContext::varOrTerm() {
  return getRuleContext<SparqlParser::VarOrTermContext>(0);
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::TriplesSameSubjectContext::propertyListNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListNotEmptyContext>(0);
}

SparqlParser::TriplesNodeContext* SparqlParser::TriplesSameSubjectContext::triplesNode() {
  return getRuleContext<SparqlParser::TriplesNodeContext>(0);
}

SparqlParser::PropertyListContext* SparqlParser::TriplesSameSubjectContext::propertyList() {
  return getRuleContext<SparqlParser::PropertyListContext>(0);
}


size_t SparqlParser::TriplesSameSubjectContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesSameSubject;
}

void SparqlParser::TriplesSameSubjectContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTriplesSameSubject(this);
}

void SparqlParser::TriplesSameSubjectContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTriplesSameSubject(this);
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::triplesSameSubject() {
  TriplesSameSubjectContext *_localctx = _tracker.createInstance<TriplesSameSubjectContext>(_ctx, getState());
  enterRule(_localctx, 62, SparqlParser::RuleTriplesSameSubject);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(370);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__56:
      case SparqlParser::T__57:
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::NIL:
      case SparqlParser::ANON: {
        enterOuterAlt(_localctx, 1);
        setState(364);
        varOrTerm();
        setState(365);
        propertyListNotEmpty();
        break;
      }

      case SparqlParser::T__25:
      case SparqlParser::T__30: {
        enterOuterAlt(_localctx, 2);
        setState(367);
        triplesNode();
        setState(368);
        propertyList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListNotEmptyContext ------------------------------------------------------------------

SparqlParser::PropertyListNotEmptyContext::PropertyListNotEmptyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::VerbContext *> SparqlParser::PropertyListNotEmptyContext::verb() {
  return getRuleContexts<SparqlParser::VerbContext>();
}

SparqlParser::VerbContext* SparqlParser::PropertyListNotEmptyContext::verb(size_t i) {
  return getRuleContext<SparqlParser::VerbContext>(i);
}

std::vector<SparqlParser::ObjectListContext *> SparqlParser::PropertyListNotEmptyContext::objectList() {
  return getRuleContexts<SparqlParser::ObjectListContext>();
}

SparqlParser::ObjectListContext* SparqlParser::PropertyListNotEmptyContext::objectList(size_t i) {
  return getRuleContext<SparqlParser::ObjectListContext>(i);
}


size_t SparqlParser::PropertyListNotEmptyContext::getRuleIndex() const {
  return SparqlParser::RulePropertyListNotEmpty;
}

void SparqlParser::PropertyListNotEmptyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyListNotEmpty(this);
}

void SparqlParser::PropertyListNotEmptyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyListNotEmpty(this);
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::propertyListNotEmpty() {
  PropertyListNotEmptyContext *_localctx = _tracker.createInstance<PropertyListNotEmptyContext>(_ctx, getState());
  enterRule(_localctx, 64, SparqlParser::RulePropertyListNotEmpty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(372);
    verb();
    setState(373);
    objectList();
    setState(382);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__28) {
      setState(374);
      match(SparqlParser::T__28);
      setState(378);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 30) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 30)) & ((1ULL << (SparqlParser::T__29 - 30))
        | (1ULL << (SparqlParser::IRI_REF - 30))
        | (1ULL << (SparqlParser::PNAME_NS - 30))
        | (1ULL << (SparqlParser::PNAME_LN - 30))
        | (1ULL << (SparqlParser::VAR1 - 30))
        | (1ULL << (SparqlParser::VAR2 - 30)))) != 0)) {
        setState(375);
        verb();
        setState(376);
        objectList();
      }
      setState(384);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListContext ------------------------------------------------------------------

SparqlParser::PropertyListContext::PropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::PropertyListContext::propertyListNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListNotEmptyContext>(0);
}


size_t SparqlParser::PropertyListContext::getRuleIndex() const {
  return SparqlParser::RulePropertyList;
}

void SparqlParser::PropertyListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyList(this);
}

void SparqlParser::PropertyListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyList(this);
}

SparqlParser::PropertyListContext* SparqlParser::propertyList() {
  PropertyListContext *_localctx = _tracker.createInstance<PropertyListContext>(_ctx, getState());
  enterRule(_localctx, 66, SparqlParser::RulePropertyList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(386);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 30) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 30)) & ((1ULL << (SparqlParser::T__29 - 30))
      | (1ULL << (SparqlParser::IRI_REF - 30))
      | (1ULL << (SparqlParser::PNAME_NS - 30))
      | (1ULL << (SparqlParser::PNAME_LN - 30))
      | (1ULL << (SparqlParser::VAR1 - 30))
      | (1ULL << (SparqlParser::VAR2 - 30)))) != 0)) {
      setState(385);
      propertyListNotEmpty();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectListContext ------------------------------------------------------------------

SparqlParser::ObjectListContext::ObjectListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ObjectContext *> SparqlParser::ObjectListContext::object() {
  return getRuleContexts<SparqlParser::ObjectContext>();
}

SparqlParser::ObjectContext* SparqlParser::ObjectListContext::object(size_t i) {
  return getRuleContext<SparqlParser::ObjectContext>(i);
}


size_t SparqlParser::ObjectListContext::getRuleIndex() const {
  return SparqlParser::RuleObjectList;
}

void SparqlParser::ObjectListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterObjectList(this);
}

void SparqlParser::ObjectListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitObjectList(this);
}

SparqlParser::ObjectListContext* SparqlParser::objectList() {
  ObjectListContext *_localctx = _tracker.createInstance<ObjectListContext>(_ctx, getState());
  enterRule(_localctx, 68, SparqlParser::RuleObjectList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(388);
    object();
    setState(393);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__26) {
      setState(389);
      match(SparqlParser::T__26);
      setState(390);
      object();
      setState(395);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectContext ------------------------------------------------------------------

SparqlParser::ObjectContext::ObjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::GraphNodeContext* SparqlParser::ObjectContext::graphNode() {
  return getRuleContext<SparqlParser::GraphNodeContext>(0);
}


size_t SparqlParser::ObjectContext::getRuleIndex() const {
  return SparqlParser::RuleObject;
}

void SparqlParser::ObjectContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterObject(this);
}

void SparqlParser::ObjectContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitObject(this);
}

SparqlParser::ObjectContext* SparqlParser::object() {
  ObjectContext *_localctx = _tracker.createInstance<ObjectContext>(_ctx, getState());
  enterRule(_localctx, 70, SparqlParser::RuleObject);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(396);
    graphNode();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VerbContext ------------------------------------------------------------------

SparqlParser::VerbContext::VerbContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrIRIrefContext* SparqlParser::VerbContext::varOrIRIref() {
  return getRuleContext<SparqlParser::VarOrIRIrefContext>(0);
}


size_t SparqlParser::VerbContext::getRuleIndex() const {
  return SparqlParser::RuleVerb;
}

void SparqlParser::VerbContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVerb(this);
}

void SparqlParser::VerbContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVerb(this);
}

SparqlParser::VerbContext* SparqlParser::verb() {
  VerbContext *_localctx = _tracker.createInstance<VerbContext>(_ctx, getState());
  enterRule(_localctx, 72, SparqlParser::RuleVerb);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(400);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(398);
        varOrIRIref();
        break;
      }

      case SparqlParser::T__29: {
        enterOuterAlt(_localctx, 2);
        setState(399);
        match(SparqlParser::T__29);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TriplesNodeContext ------------------------------------------------------------------

SparqlParser::TriplesNodeContext::TriplesNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::CollectionContext* SparqlParser::TriplesNodeContext::collection() {
  return getRuleContext<SparqlParser::CollectionContext>(0);
}

SparqlParser::BlankNodePropertyListContext* SparqlParser::TriplesNodeContext::blankNodePropertyList() {
  return getRuleContext<SparqlParser::BlankNodePropertyListContext>(0);
}


size_t SparqlParser::TriplesNodeContext::getRuleIndex() const {
  return SparqlParser::RuleTriplesNode;
}

void SparqlParser::TriplesNodeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTriplesNode(this);
}

void SparqlParser::TriplesNodeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTriplesNode(this);
}

SparqlParser::TriplesNodeContext* SparqlParser::triplesNode() {
  TriplesNodeContext *_localctx = _tracker.createInstance<TriplesNodeContext>(_ctx, getState());
  enterRule(_localctx, 74, SparqlParser::RuleTriplesNode);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(404);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(402);
        collection();
        break;
      }

      case SparqlParser::T__30: {
        enterOuterAlt(_localctx, 2);
        setState(403);
        blankNodePropertyList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlankNodePropertyListContext ------------------------------------------------------------------

SparqlParser::BlankNodePropertyListContext::BlankNodePropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::BlankNodePropertyListContext::propertyListNotEmpty() {
  return getRuleContext<SparqlParser::PropertyListNotEmptyContext>(0);
}


size_t SparqlParser::BlankNodePropertyListContext::getRuleIndex() const {
  return SparqlParser::RuleBlankNodePropertyList;
}

void SparqlParser::BlankNodePropertyListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlankNodePropertyList(this);
}

void SparqlParser::BlankNodePropertyListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlankNodePropertyList(this);
}

SparqlParser::BlankNodePropertyListContext* SparqlParser::blankNodePropertyList() {
  BlankNodePropertyListContext *_localctx = _tracker.createInstance<BlankNodePropertyListContext>(_ctx, getState());
  enterRule(_localctx, 76, SparqlParser::RuleBlankNodePropertyList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(406);
    match(SparqlParser::T__30);
    setState(407);
    propertyListNotEmpty();
    setState(408);
    match(SparqlParser::T__31);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionContext ------------------------------------------------------------------

SparqlParser::CollectionContext::CollectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::GraphNodeContext *> SparqlParser::CollectionContext::graphNode() {
  return getRuleContexts<SparqlParser::GraphNodeContext>();
}

SparqlParser::GraphNodeContext* SparqlParser::CollectionContext::graphNode(size_t i) {
  return getRuleContext<SparqlParser::GraphNodeContext>(i);
}


size_t SparqlParser::CollectionContext::getRuleIndex() const {
  return SparqlParser::RuleCollection;
}

void SparqlParser::CollectionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCollection(this);
}

void SparqlParser::CollectionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCollection(this);
}

SparqlParser::CollectionContext* SparqlParser::collection() {
  CollectionContext *_localctx = _tracker.createInstance<CollectionContext>(_ctx, getState());
  enterRule(_localctx, 78, SparqlParser::RuleCollection);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(410);
    match(SparqlParser::T__25);
    setState(412); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(411);
      graphNode();
      setState(414); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 26) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 26)) & ((1ULL << (SparqlParser::T__25 - 26))
      | (1ULL << (SparqlParser::T__30 - 26))
      | (1ULL << (SparqlParser::T__56 - 26))
      | (1ULL << (SparqlParser::T__57 - 26))
      | (1ULL << (SparqlParser::IRI_REF - 26))
      | (1ULL << (SparqlParser::PNAME_NS - 26))
      | (1ULL << (SparqlParser::PNAME_LN - 26))
      | (1ULL << (SparqlParser::BLANK_NODE_LABEL - 26))
      | (1ULL << (SparqlParser::VAR1 - 26))
      | (1ULL << (SparqlParser::VAR2 - 26))
      | (1ULL << (SparqlParser::INTEGER - 26))
      | (1ULL << (SparqlParser::DECIMAL - 26))
      | (1ULL << (SparqlParser::DOUBLE - 26))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 26))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 26))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 26))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 26))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 26))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 26))
      | (1ULL << (SparqlParser::STRING_LITERAL1 - 26))
      | (1ULL << (SparqlParser::STRING_LITERAL2 - 26))
      | (1ULL << (SparqlParser::NIL - 26))
      | (1ULL << (SparqlParser::ANON - 26)))) != 0));
    setState(416);
    match(SparqlParser::T__27);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphNodeContext ------------------------------------------------------------------

SparqlParser::GraphNodeContext::GraphNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarOrTermContext* SparqlParser::GraphNodeContext::varOrTerm() {
  return getRuleContext<SparqlParser::VarOrTermContext>(0);
}

SparqlParser::TriplesNodeContext* SparqlParser::GraphNodeContext::triplesNode() {
  return getRuleContext<SparqlParser::TriplesNodeContext>(0);
}


size_t SparqlParser::GraphNodeContext::getRuleIndex() const {
  return SparqlParser::RuleGraphNode;
}

void SparqlParser::GraphNodeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGraphNode(this);
}

void SparqlParser::GraphNodeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGraphNode(this);
}

SparqlParser::GraphNodeContext* SparqlParser::graphNode() {
  GraphNodeContext *_localctx = _tracker.createInstance<GraphNodeContext>(_ctx, getState());
  enterRule(_localctx, 80, SparqlParser::RuleGraphNode);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(420);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__56:
      case SparqlParser::T__57:
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::NIL:
      case SparqlParser::ANON: {
        enterOuterAlt(_localctx, 1);
        setState(418);
        varOrTerm();
        break;
      }

      case SparqlParser::T__25:
      case SparqlParser::T__30: {
        enterOuterAlt(_localctx, 2);
        setState(419);
        triplesNode();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarOrTermContext ------------------------------------------------------------------

SparqlParser::VarOrTermContext::VarOrTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarContext* SparqlParser::VarOrTermContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

SparqlParser::GraphTermContext* SparqlParser::VarOrTermContext::graphTerm() {
  return getRuleContext<SparqlParser::GraphTermContext>(0);
}


size_t SparqlParser::VarOrTermContext::getRuleIndex() const {
  return SparqlParser::RuleVarOrTerm;
}

void SparqlParser::VarOrTermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarOrTerm(this);
}

void SparqlParser::VarOrTermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarOrTerm(this);
}

SparqlParser::VarOrTermContext* SparqlParser::varOrTerm() {
  VarOrTermContext *_localctx = _tracker.createInstance<VarOrTermContext>(_ctx, getState());
  enterRule(_localctx, 82, SparqlParser::RuleVarOrTerm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(424);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(422);
        var();
        break;
      }

      case SparqlParser::T__56:
      case SparqlParser::T__57:
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::NIL:
      case SparqlParser::ANON: {
        enterOuterAlt(_localctx, 2);
        setState(423);
        graphTerm();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarOrIRIrefContext ------------------------------------------------------------------

SparqlParser::VarOrIRIrefContext::VarOrIRIrefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::VarContext* SparqlParser::VarOrIRIrefContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

SparqlParser::IriRefContext* SparqlParser::VarOrIRIrefContext::iriRef() {
  return getRuleContext<SparqlParser::IriRefContext>(0);
}


size_t SparqlParser::VarOrIRIrefContext::getRuleIndex() const {
  return SparqlParser::RuleVarOrIRIref;
}

void SparqlParser::VarOrIRIrefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarOrIRIref(this);
}

void SparqlParser::VarOrIRIrefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarOrIRIref(this);
}

SparqlParser::VarOrIRIrefContext* SparqlParser::varOrIRIref() {
  VarOrIRIrefContext *_localctx = _tracker.createInstance<VarOrIRIrefContext>(_ctx, getState());
  enterRule(_localctx, 84, SparqlParser::RuleVarOrIRIref);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(428);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(426);
        var();
        break;
      }

      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(427);
        iriRef();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarContext ------------------------------------------------------------------

SparqlParser::VarContext::VarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::VarContext::VAR1() {
  return getToken(SparqlParser::VAR1, 0);
}

tree::TerminalNode* SparqlParser::VarContext::VAR2() {
  return getToken(SparqlParser::VAR2, 0);
}


size_t SparqlParser::VarContext::getRuleIndex() const {
  return SparqlParser::RuleVar;
}

void SparqlParser::VarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar(this);
}

void SparqlParser::VarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar(this);
}

SparqlParser::VarContext* SparqlParser::var() {
  VarContext *_localctx = _tracker.createInstance<VarContext>(_ctx, getState());
  enterRule(_localctx, 86, SparqlParser::RuleVar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(430);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::VAR1

    || _la == SparqlParser::VAR2)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GraphTermContext ------------------------------------------------------------------

SparqlParser::GraphTermContext::GraphTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriRefContext* SparqlParser::GraphTermContext::iriRef() {
  return getRuleContext<SparqlParser::IriRefContext>(0);
}

SparqlParser::RdfLiteralContext* SparqlParser::GraphTermContext::rdfLiteral() {
  return getRuleContext<SparqlParser::RdfLiteralContext>(0);
}

SparqlParser::NumericLiteralContext* SparqlParser::GraphTermContext::numericLiteral() {
  return getRuleContext<SparqlParser::NumericLiteralContext>(0);
}

SparqlParser::BooleanLiteralContext* SparqlParser::GraphTermContext::booleanLiteral() {
  return getRuleContext<SparqlParser::BooleanLiteralContext>(0);
}

SparqlParser::BlankNodeContext* SparqlParser::GraphTermContext::blankNode() {
  return getRuleContext<SparqlParser::BlankNodeContext>(0);
}

tree::TerminalNode* SparqlParser::GraphTermContext::NIL() {
  return getToken(SparqlParser::NIL, 0);
}


size_t SparqlParser::GraphTermContext::getRuleIndex() const {
  return SparqlParser::RuleGraphTerm;
}

void SparqlParser::GraphTermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGraphTerm(this);
}

void SparqlParser::GraphTermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGraphTerm(this);
}

SparqlParser::GraphTermContext* SparqlParser::graphTerm() {
  GraphTermContext *_localctx = _tracker.createInstance<GraphTermContext>(_ctx, getState());
  enterRule(_localctx, 88, SparqlParser::RuleGraphTerm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(438);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(432);
        iriRef();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2: {
        enterOuterAlt(_localctx, 2);
        setState(433);
        rdfLiteral();
        break;
      }

      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(434);
        numericLiteral();
        break;
      }

      case SparqlParser::T__56:
      case SparqlParser::T__57: {
        enterOuterAlt(_localctx, 4);
        setState(435);
        booleanLiteral();
        break;
      }

      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::ANON: {
        enterOuterAlt(_localctx, 5);
        setState(436);
        blankNode();
        break;
      }

      case SparqlParser::NIL: {
        enterOuterAlt(_localctx, 6);
        setState(437);
        match(SparqlParser::NIL);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

SparqlParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ConditionalOrExpressionContext* SparqlParser::ExpressionContext::conditionalOrExpression() {
  return getRuleContext<SparqlParser::ConditionalOrExpressionContext>(0);
}


size_t SparqlParser::ExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleExpression;
}

void SparqlParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void SparqlParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}

SparqlParser::ExpressionContext* SparqlParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 90, SparqlParser::RuleExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(440);
    conditionalOrExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalOrExpressionContext ------------------------------------------------------------------

SparqlParser::ConditionalOrExpressionContext::ConditionalOrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ConditionalAndExpressionContext *> SparqlParser::ConditionalOrExpressionContext::conditionalAndExpression() {
  return getRuleContexts<SparqlParser::ConditionalAndExpressionContext>();
}

SparqlParser::ConditionalAndExpressionContext* SparqlParser::ConditionalOrExpressionContext::conditionalAndExpression(size_t i) {
  return getRuleContext<SparqlParser::ConditionalAndExpressionContext>(i);
}


size_t SparqlParser::ConditionalOrExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleConditionalOrExpression;
}

void SparqlParser::ConditionalOrExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConditionalOrExpression(this);
}

void SparqlParser::ConditionalOrExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConditionalOrExpression(this);
}

SparqlParser::ConditionalOrExpressionContext* SparqlParser::conditionalOrExpression() {
  ConditionalOrExpressionContext *_localctx = _tracker.createInstance<ConditionalOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 92, SparqlParser::RuleConditionalOrExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(442);
    conditionalAndExpression();
    setState(447);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__32) {
      setState(443);
      match(SparqlParser::T__32);
      setState(444);
      conditionalAndExpression();
      setState(449);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalAndExpressionContext ------------------------------------------------------------------

SparqlParser::ConditionalAndExpressionContext::ConditionalAndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ValueLogicalContext *> SparqlParser::ConditionalAndExpressionContext::valueLogical() {
  return getRuleContexts<SparqlParser::ValueLogicalContext>();
}

SparqlParser::ValueLogicalContext* SparqlParser::ConditionalAndExpressionContext::valueLogical(size_t i) {
  return getRuleContext<SparqlParser::ValueLogicalContext>(i);
}


size_t SparqlParser::ConditionalAndExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleConditionalAndExpression;
}

void SparqlParser::ConditionalAndExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConditionalAndExpression(this);
}

void SparqlParser::ConditionalAndExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConditionalAndExpression(this);
}

SparqlParser::ConditionalAndExpressionContext* SparqlParser::conditionalAndExpression() {
  ConditionalAndExpressionContext *_localctx = _tracker.createInstance<ConditionalAndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 94, SparqlParser::RuleConditionalAndExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(450);
    valueLogical();
    setState(455);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__33) {
      setState(451);
      match(SparqlParser::T__33);
      setState(452);
      valueLogical();
      setState(457);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueLogicalContext ------------------------------------------------------------------

SparqlParser::ValueLogicalContext::ValueLogicalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::RelationalExpressionContext* SparqlParser::ValueLogicalContext::relationalExpression() {
  return getRuleContext<SparqlParser::RelationalExpressionContext>(0);
}


size_t SparqlParser::ValueLogicalContext::getRuleIndex() const {
  return SparqlParser::RuleValueLogical;
}

void SparqlParser::ValueLogicalContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValueLogical(this);
}

void SparqlParser::ValueLogicalContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValueLogical(this);
}

SparqlParser::ValueLogicalContext* SparqlParser::valueLogical() {
  ValueLogicalContext *_localctx = _tracker.createInstance<ValueLogicalContext>(_ctx, getState());
  enterRule(_localctx, 96, SparqlParser::RuleValueLogical);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(458);
    relationalExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationalExpressionContext ------------------------------------------------------------------

SparqlParser::RelationalExpressionContext::RelationalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::NumericExpressionContext *> SparqlParser::RelationalExpressionContext::numericExpression() {
  return getRuleContexts<SparqlParser::NumericExpressionContext>();
}

SparqlParser::NumericExpressionContext* SparqlParser::RelationalExpressionContext::numericExpression(size_t i) {
  return getRuleContext<SparqlParser::NumericExpressionContext>(i);
}


size_t SparqlParser::RelationalExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleRelationalExpression;
}

void SparqlParser::RelationalExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelationalExpression(this);
}

void SparqlParser::RelationalExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelationalExpression(this);
}

SparqlParser::RelationalExpressionContext* SparqlParser::relationalExpression() {
  RelationalExpressionContext *_localctx = _tracker.createInstance<RelationalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 98, SparqlParser::RuleRelationalExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(460);
    numericExpression();
    setState(473);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__34: {
        setState(461);
        match(SparqlParser::T__34);
        setState(462);
        numericExpression();
        break;
      }

      case SparqlParser::T__35: {
        setState(463);
        match(SparqlParser::T__35);
        setState(464);
        numericExpression();
        break;
      }

      case SparqlParser::T__36: {
        setState(465);
        match(SparqlParser::T__36);
        setState(466);
        numericExpression();
        break;
      }

      case SparqlParser::T__37: {
        setState(467);
        match(SparqlParser::T__37);
        setState(468);
        numericExpression();
        break;
      }

      case SparqlParser::T__38: {
        setState(469);
        match(SparqlParser::T__38);
        setState(470);
        numericExpression();
        break;
      }

      case SparqlParser::T__39: {
        setState(471);
        match(SparqlParser::T__39);
        setState(472);
        numericExpression();
        break;
      }

      case SparqlParser::T__26:
      case SparqlParser::T__27:
      case SparqlParser::T__32:
      case SparqlParser::T__33: {
        break;
      }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericExpressionContext ------------------------------------------------------------------

SparqlParser::NumericExpressionContext::NumericExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::AdditiveExpressionContext* SparqlParser::NumericExpressionContext::additiveExpression() {
  return getRuleContext<SparqlParser::AdditiveExpressionContext>(0);
}


size_t SparqlParser::NumericExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleNumericExpression;
}

void SparqlParser::NumericExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericExpression(this);
}

void SparqlParser::NumericExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericExpression(this);
}

SparqlParser::NumericExpressionContext* SparqlParser::numericExpression() {
  NumericExpressionContext *_localctx = _tracker.createInstance<NumericExpressionContext>(_ctx, getState());
  enterRule(_localctx, 100, SparqlParser::RuleNumericExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(475);
    additiveExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExpressionContext ------------------------------------------------------------------

SparqlParser::AdditiveExpressionContext::AdditiveExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::MultiplicativeExpressionContext *> SparqlParser::AdditiveExpressionContext::multiplicativeExpression() {
  return getRuleContexts<SparqlParser::MultiplicativeExpressionContext>();
}

SparqlParser::MultiplicativeExpressionContext* SparqlParser::AdditiveExpressionContext::multiplicativeExpression(size_t i) {
  return getRuleContext<SparqlParser::MultiplicativeExpressionContext>(i);
}

std::vector<SparqlParser::NumericLiteralPositiveContext *> SparqlParser::AdditiveExpressionContext::numericLiteralPositive() {
  return getRuleContexts<SparqlParser::NumericLiteralPositiveContext>();
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::AdditiveExpressionContext::numericLiteralPositive(size_t i) {
  return getRuleContext<SparqlParser::NumericLiteralPositiveContext>(i);
}

std::vector<SparqlParser::NumericLiteralNegativeContext *> SparqlParser::AdditiveExpressionContext::numericLiteralNegative() {
  return getRuleContexts<SparqlParser::NumericLiteralNegativeContext>();
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::AdditiveExpressionContext::numericLiteralNegative(size_t i) {
  return getRuleContext<SparqlParser::NumericLiteralNegativeContext>(i);
}


size_t SparqlParser::AdditiveExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleAdditiveExpression;
}

void SparqlParser::AdditiveExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAdditiveExpression(this);
}

void SparqlParser::AdditiveExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAdditiveExpression(this);
}

SparqlParser::AdditiveExpressionContext* SparqlParser::additiveExpression() {
  AdditiveExpressionContext *_localctx = _tracker.createInstance<AdditiveExpressionContext>(_ctx, getState());
  enterRule(_localctx, 102, SparqlParser::RuleAdditiveExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(477);
    multiplicativeExpression();
    setState(486);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 41) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 41)) & ((1ULL << (SparqlParser::T__40 - 41))
      | (1ULL << (SparqlParser::T__41 - 41))
      | (1ULL << (SparqlParser::INTEGER_POSITIVE - 41))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 41))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 41))
      | (1ULL << (SparqlParser::INTEGER_NEGATIVE - 41))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 41))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 41)))) != 0)) {
      setState(484);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::T__40: {
          setState(478);
          match(SparqlParser::T__40);
          setState(479);
          multiplicativeExpression();
          break;
        }

        case SparqlParser::T__41: {
          setState(480);
          match(SparqlParser::T__41);
          setState(481);
          multiplicativeExpression();
          break;
        }

        case SparqlParser::INTEGER_POSITIVE:
        case SparqlParser::DECIMAL_POSITIVE:
        case SparqlParser::DOUBLE_POSITIVE: {
          setState(482);
          numericLiteralPositive();
          break;
        }

        case SparqlParser::INTEGER_NEGATIVE:
        case SparqlParser::DECIMAL_NEGATIVE:
        case SparqlParser::DOUBLE_NEGATIVE: {
          setState(483);
          numericLiteralNegative();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(488);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeExpressionContext ------------------------------------------------------------------

SparqlParser::MultiplicativeExpressionContext::MultiplicativeExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::UnaryExpressionContext *> SparqlParser::MultiplicativeExpressionContext::unaryExpression() {
  return getRuleContexts<SparqlParser::UnaryExpressionContext>();
}

SparqlParser::UnaryExpressionContext* SparqlParser::MultiplicativeExpressionContext::unaryExpression(size_t i) {
  return getRuleContext<SparqlParser::UnaryExpressionContext>(i);
}


size_t SparqlParser::MultiplicativeExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleMultiplicativeExpression;
}

void SparqlParser::MultiplicativeExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplicativeExpression(this);
}

void SparqlParser::MultiplicativeExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplicativeExpression(this);
}

SparqlParser::MultiplicativeExpressionContext* SparqlParser::multiplicativeExpression() {
  MultiplicativeExpressionContext *_localctx = _tracker.createInstance<MultiplicativeExpressionContext>(_ctx, getState());
  enterRule(_localctx, 104, SparqlParser::RuleMultiplicativeExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(489);
    unaryExpression();
    setState(496);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__5

    || _la == SparqlParser::T__42) {
      setState(494);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::T__5: {
          setState(490);
          match(SparqlParser::T__5);
          setState(491);
          unaryExpression();
          break;
        }

        case SparqlParser::T__42: {
          setState(492);
          match(SparqlParser::T__42);
          setState(493);
          unaryExpression();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(498);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpressionContext ------------------------------------------------------------------

SparqlParser::UnaryExpressionContext::UnaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::PrimaryExpressionContext* SparqlParser::UnaryExpressionContext::primaryExpression() {
  return getRuleContext<SparqlParser::PrimaryExpressionContext>(0);
}


size_t SparqlParser::UnaryExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleUnaryExpression;
}

void SparqlParser::UnaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryExpression(this);
}

void SparqlParser::UnaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryExpression(this);
}

SparqlParser::UnaryExpressionContext* SparqlParser::unaryExpression() {
  UnaryExpressionContext *_localctx = _tracker.createInstance<UnaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 106, SparqlParser::RuleUnaryExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(506);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__43: {
        enterOuterAlt(_localctx, 1);
        setState(499);
        match(SparqlParser::T__43);
        setState(500);
        primaryExpression();
        break;
      }

      case SparqlParser::T__40: {
        enterOuterAlt(_localctx, 2);
        setState(501);
        match(SparqlParser::T__40);
        setState(502);
        primaryExpression();
        break;
      }

      case SparqlParser::T__41: {
        enterOuterAlt(_localctx, 3);
        setState(503);
        match(SparqlParser::T__41);
        setState(504);
        primaryExpression();
        break;
      }

      case SparqlParser::T__25:
      case SparqlParser::T__44:
      case SparqlParser::T__45:
      case SparqlParser::T__46:
      case SparqlParser::T__47:
      case SparqlParser::T__48:
      case SparqlParser::T__49:
      case SparqlParser::T__50:
      case SparqlParser::T__51:
      case SparqlParser::T__52:
      case SparqlParser::T__53:
      case SparqlParser::T__54:
      case SparqlParser::T__56:
      case SparqlParser::T__57:
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2: {
        enterOuterAlt(_localctx, 4);
        setState(505);
        primaryExpression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpressionContext ------------------------------------------------------------------

SparqlParser::PrimaryExpressionContext::PrimaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::BrackettedExpressionContext* SparqlParser::PrimaryExpressionContext::brackettedExpression() {
  return getRuleContext<SparqlParser::BrackettedExpressionContext>(0);
}

SparqlParser::BuiltInCallContext* SparqlParser::PrimaryExpressionContext::builtInCall() {
  return getRuleContext<SparqlParser::BuiltInCallContext>(0);
}

SparqlParser::IriRefOrFunctionContext* SparqlParser::PrimaryExpressionContext::iriRefOrFunction() {
  return getRuleContext<SparqlParser::IriRefOrFunctionContext>(0);
}

SparqlParser::RdfLiteralContext* SparqlParser::PrimaryExpressionContext::rdfLiteral() {
  return getRuleContext<SparqlParser::RdfLiteralContext>(0);
}

SparqlParser::NumericLiteralContext* SparqlParser::PrimaryExpressionContext::numericLiteral() {
  return getRuleContext<SparqlParser::NumericLiteralContext>(0);
}

SparqlParser::BooleanLiteralContext* SparqlParser::PrimaryExpressionContext::booleanLiteral() {
  return getRuleContext<SparqlParser::BooleanLiteralContext>(0);
}

SparqlParser::VarContext* SparqlParser::PrimaryExpressionContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}


size_t SparqlParser::PrimaryExpressionContext::getRuleIndex() const {
  return SparqlParser::RulePrimaryExpression;
}

void SparqlParser::PrimaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimaryExpression(this);
}

void SparqlParser::PrimaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimaryExpression(this);
}

SparqlParser::PrimaryExpressionContext* SparqlParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 108, SparqlParser::RulePrimaryExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(515);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(508);
        brackettedExpression();
        break;
      }

      case SparqlParser::T__44:
      case SparqlParser::T__45:
      case SparqlParser::T__46:
      case SparqlParser::T__47:
      case SparqlParser::T__48:
      case SparqlParser::T__49:
      case SparqlParser::T__50:
      case SparqlParser::T__51:
      case SparqlParser::T__52:
      case SparqlParser::T__53:
      case SparqlParser::T__54: {
        enterOuterAlt(_localctx, 2);
        setState(509);
        builtInCall();
        break;
      }

      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 3);
        setState(510);
        iriRefOrFunction();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2: {
        enterOuterAlt(_localctx, 4);
        setState(511);
        rdfLiteral();
        break;
      }

      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 5);
        setState(512);
        numericLiteral();
        break;
      }

      case SparqlParser::T__56:
      case SparqlParser::T__57: {
        enterOuterAlt(_localctx, 6);
        setState(513);
        booleanLiteral();
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 7);
        setState(514);
        var();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BrackettedExpressionContext ------------------------------------------------------------------

SparqlParser::BrackettedExpressionContext::BrackettedExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::ExpressionContext* SparqlParser::BrackettedExpressionContext::expression() {
  return getRuleContext<SparqlParser::ExpressionContext>(0);
}


size_t SparqlParser::BrackettedExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleBrackettedExpression;
}

void SparqlParser::BrackettedExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBrackettedExpression(this);
}

void SparqlParser::BrackettedExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBrackettedExpression(this);
}

SparqlParser::BrackettedExpressionContext* SparqlParser::brackettedExpression() {
  BrackettedExpressionContext *_localctx = _tracker.createInstance<BrackettedExpressionContext>(_ctx, getState());
  enterRule(_localctx, 110, SparqlParser::RuleBrackettedExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(517);
    match(SparqlParser::T__25);
    setState(518);
    expression();
    setState(519);
    match(SparqlParser::T__27);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BuiltInCallContext ------------------------------------------------------------------

SparqlParser::BuiltInCallContext::BuiltInCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::BuiltInCallContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::BuiltInCallContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}

SparqlParser::VarContext* SparqlParser::BuiltInCallContext::var() {
  return getRuleContext<SparqlParser::VarContext>(0);
}

SparqlParser::RegexExpressionContext* SparqlParser::BuiltInCallContext::regexExpression() {
  return getRuleContext<SparqlParser::RegexExpressionContext>(0);
}


size_t SparqlParser::BuiltInCallContext::getRuleIndex() const {
  return SparqlParser::RuleBuiltInCall;
}

void SparqlParser::BuiltInCallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBuiltInCall(this);
}

void SparqlParser::BuiltInCallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBuiltInCall(this);
}

SparqlParser::BuiltInCallContext* SparqlParser::builtInCall() {
  BuiltInCallContext *_localctx = _tracker.createInstance<BuiltInCallContext>(_ctx, getState());
  enterRule(_localctx, 112, SparqlParser::RuleBuiltInCall);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(576);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__44: {
        enterOuterAlt(_localctx, 1);
        setState(521);
        match(SparqlParser::T__44);
        setState(522);
        match(SparqlParser::T__25);
        setState(523);
        expression();
        setState(524);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__45: {
        enterOuterAlt(_localctx, 2);
        setState(526);
        match(SparqlParser::T__45);
        setState(527);
        match(SparqlParser::T__25);
        setState(528);
        expression();
        setState(529);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__46: {
        enterOuterAlt(_localctx, 3);
        setState(531);
        match(SparqlParser::T__46);
        setState(532);
        match(SparqlParser::T__25);
        setState(533);
        expression();
        setState(534);
        match(SparqlParser::T__26);
        setState(535);
        expression();
        setState(536);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__47: {
        enterOuterAlt(_localctx, 4);
        setState(538);
        match(SparqlParser::T__47);
        setState(539);
        match(SparqlParser::T__25);
        setState(540);
        expression();
        setState(541);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__48: {
        enterOuterAlt(_localctx, 5);
        setState(543);
        match(SparqlParser::T__48);
        setState(544);
        match(SparqlParser::T__25);
        setState(545);
        var();
        setState(546);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__49: {
        enterOuterAlt(_localctx, 6);
        setState(548);
        match(SparqlParser::T__49);
        setState(549);
        match(SparqlParser::T__25);
        setState(550);
        expression();
        setState(551);
        match(SparqlParser::T__26);
        setState(552);
        expression();
        setState(553);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__50: {
        enterOuterAlt(_localctx, 7);
        setState(555);
        match(SparqlParser::T__50);
        setState(556);
        match(SparqlParser::T__25);
        setState(557);
        expression();
        setState(558);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__51: {
        enterOuterAlt(_localctx, 8);
        setState(560);
        match(SparqlParser::T__51);
        setState(561);
        match(SparqlParser::T__25);
        setState(562);
        expression();
        setState(563);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__52: {
        enterOuterAlt(_localctx, 9);
        setState(565);
        match(SparqlParser::T__52);
        setState(566);
        match(SparqlParser::T__25);
        setState(567);
        expression();
        setState(568);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__53: {
        enterOuterAlt(_localctx, 10);
        setState(570);
        match(SparqlParser::T__53);
        setState(571);
        match(SparqlParser::T__25);
        setState(572);
        expression();
        setState(573);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__54: {
        enterOuterAlt(_localctx, 11);
        setState(575);
        regexExpression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RegexExpressionContext ------------------------------------------------------------------

SparqlParser::RegexExpressionContext::RegexExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SparqlParser::ExpressionContext *> SparqlParser::RegexExpressionContext::expression() {
  return getRuleContexts<SparqlParser::ExpressionContext>();
}

SparqlParser::ExpressionContext* SparqlParser::RegexExpressionContext::expression(size_t i) {
  return getRuleContext<SparqlParser::ExpressionContext>(i);
}


size_t SparqlParser::RegexExpressionContext::getRuleIndex() const {
  return SparqlParser::RuleRegexExpression;
}

void SparqlParser::RegexExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRegexExpression(this);
}

void SparqlParser::RegexExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRegexExpression(this);
}

SparqlParser::RegexExpressionContext* SparqlParser::regexExpression() {
  RegexExpressionContext *_localctx = _tracker.createInstance<RegexExpressionContext>(_ctx, getState());
  enterRule(_localctx, 114, SparqlParser::RuleRegexExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(578);
    match(SparqlParser::T__54);
    setState(579);
    match(SparqlParser::T__25);
    setState(580);
    expression();
    setState(581);
    match(SparqlParser::T__26);
    setState(582);
    expression();
    setState(585);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__26) {
      setState(583);
      match(SparqlParser::T__26);
      setState(584);
      expression();
    }
    setState(587);
    match(SparqlParser::T__27);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IriRefOrFunctionContext ------------------------------------------------------------------

SparqlParser::IriRefOrFunctionContext::IriRefOrFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::IriRefContext* SparqlParser::IriRefOrFunctionContext::iriRef() {
  return getRuleContext<SparqlParser::IriRefContext>(0);
}

SparqlParser::ArgListContext* SparqlParser::IriRefOrFunctionContext::argList() {
  return getRuleContext<SparqlParser::ArgListContext>(0);
}


size_t SparqlParser::IriRefOrFunctionContext::getRuleIndex() const {
  return SparqlParser::RuleIriRefOrFunction;
}

void SparqlParser::IriRefOrFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIriRefOrFunction(this);
}

void SparqlParser::IriRefOrFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIriRefOrFunction(this);
}

SparqlParser::IriRefOrFunctionContext* SparqlParser::iriRefOrFunction() {
  IriRefOrFunctionContext *_localctx = _tracker.createInstance<IriRefOrFunctionContext>(_ctx, getState());
  enterRule(_localctx, 116, SparqlParser::RuleIriRefOrFunction);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(589);
    iriRef();
    setState(591);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__25

    || _la == SparqlParser::NIL) {
      setState(590);
      argList();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RdfLiteralContext ------------------------------------------------------------------

SparqlParser::RdfLiteralContext::RdfLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::StringContext* SparqlParser::RdfLiteralContext::string() {
  return getRuleContext<SparqlParser::StringContext>(0);
}

tree::TerminalNode* SparqlParser::RdfLiteralContext::LANGTAG() {
  return getToken(SparqlParser::LANGTAG, 0);
}

SparqlParser::IriRefContext* SparqlParser::RdfLiteralContext::iriRef() {
  return getRuleContext<SparqlParser::IriRefContext>(0);
}


size_t SparqlParser::RdfLiteralContext::getRuleIndex() const {
  return SparqlParser::RuleRdfLiteral;
}

void SparqlParser::RdfLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRdfLiteral(this);
}

void SparqlParser::RdfLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRdfLiteral(this);
}

SparqlParser::RdfLiteralContext* SparqlParser::rdfLiteral() {
  RdfLiteralContext *_localctx = _tracker.createInstance<RdfLiteralContext>(_ctx, getState());
  enterRule(_localctx, 118, SparqlParser::RuleRdfLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(593);
    string();
    setState(597);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::LANGTAG: {
        setState(594);
        match(SparqlParser::LANGTAG);
        break;
      }

      case SparqlParser::T__55: {
        setState(595);
        match(SparqlParser::T__55);
        setState(596);
        iriRef();
        break;
      }

      case SparqlParser::T__5:
      case SparqlParser::T__18:
      case SparqlParser::T__19:
      case SparqlParser::T__20:
      case SparqlParser::T__21:
      case SparqlParser::T__22:
      case SparqlParser::T__24:
      case SparqlParser::T__25:
      case SparqlParser::T__26:
      case SparqlParser::T__27:
      case SparqlParser::T__28:
      case SparqlParser::T__29:
      case SparqlParser::T__30:
      case SparqlParser::T__31:
      case SparqlParser::T__32:
      case SparqlParser::T__33:
      case SparqlParser::T__34:
      case SparqlParser::T__35:
      case SparqlParser::T__36:
      case SparqlParser::T__37:
      case SparqlParser::T__38:
      case SparqlParser::T__39:
      case SparqlParser::T__40:
      case SparqlParser::T__41:
      case SparqlParser::T__42:
      case SparqlParser::T__56:
      case SparqlParser::T__57:
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2:
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE:
      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE:
      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE:
      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2:
      case SparqlParser::NIL:
      case SparqlParser::ANON: {
        break;
      }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericLiteralContext ------------------------------------------------------------------

SparqlParser::NumericLiteralContext::NumericLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SparqlParser::NumericLiteralUnsignedContext* SparqlParser::NumericLiteralContext::numericLiteralUnsigned() {
  return getRuleContext<SparqlParser::NumericLiteralUnsignedContext>(0);
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::NumericLiteralContext::numericLiteralPositive() {
  return getRuleContext<SparqlParser::NumericLiteralPositiveContext>(0);
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::NumericLiteralContext::numericLiteralNegative() {
  return getRuleContext<SparqlParser::NumericLiteralNegativeContext>(0);
}


size_t SparqlParser::NumericLiteralContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteral;
}

void SparqlParser::NumericLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericLiteral(this);
}

void SparqlParser::NumericLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericLiteral(this);
}

SparqlParser::NumericLiteralContext* SparqlParser::numericLiteral() {
  NumericLiteralContext *_localctx = _tracker.createInstance<NumericLiteralContext>(_ctx, getState());
  enterRule(_localctx, 120, SparqlParser::RuleNumericLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(602);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE: {
        enterOuterAlt(_localctx, 1);
        setState(599);
        numericLiteralUnsigned();
        break;
      }

      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE: {
        enterOuterAlt(_localctx, 2);
        setState(600);
        numericLiteralPositive();
        break;
      }

      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(601);
        numericLiteralNegative();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericLiteralUnsignedContext ------------------------------------------------------------------

SparqlParser::NumericLiteralUnsignedContext::NumericLiteralUnsignedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NumericLiteralUnsignedContext::INTEGER() {
  return getToken(SparqlParser::INTEGER, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralUnsignedContext::DECIMAL() {
  return getToken(SparqlParser::DECIMAL, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralUnsignedContext::DOUBLE() {
  return getToken(SparqlParser::DOUBLE, 0);
}


size_t SparqlParser::NumericLiteralUnsignedContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteralUnsigned;
}

void SparqlParser::NumericLiteralUnsignedContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericLiteralUnsigned(this);
}

void SparqlParser::NumericLiteralUnsignedContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericLiteralUnsigned(this);
}

SparqlParser::NumericLiteralUnsignedContext* SparqlParser::numericLiteralUnsigned() {
  NumericLiteralUnsignedContext *_localctx = _tracker.createInstance<NumericLiteralUnsignedContext>(_ctx, getState());
  enterRule(_localctx, 122, SparqlParser::RuleNumericLiteralUnsigned);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(604);
    _la = _input->LA(1);
    if (!(((((_la - 66) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 66)) & ((1ULL << (SparqlParser::INTEGER - 66))
      | (1ULL << (SparqlParser::DECIMAL - 66))
      | (1ULL << (SparqlParser::DOUBLE - 66)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericLiteralPositiveContext ------------------------------------------------------------------

SparqlParser::NumericLiteralPositiveContext::NumericLiteralPositiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NumericLiteralPositiveContext::INTEGER_POSITIVE() {
  return getToken(SparqlParser::INTEGER_POSITIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralPositiveContext::DECIMAL_POSITIVE() {
  return getToken(SparqlParser::DECIMAL_POSITIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralPositiveContext::DOUBLE_POSITIVE() {
  return getToken(SparqlParser::DOUBLE_POSITIVE, 0);
}


size_t SparqlParser::NumericLiteralPositiveContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteralPositive;
}

void SparqlParser::NumericLiteralPositiveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericLiteralPositive(this);
}

void SparqlParser::NumericLiteralPositiveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericLiteralPositive(this);
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::numericLiteralPositive() {
  NumericLiteralPositiveContext *_localctx = _tracker.createInstance<NumericLiteralPositiveContext>(_ctx, getState());
  enterRule(_localctx, 124, SparqlParser::RuleNumericLiteralPositive);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(606);
    _la = _input->LA(1);
    if (!(((((_la - 69) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 69)) & ((1ULL << (SparqlParser::INTEGER_POSITIVE - 69))
      | (1ULL << (SparqlParser::DECIMAL_POSITIVE - 69))
      | (1ULL << (SparqlParser::DOUBLE_POSITIVE - 69)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericLiteralNegativeContext ------------------------------------------------------------------

SparqlParser::NumericLiteralNegativeContext::NumericLiteralNegativeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::NumericLiteralNegativeContext::INTEGER_NEGATIVE() {
  return getToken(SparqlParser::INTEGER_NEGATIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralNegativeContext::DECIMAL_NEGATIVE() {
  return getToken(SparqlParser::DECIMAL_NEGATIVE, 0);
}

tree::TerminalNode* SparqlParser::NumericLiteralNegativeContext::DOUBLE_NEGATIVE() {
  return getToken(SparqlParser::DOUBLE_NEGATIVE, 0);
}


size_t SparqlParser::NumericLiteralNegativeContext::getRuleIndex() const {
  return SparqlParser::RuleNumericLiteralNegative;
}

void SparqlParser::NumericLiteralNegativeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericLiteralNegative(this);
}

void SparqlParser::NumericLiteralNegativeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericLiteralNegative(this);
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::numericLiteralNegative() {
  NumericLiteralNegativeContext *_localctx = _tracker.createInstance<NumericLiteralNegativeContext>(_ctx, getState());
  enterRule(_localctx, 126, SparqlParser::RuleNumericLiteralNegative);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(608);
    _la = _input->LA(1);
    if (!(((((_la - 72) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 72)) & ((1ULL << (SparqlParser::INTEGER_NEGATIVE - 72))
      | (1ULL << (SparqlParser::DECIMAL_NEGATIVE - 72))
      | (1ULL << (SparqlParser::DOUBLE_NEGATIVE - 72)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanLiteralContext ------------------------------------------------------------------

SparqlParser::BooleanLiteralContext::BooleanLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SparqlParser::BooleanLiteralContext::getRuleIndex() const {
  return SparqlParser::RuleBooleanLiteral;
}

void SparqlParser::BooleanLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBooleanLiteral(this);
}

void SparqlParser::BooleanLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBooleanLiteral(this);
}

SparqlParser::BooleanLiteralContext* SparqlParser::booleanLiteral() {
  BooleanLiteralContext *_localctx = _tracker.createInstance<BooleanLiteralContext>(_ctx, getState());
  enterRule(_localctx, 128, SparqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(610);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::T__56

    || _la == SparqlParser::T__57)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringContext ------------------------------------------------------------------

SparqlParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::StringContext::STRING_LITERAL1() {
  return getToken(SparqlParser::STRING_LITERAL1, 0);
}

tree::TerminalNode* SparqlParser::StringContext::STRING_LITERAL2() {
  return getToken(SparqlParser::STRING_LITERAL2, 0);
}


size_t SparqlParser::StringContext::getRuleIndex() const {
  return SparqlParser::RuleString;
}

void SparqlParser::StringContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterString(this);
}

void SparqlParser::StringContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitString(this);
}

SparqlParser::StringContext* SparqlParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 130, SparqlParser::RuleString);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(612);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::STRING_LITERAL1

    || _la == SparqlParser::STRING_LITERAL2)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IriRefContext ------------------------------------------------------------------

SparqlParser::IriRefContext::IriRefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::IriRefContext::IRI_REF() {
  return getToken(SparqlParser::IRI_REF, 0);
}

SparqlParser::PrefixedNameContext* SparqlParser::IriRefContext::prefixedName() {
  return getRuleContext<SparqlParser::PrefixedNameContext>(0);
}


size_t SparqlParser::IriRefContext::getRuleIndex() const {
  return SparqlParser::RuleIriRef;
}

void SparqlParser::IriRefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIriRef(this);
}

void SparqlParser::IriRefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIriRef(this);
}

SparqlParser::IriRefContext* SparqlParser::iriRef() {
  IriRefContext *_localctx = _tracker.createInstance<IriRefContext>(_ctx, getState());
  enterRule(_localctx, 132, SparqlParser::RuleIriRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(616);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF: {
        enterOuterAlt(_localctx, 1);
        setState(614);
        match(SparqlParser::IRI_REF);
        break;
      }

      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(615);
        prefixedName();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrefixedNameContext ------------------------------------------------------------------

SparqlParser::PrefixedNameContext::PrefixedNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::PrefixedNameContext::PNAME_LN() {
  return getToken(SparqlParser::PNAME_LN, 0);
}

tree::TerminalNode* SparqlParser::PrefixedNameContext::PNAME_NS() {
  return getToken(SparqlParser::PNAME_NS, 0);
}


size_t SparqlParser::PrefixedNameContext::getRuleIndex() const {
  return SparqlParser::RulePrefixedName;
}

void SparqlParser::PrefixedNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrefixedName(this);
}

void SparqlParser::PrefixedNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrefixedName(this);
}

SparqlParser::PrefixedNameContext* SparqlParser::prefixedName() {
  PrefixedNameContext *_localctx = _tracker.createInstance<PrefixedNameContext>(_ctx, getState());
  enterRule(_localctx, 134, SparqlParser::RulePrefixedName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(618);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::PNAME_NS

    || _la == SparqlParser::PNAME_LN)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlankNodeContext ------------------------------------------------------------------

SparqlParser::BlankNodeContext::BlankNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SparqlParser::BlankNodeContext::BLANK_NODE_LABEL() {
  return getToken(SparqlParser::BLANK_NODE_LABEL, 0);
}

tree::TerminalNode* SparqlParser::BlankNodeContext::ANON() {
  return getToken(SparqlParser::ANON, 0);
}


size_t SparqlParser::BlankNodeContext::getRuleIndex() const {
  return SparqlParser::RuleBlankNode;
}

void SparqlParser::BlankNodeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlankNode(this);
}

void SparqlParser::BlankNodeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlankNode(this);
}

SparqlParser::BlankNodeContext* SparqlParser::blankNode() {
  BlankNodeContext *_localctx = _tracker.createInstance<BlankNodeContext>(_ctx, getState());
  enterRule(_localctx, 136, SparqlParser::RuleBlankNode);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(620);
    _la = _input->LA(1);
    if (!(_la == SparqlParser::BLANK_NODE_LABEL

    || _la == SparqlParser::ANON)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> SparqlParser::_decisionToDFA;
atn::PredictionContextCache SparqlParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SparqlParser::_atn;
std::vector<uint16_t> SparqlParser::_serializedATN;

std::vector<std::string> SparqlParser::_ruleNames = {
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

std::vector<std::string> SparqlParser::_literalNames = {
  "", "'BASE'", "'PREFIX'", "'SELECT'", "'DISTINCT'", "'REDUCED'", "'*'", 
  "'CONSTRUCT'", "'DESCRIBE'", "'ASK'", "'FROM'", "'NAMED'", "'WHERE'", 
  "'ORDER'", "'BY'", "'ASC'", "'DESC'", "'LIMIT'", "'OFFSET'", "'{'", "'.'", 
  "'}'", "'OPTIONAL'", "'GRAPH'", "'UNION'", "'FILTER'", "'('", "','", "')'", 
  "';'", "'a'", "'['", "']'", "'||'", "'&&'", "'='", "'!='", "'<'", "'>'", 
  "'<='", "'>='", "'+'", "'-'", "'/'", "'!'", "'STR'", "'LANG'", "'LANGMATCHES'", 
  "'DATATYPE'", "'BOUND'", "'sameTerm'", "'isIRI'", "'isURI'", "'isBLANK'", 
  "'isLITERAL'", "'REGEX'", "'^^'", "'true'", "'false'"
};

std::vector<std::string> SparqlParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "IRI_REF", "PNAME_NS", "PNAME_LN", "BLANK_NODE_LABEL", 
  "VAR1", "VAR2", "LANGTAG", "INTEGER", "DECIMAL", "DOUBLE", "INTEGER_POSITIVE", 
  "DECIMAL_POSITIVE", "DOUBLE_POSITIVE", "INTEGER_NEGATIVE", "DECIMAL_NEGATIVE", 
  "DOUBLE_NEGATIVE", "EXPONENT", "STRING_LITERAL1", "STRING_LITERAL2", "STRING_LITERAL_LONG1", 
  "STRING_LITERAL_LONG2", "ECHAR", "NIL", "ANON", "PN_CHARS_U", "VARNAME", 
  "PN_PREFIX", "PN_LOCAL", "WS"
};

dfa::Vocabulary SparqlParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SparqlParser::_tokenNames;

SparqlParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x59, 0x271, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 
    0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 
    0x25, 0x4, 0x26, 0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 
    0x4, 0x29, 0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 
    0x2c, 0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
    0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 0x9, 
    0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 0x9, 0x35, 
    0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 0x9, 0x38, 0x4, 
    0x39, 0x9, 0x39, 0x4, 0x3a, 0x9, 0x3a, 0x4, 0x3b, 0x9, 0x3b, 0x4, 0x3c, 
    0x9, 0x3c, 0x4, 0x3d, 0x9, 0x3d, 0x4, 0x3e, 0x9, 0x3e, 0x4, 0x3f, 0x9, 
    0x3f, 0x4, 0x40, 0x9, 0x40, 0x4, 0x41, 0x9, 0x41, 0x4, 0x42, 0x9, 0x42, 
    0x4, 0x43, 0x9, 0x43, 0x4, 0x44, 0x9, 0x44, 0x4, 0x45, 0x9, 0x45, 0x4, 
    0x46, 0x9, 0x46, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 
    0x2, 0x92, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x5, 0x3, 0x97, 0xa, 
    0x3, 0x3, 0x3, 0x7, 0x3, 0x9a, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x9d, 0xb, 
    0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0xa8, 0xa, 0x6, 0x3, 0x6, 0x6, 0x6, 
    0xab, 0xa, 0x6, 0xd, 0x6, 0xe, 0x6, 0xac, 0x3, 0x6, 0x5, 0x6, 0xb0, 
    0xa, 0x6, 0x3, 0x6, 0x7, 0x6, 0xb3, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0xb6, 
    0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x7, 0x7, 0xbe, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0xc1, 0xb, 0x7, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x6, 0x8, 0xc8, 0xa, 0x8, 0xd, 
    0x8, 0xe, 0x8, 0xc9, 0x3, 0x8, 0x5, 0x8, 0xcd, 0xa, 0x8, 0x3, 0x8, 0x7, 
    0x8, 0xd0, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0xd3, 0xb, 0x8, 0x3, 0x8, 0x5, 
    0x8, 0xd6, 0xa, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 
    0xdc, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0xdf, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0xe6, 0xa, 0xa, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x5, 
    0xe, 0xf0, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x5, 0xf, 0xf5, 0xa, 
    0xf, 0x3, 0xf, 0x5, 0xf, 0xf8, 0xa, 0xf, 0x3, 0x10, 0x3, 0x10, 0x5, 
    0x10, 0xfc, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x100, 0xa, 
    0x10, 0x5, 0x10, 0x102, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x6, 0x11, 0x107, 0xa, 0x11, 0xd, 0x11, 0xe, 0x11, 0x108, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x10f, 0xa, 0x12, 0x5, 0x12, 
    0x111, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x11b, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x5, 0x15, 0x11f, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 0x122, 
    0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 0x125, 0xa, 0x15, 0x7, 0x15, 0x127, 
    0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x12a, 0xb, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x131, 0xa, 0x16, 0x5, 0x16, 
    0x133, 0xa, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x138, 
    0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x7, 0x1a, 0x144, 
    0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0x147, 0xb, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x14f, 0xa, 0x1c, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 
    0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x159, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 
    0x15c, 0xb, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x160, 0xa, 0x1e, 
    0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x164, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x16b, 0xa, 0x20, 0x5, 0x20, 
    0x16d, 0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
    0x3, 0x21, 0x5, 0x21, 0x175, 0xa, 0x21, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x17d, 0xa, 0x22, 0x7, 0x22, 
    0x17f, 0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 0x182, 0xb, 0x22, 0x3, 0x23, 
    0x5, 0x23, 0x185, 0xa, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 
    0x18a, 0xa, 0x24, 0xc, 0x24, 0xe, 0x24, 0x18d, 0xb, 0x24, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x193, 0xa, 0x26, 0x3, 0x27, 
    0x3, 0x27, 0x5, 0x27, 0x197, 0xa, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x6, 0x29, 0x19f, 0xa, 0x29, 0xd, 0x29, 
    0xe, 0x29, 0x1a0, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 
    0x1a7, 0xa, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x1ab, 0xa, 0x2b, 
    0x3, 0x2c, 0x3, 0x2c, 0x5, 0x2c, 0x1af, 0xa, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x5, 
    0x2e, 0x1b9, 0xa, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x7, 0x30, 0x1c0, 0xa, 0x30, 0xc, 0x30, 0xe, 0x30, 0x1c3, 
    0xb, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 0x1c8, 0xa, 0x31, 
    0xc, 0x31, 0xe, 0x31, 0x1cb, 0xb, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 
    0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 
    0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 
    0x1dc, 0xa, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 
    0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x7, 0x35, 0x1e7, 0xa, 0x35, 
    0xc, 0x35, 0xe, 0x35, 0x1ea, 0xb, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 
    0x3, 0x36, 0x3, 0x36, 0x7, 0x36, 0x1f1, 0xa, 0x36, 0xc, 0x36, 0xe, 0x36, 
    0x1f4, 0xb, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 0x1fd, 0xa, 0x37, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x206, 
    0xa, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x3a, 0x3, 
    0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 
    0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 
    0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 
    0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 
    0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x243, 0xa, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x5, 0x3b, 0x24c, 
    0xa, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x252, 
    0xa, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x258, 
    0xa, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x25d, 0xa, 0x3e, 
    0x3, 0x3f, 0x3, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 
    0x42, 0x3, 0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x44, 0x3, 0x44, 0x5, 0x44, 
    0x26b, 0xa, 0x44, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x46, 
    0x2, 0x2, 0x47, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 
    0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 
    0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 
    0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 
    0x5e, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 
    0x76, 0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x2, 
    0xc, 0x3, 0x2, 0x6, 0x7, 0x3, 0x2, 0x11, 0x12, 0x3, 0x2, 0x41, 0x42, 
    0x3, 0x2, 0x44, 0x46, 0x3, 0x2, 0x47, 0x49, 0x3, 0x2, 0x4a, 0x4c, 0x3, 
    0x2, 0x3b, 0x3c, 0x3, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x3e, 0x3f, 0x4, 0x2, 
    0x40, 0x40, 0x54, 0x54, 0x2, 0x28d, 0x2, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x96, 0x3, 0x2, 0x2, 0x2, 0x6, 0x9e, 0x3, 0x2, 0x2, 0x2, 0x8, 0xa1, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xc, 0xba, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x10, 0xd9, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0xe2, 0x3, 0x2, 0x2, 0x2, 0x14, 0xe7, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0xe9, 0x3, 0x2, 0x2, 0x2, 0x18, 0xec, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0xef, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xf4, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x101, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x103, 0x3, 0x2, 0x2, 0x2, 0x22, 0x110, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0x112, 0x3, 0x2, 0x2, 0x2, 0x26, 0x115, 0x3, 0x2, 
    0x2, 0x2, 0x28, 0x118, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x12d, 0x3, 0x2, 0x2, 
    0x2, 0x2c, 0x137, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x139, 0x3, 0x2, 0x2, 0x2, 
    0x30, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x32, 0x140, 0x3, 0x2, 0x2, 0x2, 0x34, 
    0x148, 0x3, 0x2, 0x2, 0x2, 0x36, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x38, 0x150, 
    0x3, 0x2, 0x2, 0x2, 0x3a, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x161, 0x3, 
    0x2, 0x2, 0x2, 0x3e, 0x167, 0x3, 0x2, 0x2, 0x2, 0x40, 0x174, 0x3, 0x2, 
    0x2, 0x2, 0x42, 0x176, 0x3, 0x2, 0x2, 0x2, 0x44, 0x184, 0x3, 0x2, 0x2, 
    0x2, 0x46, 0x186, 0x3, 0x2, 0x2, 0x2, 0x48, 0x18e, 0x3, 0x2, 0x2, 0x2, 
    0x4a, 0x192, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x196, 0x3, 0x2, 0x2, 0x2, 0x4e, 
    0x198, 0x3, 0x2, 0x2, 0x2, 0x50, 0x19c, 0x3, 0x2, 0x2, 0x2, 0x52, 0x1a6, 
    0x3, 0x2, 0x2, 0x2, 0x54, 0x1aa, 0x3, 0x2, 0x2, 0x2, 0x56, 0x1ae, 0x3, 
    0x2, 0x2, 0x2, 0x58, 0x1b0, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x1b8, 0x3, 0x2, 
    0x2, 0x2, 0x5c, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x1bc, 0x3, 0x2, 0x2, 
    0x2, 0x60, 0x1c4, 0x3, 0x2, 0x2, 0x2, 0x62, 0x1cc, 0x3, 0x2, 0x2, 0x2, 
    0x64, 0x1ce, 0x3, 0x2, 0x2, 0x2, 0x66, 0x1dd, 0x3, 0x2, 0x2, 0x2, 0x68, 
    0x1df, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x1fc, 
    0x3, 0x2, 0x2, 0x2, 0x6e, 0x205, 0x3, 0x2, 0x2, 0x2, 0x70, 0x207, 0x3, 
    0x2, 0x2, 0x2, 0x72, 0x242, 0x3, 0x2, 0x2, 0x2, 0x74, 0x244, 0x3, 0x2, 
    0x2, 0x2, 0x76, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x78, 0x253, 0x3, 0x2, 0x2, 
    0x2, 0x7a, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x25e, 0x3, 0x2, 0x2, 0x2, 
    0x7e, 0x260, 0x3, 0x2, 0x2, 0x2, 0x80, 0x262, 0x3, 0x2, 0x2, 0x2, 0x82, 
    0x264, 0x3, 0x2, 0x2, 0x2, 0x84, 0x266, 0x3, 0x2, 0x2, 0x2, 0x86, 0x26a, 
    0x3, 0x2, 0x2, 0x2, 0x88, 0x26c, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x26e, 0x3, 
    0x2, 0x2, 0x2, 0x8c, 0x91, 0x5, 0x4, 0x3, 0x2, 0x8d, 0x92, 0x5, 0xa, 
    0x6, 0x2, 0x8e, 0x92, 0x5, 0xc, 0x7, 0x2, 0x8f, 0x92, 0x5, 0xe, 0x8, 
    0x2, 0x90, 0x92, 0x5, 0x10, 0x9, 0x2, 0x91, 0x8d, 0x3, 0x2, 0x2, 0x2, 
    0x91, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x91, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x91, 
    0x90, 0x3, 0x2, 0x2, 0x2, 0x92, 0x93, 0x3, 0x2, 0x2, 0x2, 0x93, 0x94, 
    0x7, 0x2, 0x2, 0x3, 0x94, 0x3, 0x3, 0x2, 0x2, 0x2, 0x95, 0x97, 0x5, 
    0x6, 0x4, 0x2, 0x96, 0x95, 0x3, 0x2, 0x2, 0x2, 0x96, 0x97, 0x3, 0x2, 
    0x2, 0x2, 0x97, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x98, 0x9a, 0x5, 0x8, 0x5, 
    0x2, 0x99, 0x98, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9d, 0x3, 0x2, 0x2, 0x2, 
    0x9b, 0x99, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x9c, 0x3, 0x2, 0x2, 0x2, 0x9c, 
    0x5, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x9f, 
    0x7, 0x3, 0x2, 0x2, 0x9f, 0xa0, 0x7, 0x3d, 0x2, 0x2, 0xa0, 0x7, 0x3, 
    0x2, 0x2, 0x2, 0xa1, 0xa2, 0x7, 0x4, 0x2, 0x2, 0xa2, 0xa3, 0x7, 0x3e, 
    0x2, 0x2, 0xa3, 0xa4, 0x7, 0x3d, 0x2, 0x2, 0xa4, 0x9, 0x3, 0x2, 0x2, 
    0x2, 0xa5, 0xa7, 0x7, 0x5, 0x2, 0x2, 0xa6, 0xa8, 0x9, 0x2, 0x2, 0x2, 
    0xa7, 0xa6, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa8, 
    0xaf, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xab, 0x5, 0x58, 0x2d, 0x2, 0xaa, 0xa9, 
    0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 0x3, 0x2, 0x2, 0x2, 0xac, 0xaa, 0x3, 
    0x2, 0x2, 0x2, 0xac, 0xad, 0x3, 0x2, 0x2, 0x2, 0xad, 0xb0, 0x3, 0x2, 
    0x2, 0x2, 0xae, 0xb0, 0x7, 0x8, 0x2, 0x2, 0xaf, 0xaa, 0x3, 0x2, 0x2, 
    0x2, 0xaf, 0xae, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb4, 0x3, 0x2, 0x2, 0x2, 
    0xb1, 0xb3, 0x5, 0x12, 0xa, 0x2, 0xb2, 0xb1, 0x3, 0x2, 0x2, 0x2, 0xb3, 
    0xb6, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb5, 
    0x3, 0x2, 0x2, 0x2, 0xb5, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb4, 0x3, 
    0x2, 0x2, 0x2, 0xb7, 0xb8, 0x5, 0x1a, 0xe, 0x2, 0xb8, 0xb9, 0x5, 0x1c, 
    0xf, 0x2, 0xb9, 0xb, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbb, 0x7, 0x9, 0x2, 
    0x2, 0xbb, 0xbf, 0x5, 0x3c, 0x1f, 0x2, 0xbc, 0xbe, 0x5, 0x12, 0xa, 0x2, 
    0xbd, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xbf, 
    0xbd, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc2, 
    0x3, 0x2, 0x2, 0x2, 0xc1, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc3, 0x5, 
    0x1a, 0xe, 0x2, 0xc3, 0xc4, 0x5, 0x1c, 0xf, 0x2, 0xc4, 0xd, 0x3, 0x2, 
    0x2, 0x2, 0xc5, 0xcc, 0x7, 0xa, 0x2, 0x2, 0xc6, 0xc8, 0x5, 0x56, 0x2c, 
    0x2, 0xc7, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x3, 0x2, 0x2, 0x2, 
    0xc9, 0xc7, 0x3, 0x2, 0x2, 0x2, 0xc9, 0xca, 0x3, 0x2, 0x2, 0x2, 0xca, 
    0xcd, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xcd, 0x7, 0x8, 0x2, 0x2, 0xcc, 0xc7, 
    0x3, 0x2, 0x2, 0x2, 0xcc, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xd1, 0x3, 
    0x2, 0x2, 0x2, 0xce, 0xd0, 0x5, 0x12, 0xa, 0x2, 0xcf, 0xce, 0x3, 0x2, 
    0x2, 0x2, 0xd0, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd1, 0xcf, 0x3, 0x2, 0x2, 
    0x2, 0xd1, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd5, 0x3, 0x2, 0x2, 0x2, 
    0xd3, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd6, 0x5, 0x1a, 0xe, 0x2, 0xd5, 
    0xd4, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd7, 
    0x3, 0x2, 0x2, 0x2, 0xd7, 0xd8, 0x5, 0x1c, 0xf, 0x2, 0xd8, 0xf, 0x3, 
    0x2, 0x2, 0x2, 0xd9, 0xdd, 0x7, 0xb, 0x2, 0x2, 0xda, 0xdc, 0x5, 0x12, 
    0xa, 0x2, 0xdb, 0xda, 0x3, 0x2, 0x2, 0x2, 0xdc, 0xdf, 0x3, 0x2, 0x2, 
    0x2, 0xdd, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xde, 0x3, 0x2, 0x2, 0x2, 
    0xde, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xdd, 0x3, 0x2, 0x2, 0x2, 0xe0, 
    0xe1, 0x5, 0x1a, 0xe, 0x2, 0xe1, 0x11, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe5, 
    0x7, 0xc, 0x2, 0x2, 0xe3, 0xe6, 0x5, 0x14, 0xb, 0x2, 0xe4, 0xe6, 0x5, 
    0x16, 0xc, 0x2, 0xe5, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe4, 0x3, 0x2, 
    0x2, 0x2, 0xe6, 0x13, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x5, 0x18, 0xd, 
    0x2, 0xe8, 0x15, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xea, 0x7, 0xd, 0x2, 0x2, 
    0xea, 0xeb, 0x5, 0x18, 0xd, 0x2, 0xeb, 0x17, 0x3, 0x2, 0x2, 0x2, 0xec, 
    0xed, 0x5, 0x86, 0x44, 0x2, 0xed, 0x19, 0x3, 0x2, 0x2, 0x2, 0xee, 0xf0, 
    0x7, 0xe, 0x2, 0x2, 0xef, 0xee, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf0, 0x3, 
    0x2, 0x2, 0x2, 0xf0, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf2, 0x5, 0x28, 
    0x15, 0x2, 0xf2, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf5, 0x5, 0x20, 0x11, 
    0x2, 0xf4, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x3, 0x2, 0x2, 0x2, 
    0xf5, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf8, 0x5, 0x1e, 0x10, 0x2, 0xf7, 
    0xf6, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf8, 0x1d, 
    0x3, 0x2, 0x2, 0x2, 0xf9, 0xfb, 0x5, 0x24, 0x13, 0x2, 0xfa, 0xfc, 0x5, 
    0x26, 0x14, 0x2, 0xfb, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfb, 0xfc, 0x3, 0x2, 
    0x2, 0x2, 0xfc, 0x102, 0x3, 0x2, 0x2, 0x2, 0xfd, 0xff, 0x5, 0x26, 0x14, 
    0x2, 0xfe, 0x100, 0x5, 0x24, 0x13, 0x2, 0xff, 0xfe, 0x3, 0x2, 0x2, 0x2, 
    0xff, 0x100, 0x3, 0x2, 0x2, 0x2, 0x100, 0x102, 0x3, 0x2, 0x2, 0x2, 0x101, 
    0xf9, 0x3, 0x2, 0x2, 0x2, 0x101, 0xfd, 0x3, 0x2, 0x2, 0x2, 0x102, 0x1f, 
    0x3, 0x2, 0x2, 0x2, 0x103, 0x104, 0x7, 0xf, 0x2, 0x2, 0x104, 0x106, 
    0x7, 0x10, 0x2, 0x2, 0x105, 0x107, 0x5, 0x22, 0x12, 0x2, 0x106, 0x105, 
    0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 0x3, 0x2, 0x2, 0x2, 0x108, 0x106, 
    0x3, 0x2, 0x2, 0x2, 0x108, 0x109, 0x3, 0x2, 0x2, 0x2, 0x109, 0x21, 0x3, 
    0x2, 0x2, 0x2, 0x10a, 0x10b, 0x9, 0x3, 0x2, 0x2, 0x10b, 0x111, 0x5, 
    0x70, 0x39, 0x2, 0x10c, 0x10f, 0x5, 0x36, 0x1c, 0x2, 0x10d, 0x10f, 0x5, 
    0x58, 0x2d, 0x2, 0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10d, 0x3, 
    0x2, 0x2, 0x2, 0x10f, 0x111, 0x3, 0x2, 0x2, 0x2, 0x110, 0x10a, 0x3, 
    0x2, 0x2, 0x2, 0x110, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x111, 0x23, 0x3, 0x2, 
    0x2, 0x2, 0x112, 0x113, 0x7, 0x13, 0x2, 0x2, 0x113, 0x114, 0x7, 0x44, 
    0x2, 0x2, 0x114, 0x25, 0x3, 0x2, 0x2, 0x2, 0x115, 0x116, 0x7, 0x14, 
    0x2, 0x2, 0x116, 0x117, 0x7, 0x44, 0x2, 0x2, 0x117, 0x27, 0x3, 0x2, 
    0x2, 0x2, 0x118, 0x11a, 0x7, 0x15, 0x2, 0x2, 0x119, 0x11b, 0x5, 0x2a, 
    0x16, 0x2, 0x11a, 0x119, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x11b, 0x3, 0x2, 
    0x2, 0x2, 0x11b, 0x128, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x11f, 0x5, 0x2c, 
    0x17, 0x2, 0x11d, 0x11f, 0x5, 0x34, 0x1b, 0x2, 0x11e, 0x11c, 0x3, 0x2, 
    0x2, 0x2, 0x11e, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x121, 0x3, 0x2, 
    0x2, 0x2, 0x120, 0x122, 0x7, 0x16, 0x2, 0x2, 0x121, 0x120, 0x3, 0x2, 
    0x2, 0x2, 0x121, 0x122, 0x3, 0x2, 0x2, 0x2, 0x122, 0x124, 0x3, 0x2, 
    0x2, 0x2, 0x123, 0x125, 0x5, 0x2a, 0x16, 0x2, 0x124, 0x123, 0x3, 0x2, 
    0x2, 0x2, 0x124, 0x125, 0x3, 0x2, 0x2, 0x2, 0x125, 0x127, 0x3, 0x2, 
    0x2, 0x2, 0x126, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x127, 0x12a, 0x3, 0x2, 
    0x2, 0x2, 0x128, 0x126, 0x3, 0x2, 0x2, 0x2, 0x128, 0x129, 0x3, 0x2, 
    0x2, 0x2, 0x129, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x128, 0x3, 0x2, 
    0x2, 0x2, 0x12b, 0x12c, 0x7, 0x17, 0x2, 0x2, 0x12c, 0x29, 0x3, 0x2, 
    0x2, 0x2, 0x12d, 0x132, 0x5, 0x40, 0x21, 0x2, 0x12e, 0x130, 0x7, 0x16, 
    0x2, 0x2, 0x12f, 0x131, 0x5, 0x2a, 0x16, 0x2, 0x130, 0x12f, 0x3, 0x2, 
    0x2, 0x2, 0x130, 0x131, 0x3, 0x2, 0x2, 0x2, 0x131, 0x133, 0x3, 0x2, 
    0x2, 0x2, 0x132, 0x12e, 0x3, 0x2, 0x2, 0x2, 0x132, 0x133, 0x3, 0x2, 
    0x2, 0x2, 0x133, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x134, 0x138, 0x5, 0x2e, 
    0x18, 0x2, 0x135, 0x138, 0x5, 0x32, 0x1a, 0x2, 0x136, 0x138, 0x5, 0x30, 
    0x19, 0x2, 0x137, 0x134, 0x3, 0x2, 0x2, 0x2, 0x137, 0x135, 0x3, 0x2, 
    0x2, 0x2, 0x137, 0x136, 0x3, 0x2, 0x2, 0x2, 0x138, 0x2d, 0x3, 0x2, 0x2, 
    0x2, 0x139, 0x13a, 0x7, 0x18, 0x2, 0x2, 0x13a, 0x13b, 0x5, 0x28, 0x15, 
    0x2, 0x13b, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x13c, 0x13d, 0x7, 0x19, 0x2, 
    0x2, 0x13d, 0x13e, 0x5, 0x56, 0x2c, 0x2, 0x13e, 0x13f, 0x5, 0x28, 0x15, 
    0x2, 0x13f, 0x31, 0x3, 0x2, 0x2, 0x2, 0x140, 0x145, 0x5, 0x28, 0x15, 
    0x2, 0x141, 0x142, 0x7, 0x1a, 0x2, 0x2, 0x142, 0x144, 0x5, 0x28, 0x15, 
    0x2, 0x143, 0x141, 0x3, 0x2, 0x2, 0x2, 0x144, 0x147, 0x3, 0x2, 0x2, 
    0x2, 0x145, 0x143, 0x3, 0x2, 0x2, 0x2, 0x145, 0x146, 0x3, 0x2, 0x2, 
    0x2, 0x146, 0x33, 0x3, 0x2, 0x2, 0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 0x2, 
    0x148, 0x149, 0x7, 0x1b, 0x2, 0x2, 0x149, 0x14a, 0x5, 0x36, 0x1c, 0x2, 
    0x14a, 0x35, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x14f, 0x5, 0x70, 0x39, 0x2, 
    0x14c, 0x14f, 0x5, 0x72, 0x3a, 0x2, 0x14d, 0x14f, 0x5, 0x38, 0x1d, 0x2, 
    0x14e, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x14e, 0x14c, 0x3, 0x2, 0x2, 0x2, 
    0x14e, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x37, 0x3, 0x2, 0x2, 0x2, 0x150, 
    0x151, 0x5, 0x86, 0x44, 0x2, 0x151, 0x152, 0x5, 0x3a, 0x1e, 0x2, 0x152, 
    0x39, 0x3, 0x2, 0x2, 0x2, 0x153, 0x160, 0x7, 0x53, 0x2, 0x2, 0x154, 
    0x155, 0x7, 0x1c, 0x2, 0x2, 0x155, 0x15a, 0x5, 0x5c, 0x2f, 0x2, 0x156, 
    0x157, 0x7, 0x1d, 0x2, 0x2, 0x157, 0x159, 0x5, 0x5c, 0x2f, 0x2, 0x158, 
    0x156, 0x3, 0x2, 0x2, 0x2, 0x159, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x15a, 
    0x158, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 0x3, 0x2, 0x2, 0x2, 0x15b, 
    0x15d, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x15d, 
    0x15e, 0x7, 0x1e, 0x2, 0x2, 0x15e, 0x160, 0x3, 0x2, 0x2, 0x2, 0x15f, 
    0x153, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x154, 0x3, 0x2, 0x2, 0x2, 0x160, 
    0x3b, 0x3, 0x2, 0x2, 0x2, 0x161, 0x163, 0x7, 0x15, 0x2, 0x2, 0x162, 
    0x164, 0x5, 0x3e, 0x20, 0x2, 0x163, 0x162, 0x3, 0x2, 0x2, 0x2, 0x163, 
    0x164, 0x3, 0x2, 0x2, 0x2, 0x164, 0x165, 0x3, 0x2, 0x2, 0x2, 0x165, 
    0x166, 0x7, 0x17, 0x2, 0x2, 0x166, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x167, 
    0x16c, 0x5, 0x40, 0x21, 0x2, 0x168, 0x16a, 0x7, 0x16, 0x2, 0x2, 0x169, 
    0x16b, 0x5, 0x3e, 0x20, 0x2, 0x16a, 0x169, 0x3, 0x2, 0x2, 0x2, 0x16a, 
    0x16b, 0x3, 0x2, 0x2, 0x2, 0x16b, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16c, 
    0x168, 0x3, 0x2, 0x2, 0x2, 0x16c, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16d, 
    0x3f, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x16f, 0x5, 0x54, 0x2b, 0x2, 0x16f, 
    0x170, 0x5, 0x42, 0x22, 0x2, 0x170, 0x175, 0x3, 0x2, 0x2, 0x2, 0x171, 
    0x172, 0x5, 0x4c, 0x27, 0x2, 0x172, 0x173, 0x5, 0x44, 0x23, 0x2, 0x173, 
    0x175, 0x3, 0x2, 0x2, 0x2, 0x174, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x174, 
    0x171, 0x3, 0x2, 0x2, 0x2, 0x175, 0x41, 0x3, 0x2, 0x2, 0x2, 0x176, 0x177, 
    0x5, 0x4a, 0x26, 0x2, 0x177, 0x180, 0x5, 0x46, 0x24, 0x2, 0x178, 0x17c, 
    0x7, 0x1f, 0x2, 0x2, 0x179, 0x17a, 0x5, 0x4a, 0x26, 0x2, 0x17a, 0x17b, 
    0x5, 0x46, 0x24, 0x2, 0x17b, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x17c, 0x179, 
    0x3, 0x2, 0x2, 0x2, 0x17c, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x17d, 0x17f, 
    0x3, 0x2, 0x2, 0x2, 0x17e, 0x178, 0x3, 0x2, 0x2, 0x2, 0x17f, 0x182, 
    0x3, 0x2, 0x2, 0x2, 0x180, 0x17e, 0x3, 0x2, 0x2, 0x2, 0x180, 0x181, 
    0x3, 0x2, 0x2, 0x2, 0x181, 0x43, 0x3, 0x2, 0x2, 0x2, 0x182, 0x180, 0x3, 
    0x2, 0x2, 0x2, 0x183, 0x185, 0x5, 0x42, 0x22, 0x2, 0x184, 0x183, 0x3, 
    0x2, 0x2, 0x2, 0x184, 0x185, 0x3, 0x2, 0x2, 0x2, 0x185, 0x45, 0x3, 0x2, 
    0x2, 0x2, 0x186, 0x18b, 0x5, 0x48, 0x25, 0x2, 0x187, 0x188, 0x7, 0x1d, 
    0x2, 0x2, 0x188, 0x18a, 0x5, 0x48, 0x25, 0x2, 0x189, 0x187, 0x3, 0x2, 
    0x2, 0x2, 0x18a, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x18b, 0x189, 0x3, 0x2, 
    0x2, 0x2, 0x18b, 0x18c, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x47, 0x3, 0x2, 0x2, 
    0x2, 0x18d, 0x18b, 0x3, 0x2, 0x2, 0x2, 0x18e, 0x18f, 0x5, 0x52, 0x2a, 
    0x2, 0x18f, 0x49, 0x3, 0x2, 0x2, 0x2, 0x190, 0x193, 0x5, 0x56, 0x2c, 
    0x2, 0x191, 0x193, 0x7, 0x20, 0x2, 0x2, 0x192, 0x190, 0x3, 0x2, 0x2, 
    0x2, 0x192, 0x191, 0x3, 0x2, 0x2, 0x2, 0x193, 0x4b, 0x3, 0x2, 0x2, 0x2, 
    0x194, 0x197, 0x5, 0x50, 0x29, 0x2, 0x195, 0x197, 0x5, 0x4e, 0x28, 0x2, 
    0x196, 0x194, 0x3, 0x2, 0x2, 0x2, 0x196, 0x195, 0x3, 0x2, 0x2, 0x2, 
    0x197, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x7, 0x21, 0x2, 0x2, 
    0x199, 0x19a, 0x5, 0x42, 0x22, 0x2, 0x19a, 0x19b, 0x7, 0x22, 0x2, 0x2, 
    0x19b, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19e, 0x7, 0x1c, 0x2, 0x2, 
    0x19d, 0x19f, 0x5, 0x52, 0x2a, 0x2, 0x19e, 0x19d, 0x3, 0x2, 0x2, 0x2, 
    0x19f, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x1a0, 0x19e, 0x3, 0x2, 0x2, 0x2, 
    0x1a0, 0x1a1, 0x3, 0x2, 0x2, 0x2, 0x1a1, 0x1a2, 0x3, 0x2, 0x2, 0x2, 
    0x1a2, 0x1a3, 0x7, 0x1e, 0x2, 0x2, 0x1a3, 0x51, 0x3, 0x2, 0x2, 0x2, 
    0x1a4, 0x1a7, 0x5, 0x54, 0x2b, 0x2, 0x1a5, 0x1a7, 0x5, 0x4c, 0x27, 0x2, 
    0x1a6, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1a5, 0x3, 0x2, 0x2, 0x2, 
    0x1a7, 0x53, 0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1ab, 0x5, 0x58, 0x2d, 0x2, 
    0x1a9, 0x1ab, 0x5, 0x5a, 0x2e, 0x2, 0x1aa, 0x1a8, 0x3, 0x2, 0x2, 0x2, 
    0x1aa, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x55, 0x3, 0x2, 0x2, 0x2, 0x1ac, 
    0x1af, 0x5, 0x58, 0x2d, 0x2, 0x1ad, 0x1af, 0x5, 0x86, 0x44, 0x2, 0x1ae, 
    0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1ae, 0x1ad, 0x3, 0x2, 0x2, 0x2, 0x1af, 
    0x57, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x1b1, 0x9, 0x4, 0x2, 0x2, 0x1b1, 0x59, 
    0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1b9, 0x5, 0x86, 0x44, 0x2, 0x1b3, 0x1b9, 
    0x5, 0x78, 0x3d, 0x2, 0x1b4, 0x1b9, 0x5, 0x7a, 0x3e, 0x2, 0x1b5, 0x1b9, 
    0x5, 0x82, 0x42, 0x2, 0x1b6, 0x1b9, 0x5, 0x8a, 0x46, 0x2, 0x1b7, 0x1b9, 
    0x7, 0x53, 0x2, 0x2, 0x1b8, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b3, 
    0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b4, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b5, 
    0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b6, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b7, 
    0x3, 0x2, 0x2, 0x2, 0x1b9, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1bb, 0x5, 
    0x5e, 0x30, 0x2, 0x1bb, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1c1, 0x5, 
    0x60, 0x31, 0x2, 0x1bd, 0x1be, 0x7, 0x23, 0x2, 0x2, 0x1be, 0x1c0, 0x5, 
    0x60, 0x31, 0x2, 0x1bf, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c3, 0x3, 
    0x2, 0x2, 0x2, 0x1c1, 0x1bf, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c2, 0x3, 
    0x2, 0x2, 0x2, 0x1c2, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c1, 0x3, 0x2, 
    0x2, 0x2, 0x1c4, 0x1c9, 0x5, 0x62, 0x32, 0x2, 0x1c5, 0x1c6, 0x7, 0x24, 
    0x2, 0x2, 0x1c6, 0x1c8, 0x5, 0x62, 0x32, 0x2, 0x1c7, 0x1c5, 0x3, 0x2, 
    0x2, 0x2, 0x1c8, 0x1cb, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1c7, 0x3, 0x2, 
    0x2, 0x2, 0x1c9, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x61, 0x3, 0x2, 0x2, 
    0x2, 0x1cb, 0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1cd, 0x5, 0x64, 0x33, 
    0x2, 0x1cd, 0x63, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1db, 0x5, 0x66, 0x34, 
    0x2, 0x1cf, 0x1d0, 0x7, 0x25, 0x2, 0x2, 0x1d0, 0x1dc, 0x5, 0x66, 0x34, 
    0x2, 0x1d1, 0x1d2, 0x7, 0x26, 0x2, 0x2, 0x1d2, 0x1dc, 0x5, 0x66, 0x34, 
    0x2, 0x1d3, 0x1d4, 0x7, 0x27, 0x2, 0x2, 0x1d4, 0x1dc, 0x5, 0x66, 0x34, 
    0x2, 0x1d5, 0x1d6, 0x7, 0x28, 0x2, 0x2, 0x1d6, 0x1dc, 0x5, 0x66, 0x34, 
    0x2, 0x1d7, 0x1d8, 0x7, 0x29, 0x2, 0x2, 0x1d8, 0x1dc, 0x5, 0x66, 0x34, 
    0x2, 0x1d9, 0x1da, 0x7, 0x2a, 0x2, 0x2, 0x1da, 0x1dc, 0x5, 0x66, 0x34, 
    0x2, 0x1db, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1d1, 0x3, 0x2, 0x2, 
    0x2, 0x1db, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1d5, 0x3, 0x2, 0x2, 
    0x2, 0x1db, 0x1d7, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1d9, 0x3, 0x2, 0x2, 
    0x2, 0x1db, 0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1dc, 0x65, 0x3, 0x2, 0x2, 0x2, 
    0x1dd, 0x1de, 0x5, 0x68, 0x35, 0x2, 0x1de, 0x67, 0x3, 0x2, 0x2, 0x2, 
    0x1df, 0x1e8, 0x5, 0x6a, 0x36, 0x2, 0x1e0, 0x1e1, 0x7, 0x2b, 0x2, 0x2, 
    0x1e1, 0x1e7, 0x5, 0x6a, 0x36, 0x2, 0x1e2, 0x1e3, 0x7, 0x2c, 0x2, 0x2, 
    0x1e3, 0x1e7, 0x5, 0x6a, 0x36, 0x2, 0x1e4, 0x1e7, 0x5, 0x7e, 0x40, 0x2, 
    0x1e5, 0x1e7, 0x5, 0x80, 0x41, 0x2, 0x1e6, 0x1e0, 0x3, 0x2, 0x2, 0x2, 
    0x1e6, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e6, 0x1e4, 0x3, 0x2, 0x2, 0x2, 
    0x1e6, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1ea, 0x3, 0x2, 0x2, 0x2, 
    0x1e8, 0x1e6, 0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1e9, 0x3, 0x2, 0x2, 0x2, 
    0x1e9, 0x69, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1eb, 
    0x1f2, 0x5, 0x6c, 0x37, 0x2, 0x1ec, 0x1ed, 0x7, 0x8, 0x2, 0x2, 0x1ed, 
    0x1f1, 0x5, 0x6c, 0x37, 0x2, 0x1ee, 0x1ef, 0x7, 0x2d, 0x2, 0x2, 0x1ef, 
    0x1f1, 0x5, 0x6c, 0x37, 0x2, 0x1f0, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1f0, 
    0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1f1, 0x1f4, 0x3, 0x2, 0x2, 0x2, 0x1f2, 
    0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1f2, 0x1f3, 0x3, 0x2, 0x2, 0x2, 0x1f3, 
    0x6b, 0x3, 0x2, 0x2, 0x2, 0x1f4, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x1f5, 0x1f6, 
    0x7, 0x2e, 0x2, 0x2, 0x1f6, 0x1fd, 0x5, 0x6e, 0x38, 0x2, 0x1f7, 0x1f8, 
    0x7, 0x2b, 0x2, 0x2, 0x1f8, 0x1fd, 0x5, 0x6e, 0x38, 0x2, 0x1f9, 0x1fa, 
    0x7, 0x2c, 0x2, 0x2, 0x1fa, 0x1fd, 0x5, 0x6e, 0x38, 0x2, 0x1fb, 0x1fd, 
    0x5, 0x6e, 0x38, 0x2, 0x1fc, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f7, 
    0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fb, 
    0x3, 0x2, 0x2, 0x2, 0x1fd, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x1fe, 0x206, 0x5, 
    0x70, 0x39, 0x2, 0x1ff, 0x206, 0x5, 0x72, 0x3a, 0x2, 0x200, 0x206, 0x5, 
    0x76, 0x3c, 0x2, 0x201, 0x206, 0x5, 0x78, 0x3d, 0x2, 0x202, 0x206, 0x5, 
    0x7a, 0x3e, 0x2, 0x203, 0x206, 0x5, 0x82, 0x42, 0x2, 0x204, 0x206, 0x5, 
    0x58, 0x2d, 0x2, 0x205, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x205, 0x1ff, 0x3, 
    0x2, 0x2, 0x2, 0x205, 0x200, 0x3, 0x2, 0x2, 0x2, 0x205, 0x201, 0x3, 
    0x2, 0x2, 0x2, 0x205, 0x202, 0x3, 0x2, 0x2, 0x2, 0x205, 0x203, 0x3, 
    0x2, 0x2, 0x2, 0x205, 0x204, 0x3, 0x2, 0x2, 0x2, 0x206, 0x6f, 0x3, 0x2, 
    0x2, 0x2, 0x207, 0x208, 0x7, 0x1c, 0x2, 0x2, 0x208, 0x209, 0x5, 0x5c, 
    0x2f, 0x2, 0x209, 0x20a, 0x7, 0x1e, 0x2, 0x2, 0x20a, 0x71, 0x3, 0x2, 
    0x2, 0x2, 0x20b, 0x20c, 0x7, 0x2f, 0x2, 0x2, 0x20c, 0x20d, 0x7, 0x1c, 
    0x2, 0x2, 0x20d, 0x20e, 0x5, 0x5c, 0x2f, 0x2, 0x20e, 0x20f, 0x7, 0x1e, 
    0x2, 0x2, 0x20f, 0x243, 0x3, 0x2, 0x2, 0x2, 0x210, 0x211, 0x7, 0x30, 
    0x2, 0x2, 0x211, 0x212, 0x7, 0x1c, 0x2, 0x2, 0x212, 0x213, 0x5, 0x5c, 
    0x2f, 0x2, 0x213, 0x214, 0x7, 0x1e, 0x2, 0x2, 0x214, 0x243, 0x3, 0x2, 
    0x2, 0x2, 0x215, 0x216, 0x7, 0x31, 0x2, 0x2, 0x216, 0x217, 0x7, 0x1c, 
    0x2, 0x2, 0x217, 0x218, 0x5, 0x5c, 0x2f, 0x2, 0x218, 0x219, 0x7, 0x1d, 
    0x2, 0x2, 0x219, 0x21a, 0x5, 0x5c, 0x2f, 0x2, 0x21a, 0x21b, 0x7, 0x1e, 
    0x2, 0x2, 0x21b, 0x243, 0x3, 0x2, 0x2, 0x2, 0x21c, 0x21d, 0x7, 0x32, 
    0x2, 0x2, 0x21d, 0x21e, 0x7, 0x1c, 0x2, 0x2, 0x21e, 0x21f, 0x5, 0x5c, 
    0x2f, 0x2, 0x21f, 0x220, 0x7, 0x1e, 0x2, 0x2, 0x220, 0x243, 0x3, 0x2, 
    0x2, 0x2, 0x221, 0x222, 0x7, 0x33, 0x2, 0x2, 0x222, 0x223, 0x7, 0x1c, 
    0x2, 0x2, 0x223, 0x224, 0x5, 0x58, 0x2d, 0x2, 0x224, 0x225, 0x7, 0x1e, 
    0x2, 0x2, 0x225, 0x243, 0x3, 0x2, 0x2, 0x2, 0x226, 0x227, 0x7, 0x34, 
    0x2, 0x2, 0x227, 0x228, 0x7, 0x1c, 0x2, 0x2, 0x228, 0x229, 0x5, 0x5c, 
    0x2f, 0x2, 0x229, 0x22a, 0x7, 0x1d, 0x2, 0x2, 0x22a, 0x22b, 0x5, 0x5c, 
    0x2f, 0x2, 0x22b, 0x22c, 0x7, 0x1e, 0x2, 0x2, 0x22c, 0x243, 0x3, 0x2, 
    0x2, 0x2, 0x22d, 0x22e, 0x7, 0x35, 0x2, 0x2, 0x22e, 0x22f, 0x7, 0x1c, 
    0x2, 0x2, 0x22f, 0x230, 0x5, 0x5c, 0x2f, 0x2, 0x230, 0x231, 0x7, 0x1e, 
    0x2, 0x2, 0x231, 0x243, 0x3, 0x2, 0x2, 0x2, 0x232, 0x233, 0x7, 0x36, 
    0x2, 0x2, 0x233, 0x234, 0x7, 0x1c, 0x2, 0x2, 0x234, 0x235, 0x5, 0x5c, 
    0x2f, 0x2, 0x235, 0x236, 0x7, 0x1e, 0x2, 0x2, 0x236, 0x243, 0x3, 0x2, 
    0x2, 0x2, 0x237, 0x238, 0x7, 0x37, 0x2, 0x2, 0x238, 0x239, 0x7, 0x1c, 
    0x2, 0x2, 0x239, 0x23a, 0x5, 0x5c, 0x2f, 0x2, 0x23a, 0x23b, 0x7, 0x1e, 
    0x2, 0x2, 0x23b, 0x243, 0x3, 0x2, 0x2, 0x2, 0x23c, 0x23d, 0x7, 0x38, 
    0x2, 0x2, 0x23d, 0x23e, 0x7, 0x1c, 0x2, 0x2, 0x23e, 0x23f, 0x5, 0x5c, 
    0x2f, 0x2, 0x23f, 0x240, 0x7, 0x1e, 0x2, 0x2, 0x240, 0x243, 0x3, 0x2, 
    0x2, 0x2, 0x241, 0x243, 0x5, 0x74, 0x3b, 0x2, 0x242, 0x20b, 0x3, 0x2, 
    0x2, 0x2, 0x242, 0x210, 0x3, 0x2, 0x2, 0x2, 0x242, 0x215, 0x3, 0x2, 
    0x2, 0x2, 0x242, 0x21c, 0x3, 0x2, 0x2, 0x2, 0x242, 0x221, 0x3, 0x2, 
    0x2, 0x2, 0x242, 0x226, 0x3, 0x2, 0x2, 0x2, 0x242, 0x22d, 0x3, 0x2, 
    0x2, 0x2, 0x242, 0x232, 0x3, 0x2, 0x2, 0x2, 0x242, 0x237, 0x3, 0x2, 
    0x2, 0x2, 0x242, 0x23c, 0x3, 0x2, 0x2, 0x2, 0x242, 0x241, 0x3, 0x2, 
    0x2, 0x2, 0x243, 0x73, 0x3, 0x2, 0x2, 0x2, 0x244, 0x245, 0x7, 0x39, 
    0x2, 0x2, 0x245, 0x246, 0x7, 0x1c, 0x2, 0x2, 0x246, 0x247, 0x5, 0x5c, 
    0x2f, 0x2, 0x247, 0x248, 0x7, 0x1d, 0x2, 0x2, 0x248, 0x24b, 0x5, 0x5c, 
    0x2f, 0x2, 0x249, 0x24a, 0x7, 0x1d, 0x2, 0x2, 0x24a, 0x24c, 0x5, 0x5c, 
    0x2f, 0x2, 0x24b, 0x249, 0x3, 0x2, 0x2, 0x2, 0x24b, 0x24c, 0x3, 0x2, 
    0x2, 0x2, 0x24c, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x24d, 0x24e, 0x7, 0x1e, 
    0x2, 0x2, 0x24e, 0x75, 0x3, 0x2, 0x2, 0x2, 0x24f, 0x251, 0x5, 0x86, 
    0x44, 0x2, 0x250, 0x252, 0x5, 0x3a, 0x1e, 0x2, 0x251, 0x250, 0x3, 0x2, 
    0x2, 0x2, 0x251, 0x252, 0x3, 0x2, 0x2, 0x2, 0x252, 0x77, 0x3, 0x2, 0x2, 
    0x2, 0x253, 0x257, 0x5, 0x84, 0x43, 0x2, 0x254, 0x258, 0x7, 0x43, 0x2, 
    0x2, 0x255, 0x256, 0x7, 0x3a, 0x2, 0x2, 0x256, 0x258, 0x5, 0x86, 0x44, 
    0x2, 0x257, 0x254, 0x3, 0x2, 0x2, 0x2, 0x257, 0x255, 0x3, 0x2, 0x2, 
    0x2, 0x257, 0x258, 0x3, 0x2, 0x2, 0x2, 0x258, 0x79, 0x3, 0x2, 0x2, 0x2, 
    0x259, 0x25d, 0x5, 0x7c, 0x3f, 0x2, 0x25a, 0x25d, 0x5, 0x7e, 0x40, 0x2, 
    0x25b, 0x25d, 0x5, 0x80, 0x41, 0x2, 0x25c, 0x259, 0x3, 0x2, 0x2, 0x2, 
    0x25c, 0x25a, 0x3, 0x2, 0x2, 0x2, 0x25c, 0x25b, 0x3, 0x2, 0x2, 0x2, 
    0x25d, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x25f, 0x9, 0x5, 0x2, 0x2, 0x25f, 
    0x7d, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 0x9, 0x6, 0x2, 0x2, 0x261, 0x7f, 
    0x3, 0x2, 0x2, 0x2, 0x262, 0x263, 0x9, 0x7, 0x2, 0x2, 0x263, 0x81, 0x3, 
    0x2, 0x2, 0x2, 0x264, 0x265, 0x9, 0x8, 0x2, 0x2, 0x265, 0x83, 0x3, 0x2, 
    0x2, 0x2, 0x266, 0x267, 0x9, 0x9, 0x2, 0x2, 0x267, 0x85, 0x3, 0x2, 0x2, 
    0x2, 0x268, 0x26b, 0x7, 0x3d, 0x2, 0x2, 0x269, 0x26b, 0x5, 0x88, 0x45, 
    0x2, 0x26a, 0x268, 0x3, 0x2, 0x2, 0x2, 0x26a, 0x269, 0x3, 0x2, 0x2, 
    0x2, 0x26b, 0x87, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x26d, 0x9, 0xa, 0x2, 0x2, 
    0x26d, 0x89, 0x3, 0x2, 0x2, 0x2, 0x26e, 0x26f, 0x9, 0xb, 0x2, 0x2, 0x26f, 
    0x8b, 0x3, 0x2, 0x2, 0x2, 0x43, 0x91, 0x96, 0x9b, 0xa7, 0xac, 0xaf, 
    0xb4, 0xbf, 0xc9, 0xcc, 0xd1, 0xd5, 0xdd, 0xe5, 0xef, 0xf4, 0xf7, 0xfb, 
    0xff, 0x101, 0x108, 0x10e, 0x110, 0x11a, 0x11e, 0x121, 0x124, 0x128, 
    0x130, 0x132, 0x137, 0x145, 0x14e, 0x15a, 0x15f, 0x163, 0x16a, 0x16c, 
    0x174, 0x17c, 0x180, 0x184, 0x18b, 0x192, 0x196, 0x1a0, 0x1a6, 0x1aa, 
    0x1ae, 0x1b8, 0x1c1, 0x1c9, 0x1db, 0x1e6, 0x1e8, 0x1f0, 0x1f2, 0x1fc, 
    0x205, 0x242, 0x24b, 0x251, 0x257, 0x25c, 0x26a, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SparqlParser::Initializer SparqlParser::_init;
