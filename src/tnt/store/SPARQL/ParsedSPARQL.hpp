#ifndef TNT_SPARQLPARSER_HPP
#define TNT_SPARQLPARSER_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <optional>
#include <exception>
#include <memory>

#include <antlr4-runtime.h>
#include <SparqlParser.h>
#include <SparqlLexer.h>
#include <SparqlBaseListener.h>

#include "tnt/tensor/einsum/Subscript.hpp"
#include "tnt/util/All.hpp"
#include "tnt/store/RDF/Term.hpp"
#include "tnt/store/SPARQL/Variable.hpp"


namespace {
    using namespace tnt::util::types;
    using namespace tnt::tensor::einsum;
};
namespace tnt::store::sparql {
    enum SelectModifier {
        NONE,
        DISTINCT,
        REDUCE
    };

    class LexerErrorListener : public antlr4::BaseErrorListener {
    public:
        LexerErrorListener() = default;

        virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                                 const std::string &msg, std::exception_ptr e) override {
            throw std::invalid_argument{msg};
        }
    };

    class ParserErrorListener : public antlr4::BaseErrorListener {
    public:
        ParserErrorListener() = default;

        virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                                 const std::string &msg, std::exception_ptr e) override {
            throw std::invalid_argument{msg};
        }
    };

    class ParsedSPARQL {

        std::string _sparql_str;
        std::istringstream _str_stream;
        antlr4::ANTLRInputStream _input;
        tnt::a4grammar::sparql::SparqlLexer _lexer;
        antlr4::CommonTokenStream _tokens;
        tnt::a4grammar::sparql::SparqlParser _parser;
        tnt::a4grammar::sparql::SparqlParser::QueryContext *_query;

        std::map<std::string, std::string> prefixes{};
        SelectModifier select_modifier;
        std::vector<Variable> query_variables{};
        std::set<Variable> variables{};
        std::set<Variable> anonym_variables{};
        std::set<std::vector<std::variant<Variable, Term>>> bgps;
        uint next_anon_var_id = 0;
        std::shared_ptr<Subscript> _subscript;
        std::vector<std::vector<std::optional<Term>>> _operand_keys;

    public:

        ParsedSPARQL(std::string sparql_str) :
                _sparql_str{sparql_str},
                _str_stream{sparql_str},
                _input{_str_stream},
                _lexer{&_input},
                _tokens{&_lexer},
                _parser{&_tokens} {
            // replace the error handler
            LexerErrorListener lexerErrorListener = LexerErrorListener{};
            _lexer.removeErrorListeners();
            _lexer.addErrorListener(&lexerErrorListener);

            ParserErrorListener parserErrorListener = ParserErrorListener{};
            _parser.removeParseListeners();
            _parser.removeErrorListeners();
            _parser.addErrorListener(&parserErrorListener);
            // check that _query is present
            _query = _parser.query();
            if (_query == nullptr)
                throw std::invalid_argument("The query was not parsable");
            if (_query) {
                const std::vector<tnt::a4grammar::sparql::SparqlParser::PrefixDeclContext *> &prefixDecl = _query->prologue()->prefixDecl();
                for (auto &prefix : prefixDecl)
                    // remove < and > from <...>
                    prefixes[prefix->PNAME_NS()->getText()] = std::string(prefix->IRI_REF()->getText(), 1,
                                                                          prefix->IRI_REF()->getText().size() - 2);


                tnt::a4grammar::sparql::SparqlParser::SelectQueryContext *select = _query->selectQuery();
                select_modifier = getSelectModifier(select);
                bool all_vars = false;
                if (std::vector<tnt::a4grammar::sparql::SparqlParser::VarContext *> vars = select->var(); not vars.empty())
                    for (auto &var : vars)
                        query_variables.push_back(extractVariable(var));
                else
                    all_vars = true;

                std::queue<tnt::a4grammar::sparql::SparqlParser::TriplesBlockContext *> tripleBlocks;
                for (auto &block : select->whereClause()->groupGraphPattern()->triplesBlock())
                    tripleBlocks.push(block);
                while (not tripleBlocks.empty()) {
                    auto block = tripleBlocks.front();
                    tripleBlocks.pop();
                    tnt::a4grammar::sparql::SparqlParser::TriplesSameSubjectContext *triplesSameSubject = block->triplesSameSubject();

                    std::variant<Variable, Term> subj = parseVarOrTerm(triplesSameSubject->varOrTerm());
                    registerVariable(subj);
                    tnt::a4grammar::sparql::SparqlParser::PropertyListNotEmptyContext *propertyListNotEmpty = triplesSameSubject->propertyListNotEmpty();
                    for (auto[pred_node, obj_nodes] : zip(propertyListNotEmpty->verb(),
                                                          propertyListNotEmpty->objectList())) {
                        std::variant<Variable, Term> pred = parseVerb(pred_node);
                        registerVariable(pred);

                        for (auto &obj_node : obj_nodes->object()) {
                            std::variant<Variable, Term> obj = parseObject(obj_node);
                            registerVariable(obj);

                            bgps.insert(std::vector<std::variant<Variable, Term>>{subj, pred, obj});
                        }
                    }
                    if (auto *next_block = block->triplesBlock(); next_block)
                        tripleBlocks.push(next_block);
                }
                if (all_vars)
                    for (const auto &variable : variables)
                        query_variables.push_back(variable);

                if (not query_variables.size())
                    throw std::invalid_argument{"Empty query variables is not allowed."};
                if (std::set<Variable> query_variables_set{query_variables.begin(), query_variables.end()};
                        not std::includes(variables.cbegin(), variables.cend(), query_variables_set.cbegin(),
                                          query_variables_set.cend())) {
                    throw std::invalid_argument{"query variables must be a subset of BGP variables."};
                }



                // generate subscript
                std::map<Variable, label_t> var_to_label{};
                for (const auto &[id, var] : enumerate(variables)) {
                    var_to_label[var] = id;
                }
                std::vector<std::vector<label_t>> ops_labels{};
                for (const auto &bgp : bgps) {
                    std::vector<label_t> op_labels{};
                    int count = 0;
                    for (const std::variant<Variable, Term> &res : bgp)
                        if (std::holds_alternative<Variable>(res)) {
                            op_labels.push_back(var_to_label[std::get<Variable>(res)]);
                            ++count;
                        }
                    if (count) // removes operands without labels/variables
                        ops_labels.push_back(op_labels);
                }

                std::vector<label_t> result_labels{};
                for (const auto &query_variable : query_variables) {
                    result_labels.push_back(var_to_label[query_variable]);
                }

                _subscript = std::shared_ptr<Subscript>{new Subscript{ops_labels, result_labels}};
                if (auto optimized = _subscript->optimized(); optimized) {
                    _subscript = std::move(optimized);
                }

                // generate operand keys
                for (const auto &bgp : bgps) {
                    std::vector<std::optional<Term>> op_key{};
                    for (const std::variant<Variable, Term> &res : bgp)
                        if (std::holds_alternative<Term>(res))
                            op_key.push_back({std::get<Term>(res)});
                        else
                            op_key.push_back(std::nullopt);
                    _operand_keys.push_back(op_key);
                }
            } else
                throw std::invalid_argument{"query could not be parsed."};

        }

        SelectModifier getSelectModifier() const {
            return select_modifier;
        }

        const std::vector<Variable> &getQueryVariables() const {
            return query_variables;
        }

        const std::string &getSparqlStr() const {
            return _sparql_str;
        }

        const std::shared_ptr<const Subscript> getSubscript() const {
            return _subscript;
        }

        const std::vector<std::vector<std::optional<Term>>> &getOperandKeys() const {
            return _operand_keys;
        }

    private:

        void registerVariable(std::variant<Variable, Term> &variant) {
            if (std::holds_alternative<Variable>(variant)) {
                Variable &var = std::get<Variable>(variant);
                if (not var.is_anonym)
                    variables.insert(var);
                else
                    anonym_variables.insert(var);
            }
        }


        auto parseGraphTerm(tnt::a4grammar::sparql::SparqlParser::GraphTermContext *termContext) -> std::variant<Variable, Term> {
            if (auto *iriRef = termContext->iriRef(); iriRef) {
                return URIRef{getFullIriString(iriRef)};

            } else if (auto *rdfLiteral = termContext->rdfLiteral(); rdfLiteral) {
                auto string_node = rdfLiteral->string();
                std::string literal_string;
                if (auto *stringLiteral1 = string_node->STRING_LITERAL1(); stringLiteral1) {
                    auto literal1 = stringLiteral1->getText();

                    static std::regex double_quote{"\"", std::regex::optimize};
                    static std::regex single_quote("\\'", std::regex::optimize);

                    std::string temp;

                    std::regex_replace(std::back_inserter(temp), literal1.begin() + 1, literal1.end() - 1,
                                       double_quote,
                                       "\\\"");
                    std::regex_replace(std::back_inserter(literal_string), temp.begin() + 1, temp.end() - 1,
                                       single_quote, "'");
                } else {
                    auto literal2 = string_node->STRING_LITERAL2()->getText();
                    literal_string = std::string{literal2, 1, literal2.size() - 2};
                }


                if (auto *langtag = rdfLiteral->LANGTAG(); langtag) {
                    return Literal{literal_string, std::string{langtag->getText(), 1}, std::nullopt};
                } else if (auto *type = rdfLiteral->iriRef(); rdfLiteral->iriRef()) {
                    return Literal{"\"" + literal_string + "\"^^" + getFullIriString(type)};
                } else {
                    return Literal{"\"" + literal_string + "\""};
                }

            } else if (auto *numericLiteral = termContext->numericLiteral();numericLiteral) {

                if (auto *decimalNumeric = numericLiteral->decimalNumeric(); decimalNumeric) {

                    if (auto *plus = decimalNumeric->DECIMAL(); plus) {
                        return Literal{
                                "\"" + plus->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#decimal>"};
                    } else {
                        return Literal{
                                "\"" + decimalNumeric->getText() +
                                "\"^^<http://www.w3.org/2001/XMLSchema#decimal>"};
                    }
                } else if (auto *doubleNumberic = numericLiteral->doubleNumberic();doubleNumberic) {
                    if (antlr4::tree::TerminalNode *plus = doubleNumberic->DOUBLE(); plus) {
                        return Literal{
                                "\"" + plus->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#double>"
                        };
                    } else {
                        return Literal{
                                "\"" + decimalNumeric->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#double>"};
                    }
                } else {
                    auto *integerNumeric = numericLiteral->integerNumeric();
                    if (auto *plus = integerNumeric->INTEGER(); plus) {
                        return Literal{
                                "\"" + plus->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#integer>"};
                    } else {
                        return Literal{
                                "\"" + decimalNumeric->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#integer>"};
                    }
                }

            } else if (termContext->booleanLiteral()) {
                return Literal{
                        "\"" + termContext->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#boolean>"};
            } else if (tnt::a4grammar::sparql::SparqlParser::BlankNodeContext *blankNode = termContext->blankNode();blankNode) {
                if (blankNode->BLANK_NODE_LABEL())
                    return Variable{termContext->getText()};
                else
                    return Variable{"__:" + std::to_string(next_anon_var_id++)};
            } else {
                // TODO: handle NIL value "( )"
            }
        }

        auto parseVarOrTerm(tnt::a4grammar::sparql::SparqlParser::VarOrTermContext *varOrTerm) -> std::variant<Variable, Term> {
            if (varOrTerm->var())
                return std::variant<Variable, Term>{extractVariable(varOrTerm->var())};
            else
                return parseGraphTerm(varOrTerm->graphTerm());
        }

        auto parseObject(tnt::a4grammar::sparql::SparqlParser::ObjectContext *obj) -> std::variant<Variable, Term> {
            tnt::a4grammar::sparql::SparqlParser::VarOrTermContext *varOrTerm = obj->graphNode()->varOrTerm();
            // TODO: consider obj->graphNode()->triplesNode()
            return parseVarOrTerm(varOrTerm);
        }

        auto parseVerb(tnt::a4grammar::sparql::SparqlParser::VerbContext *verb) -> std::variant<Variable, Term> {
            if (auto *varOrIRIref = verb->varOrIRIref(); varOrIRIref) {
                if (varOrIRIref->var()) {
                    return std::variant<Variable, Term>{extractVariable(varOrIRIref->var())};
                } else {
                    tnt::a4grammar::sparql::SparqlParser::IriRefContext *iriRef = varOrIRIref->iriRef();

                    return std::variant<Variable, Term>{URIRef{getFullIriString(iriRef)}};

                }
            } else { // is 'a'
                return std::variant<Variable, Term>{
                        URIRef{"<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"}};
            }
        }


        auto getFullIriString(tnt::a4grammar::sparql::SparqlParser::IriRefContext *iriRef) const -> std::string {
            if (antlr4::tree::TerminalNode *complete_ref = iriRef->IRI_REF(); complete_ref) {
                return complete_ref->getText();
            } else {
                auto *prefixedName = iriRef->prefixedName();

                if (auto *pname_both = prefixedName->PNAME_LN();pname_both) {
                    const std::string &pname_both_str = pname_both->getText();
                    unsigned long i = pname_both_str.find(':') + 1;
                    const std::string &resolvedPrefix = prefixes.at(
                            std::string{pname_both_str, 0, i});
                    return {"<" + resolvedPrefix +
                            std::string{pname_both_str, i,
                                        pname_both_str.size() - i} +
                            ">"};
                } else {
                    auto *default_prefix = prefixedName->PNAME_NS();
                    const std::string &resolvedPrefix = prefixes.at(default_prefix->getText());
                    return {"<" + resolvedPrefix + ">"};
                }
            }
        }

        auto getSelectModifier(tnt::a4grammar::sparql::SparqlParser::SelectQueryContext *select) -> SelectModifier {
            auto *modifier = select->selectModifier();
            if (modifier->children.size() != 0) {
                const std::string &string = modifier->children[0]->toString();
                if (string.compare("DISTINCT") == 0) {
                    return DISTINCT;
                } else {
                    return REDUCE;
                }
            } else {
                return NONE;
            }
        }

        auto extractVariable(tnt::a4grammar::sparql::SparqlParser::VarContext *var) -> Variable {

            const std::string &data = var->getText();
            return Variable{std::string{data, 1, data.length() - 1}};
        }

    public:
        friend std::ostream &operator<<(std::ostream &os, const ParsedSPARQL &sparql) {
            os << "prefixes: " << sparql.prefixes << "\n select_modifier: " << sparql.select_modifier
               << "\n query_variables: " << sparql.query_variables << "\n variables: " << sparql.variables
               << "\n anonym_variables: " << sparql.anonym_variables << "\n bgps: " << sparql.bgps;
            return os;
        }

    };
}


#endif //TNT_SPARQLPARSER_HPP

