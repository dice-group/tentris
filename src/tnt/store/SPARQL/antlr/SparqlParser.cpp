
// Generated from Sparql.g4 by ANTLR 4.7.1


#include "SparqlListener.h"
#include "SparqlVisitor.h"

#include "SparqlParser.h"


using namespace antlrcpp;
using namespace tnt::store::sparql::detail;
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


antlrcpp::Any SparqlParser::QueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::QueryContext* SparqlParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 0, SparqlParser::RuleQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(140);
    prologue();
    setState(145);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__2: {
        setState(141);
        selectQuery();
        break;
      }

      case SparqlParser::T__6: {
        setState(142);
        constructQuery();
        break;
      }

      case SparqlParser::T__7: {
        setState(143);
        describeQuery();
        break;
      }

      case SparqlParser::T__8: {
        setState(144);
        askQuery();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(147);
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


antlrcpp::Any SparqlParser::PrologueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitPrologue(this);
  else
    return visitor->visitChildren(this);
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
    setState(150);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__0) {
      setState(149);
      baseDecl();
    }
    setState(155);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__1) {
      setState(152);
      prefixDecl();
      setState(157);
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


antlrcpp::Any SparqlParser::BaseDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitBaseDecl(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BaseDeclContext* SparqlParser::baseDecl() {
  BaseDeclContext *_localctx = _tracker.createInstance<BaseDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, SparqlParser::RuleBaseDecl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(158);
    match(SparqlParser::T__0);
    setState(159);
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


antlrcpp::Any SparqlParser::PrefixDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitPrefixDecl(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrefixDeclContext* SparqlParser::prefixDecl() {
  PrefixDeclContext *_localctx = _tracker.createInstance<PrefixDeclContext>(_ctx, getState());
  enterRule(_localctx, 6, SparqlParser::RulePrefixDecl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(161);
    match(SparqlParser::T__1);
    setState(162);
    match(SparqlParser::PNAME_NS);
    setState(163);
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

SparqlParser::SelectModifierContext* SparqlParser::SelectQueryContext::selectModifier() {
  return getRuleContext<SparqlParser::SelectModifierContext>(0);
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


antlrcpp::Any SparqlParser::SelectQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitSelectQuery(this);
  else
    return visitor->visitChildren(this);
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
    setState(165);
    match(SparqlParser::T__2);
    setState(166);
    selectModifier();
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

      case SparqlParser::T__3: {
        setState(172);
        match(SparqlParser::T__3);
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

//----------------- SelectModifierContext ------------------------------------------------------------------

SparqlParser::SelectModifierContext::SelectModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SparqlParser::SelectModifierContext::getRuleIndex() const {
  return SparqlParser::RuleSelectModifier;
}

void SparqlParser::SelectModifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelectModifier(this);
}

void SparqlParser::SelectModifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SparqlListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelectModifier(this);
}


antlrcpp::Any SparqlParser::SelectModifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitSelectModifier(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SelectModifierContext* SparqlParser::selectModifier() {
  SelectModifierContext *_localctx = _tracker.createInstance<SelectModifierContext>(_ctx, getState());
  enterRule(_localctx, 10, SparqlParser::RuleSelectModifier);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(185);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__4

    || _la == SparqlParser::T__5) {
      setState(184);
      _la = _input->LA(1);
      if (!(_la == SparqlParser::T__4

      || _la == SparqlParser::T__5)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
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


antlrcpp::Any SparqlParser::ConstructQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitConstructQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstructQueryContext* SparqlParser::constructQuery() {
  ConstructQueryContext *_localctx = _tracker.createInstance<ConstructQueryContext>(_ctx, getState());
  enterRule(_localctx, 12, SparqlParser::RuleConstructQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(187);
    match(SparqlParser::T__6);
    setState(188);
    constructTemplate();
    setState(192);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(189);
      datasetClause();
      setState(194);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(195);
    whereClause();
    setState(196);
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


antlrcpp::Any SparqlParser::DescribeQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitDescribeQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DescribeQueryContext* SparqlParser::describeQuery() {
  DescribeQueryContext *_localctx = _tracker.createInstance<DescribeQueryContext>(_ctx, getState());
  enterRule(_localctx, 14, SparqlParser::RuleDescribeQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(198);
    match(SparqlParser::T__7);
    setState(205);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        setState(200); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(199);
          varOrIRIref();
          setState(202); 
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

      case SparqlParser::T__3: {
        setState(204);
        match(SparqlParser::T__3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(210);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(207);
      datasetClause();
      setState(212);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(214);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__11

    || _la == SparqlParser::T__18) {
      setState(213);
      whereClause();
    }
    setState(216);
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


antlrcpp::Any SparqlParser::AskQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitAskQuery(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::AskQueryContext* SparqlParser::askQuery() {
  AskQueryContext *_localctx = _tracker.createInstance<AskQueryContext>(_ctx, getState());
  enterRule(_localctx, 16, SparqlParser::RuleAskQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(218);
    match(SparqlParser::T__8);
    setState(222);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__9) {
      setState(219);
      datasetClause();
      setState(224);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(225);
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


antlrcpp::Any SparqlParser::DatasetClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitDatasetClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DatasetClauseContext* SparqlParser::datasetClause() {
  DatasetClauseContext *_localctx = _tracker.createInstance<DatasetClauseContext>(_ctx, getState());
  enterRule(_localctx, 18, SparqlParser::RuleDatasetClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(227);
    match(SparqlParser::T__9);
    setState(230);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        setState(228);
        defaultGraphClause();
        break;
      }

      case SparqlParser::T__10: {
        setState(229);
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


antlrcpp::Any SparqlParser::DefaultGraphClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitDefaultGraphClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::DefaultGraphClauseContext* SparqlParser::defaultGraphClause() {
  DefaultGraphClauseContext *_localctx = _tracker.createInstance<DefaultGraphClauseContext>(_ctx, getState());
  enterRule(_localctx, 20, SparqlParser::RuleDefaultGraphClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
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


antlrcpp::Any SparqlParser::NamedGraphClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitNamedGraphClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NamedGraphClauseContext* SparqlParser::namedGraphClause() {
  NamedGraphClauseContext *_localctx = _tracker.createInstance<NamedGraphClauseContext>(_ctx, getState());
  enterRule(_localctx, 22, SparqlParser::RuleNamedGraphClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
    match(SparqlParser::T__10);
    setState(235);
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


antlrcpp::Any SparqlParser::SourceSelectorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitSourceSelector(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SourceSelectorContext* SparqlParser::sourceSelector() {
  SourceSelectorContext *_localctx = _tracker.createInstance<SourceSelectorContext>(_ctx, getState());
  enterRule(_localctx, 24, SparqlParser::RuleSourceSelector);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(237);
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


antlrcpp::Any SparqlParser::WhereClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitWhereClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::WhereClauseContext* SparqlParser::whereClause() {
  WhereClauseContext *_localctx = _tracker.createInstance<WhereClauseContext>(_ctx, getState());
  enterRule(_localctx, 26, SparqlParser::RuleWhereClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(240);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__11) {
      setState(239);
      match(SparqlParser::T__11);
    }
    setState(242);
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


antlrcpp::Any SparqlParser::SolutionModifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitSolutionModifier(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::SolutionModifierContext* SparqlParser::solutionModifier() {
  SolutionModifierContext *_localctx = _tracker.createInstance<SolutionModifierContext>(_ctx, getState());
  enterRule(_localctx, 28, SparqlParser::RuleSolutionModifier);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(245);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__12) {
      setState(244);
      orderClause();
    }
    setState(248);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__16

    || _la == SparqlParser::T__17) {
      setState(247);
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


antlrcpp::Any SparqlParser::LimitOffsetClausesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitLimitOffsetClauses(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::LimitOffsetClausesContext* SparqlParser::limitOffsetClauses() {
  LimitOffsetClausesContext *_localctx = _tracker.createInstance<LimitOffsetClausesContext>(_ctx, getState());
  enterRule(_localctx, 30, SparqlParser::RuleLimitOffsetClauses);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(258);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__16: {
        setState(250);
        limitClause();
        setState(252);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::T__17) {
          setState(251);
          offsetClause();
        }
        break;
      }

      case SparqlParser::T__17: {
        setState(254);
        offsetClause();
        setState(256);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SparqlParser::T__16) {
          setState(255);
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


antlrcpp::Any SparqlParser::OrderClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitOrderClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OrderClauseContext* SparqlParser::orderClause() {
  OrderClauseContext *_localctx = _tracker.createInstance<OrderClauseContext>(_ctx, getState());
  enterRule(_localctx, 32, SparqlParser::RuleOrderClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(260);
    match(SparqlParser::T__12);
    setState(261);
    match(SparqlParser::T__13);
    setState(263); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(262);
      orderCondition();
      setState(265); 
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


antlrcpp::Any SparqlParser::OrderConditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitOrderCondition(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OrderConditionContext* SparqlParser::orderCondition() {
  OrderConditionContext *_localctx = _tracker.createInstance<OrderConditionContext>(_ctx, getState());
  enterRule(_localctx, 34, SparqlParser::RuleOrderCondition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(273);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__14:
      case SparqlParser::T__15: {
        enterOuterAlt(_localctx, 1);
        setState(267);
        _la = _input->LA(1);
        if (!(_la == SparqlParser::T__14

        || _la == SparqlParser::T__15)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(268);
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
        setState(271);
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
            setState(269);
            constraint();
            break;
          }

          case SparqlParser::VAR1:
          case SparqlParser::VAR2: {
            setState(270);
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


antlrcpp::Any SparqlParser::LimitClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitLimitClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::LimitClauseContext* SparqlParser::limitClause() {
  LimitClauseContext *_localctx = _tracker.createInstance<LimitClauseContext>(_ctx, getState());
  enterRule(_localctx, 36, SparqlParser::RuleLimitClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(275);
    match(SparqlParser::T__16);
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


antlrcpp::Any SparqlParser::OffsetClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitOffsetClause(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OffsetClauseContext* SparqlParser::offsetClause() {
  OffsetClauseContext *_localctx = _tracker.createInstance<OffsetClauseContext>(_ctx, getState());
  enterRule(_localctx, 38, SparqlParser::RuleOffsetClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(278);
    match(SparqlParser::T__17);
    setState(279);
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


antlrcpp::Any SparqlParser::GroupGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitGroupGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupGraphPatternContext* SparqlParser::groupGraphPattern() {
  GroupGraphPatternContext *_localctx = _tracker.createInstance<GroupGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 40, SparqlParser::RuleGroupGraphPattern);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(281);
    match(SparqlParser::T__18);
    setState(283);
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
      setState(282);
      triplesBlock();
    }
    setState(297);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SparqlParser::T__18)
      | (1ULL << SparqlParser::T__21)
      | (1ULL << SparqlParser::T__22)
      | (1ULL << SparqlParser::T__24))) != 0)) {
      setState(287);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::T__18:
        case SparqlParser::T__21:
        case SparqlParser::T__22: {
          setState(285);
          graphPatternNotTriples();
          break;
        }

        case SparqlParser::T__24: {
          setState(286);
          filter();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(290);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SparqlParser::T__19) {
        setState(289);
        match(SparqlParser::T__19);
      }
      setState(293);
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
        setState(292);
        triplesBlock();
      }
      setState(299);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(300);
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


antlrcpp::Any SparqlParser::TriplesBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitTriplesBlock(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesBlockContext* SparqlParser::triplesBlock() {
  TriplesBlockContext *_localctx = _tracker.createInstance<TriplesBlockContext>(_ctx, getState());
  enterRule(_localctx, 42, SparqlParser::RuleTriplesBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(302);
    triplesSameSubject();
    setState(307);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__19) {
      setState(303);
      match(SparqlParser::T__19);
      setState(305);
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
        setState(304);
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


antlrcpp::Any SparqlParser::GraphPatternNotTriplesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitGraphPatternNotTriples(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphPatternNotTriplesContext* SparqlParser::graphPatternNotTriples() {
  GraphPatternNotTriplesContext *_localctx = _tracker.createInstance<GraphPatternNotTriplesContext>(_ctx, getState());
  enterRule(_localctx, 44, SparqlParser::RuleGraphPatternNotTriples);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(312);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__21: {
        enterOuterAlt(_localctx, 1);
        setState(309);
        optionalGraphPattern();
        break;
      }

      case SparqlParser::T__18: {
        enterOuterAlt(_localctx, 2);
        setState(310);
        groupOrUnionGraphPattern();
        break;
      }

      case SparqlParser::T__22: {
        enterOuterAlt(_localctx, 3);
        setState(311);
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


antlrcpp::Any SparqlParser::OptionalGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitOptionalGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::OptionalGraphPatternContext* SparqlParser::optionalGraphPattern() {
  OptionalGraphPatternContext *_localctx = _tracker.createInstance<OptionalGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 46, SparqlParser::RuleOptionalGraphPattern);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(314);
    match(SparqlParser::T__21);
    setState(315);
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


antlrcpp::Any SparqlParser::GraphGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitGraphGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphGraphPatternContext* SparqlParser::graphGraphPattern() {
  GraphGraphPatternContext *_localctx = _tracker.createInstance<GraphGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 48, SparqlParser::RuleGraphGraphPattern);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(317);
    match(SparqlParser::T__22);
    setState(318);
    varOrIRIref();
    setState(319);
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


antlrcpp::Any SparqlParser::GroupOrUnionGraphPatternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitGroupOrUnionGraphPattern(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GroupOrUnionGraphPatternContext* SparqlParser::groupOrUnionGraphPattern() {
  GroupOrUnionGraphPatternContext *_localctx = _tracker.createInstance<GroupOrUnionGraphPatternContext>(_ctx, getState());
  enterRule(_localctx, 50, SparqlParser::RuleGroupOrUnionGraphPattern);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(321);
    groupGraphPattern();
    setState(326);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__23) {
      setState(322);
      match(SparqlParser::T__23);
      setState(323);
      groupGraphPattern();
      setState(328);
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


antlrcpp::Any SparqlParser::FilterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitFilter(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::FilterContext* SparqlParser::filter() {
  FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, getState());
  enterRule(_localctx, 52, SparqlParser::RuleFilter);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(329);
    match(SparqlParser::T__24);
    setState(330);
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


antlrcpp::Any SparqlParser::ConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitConstraint(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstraintContext* SparqlParser::constraint() {
  ConstraintContext *_localctx = _tracker.createInstance<ConstraintContext>(_ctx, getState());
  enterRule(_localctx, 54, SparqlParser::RuleConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(335);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(332);
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
        setState(333);
        builtInCall();
        break;
      }

      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 3);
        setState(334);
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


antlrcpp::Any SparqlParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::FunctionCallContext* SparqlParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 56, SparqlParser::RuleFunctionCall);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(337);
    iriRef();
    setState(338);
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


antlrcpp::Any SparqlParser::ArgListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitArgList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ArgListContext* SparqlParser::argList() {
  ArgListContext *_localctx = _tracker.createInstance<ArgListContext>(_ctx, getState());
  enterRule(_localctx, 58, SparqlParser::RuleArgList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::NIL: {
        setState(340);
        match(SparqlParser::NIL);
        break;
      }

      case SparqlParser::T__25: {
        setState(341);
        match(SparqlParser::T__25);
        setState(342);
        expression();
        setState(347);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SparqlParser::T__26) {
          setState(343);
          match(SparqlParser::T__26);
          setState(344);
          expression();
          setState(349);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(350);
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


antlrcpp::Any SparqlParser::ConstructTemplateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitConstructTemplate(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstructTemplateContext* SparqlParser::constructTemplate() {
  ConstructTemplateContext *_localctx = _tracker.createInstance<ConstructTemplateContext>(_ctx, getState());
  enterRule(_localctx, 60, SparqlParser::RuleConstructTemplate);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(354);
    match(SparqlParser::T__18);
    setState(356);
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
      setState(355);
      constructTriples();
    }
    setState(358);
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


antlrcpp::Any SparqlParser::ConstructTriplesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitConstructTriples(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConstructTriplesContext* SparqlParser::constructTriples() {
  ConstructTriplesContext *_localctx = _tracker.createInstance<ConstructTriplesContext>(_ctx, getState());
  enterRule(_localctx, 62, SparqlParser::RuleConstructTriples);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(360);
    triplesSameSubject();
    setState(365);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__19) {
      setState(361);
      match(SparqlParser::T__19);
      setState(363);
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
        setState(362);
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


antlrcpp::Any SparqlParser::TriplesSameSubjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitTriplesSameSubject(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesSameSubjectContext* SparqlParser::triplesSameSubject() {
  TriplesSameSubjectContext *_localctx = _tracker.createInstance<TriplesSameSubjectContext>(_ctx, getState());
  enterRule(_localctx, 64, SparqlParser::RuleTriplesSameSubject);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(373);
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
        setState(367);
        varOrTerm();
        setState(368);
        propertyListNotEmpty();
        break;
      }

      case SparqlParser::T__25:
      case SparqlParser::T__30: {
        enterOuterAlt(_localctx, 2);
        setState(370);
        triplesNode();
        setState(371);
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


antlrcpp::Any SparqlParser::PropertyListNotEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitPropertyListNotEmpty(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListNotEmptyContext* SparqlParser::propertyListNotEmpty() {
  PropertyListNotEmptyContext *_localctx = _tracker.createInstance<PropertyListNotEmptyContext>(_ctx, getState());
  enterRule(_localctx, 66, SparqlParser::RulePropertyListNotEmpty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(375);
    verb();
    setState(376);
    objectList();
    setState(385);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__28) {
      setState(377);
      match(SparqlParser::T__28);
      setState(381);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 30) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 30)) & ((1ULL << (SparqlParser::T__29 - 30))
        | (1ULL << (SparqlParser::IRI_REF - 30))
        | (1ULL << (SparqlParser::PNAME_NS - 30))
        | (1ULL << (SparqlParser::PNAME_LN - 30))
        | (1ULL << (SparqlParser::VAR1 - 30))
        | (1ULL << (SparqlParser::VAR2 - 30)))) != 0)) {
        setState(378);
        verb();
        setState(379);
        objectList();
      }
      setState(387);
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


antlrcpp::Any SparqlParser::PropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitPropertyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PropertyListContext* SparqlParser::propertyList() {
  PropertyListContext *_localctx = _tracker.createInstance<PropertyListContext>(_ctx, getState());
  enterRule(_localctx, 68, SparqlParser::RulePropertyList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(389);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 30) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 30)) & ((1ULL << (SparqlParser::T__29 - 30))
      | (1ULL << (SparqlParser::IRI_REF - 30))
      | (1ULL << (SparqlParser::PNAME_NS - 30))
      | (1ULL << (SparqlParser::PNAME_LN - 30))
      | (1ULL << (SparqlParser::VAR1 - 30))
      | (1ULL << (SparqlParser::VAR2 - 30)))) != 0)) {
      setState(388);
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


antlrcpp::Any SparqlParser::ObjectListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitObjectList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ObjectListContext* SparqlParser::objectList() {
  ObjectListContext *_localctx = _tracker.createInstance<ObjectListContext>(_ctx, getState());
  enterRule(_localctx, 70, SparqlParser::RuleObjectList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(391);
    object();
    setState(396);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__26) {
      setState(392);
      match(SparqlParser::T__26);
      setState(393);
      object();
      setState(398);
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


antlrcpp::Any SparqlParser::ObjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitObject(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ObjectContext* SparqlParser::object() {
  ObjectContext *_localctx = _tracker.createInstance<ObjectContext>(_ctx, getState());
  enterRule(_localctx, 72, SparqlParser::RuleObject);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(399);
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


antlrcpp::Any SparqlParser::VerbContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitVerb(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VerbContext* SparqlParser::verb() {
  VerbContext *_localctx = _tracker.createInstance<VerbContext>(_ctx, getState());
  enterRule(_localctx, 74, SparqlParser::RuleVerb);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(403);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN:
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(401);
        varOrIRIref();
        break;
      }

      case SparqlParser::T__29: {
        enterOuterAlt(_localctx, 2);
        setState(402);
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


antlrcpp::Any SparqlParser::TriplesNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitTriplesNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::TriplesNodeContext* SparqlParser::triplesNode() {
  TriplesNodeContext *_localctx = _tracker.createInstance<TriplesNodeContext>(_ctx, getState());
  enterRule(_localctx, 76, SparqlParser::RuleTriplesNode);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(407);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(405);
        collection();
        break;
      }

      case SparqlParser::T__30: {
        enterOuterAlt(_localctx, 2);
        setState(406);
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


antlrcpp::Any SparqlParser::BlankNodePropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitBlankNodePropertyList(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BlankNodePropertyListContext* SparqlParser::blankNodePropertyList() {
  BlankNodePropertyListContext *_localctx = _tracker.createInstance<BlankNodePropertyListContext>(_ctx, getState());
  enterRule(_localctx, 78, SparqlParser::RuleBlankNodePropertyList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(409);
    match(SparqlParser::T__30);
    setState(410);
    propertyListNotEmpty();
    setState(411);
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


antlrcpp::Any SparqlParser::CollectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitCollection(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::CollectionContext* SparqlParser::collection() {
  CollectionContext *_localctx = _tracker.createInstance<CollectionContext>(_ctx, getState());
  enterRule(_localctx, 80, SparqlParser::RuleCollection);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(413);
    match(SparqlParser::T__25);
    setState(415); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(414);
      graphNode();
      setState(417); 
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
    setState(419);
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


antlrcpp::Any SparqlParser::GraphNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitGraphNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphNodeContext* SparqlParser::graphNode() {
  GraphNodeContext *_localctx = _tracker.createInstance<GraphNodeContext>(_ctx, getState());
  enterRule(_localctx, 82, SparqlParser::RuleGraphNode);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(423);
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
        setState(421);
        varOrTerm();
        break;
      }

      case SparqlParser::T__25:
      case SparqlParser::T__30: {
        enterOuterAlt(_localctx, 2);
        setState(422);
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


antlrcpp::Any SparqlParser::VarOrTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitVarOrTerm(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VarOrTermContext* SparqlParser::varOrTerm() {
  VarOrTermContext *_localctx = _tracker.createInstance<VarOrTermContext>(_ctx, getState());
  enterRule(_localctx, 84, SparqlParser::RuleVarOrTerm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(427);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(425);
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
        setState(426);
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


antlrcpp::Any SparqlParser::VarOrIRIrefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitVarOrIRIref(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VarOrIRIrefContext* SparqlParser::varOrIRIref() {
  VarOrIRIrefContext *_localctx = _tracker.createInstance<VarOrIRIrefContext>(_ctx, getState());
  enterRule(_localctx, 86, SparqlParser::RuleVarOrIRIref);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(431);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 1);
        setState(429);
        var();
        break;
      }

      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(430);
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


antlrcpp::Any SparqlParser::VarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitVar(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::VarContext* SparqlParser::var() {
  VarContext *_localctx = _tracker.createInstance<VarContext>(_ctx, getState());
  enterRule(_localctx, 88, SparqlParser::RuleVar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(433);
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


antlrcpp::Any SparqlParser::GraphTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitGraphTerm(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::GraphTermContext* SparqlParser::graphTerm() {
  GraphTermContext *_localctx = _tracker.createInstance<GraphTermContext>(_ctx, getState());
  enterRule(_localctx, 90, SparqlParser::RuleGraphTerm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(441);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 1);
        setState(435);
        iriRef();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2: {
        enterOuterAlt(_localctx, 2);
        setState(436);
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
        setState(437);
        numericLiteral();
        break;
      }

      case SparqlParser::T__56:
      case SparqlParser::T__57: {
        enterOuterAlt(_localctx, 4);
        setState(438);
        booleanLiteral();
        break;
      }

      case SparqlParser::BLANK_NODE_LABEL:
      case SparqlParser::ANON: {
        enterOuterAlt(_localctx, 5);
        setState(439);
        blankNode();
        break;
      }

      case SparqlParser::NIL: {
        enterOuterAlt(_localctx, 6);
        setState(440);
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


antlrcpp::Any SparqlParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ExpressionContext* SparqlParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 92, SparqlParser::RuleExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(443);
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


antlrcpp::Any SparqlParser::ConditionalOrExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitConditionalOrExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConditionalOrExpressionContext* SparqlParser::conditionalOrExpression() {
  ConditionalOrExpressionContext *_localctx = _tracker.createInstance<ConditionalOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 94, SparqlParser::RuleConditionalOrExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(445);
    conditionalAndExpression();
    setState(450);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__32) {
      setState(446);
      match(SparqlParser::T__32);
      setState(447);
      conditionalAndExpression();
      setState(452);
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


antlrcpp::Any SparqlParser::ConditionalAndExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitConditionalAndExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ConditionalAndExpressionContext* SparqlParser::conditionalAndExpression() {
  ConditionalAndExpressionContext *_localctx = _tracker.createInstance<ConditionalAndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 96, SparqlParser::RuleConditionalAndExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(453);
    valueLogical();
    setState(458);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__33) {
      setState(454);
      match(SparqlParser::T__33);
      setState(455);
      valueLogical();
      setState(460);
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


antlrcpp::Any SparqlParser::ValueLogicalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitValueLogical(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::ValueLogicalContext* SparqlParser::valueLogical() {
  ValueLogicalContext *_localctx = _tracker.createInstance<ValueLogicalContext>(_ctx, getState());
  enterRule(_localctx, 98, SparqlParser::RuleValueLogical);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(461);
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


antlrcpp::Any SparqlParser::RelationalExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitRelationalExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::RelationalExpressionContext* SparqlParser::relationalExpression() {
  RelationalExpressionContext *_localctx = _tracker.createInstance<RelationalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 100, SparqlParser::RuleRelationalExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(463);
    numericExpression();
    setState(476);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__34: {
        setState(464);
        match(SparqlParser::T__34);
        setState(465);
        numericExpression();
        break;
      }

      case SparqlParser::T__35: {
        setState(466);
        match(SparqlParser::T__35);
        setState(467);
        numericExpression();
        break;
      }

      case SparqlParser::T__36: {
        setState(468);
        match(SparqlParser::T__36);
        setState(469);
        numericExpression();
        break;
      }

      case SparqlParser::T__37: {
        setState(470);
        match(SparqlParser::T__37);
        setState(471);
        numericExpression();
        break;
      }

      case SparqlParser::T__38: {
        setState(472);
        match(SparqlParser::T__38);
        setState(473);
        numericExpression();
        break;
      }

      case SparqlParser::T__39: {
        setState(474);
        match(SparqlParser::T__39);
        setState(475);
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


antlrcpp::Any SparqlParser::NumericExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitNumericExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericExpressionContext* SparqlParser::numericExpression() {
  NumericExpressionContext *_localctx = _tracker.createInstance<NumericExpressionContext>(_ctx, getState());
  enterRule(_localctx, 102, SparqlParser::RuleNumericExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(478);
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


antlrcpp::Any SparqlParser::AdditiveExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::AdditiveExpressionContext* SparqlParser::additiveExpression() {
  AdditiveExpressionContext *_localctx = _tracker.createInstance<AdditiveExpressionContext>(_ctx, getState());
  enterRule(_localctx, 104, SparqlParser::RuleAdditiveExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(480);
    multiplicativeExpression();
    setState(489);
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
      setState(487);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::T__40: {
          setState(481);
          match(SparqlParser::T__40);
          setState(482);
          multiplicativeExpression();
          break;
        }

        case SparqlParser::T__41: {
          setState(483);
          match(SparqlParser::T__41);
          setState(484);
          multiplicativeExpression();
          break;
        }

        case SparqlParser::INTEGER_POSITIVE:
        case SparqlParser::DECIMAL_POSITIVE:
        case SparqlParser::DOUBLE_POSITIVE: {
          setState(485);
          numericLiteralPositive();
          break;
        }

        case SparqlParser::INTEGER_NEGATIVE:
        case SparqlParser::DECIMAL_NEGATIVE:
        case SparqlParser::DOUBLE_NEGATIVE: {
          setState(486);
          numericLiteralNegative();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(491);
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


antlrcpp::Any SparqlParser::MultiplicativeExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::MultiplicativeExpressionContext* SparqlParser::multiplicativeExpression() {
  MultiplicativeExpressionContext *_localctx = _tracker.createInstance<MultiplicativeExpressionContext>(_ctx, getState());
  enterRule(_localctx, 106, SparqlParser::RuleMultiplicativeExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(492);
    unaryExpression();
    setState(499);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SparqlParser::T__3

    || _la == SparqlParser::T__42) {
      setState(497);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SparqlParser::T__3: {
          setState(493);
          match(SparqlParser::T__3);
          setState(494);
          unaryExpression();
          break;
        }

        case SparqlParser::T__42: {
          setState(495);
          match(SparqlParser::T__42);
          setState(496);
          unaryExpression();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(501);
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


antlrcpp::Any SparqlParser::UnaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitUnaryExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::UnaryExpressionContext* SparqlParser::unaryExpression() {
  UnaryExpressionContext *_localctx = _tracker.createInstance<UnaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 108, SparqlParser::RuleUnaryExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(509);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__43: {
        enterOuterAlt(_localctx, 1);
        setState(502);
        match(SparqlParser::T__43);
        setState(503);
        primaryExpression();
        break;
      }

      case SparqlParser::T__40: {
        enterOuterAlt(_localctx, 2);
        setState(504);
        match(SparqlParser::T__40);
        setState(505);
        primaryExpression();
        break;
      }

      case SparqlParser::T__41: {
        enterOuterAlt(_localctx, 3);
        setState(506);
        match(SparqlParser::T__41);
        setState(507);
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
        setState(508);
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


antlrcpp::Any SparqlParser::PrimaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitPrimaryExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrimaryExpressionContext* SparqlParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 110, SparqlParser::RulePrimaryExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(518);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(511);
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
        setState(512);
        builtInCall();
        break;
      }

      case SparqlParser::IRI_REF:
      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 3);
        setState(513);
        iriRefOrFunction();
        break;
      }

      case SparqlParser::STRING_LITERAL1:
      case SparqlParser::STRING_LITERAL2: {
        enterOuterAlt(_localctx, 4);
        setState(514);
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
        setState(515);
        numericLiteral();
        break;
      }

      case SparqlParser::T__56:
      case SparqlParser::T__57: {
        enterOuterAlt(_localctx, 6);
        setState(516);
        booleanLiteral();
        break;
      }

      case SparqlParser::VAR1:
      case SparqlParser::VAR2: {
        enterOuterAlt(_localctx, 7);
        setState(517);
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


antlrcpp::Any SparqlParser::BrackettedExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitBrackettedExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BrackettedExpressionContext* SparqlParser::brackettedExpression() {
  BrackettedExpressionContext *_localctx = _tracker.createInstance<BrackettedExpressionContext>(_ctx, getState());
  enterRule(_localctx, 112, SparqlParser::RuleBrackettedExpression);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(520);
    match(SparqlParser::T__25);
    setState(521);
    expression();
    setState(522);
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


antlrcpp::Any SparqlParser::BuiltInCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitBuiltInCall(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BuiltInCallContext* SparqlParser::builtInCall() {
  BuiltInCallContext *_localctx = _tracker.createInstance<BuiltInCallContext>(_ctx, getState());
  enterRule(_localctx, 114, SparqlParser::RuleBuiltInCall);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(579);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::T__44: {
        enterOuterAlt(_localctx, 1);
        setState(524);
        match(SparqlParser::T__44);
        setState(525);
        match(SparqlParser::T__25);
        setState(526);
        expression();
        setState(527);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__45: {
        enterOuterAlt(_localctx, 2);
        setState(529);
        match(SparqlParser::T__45);
        setState(530);
        match(SparqlParser::T__25);
        setState(531);
        expression();
        setState(532);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__46: {
        enterOuterAlt(_localctx, 3);
        setState(534);
        match(SparqlParser::T__46);
        setState(535);
        match(SparqlParser::T__25);
        setState(536);
        expression();
        setState(537);
        match(SparqlParser::T__26);
        setState(538);
        expression();
        setState(539);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__47: {
        enterOuterAlt(_localctx, 4);
        setState(541);
        match(SparqlParser::T__47);
        setState(542);
        match(SparqlParser::T__25);
        setState(543);
        expression();
        setState(544);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__48: {
        enterOuterAlt(_localctx, 5);
        setState(546);
        match(SparqlParser::T__48);
        setState(547);
        match(SparqlParser::T__25);
        setState(548);
        var();
        setState(549);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__49: {
        enterOuterAlt(_localctx, 6);
        setState(551);
        match(SparqlParser::T__49);
        setState(552);
        match(SparqlParser::T__25);
        setState(553);
        expression();
        setState(554);
        match(SparqlParser::T__26);
        setState(555);
        expression();
        setState(556);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__50: {
        enterOuterAlt(_localctx, 7);
        setState(558);
        match(SparqlParser::T__50);
        setState(559);
        match(SparqlParser::T__25);
        setState(560);
        expression();
        setState(561);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__51: {
        enterOuterAlt(_localctx, 8);
        setState(563);
        match(SparqlParser::T__51);
        setState(564);
        match(SparqlParser::T__25);
        setState(565);
        expression();
        setState(566);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__52: {
        enterOuterAlt(_localctx, 9);
        setState(568);
        match(SparqlParser::T__52);
        setState(569);
        match(SparqlParser::T__25);
        setState(570);
        expression();
        setState(571);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__53: {
        enterOuterAlt(_localctx, 10);
        setState(573);
        match(SparqlParser::T__53);
        setState(574);
        match(SparqlParser::T__25);
        setState(575);
        expression();
        setState(576);
        match(SparqlParser::T__27);
        break;
      }

      case SparqlParser::T__54: {
        enterOuterAlt(_localctx, 11);
        setState(578);
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


antlrcpp::Any SparqlParser::RegexExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitRegexExpression(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::RegexExpressionContext* SparqlParser::regexExpression() {
  RegexExpressionContext *_localctx = _tracker.createInstance<RegexExpressionContext>(_ctx, getState());
  enterRule(_localctx, 116, SparqlParser::RuleRegexExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(581);
    match(SparqlParser::T__54);
    setState(582);
    match(SparqlParser::T__25);
    setState(583);
    expression();
    setState(584);
    match(SparqlParser::T__26);
    setState(585);
    expression();
    setState(588);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__26) {
      setState(586);
      match(SparqlParser::T__26);
      setState(587);
      expression();
    }
    setState(590);
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


antlrcpp::Any SparqlParser::IriRefOrFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitIriRefOrFunction(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::IriRefOrFunctionContext* SparqlParser::iriRefOrFunction() {
  IriRefOrFunctionContext *_localctx = _tracker.createInstance<IriRefOrFunctionContext>(_ctx, getState());
  enterRule(_localctx, 118, SparqlParser::RuleIriRefOrFunction);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(592);
    iriRef();
    setState(594);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SparqlParser::T__25

    || _la == SparqlParser::NIL) {
      setState(593);
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


antlrcpp::Any SparqlParser::RdfLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitRdfLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::RdfLiteralContext* SparqlParser::rdfLiteral() {
  RdfLiteralContext *_localctx = _tracker.createInstance<RdfLiteralContext>(_ctx, getState());
  enterRule(_localctx, 120, SparqlParser::RuleRdfLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(596);
    string();
    setState(600);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::LANGTAG: {
        setState(597);
        match(SparqlParser::LANGTAG);
        break;
      }

      case SparqlParser::T__55: {
        setState(598);
        match(SparqlParser::T__55);
        setState(599);
        iriRef();
        break;
      }

      case SparqlParser::T__3:
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


antlrcpp::Any SparqlParser::NumericLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitNumericLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralContext* SparqlParser::numericLiteral() {
  NumericLiteralContext *_localctx = _tracker.createInstance<NumericLiteralContext>(_ctx, getState());
  enterRule(_localctx, 122, SparqlParser::RuleNumericLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(605);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::INTEGER:
      case SparqlParser::DECIMAL:
      case SparqlParser::DOUBLE: {
        enterOuterAlt(_localctx, 1);
        setState(602);
        numericLiteralUnsigned();
        break;
      }

      case SparqlParser::INTEGER_POSITIVE:
      case SparqlParser::DECIMAL_POSITIVE:
      case SparqlParser::DOUBLE_POSITIVE: {
        enterOuterAlt(_localctx, 2);
        setState(603);
        numericLiteralPositive();
        break;
      }

      case SparqlParser::INTEGER_NEGATIVE:
      case SparqlParser::DECIMAL_NEGATIVE:
      case SparqlParser::DOUBLE_NEGATIVE: {
        enterOuterAlt(_localctx, 3);
        setState(604);
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


antlrcpp::Any SparqlParser::NumericLiteralUnsignedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralUnsigned(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralUnsignedContext* SparqlParser::numericLiteralUnsigned() {
  NumericLiteralUnsignedContext *_localctx = _tracker.createInstance<NumericLiteralUnsignedContext>(_ctx, getState());
  enterRule(_localctx, 124, SparqlParser::RuleNumericLiteralUnsigned);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(607);
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


antlrcpp::Any SparqlParser::NumericLiteralPositiveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralPositive(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralPositiveContext* SparqlParser::numericLiteralPositive() {
  NumericLiteralPositiveContext *_localctx = _tracker.createInstance<NumericLiteralPositiveContext>(_ctx, getState());
  enterRule(_localctx, 126, SparqlParser::RuleNumericLiteralPositive);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(609);
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


antlrcpp::Any SparqlParser::NumericLiteralNegativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitNumericLiteralNegative(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::NumericLiteralNegativeContext* SparqlParser::numericLiteralNegative() {
  NumericLiteralNegativeContext *_localctx = _tracker.createInstance<NumericLiteralNegativeContext>(_ctx, getState());
  enterRule(_localctx, 128, SparqlParser::RuleNumericLiteralNegative);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(611);
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


antlrcpp::Any SparqlParser::BooleanLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitBooleanLiteral(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BooleanLiteralContext* SparqlParser::booleanLiteral() {
  BooleanLiteralContext *_localctx = _tracker.createInstance<BooleanLiteralContext>(_ctx, getState());
  enterRule(_localctx, 130, SparqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(613);
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


antlrcpp::Any SparqlParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::StringContext* SparqlParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 132, SparqlParser::RuleString);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(615);
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


antlrcpp::Any SparqlParser::IriRefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitIriRef(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::IriRefContext* SparqlParser::iriRef() {
  IriRefContext *_localctx = _tracker.createInstance<IriRefContext>(_ctx, getState());
  enterRule(_localctx, 134, SparqlParser::RuleIriRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(619);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SparqlParser::IRI_REF: {
        enterOuterAlt(_localctx, 1);
        setState(617);
        match(SparqlParser::IRI_REF);
        break;
      }

      case SparqlParser::PNAME_NS:
      case SparqlParser::PNAME_LN: {
        enterOuterAlt(_localctx, 2);
        setState(618);
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


antlrcpp::Any SparqlParser::PrefixedNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitPrefixedName(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::PrefixedNameContext* SparqlParser::prefixedName() {
  PrefixedNameContext *_localctx = _tracker.createInstance<PrefixedNameContext>(_ctx, getState());
  enterRule(_localctx, 136, SparqlParser::RulePrefixedName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(621);
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


antlrcpp::Any SparqlParser::BlankNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SparqlVisitor*>(visitor))
    return parserVisitor->visitBlankNode(this);
  else
    return visitor->visitChildren(this);
}

SparqlParser::BlankNodeContext* SparqlParser::blankNode() {
  BlankNodeContext *_localctx = _tracker.createInstance<BlankNodeContext>(_ctx, getState());
  enterRule(_localctx, 138, SparqlParser::RuleBlankNode);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(623);
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
  "query", "prologue", "baseDecl", "prefixDecl", "selectQuery", "selectModifier", 
  "constructQuery", "describeQuery", "askQuery", "datasetClause", "defaultGraphClause", 
  "namedGraphClause", "sourceSelector", "whereClause", "solutionModifier", 
  "limitOffsetClauses", "orderClause", "orderCondition", "limitClause", 
  "offsetClause", "groupGraphPattern", "triplesBlock", "graphPatternNotTriples", 
  "optionalGraphPattern", "graphGraphPattern", "groupOrUnionGraphPattern", 
  "filter", "constraint", "functionCall", "argList", "constructTemplate", 
  "constructTriples", "triplesSameSubject", "propertyListNotEmpty", "propertyList", 
  "objectList", "object", "verb", "triplesNode", "blankNodePropertyList", 
  "collection", "graphNode", "varOrTerm", "varOrIRIref", "var", "graphTerm", 
  "expression", "conditionalOrExpression", "conditionalAndExpression", "valueLogical", 
  "relationalExpression", "numericExpression", "additiveExpression", "multiplicativeExpression", 
  "unaryExpression", "primaryExpression", "brackettedExpression", "builtInCall", 
  "regexExpression", "iriRefOrFunction", "rdfLiteral", "numericLiteral", 
  "numericLiteralUnsigned", "numericLiteralPositive", "numericLiteralNegative", 
  "booleanLiteral", "string", "iriRef", "prefixedName", "blankNode"
};

std::vector<std::string> SparqlParser::_literalNames = {
  "", "'BASE'", "'PREFIX'", "'SELECT'", "'*'", "'DISTINCT'", "'REDUCE'", 
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
    0x3, 0x59, 0x274, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x46, 0x9, 0x46, 0x4, 0x47, 0x9, 0x47, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x94, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x3, 0x5, 0x3, 0x99, 0xa, 0x3, 0x3, 0x3, 0x7, 0x3, 0x9c, 0xa, 0x3, 0xc, 
    0x3, 0xe, 0x3, 0x9f, 0xb, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x6, 0x6, 
    0xab, 0xa, 0x6, 0xd, 0x6, 0xe, 0x6, 0xac, 0x3, 0x6, 0x5, 0x6, 0xb0, 
    0xa, 0x6, 0x3, 0x6, 0x7, 0x6, 0xb3, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0xb6, 
    0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x5, 0x7, 0xbc, 0xa, 
    0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0xc1, 0xa, 0x8, 0xc, 0x8, 
    0xe, 0x8, 0xc4, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 
    0x9, 0x6, 0x9, 0xcb, 0xa, 0x9, 0xd, 0x9, 0xe, 0x9, 0xcc, 0x3, 0x9, 0x5, 
    0x9, 0xd0, 0xa, 0x9, 0x3, 0x9, 0x7, 0x9, 0xd3, 0xa, 0x9, 0xc, 0x9, 0xe, 
    0x9, 0xd6, 0xb, 0x9, 0x3, 0x9, 0x5, 0x9, 0xd9, 0xa, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 0xdf, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 
    0xe2, 0xb, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 
    0xb, 0xe9, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x5, 0xf, 0xf3, 0xa, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0x10, 0x5, 0x10, 0xf8, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0xfb, 
    0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0xff, 0xa, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x5, 0x11, 0x103, 0xa, 0x11, 0x5, 0x11, 0x105, 0xa, 0x11, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x6, 0x12, 0x10a, 0xa, 0x12, 0xd, 0x12, 
    0xe, 0x12, 0x10b, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 
    0x112, 0xa, 0x13, 0x5, 0x13, 0x114, 0xa, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x5, 
    0x16, 0x11e, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x122, 0xa, 
    0x16, 0x3, 0x16, 0x5, 0x16, 0x125, 0xa, 0x16, 0x3, 0x16, 0x5, 0x16, 
    0x128, 0xa, 0x16, 0x7, 0x16, 0x12a, 0xa, 0x16, 0xc, 0x16, 0xe, 0x16, 
    0x12d, 0xb, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x5, 0x17, 0x134, 0xa, 0x17, 0x5, 0x17, 0x136, 0xa, 0x17, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x13b, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 
    0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x147, 0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 
    0x14a, 0xb, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 
    0x3, 0x1d, 0x5, 0x1d, 0x152, 0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x7, 0x1f, 0x15c, 
    0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 0x15f, 0xb, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x5, 0x1f, 0x163, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x167, 
    0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 
    0x21, 0x16e, 0xa, 0x21, 0x5, 0x21, 0x170, 0xa, 0x21, 0x3, 0x22, 0x3, 
    0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x178, 
    0xa, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 
    0x23, 0x5, 0x23, 0x180, 0xa, 0x23, 0x7, 0x23, 0x182, 0xa, 0x23, 0xc, 
    0x23, 0xe, 0x23, 0x185, 0xb, 0x23, 0x3, 0x24, 0x5, 0x24, 0x188, 0xa, 
    0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x7, 0x25, 0x18d, 0xa, 0x25, 
    0xc, 0x25, 0xe, 0x25, 0x190, 0xb, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 
    0x3, 0x27, 0x5, 0x27, 0x196, 0xa, 0x27, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 
    0x19a, 0xa, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 
    0x3, 0x2a, 0x6, 0x2a, 0x1a2, 0xa, 0x2a, 0xd, 0x2a, 0xe, 0x2a, 0x1a3, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x1aa, 0xa, 0x2b, 
    0x3, 0x2c, 0x3, 0x2c, 0x5, 0x2c, 0x1ae, 0xa, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 
    0x5, 0x2d, 0x1b2, 0xa, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 
    0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x1bc, 0xa, 0x2f, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 0x1c3, 
    0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 0x1c6, 0xb, 0x31, 0x3, 0x32, 0x3, 0x32, 
    0x3, 0x32, 0x7, 0x32, 0x1cb, 0xa, 0x32, 0xc, 0x32, 0xe, 0x32, 0x1ce, 
    0xb, 0x32, 0x3, 0x33, 0x3, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 
    0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 
    0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x5, 0x34, 0x1df, 0xa, 0x34, 0x3, 0x35, 
    0x3, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x3, 
    0x36, 0x3, 0x36, 0x7, 0x36, 0x1ea, 0xa, 0x36, 0xc, 0x36, 0xe, 0x36, 
    0x1ed, 0xb, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x7, 0x37, 0x1f4, 0xa, 0x37, 0xc, 0x37, 0xe, 0x37, 0x1f7, 0xb, 0x37, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x5, 0x38, 0x200, 0xa, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 
    0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x5, 0x39, 0x209, 0xa, 0x39, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
    0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
    0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
    0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
    0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
    0x3b, 0x5, 0x3b, 0x246, 0xa, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 
    0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x24f, 0xa, 0x3c, 
    0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x255, 0xa, 0x3d, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x25b, 0xa, 0x3e, 
    0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 0x260, 0xa, 0x3f, 0x3, 0x40, 
    0x3, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 0x42, 0x3, 0x42, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x44, 0x3, 0x44, 0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 0x26e, 
    0xa, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 0x47, 0x3, 0x47, 0x2, 
    0x2, 0x48, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
    0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 
    0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 
    0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x2, 
    0xc, 0x3, 0x2, 0x7, 0x8, 0x3, 0x2, 0x11, 0x12, 0x3, 0x2, 0x41, 0x42, 
    0x3, 0x2, 0x44, 0x46, 0x3, 0x2, 0x47, 0x49, 0x3, 0x2, 0x4a, 0x4c, 0x3, 
    0x2, 0x3b, 0x3c, 0x3, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x3e, 0x3f, 0x4, 0x2, 
    0x40, 0x40, 0x54, 0x54, 0x2, 0x28f, 0x2, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x98, 0x3, 0x2, 0x2, 0x2, 0x6, 0xa0, 0x3, 0x2, 0x2, 0x2, 0x8, 0xa3, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xc, 0xbb, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0xbd, 0x3, 0x2, 0x2, 0x2, 0x10, 0xc8, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0xdc, 0x3, 0x2, 0x2, 0x2, 0x14, 0xe5, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0xea, 0x3, 0x2, 0x2, 0x2, 0x18, 0xec, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0xef, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xf2, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xf7, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x104, 0x3, 0x2, 0x2, 0x2, 0x22, 0x106, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0x113, 0x3, 0x2, 0x2, 0x2, 0x26, 0x115, 0x3, 0x2, 
    0x2, 0x2, 0x28, 0x118, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x11b, 0x3, 0x2, 0x2, 
    0x2, 0x2c, 0x130, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x13a, 0x3, 0x2, 0x2, 0x2, 
    0x30, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x32, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x34, 
    0x143, 0x3, 0x2, 0x2, 0x2, 0x36, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x38, 0x151, 
    0x3, 0x2, 0x2, 0x2, 0x3a, 0x153, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x162, 0x3, 
    0x2, 0x2, 0x2, 0x3e, 0x164, 0x3, 0x2, 0x2, 0x2, 0x40, 0x16a, 0x3, 0x2, 
    0x2, 0x2, 0x42, 0x177, 0x3, 0x2, 0x2, 0x2, 0x44, 0x179, 0x3, 0x2, 0x2, 
    0x2, 0x46, 0x187, 0x3, 0x2, 0x2, 0x2, 0x48, 0x189, 0x3, 0x2, 0x2, 0x2, 
    0x4a, 0x191, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x195, 0x3, 0x2, 0x2, 0x2, 0x4e, 
    0x199, 0x3, 0x2, 0x2, 0x2, 0x50, 0x19b, 0x3, 0x2, 0x2, 0x2, 0x52, 0x19f, 
    0x3, 0x2, 0x2, 0x2, 0x54, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x56, 0x1ad, 0x3, 
    0x2, 0x2, 0x2, 0x58, 0x1b1, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x1b3, 0x3, 0x2, 
    0x2, 0x2, 0x5c, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x1bd, 0x3, 0x2, 0x2, 
    0x2, 0x60, 0x1bf, 0x3, 0x2, 0x2, 0x2, 0x62, 0x1c7, 0x3, 0x2, 0x2, 0x2, 
    0x64, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x66, 0x1d1, 0x3, 0x2, 0x2, 0x2, 0x68, 
    0x1e0, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x1ee, 
    0x3, 0x2, 0x2, 0x2, 0x6e, 0x1ff, 0x3, 0x2, 0x2, 0x2, 0x70, 0x208, 0x3, 
    0x2, 0x2, 0x2, 0x72, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x74, 0x245, 0x3, 0x2, 
    0x2, 0x2, 0x76, 0x247, 0x3, 0x2, 0x2, 0x2, 0x78, 0x252, 0x3, 0x2, 0x2, 
    0x2, 0x7a, 0x256, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x25f, 0x3, 0x2, 0x2, 0x2, 
    0x7e, 0x261, 0x3, 0x2, 0x2, 0x2, 0x80, 0x263, 0x3, 0x2, 0x2, 0x2, 0x82, 
    0x265, 0x3, 0x2, 0x2, 0x2, 0x84, 0x267, 0x3, 0x2, 0x2, 0x2, 0x86, 0x269, 
    0x3, 0x2, 0x2, 0x2, 0x88, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x26f, 0x3, 
    0x2, 0x2, 0x2, 0x8c, 0x271, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x93, 0x5, 0x4, 
    0x3, 0x2, 0x8f, 0x94, 0x5, 0xa, 0x6, 0x2, 0x90, 0x94, 0x5, 0xe, 0x8, 
    0x2, 0x91, 0x94, 0x5, 0x10, 0x9, 0x2, 0x92, 0x94, 0x5, 0x12, 0xa, 0x2, 
    0x93, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x93, 0x90, 0x3, 0x2, 0x2, 0x2, 0x93, 
    0x91, 0x3, 0x2, 0x2, 0x2, 0x93, 0x92, 0x3, 0x2, 0x2, 0x2, 0x94, 0x95, 
    0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x7, 0x2, 0x2, 0x3, 0x96, 0x3, 0x3, 
    0x2, 0x2, 0x2, 0x97, 0x99, 0x5, 0x6, 0x4, 0x2, 0x98, 0x97, 0x3, 0x2, 
    0x2, 0x2, 0x98, 0x99, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9d, 0x3, 0x2, 0x2, 
    0x2, 0x9a, 0x9c, 0x5, 0x8, 0x5, 0x2, 0x9b, 0x9a, 0x3, 0x2, 0x2, 0x2, 
    0x9c, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x9d, 
    0x9e, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x5, 0x3, 0x2, 0x2, 0x2, 0x9f, 0x9d, 
    0x3, 0x2, 0x2, 0x2, 0xa0, 0xa1, 0x7, 0x3, 0x2, 0x2, 0xa1, 0xa2, 0x7, 
    0x3d, 0x2, 0x2, 0xa2, 0x7, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa4, 0x7, 0x4, 
    0x2, 0x2, 0xa4, 0xa5, 0x7, 0x3e, 0x2, 0x2, 0xa5, 0xa6, 0x7, 0x3d, 0x2, 
    0x2, 0xa6, 0x9, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x7, 0x5, 0x2, 0x2, 
    0xa8, 0xaf, 0x5, 0xc, 0x7, 0x2, 0xa9, 0xab, 0x5, 0x5a, 0x2e, 0x2, 0xaa, 
    0xa9, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 0x3, 0x2, 0x2, 0x2, 0xac, 0xaa, 
    0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 0x3, 0x2, 0x2, 0x2, 0xad, 0xb0, 0x3, 
    0x2, 0x2, 0x2, 0xae, 0xb0, 0x7, 0x6, 0x2, 0x2, 0xaf, 0xaa, 0x3, 0x2, 
    0x2, 0x2, 0xaf, 0xae, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb4, 0x3, 0x2, 0x2, 
    0x2, 0xb1, 0xb3, 0x5, 0x14, 0xb, 0x2, 0xb2, 0xb1, 0x3, 0x2, 0x2, 0x2, 
    0xb3, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xb4, 
    0xb5, 0x3, 0x2, 0x2, 0x2, 0xb5, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb4, 
    0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 0x5, 0x1c, 0xf, 0x2, 0xb8, 0xb9, 0x5, 
    0x1e, 0x10, 0x2, 0xb9, 0xb, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbc, 0x9, 0x2, 
    0x2, 0x2, 0xbb, 0xba, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0x3, 0x2, 0x2, 
    0x2, 0xbc, 0xd, 0x3, 0x2, 0x2, 0x2, 0xbd, 0xbe, 0x7, 0x9, 0x2, 0x2, 
    0xbe, 0xc2, 0x5, 0x3e, 0x20, 0x2, 0xbf, 0xc1, 0x5, 0x14, 0xb, 0x2, 0xc0, 
    0xbf, 0x3, 0x2, 0x2, 0x2, 0xc1, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc0, 
    0x3, 0x2, 0x2, 0x2, 0xc2, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc3, 0xc5, 0x3, 
    0x2, 0x2, 0x2, 0xc4, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc6, 0x5, 0x1c, 
    0xf, 0x2, 0xc6, 0xc7, 0x5, 0x1e, 0x10, 0x2, 0xc7, 0xf, 0x3, 0x2, 0x2, 
    0x2, 0xc8, 0xcf, 0x7, 0xa, 0x2, 0x2, 0xc9, 0xcb, 0x5, 0x58, 0x2d, 0x2, 
    0xca, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xcc, 
    0xca, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xd0, 
    0x3, 0x2, 0x2, 0x2, 0xce, 0xd0, 0x7, 0x6, 0x2, 0x2, 0xcf, 0xca, 0x3, 
    0x2, 0x2, 0x2, 0xcf, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xd4, 0x3, 0x2, 
    0x2, 0x2, 0xd1, 0xd3, 0x5, 0x14, 0xb, 0x2, 0xd2, 0xd1, 0x3, 0x2, 0x2, 
    0x2, 0xd3, 0xd6, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd2, 0x3, 0x2, 0x2, 0x2, 
    0xd4, 0xd5, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xd6, 
    0xd4, 0x3, 0x2, 0x2, 0x2, 0xd7, 0xd9, 0x5, 0x1c, 0xf, 0x2, 0xd8, 0xd7, 
    0x3, 0x2, 0x2, 0x2, 0xd8, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xda, 0x3, 
    0x2, 0x2, 0x2, 0xda, 0xdb, 0x5, 0x1e, 0x10, 0x2, 0xdb, 0x11, 0x3, 0x2, 
    0x2, 0x2, 0xdc, 0xe0, 0x7, 0xb, 0x2, 0x2, 0xdd, 0xdf, 0x5, 0x14, 0xb, 
    0x2, 0xde, 0xdd, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe2, 0x3, 0x2, 0x2, 0x2, 
    0xe0, 0xde, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xe1, 
    0xe3, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 
    0x5, 0x1c, 0xf, 0x2, 0xe4, 0x13, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe8, 0x7, 
    0xc, 0x2, 0x2, 0xe6, 0xe9, 0x5, 0x16, 0xc, 0x2, 0xe7, 0xe9, 0x5, 0x18, 
    0xd, 0x2, 0xe8, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe7, 0x3, 0x2, 0x2, 
    0x2, 0xe9, 0x15, 0x3, 0x2, 0x2, 0x2, 0xea, 0xeb, 0x5, 0x1a, 0xe, 0x2, 
    0xeb, 0x17, 0x3, 0x2, 0x2, 0x2, 0xec, 0xed, 0x7, 0xd, 0x2, 0x2, 0xed, 
    0xee, 0x5, 0x1a, 0xe, 0x2, 0xee, 0x19, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf0, 
    0x5, 0x88, 0x45, 0x2, 0xf0, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf3, 0x7, 
    0xe, 0x2, 0x2, 0xf2, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf3, 0x3, 0x2, 
    0x2, 0x2, 0xf3, 0xf4, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x5, 0x2a, 0x16, 
    0x2, 0xf5, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf8, 0x5, 0x22, 0x12, 0x2, 
    0xf7, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf8, 
    0xfa, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xfb, 0x5, 0x20, 0x11, 0x2, 0xfa, 0xf9, 
    0x3, 0x2, 0x2, 0x2, 0xfa, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xfb, 0x1f, 0x3, 
    0x2, 0x2, 0x2, 0xfc, 0xfe, 0x5, 0x26, 0x14, 0x2, 0xfd, 0xff, 0x5, 0x28, 
    0x15, 0x2, 0xfe, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xfe, 0xff, 0x3, 0x2, 0x2, 
    0x2, 0xff, 0x105, 0x3, 0x2, 0x2, 0x2, 0x100, 0x102, 0x5, 0x28, 0x15, 
    0x2, 0x101, 0x103, 0x5, 0x26, 0x14, 0x2, 0x102, 0x101, 0x3, 0x2, 0x2, 
    0x2, 0x102, 0x103, 0x3, 0x2, 0x2, 0x2, 0x103, 0x105, 0x3, 0x2, 0x2, 
    0x2, 0x104, 0xfc, 0x3, 0x2, 0x2, 0x2, 0x104, 0x100, 0x3, 0x2, 0x2, 0x2, 
    0x105, 0x21, 0x3, 0x2, 0x2, 0x2, 0x106, 0x107, 0x7, 0xf, 0x2, 0x2, 0x107, 
    0x109, 0x7, 0x10, 0x2, 0x2, 0x108, 0x10a, 0x5, 0x24, 0x13, 0x2, 0x109, 
    0x108, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10b, 
    0x109, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10c, 
    0x23, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10e, 0x9, 0x3, 0x2, 0x2, 0x10e, 0x114, 
    0x5, 0x72, 0x3a, 0x2, 0x10f, 0x112, 0x5, 0x38, 0x1d, 0x2, 0x110, 0x112, 
    0x5, 0x5a, 0x2e, 0x2, 0x111, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x111, 0x110, 
    0x3, 0x2, 0x2, 0x2, 0x112, 0x114, 0x3, 0x2, 0x2, 0x2, 0x113, 0x10d, 
    0x3, 0x2, 0x2, 0x2, 0x113, 0x111, 0x3, 0x2, 0x2, 0x2, 0x114, 0x25, 0x3, 
    0x2, 0x2, 0x2, 0x115, 0x116, 0x7, 0x13, 0x2, 0x2, 0x116, 0x117, 0x7, 
    0x44, 0x2, 0x2, 0x117, 0x27, 0x3, 0x2, 0x2, 0x2, 0x118, 0x119, 0x7, 
    0x14, 0x2, 0x2, 0x119, 0x11a, 0x7, 0x44, 0x2, 0x2, 0x11a, 0x29, 0x3, 
    0x2, 0x2, 0x2, 0x11b, 0x11d, 0x7, 0x15, 0x2, 0x2, 0x11c, 0x11e, 0x5, 
    0x2c, 0x17, 0x2, 0x11d, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11e, 0x3, 
    0x2, 0x2, 0x2, 0x11e, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x122, 0x5, 
    0x2e, 0x18, 0x2, 0x120, 0x122, 0x5, 0x36, 0x1c, 0x2, 0x121, 0x11f, 0x3, 
    0x2, 0x2, 0x2, 0x121, 0x120, 0x3, 0x2, 0x2, 0x2, 0x122, 0x124, 0x3, 
    0x2, 0x2, 0x2, 0x123, 0x125, 0x7, 0x16, 0x2, 0x2, 0x124, 0x123, 0x3, 
    0x2, 0x2, 0x2, 0x124, 0x125, 0x3, 0x2, 0x2, 0x2, 0x125, 0x127, 0x3, 
    0x2, 0x2, 0x2, 0x126, 0x128, 0x5, 0x2c, 0x17, 0x2, 0x127, 0x126, 0x3, 
    0x2, 0x2, 0x2, 0x127, 0x128, 0x3, 0x2, 0x2, 0x2, 0x128, 0x12a, 0x3, 
    0x2, 0x2, 0x2, 0x129, 0x121, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x12d, 0x3, 
    0x2, 0x2, 0x2, 0x12b, 0x129, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x12c, 0x3, 
    0x2, 0x2, 0x2, 0x12c, 0x12e, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x12b, 0x3, 
    0x2, 0x2, 0x2, 0x12e, 0x12f, 0x7, 0x17, 0x2, 0x2, 0x12f, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0x130, 0x135, 0x5, 0x42, 0x22, 0x2, 0x131, 0x133, 0x7, 
    0x16, 0x2, 0x2, 0x132, 0x134, 0x5, 0x2c, 0x17, 0x2, 0x133, 0x132, 0x3, 
    0x2, 0x2, 0x2, 0x133, 0x134, 0x3, 0x2, 0x2, 0x2, 0x134, 0x136, 0x3, 
    0x2, 0x2, 0x2, 0x135, 0x131, 0x3, 0x2, 0x2, 0x2, 0x135, 0x136, 0x3, 
    0x2, 0x2, 0x2, 0x136, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x137, 0x13b, 0x5, 0x30, 
    0x19, 0x2, 0x138, 0x13b, 0x5, 0x34, 0x1b, 0x2, 0x139, 0x13b, 0x5, 0x32, 
    0x1a, 0x2, 0x13a, 0x137, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x138, 0x3, 0x2, 
    0x2, 0x2, 0x13a, 0x139, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x2f, 0x3, 0x2, 0x2, 
    0x2, 0x13c, 0x13d, 0x7, 0x18, 0x2, 0x2, 0x13d, 0x13e, 0x5, 0x2a, 0x16, 
    0x2, 0x13e, 0x31, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x7, 0x19, 0x2, 
    0x2, 0x140, 0x141, 0x5, 0x58, 0x2d, 0x2, 0x141, 0x142, 0x5, 0x2a, 0x16, 
    0x2, 0x142, 0x33, 0x3, 0x2, 0x2, 0x2, 0x143, 0x148, 0x5, 0x2a, 0x16, 
    0x2, 0x144, 0x145, 0x7, 0x1a, 0x2, 0x2, 0x145, 0x147, 0x5, 0x2a, 0x16, 
    0x2, 0x146, 0x144, 0x3, 0x2, 0x2, 0x2, 0x147, 0x14a, 0x3, 0x2, 0x2, 
    0x2, 0x148, 0x146, 0x3, 0x2, 0x2, 0x2, 0x148, 0x149, 0x3, 0x2, 0x2, 
    0x2, 0x149, 0x35, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x148, 0x3, 0x2, 0x2, 0x2, 
    0x14b, 0x14c, 0x7, 0x1b, 0x2, 0x2, 0x14c, 0x14d, 0x5, 0x38, 0x1d, 0x2, 
    0x14d, 0x37, 0x3, 0x2, 0x2, 0x2, 0x14e, 0x152, 0x5, 0x72, 0x3a, 0x2, 
    0x14f, 0x152, 0x5, 0x74, 0x3b, 0x2, 0x150, 0x152, 0x5, 0x3a, 0x1e, 0x2, 
    0x151, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x151, 0x14f, 0x3, 0x2, 0x2, 0x2, 
    0x151, 0x150, 0x3, 0x2, 0x2, 0x2, 0x152, 0x39, 0x3, 0x2, 0x2, 0x2, 0x153, 
    0x154, 0x5, 0x88, 0x45, 0x2, 0x154, 0x155, 0x5, 0x3c, 0x1f, 0x2, 0x155, 
    0x3b, 0x3, 0x2, 0x2, 0x2, 0x156, 0x163, 0x7, 0x53, 0x2, 0x2, 0x157, 
    0x158, 0x7, 0x1c, 0x2, 0x2, 0x158, 0x15d, 0x5, 0x5e, 0x30, 0x2, 0x159, 
    0x15a, 0x7, 0x1d, 0x2, 0x2, 0x15a, 0x15c, 0x5, 0x5e, 0x30, 0x2, 0x15b, 
    0x159, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15d, 
    0x15b, 0x3, 0x2, 0x2, 0x2, 0x15d, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x15e, 
    0x160, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x160, 
    0x161, 0x7, 0x1e, 0x2, 0x2, 0x161, 0x163, 0x3, 0x2, 0x2, 0x2, 0x162, 
    0x156, 0x3, 0x2, 0x2, 0x2, 0x162, 0x157, 0x3, 0x2, 0x2, 0x2, 0x163, 
    0x3d, 0x3, 0x2, 0x2, 0x2, 0x164, 0x166, 0x7, 0x15, 0x2, 0x2, 0x165, 
    0x167, 0x5, 0x40, 0x21, 0x2, 0x166, 0x165, 0x3, 0x2, 0x2, 0x2, 0x166, 
    0x167, 0x3, 0x2, 0x2, 0x2, 0x167, 0x168, 0x3, 0x2, 0x2, 0x2, 0x168, 
    0x169, 0x7, 0x17, 0x2, 0x2, 0x169, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x16a, 
    0x16f, 0x5, 0x42, 0x22, 0x2, 0x16b, 0x16d, 0x7, 0x16, 0x2, 0x2, 0x16c, 
    0x16e, 0x5, 0x40, 0x21, 0x2, 0x16d, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x16d, 
    0x16e, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x170, 0x3, 0x2, 0x2, 0x2, 0x16f, 
    0x16b, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x170, 0x3, 0x2, 0x2, 0x2, 0x170, 
    0x41, 0x3, 0x2, 0x2, 0x2, 0x171, 0x172, 0x5, 0x56, 0x2c, 0x2, 0x172, 
    0x173, 0x5, 0x44, 0x23, 0x2, 0x173, 0x178, 0x3, 0x2, 0x2, 0x2, 0x174, 
    0x175, 0x5, 0x4e, 0x28, 0x2, 0x175, 0x176, 0x5, 0x46, 0x24, 0x2, 0x176, 
    0x178, 0x3, 0x2, 0x2, 0x2, 0x177, 0x171, 0x3, 0x2, 0x2, 0x2, 0x177, 
    0x174, 0x3, 0x2, 0x2, 0x2, 0x178, 0x43, 0x3, 0x2, 0x2, 0x2, 0x179, 0x17a, 
    0x5, 0x4c, 0x27, 0x2, 0x17a, 0x183, 0x5, 0x48, 0x25, 0x2, 0x17b, 0x17f, 
    0x7, 0x1f, 0x2, 0x2, 0x17c, 0x17d, 0x5, 0x4c, 0x27, 0x2, 0x17d, 0x17e, 
    0x5, 0x48, 0x25, 0x2, 0x17e, 0x180, 0x3, 0x2, 0x2, 0x2, 0x17f, 0x17c, 
    0x3, 0x2, 0x2, 0x2, 0x17f, 0x180, 0x3, 0x2, 0x2, 0x2, 0x180, 0x182, 
    0x3, 0x2, 0x2, 0x2, 0x181, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x182, 0x185, 
    0x3, 0x2, 0x2, 0x2, 0x183, 0x181, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 
    0x3, 0x2, 0x2, 0x2, 0x184, 0x45, 0x3, 0x2, 0x2, 0x2, 0x185, 0x183, 0x3, 
    0x2, 0x2, 0x2, 0x186, 0x188, 0x5, 0x44, 0x23, 0x2, 0x187, 0x186, 0x3, 
    0x2, 0x2, 0x2, 0x187, 0x188, 0x3, 0x2, 0x2, 0x2, 0x188, 0x47, 0x3, 0x2, 
    0x2, 0x2, 0x189, 0x18e, 0x5, 0x4a, 0x26, 0x2, 0x18a, 0x18b, 0x7, 0x1d, 
    0x2, 0x2, 0x18b, 0x18d, 0x5, 0x4a, 0x26, 0x2, 0x18c, 0x18a, 0x3, 0x2, 
    0x2, 0x2, 0x18d, 0x190, 0x3, 0x2, 0x2, 0x2, 0x18e, 0x18c, 0x3, 0x2, 
    0x2, 0x2, 0x18e, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x49, 0x3, 0x2, 0x2, 
    0x2, 0x190, 0x18e, 0x3, 0x2, 0x2, 0x2, 0x191, 0x192, 0x5, 0x54, 0x2b, 
    0x2, 0x192, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x193, 0x196, 0x5, 0x58, 0x2d, 
    0x2, 0x194, 0x196, 0x7, 0x20, 0x2, 0x2, 0x195, 0x193, 0x3, 0x2, 0x2, 
    0x2, 0x195, 0x194, 0x3, 0x2, 0x2, 0x2, 0x196, 0x4d, 0x3, 0x2, 0x2, 0x2, 
    0x197, 0x19a, 0x5, 0x52, 0x2a, 0x2, 0x198, 0x19a, 0x5, 0x50, 0x29, 0x2, 
    0x199, 0x197, 0x3, 0x2, 0x2, 0x2, 0x199, 0x198, 0x3, 0x2, 0x2, 0x2, 
    0x19a, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x19b, 0x19c, 0x7, 0x21, 0x2, 0x2, 
    0x19c, 0x19d, 0x5, 0x44, 0x23, 0x2, 0x19d, 0x19e, 0x7, 0x22, 0x2, 0x2, 
    0x19e, 0x51, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x1a1, 0x7, 0x1c, 0x2, 0x2, 
    0x1a0, 0x1a2, 0x5, 0x54, 0x2b, 0x2, 0x1a1, 0x1a0, 0x3, 0x2, 0x2, 0x2, 
    0x1a2, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a3, 0x1a1, 0x3, 0x2, 0x2, 0x2, 
    0x1a3, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a5, 0x3, 0x2, 0x2, 0x2, 
    0x1a5, 0x1a6, 0x7, 0x1e, 0x2, 0x2, 0x1a6, 0x53, 0x3, 0x2, 0x2, 0x2, 
    0x1a7, 0x1aa, 0x5, 0x56, 0x2c, 0x2, 0x1a8, 0x1aa, 0x5, 0x4e, 0x28, 0x2, 
    0x1a9, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1a8, 0x3, 0x2, 0x2, 0x2, 
    0x1aa, 0x55, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x1ae, 0x5, 0x5a, 0x2e, 0x2, 
    0x1ac, 0x1ae, 0x5, 0x5c, 0x2f, 0x2, 0x1ad, 0x1ab, 0x3, 0x2, 0x2, 0x2, 
    0x1ad, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1ae, 0x57, 0x3, 0x2, 0x2, 0x2, 0x1af, 
    0x1b2, 0x5, 0x5a, 0x2e, 0x2, 0x1b0, 0x1b2, 0x5, 0x88, 0x45, 0x2, 0x1b1, 
    0x1af, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1b0, 0x3, 0x2, 0x2, 0x2, 0x1b2, 
    0x59, 0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b4, 0x9, 0x4, 0x2, 0x2, 0x1b4, 0x5b, 
    0x3, 0x2, 0x2, 0x2, 0x1b5, 0x1bc, 0x5, 0x88, 0x45, 0x2, 0x1b6, 0x1bc, 
    0x5, 0x7a, 0x3e, 0x2, 0x1b7, 0x1bc, 0x5, 0x7c, 0x3f, 0x2, 0x1b8, 0x1bc, 
    0x5, 0x84, 0x43, 0x2, 0x1b9, 0x1bc, 0x5, 0x8c, 0x47, 0x2, 0x1ba, 0x1bc, 
    0x7, 0x53, 0x2, 0x2, 0x1bb, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1b6, 
    0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1b7, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1b8, 
    0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1ba, 
    0x3, 0x2, 0x2, 0x2, 0x1bc, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1be, 0x5, 
    0x60, 0x31, 0x2, 0x1be, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x1bf, 0x1c4, 0x5, 
    0x62, 0x32, 0x2, 0x1c0, 0x1c1, 0x7, 0x23, 0x2, 0x2, 0x1c1, 0x1c3, 0x5, 
    0x62, 0x32, 0x2, 0x1c2, 0x1c0, 0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c6, 0x3, 
    0x2, 0x2, 0x2, 0x1c4, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c4, 0x1c5, 0x3, 
    0x2, 0x2, 0x2, 0x1c5, 0x61, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c4, 0x3, 0x2, 
    0x2, 0x2, 0x1c7, 0x1cc, 0x5, 0x64, 0x33, 0x2, 0x1c8, 0x1c9, 0x7, 0x24, 
    0x2, 0x2, 0x1c9, 0x1cb, 0x5, 0x64, 0x33, 0x2, 0x1ca, 0x1c8, 0x3, 0x2, 
    0x2, 0x2, 0x1cb, 0x1ce, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1ca, 0x3, 0x2, 
    0x2, 0x2, 0x1cc, 0x1cd, 0x3, 0x2, 0x2, 0x2, 0x1cd, 0x63, 0x3, 0x2, 0x2, 
    0x2, 0x1ce, 0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1cf, 0x1d0, 0x5, 0x66, 0x34, 
    0x2, 0x1d0, 0x65, 0x3, 0x2, 0x2, 0x2, 0x1d1, 0x1de, 0x5, 0x68, 0x35, 
    0x2, 0x1d2, 0x1d3, 0x7, 0x25, 0x2, 0x2, 0x1d3, 0x1df, 0x5, 0x68, 0x35, 
    0x2, 0x1d4, 0x1d5, 0x7, 0x26, 0x2, 0x2, 0x1d5, 0x1df, 0x5, 0x68, 0x35, 
    0x2, 0x1d6, 0x1d7, 0x7, 0x27, 0x2, 0x2, 0x1d7, 0x1df, 0x5, 0x68, 0x35, 
    0x2, 0x1d8, 0x1d9, 0x7, 0x28, 0x2, 0x2, 0x1d9, 0x1df, 0x5, 0x68, 0x35, 
    0x2, 0x1da, 0x1db, 0x7, 0x29, 0x2, 0x2, 0x1db, 0x1df, 0x5, 0x68, 0x35, 
    0x2, 0x1dc, 0x1dd, 0x7, 0x2a, 0x2, 0x2, 0x1dd, 0x1df, 0x5, 0x68, 0x35, 
    0x2, 0x1de, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1de, 0x1d4, 0x3, 0x2, 0x2, 
    0x2, 0x1de, 0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1de, 0x1d8, 0x3, 0x2, 0x2, 
    0x2, 0x1de, 0x1da, 0x3, 0x2, 0x2, 0x2, 0x1de, 0x1dc, 0x3, 0x2, 0x2, 
    0x2, 0x1de, 0x1df, 0x3, 0x2, 0x2, 0x2, 0x1df, 0x67, 0x3, 0x2, 0x2, 0x2, 
    0x1e0, 0x1e1, 0x5, 0x6a, 0x36, 0x2, 0x1e1, 0x69, 0x3, 0x2, 0x2, 0x2, 
    0x1e2, 0x1eb, 0x5, 0x6c, 0x37, 0x2, 0x1e3, 0x1e4, 0x7, 0x2b, 0x2, 0x2, 
    0x1e4, 0x1ea, 0x5, 0x6c, 0x37, 0x2, 0x1e5, 0x1e6, 0x7, 0x2c, 0x2, 0x2, 
    0x1e6, 0x1ea, 0x5, 0x6c, 0x37, 0x2, 0x1e7, 0x1ea, 0x5, 0x80, 0x41, 0x2, 
    0x1e8, 0x1ea, 0x5, 0x82, 0x42, 0x2, 0x1e9, 0x1e3, 0x3, 0x2, 0x2, 0x2, 
    0x1e9, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1e7, 0x3, 0x2, 0x2, 0x2, 
    0x1e9, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1ed, 0x3, 0x2, 0x2, 0x2, 
    0x1eb, 0x1e9, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1ec, 0x3, 0x2, 0x2, 0x2, 
    0x1ec, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1ee, 
    0x1f5, 0x5, 0x6e, 0x38, 0x2, 0x1ef, 0x1f0, 0x7, 0x6, 0x2, 0x2, 0x1f0, 
    0x1f4, 0x5, 0x6e, 0x38, 0x2, 0x1f1, 0x1f2, 0x7, 0x2d, 0x2, 0x2, 0x1f2, 
    0x1f4, 0x5, 0x6e, 0x38, 0x2, 0x1f3, 0x1ef, 0x3, 0x2, 0x2, 0x2, 0x1f3, 
    0x1f1, 0x3, 0x2, 0x2, 0x2, 0x1f4, 0x1f7, 0x3, 0x2, 0x2, 0x2, 0x1f5, 
    0x1f3, 0x3, 0x2, 0x2, 0x2, 0x1f5, 0x1f6, 0x3, 0x2, 0x2, 0x2, 0x1f6, 
    0x6d, 0x3, 0x2, 0x2, 0x2, 0x1f7, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1f8, 0x1f9, 
    0x7, 0x2e, 0x2, 0x2, 0x1f9, 0x200, 0x5, 0x70, 0x39, 0x2, 0x1fa, 0x1fb, 
    0x7, 0x2b, 0x2, 0x2, 0x1fb, 0x200, 0x5, 0x70, 0x39, 0x2, 0x1fc, 0x1fd, 
    0x7, 0x2c, 0x2, 0x2, 0x1fd, 0x200, 0x5, 0x70, 0x39, 0x2, 0x1fe, 0x200, 
    0x5, 0x70, 0x39, 0x2, 0x1ff, 0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x1fa, 
    0x3, 0x2, 0x2, 0x2, 0x1ff, 0x1fc, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x1fe, 
    0x3, 0x2, 0x2, 0x2, 0x200, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x201, 0x209, 0x5, 
    0x72, 0x3a, 0x2, 0x202, 0x209, 0x5, 0x74, 0x3b, 0x2, 0x203, 0x209, 0x5, 
    0x78, 0x3d, 0x2, 0x204, 0x209, 0x5, 0x7a, 0x3e, 0x2, 0x205, 0x209, 0x5, 
    0x7c, 0x3f, 0x2, 0x206, 0x209, 0x5, 0x84, 0x43, 0x2, 0x207, 0x209, 0x5, 
    0x5a, 0x2e, 0x2, 0x208, 0x201, 0x3, 0x2, 0x2, 0x2, 0x208, 0x202, 0x3, 
    0x2, 0x2, 0x2, 0x208, 0x203, 0x3, 0x2, 0x2, 0x2, 0x208, 0x204, 0x3, 
    0x2, 0x2, 0x2, 0x208, 0x205, 0x3, 0x2, 0x2, 0x2, 0x208, 0x206, 0x3, 
    0x2, 0x2, 0x2, 0x208, 0x207, 0x3, 0x2, 0x2, 0x2, 0x209, 0x71, 0x3, 0x2, 
    0x2, 0x2, 0x20a, 0x20b, 0x7, 0x1c, 0x2, 0x2, 0x20b, 0x20c, 0x5, 0x5e, 
    0x30, 0x2, 0x20c, 0x20d, 0x7, 0x1e, 0x2, 0x2, 0x20d, 0x73, 0x3, 0x2, 
    0x2, 0x2, 0x20e, 0x20f, 0x7, 0x2f, 0x2, 0x2, 0x20f, 0x210, 0x7, 0x1c, 
    0x2, 0x2, 0x210, 0x211, 0x5, 0x5e, 0x30, 0x2, 0x211, 0x212, 0x7, 0x1e, 
    0x2, 0x2, 0x212, 0x246, 0x3, 0x2, 0x2, 0x2, 0x213, 0x214, 0x7, 0x30, 
    0x2, 0x2, 0x214, 0x215, 0x7, 0x1c, 0x2, 0x2, 0x215, 0x216, 0x5, 0x5e, 
    0x30, 0x2, 0x216, 0x217, 0x7, 0x1e, 0x2, 0x2, 0x217, 0x246, 0x3, 0x2, 
    0x2, 0x2, 0x218, 0x219, 0x7, 0x31, 0x2, 0x2, 0x219, 0x21a, 0x7, 0x1c, 
    0x2, 0x2, 0x21a, 0x21b, 0x5, 0x5e, 0x30, 0x2, 0x21b, 0x21c, 0x7, 0x1d, 
    0x2, 0x2, 0x21c, 0x21d, 0x5, 0x5e, 0x30, 0x2, 0x21d, 0x21e, 0x7, 0x1e, 
    0x2, 0x2, 0x21e, 0x246, 0x3, 0x2, 0x2, 0x2, 0x21f, 0x220, 0x7, 0x32, 
    0x2, 0x2, 0x220, 0x221, 0x7, 0x1c, 0x2, 0x2, 0x221, 0x222, 0x5, 0x5e, 
    0x30, 0x2, 0x222, 0x223, 0x7, 0x1e, 0x2, 0x2, 0x223, 0x246, 0x3, 0x2, 
    0x2, 0x2, 0x224, 0x225, 0x7, 0x33, 0x2, 0x2, 0x225, 0x226, 0x7, 0x1c, 
    0x2, 0x2, 0x226, 0x227, 0x5, 0x5a, 0x2e, 0x2, 0x227, 0x228, 0x7, 0x1e, 
    0x2, 0x2, 0x228, 0x246, 0x3, 0x2, 0x2, 0x2, 0x229, 0x22a, 0x7, 0x34, 
    0x2, 0x2, 0x22a, 0x22b, 0x7, 0x1c, 0x2, 0x2, 0x22b, 0x22c, 0x5, 0x5e, 
    0x30, 0x2, 0x22c, 0x22d, 0x7, 0x1d, 0x2, 0x2, 0x22d, 0x22e, 0x5, 0x5e, 
    0x30, 0x2, 0x22e, 0x22f, 0x7, 0x1e, 0x2, 0x2, 0x22f, 0x246, 0x3, 0x2, 
    0x2, 0x2, 0x230, 0x231, 0x7, 0x35, 0x2, 0x2, 0x231, 0x232, 0x7, 0x1c, 
    0x2, 0x2, 0x232, 0x233, 0x5, 0x5e, 0x30, 0x2, 0x233, 0x234, 0x7, 0x1e, 
    0x2, 0x2, 0x234, 0x246, 0x3, 0x2, 0x2, 0x2, 0x235, 0x236, 0x7, 0x36, 
    0x2, 0x2, 0x236, 0x237, 0x7, 0x1c, 0x2, 0x2, 0x237, 0x238, 0x5, 0x5e, 
    0x30, 0x2, 0x238, 0x239, 0x7, 0x1e, 0x2, 0x2, 0x239, 0x246, 0x3, 0x2, 
    0x2, 0x2, 0x23a, 0x23b, 0x7, 0x37, 0x2, 0x2, 0x23b, 0x23c, 0x7, 0x1c, 
    0x2, 0x2, 0x23c, 0x23d, 0x5, 0x5e, 0x30, 0x2, 0x23d, 0x23e, 0x7, 0x1e, 
    0x2, 0x2, 0x23e, 0x246, 0x3, 0x2, 0x2, 0x2, 0x23f, 0x240, 0x7, 0x38, 
    0x2, 0x2, 0x240, 0x241, 0x7, 0x1c, 0x2, 0x2, 0x241, 0x242, 0x5, 0x5e, 
    0x30, 0x2, 0x242, 0x243, 0x7, 0x1e, 0x2, 0x2, 0x243, 0x246, 0x3, 0x2, 
    0x2, 0x2, 0x244, 0x246, 0x5, 0x76, 0x3c, 0x2, 0x245, 0x20e, 0x3, 0x2, 
    0x2, 0x2, 0x245, 0x213, 0x3, 0x2, 0x2, 0x2, 0x245, 0x218, 0x3, 0x2, 
    0x2, 0x2, 0x245, 0x21f, 0x3, 0x2, 0x2, 0x2, 0x245, 0x224, 0x3, 0x2, 
    0x2, 0x2, 0x245, 0x229, 0x3, 0x2, 0x2, 0x2, 0x245, 0x230, 0x3, 0x2, 
    0x2, 0x2, 0x245, 0x235, 0x3, 0x2, 0x2, 0x2, 0x245, 0x23a, 0x3, 0x2, 
    0x2, 0x2, 0x245, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x245, 0x244, 0x3, 0x2, 
    0x2, 0x2, 0x246, 0x75, 0x3, 0x2, 0x2, 0x2, 0x247, 0x248, 0x7, 0x39, 
    0x2, 0x2, 0x248, 0x249, 0x7, 0x1c, 0x2, 0x2, 0x249, 0x24a, 0x5, 0x5e, 
    0x30, 0x2, 0x24a, 0x24b, 0x7, 0x1d, 0x2, 0x2, 0x24b, 0x24e, 0x5, 0x5e, 
    0x30, 0x2, 0x24c, 0x24d, 0x7, 0x1d, 0x2, 0x2, 0x24d, 0x24f, 0x5, 0x5e, 
    0x30, 0x2, 0x24e, 0x24c, 0x3, 0x2, 0x2, 0x2, 0x24e, 0x24f, 0x3, 0x2, 
    0x2, 0x2, 0x24f, 0x250, 0x3, 0x2, 0x2, 0x2, 0x250, 0x251, 0x7, 0x1e, 
    0x2, 0x2, 0x251, 0x77, 0x3, 0x2, 0x2, 0x2, 0x252, 0x254, 0x5, 0x88, 
    0x45, 0x2, 0x253, 0x255, 0x5, 0x3c, 0x1f, 0x2, 0x254, 0x253, 0x3, 0x2, 
    0x2, 0x2, 0x254, 0x255, 0x3, 0x2, 0x2, 0x2, 0x255, 0x79, 0x3, 0x2, 0x2, 
    0x2, 0x256, 0x25a, 0x5, 0x86, 0x44, 0x2, 0x257, 0x25b, 0x7, 0x43, 0x2, 
    0x2, 0x258, 0x259, 0x7, 0x3a, 0x2, 0x2, 0x259, 0x25b, 0x5, 0x88, 0x45, 
    0x2, 0x25a, 0x257, 0x3, 0x2, 0x2, 0x2, 0x25a, 0x258, 0x3, 0x2, 0x2, 
    0x2, 0x25a, 0x25b, 0x3, 0x2, 0x2, 0x2, 0x25b, 0x7b, 0x3, 0x2, 0x2, 0x2, 
    0x25c, 0x260, 0x5, 0x7e, 0x40, 0x2, 0x25d, 0x260, 0x5, 0x80, 0x41, 0x2, 
    0x25e, 0x260, 0x5, 0x82, 0x42, 0x2, 0x25f, 0x25c, 0x3, 0x2, 0x2, 0x2, 
    0x25f, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x25f, 0x25e, 0x3, 0x2, 0x2, 0x2, 
    0x260, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x261, 0x262, 0x9, 0x5, 0x2, 0x2, 0x262, 
    0x7f, 0x3, 0x2, 0x2, 0x2, 0x263, 0x264, 0x9, 0x6, 0x2, 0x2, 0x264, 0x81, 
    0x3, 0x2, 0x2, 0x2, 0x265, 0x266, 0x9, 0x7, 0x2, 0x2, 0x266, 0x83, 0x3, 
    0x2, 0x2, 0x2, 0x267, 0x268, 0x9, 0x8, 0x2, 0x2, 0x268, 0x85, 0x3, 0x2, 
    0x2, 0x2, 0x269, 0x26a, 0x9, 0x9, 0x2, 0x2, 0x26a, 0x87, 0x3, 0x2, 0x2, 
    0x2, 0x26b, 0x26e, 0x7, 0x3d, 0x2, 0x2, 0x26c, 0x26e, 0x5, 0x8a, 0x46, 
    0x2, 0x26d, 0x26b, 0x3, 0x2, 0x2, 0x2, 0x26d, 0x26c, 0x3, 0x2, 0x2, 
    0x2, 0x26e, 0x89, 0x3, 0x2, 0x2, 0x2, 0x26f, 0x270, 0x9, 0xa, 0x2, 0x2, 
    0x270, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x271, 0x272, 0x9, 0xb, 0x2, 0x2, 0x272, 
    0x8d, 0x3, 0x2, 0x2, 0x2, 0x43, 0x93, 0x98, 0x9d, 0xac, 0xaf, 0xb4, 
    0xbb, 0xc2, 0xcc, 0xcf, 0xd4, 0xd8, 0xe0, 0xe8, 0xf2, 0xf7, 0xfa, 0xfe, 
    0x102, 0x104, 0x10b, 0x111, 0x113, 0x11d, 0x121, 0x124, 0x127, 0x12b, 
    0x133, 0x135, 0x13a, 0x148, 0x151, 0x15d, 0x162, 0x166, 0x16d, 0x16f, 
    0x177, 0x17f, 0x183, 0x187, 0x18e, 0x195, 0x199, 0x1a3, 0x1a9, 0x1ad, 
    0x1b1, 0x1bb, 0x1c4, 0x1cc, 0x1de, 0x1e9, 0x1eb, 0x1f3, 0x1f5, 0x1ff, 
    0x208, 0x245, 0x24e, 0x254, 0x25a, 0x25f, 0x26d, 
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
