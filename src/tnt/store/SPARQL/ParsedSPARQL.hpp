#ifndef TNT_SPARQLPARSER_HPP
#define TNT_SPARQLPARSER_HPP

// ---> move to own library to remove it from global namespace
#include "tnt/store/SPARQL/antlr/SparqlParser.cpp"
#include "tnt/store/SPARQL/antlr/SparqlLexer.cpp"
#include "tnt/store/SPARQL/antlr/SparqlBaseListener.cpp"
// <--- move to own library to remove it from global namespace
#include "tnt/tensor/einsum/Subscript.hpp"
#include <antlr4-runtime.h>
#include "tnt/util/All.hpp"
#include "tnt/store/RDF/Term.hpp"
#include "tnt/store/SPARQL/Variable.hpp"

#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <optional>


namespace tnt::store::sparql {
    enum SelectModifier {
        NONE,
        DISTINCT,
        REDUCE
    };

    class ParsedSPARQL {

        std::string _sparql_str;
        std::istringstream _str_stream;
        ANTLRInputStream _input;
        SparqlLexer _lexer;
        CommonTokenStream _tokens;
        SparqlParser _parser;
        SparqlParser::QueryContext *_query;

        std::map<std::string, std::string> prefixes{};
        SelectModifier select_modifier;
        std::vector<Variable> query_variables{};
        std::set<Variable> variables{};
        std::set<Variable> anonym_variables{};
        std::set<std::vector<std::variant<Variable, Term>>> bgps;
        uint next_anon_var_id = 0;

    public:

        ParsedSPARQL(std::string sparql_str) :
                _sparql_str{sparql_str},
                _str_stream{sparql_str},
                _input{_str_stream},
                _lexer{&_input},
                _tokens{&_lexer},
                _parser{&_tokens},
                _query{_parser.query()} {
            if (_query) {
                const std::vector<SparqlParser::PrefixDeclContext *> &prefixDecl = _query->prologue()->prefixDecl();
                for (auto &decl : prefixDecl)
                    // remove < and > from <...>
                    prefixes[decl->PNAME_NS()->getText()] = std::string(decl->IRI_REF()->getText(), 1,
                                                                        decl->IRI_REF()->getText().size() - 2);


                SparqlParser::SelectQueryContext *select = _query->selectQuery();
                select_modifier = getSelectModifier(select);
                bool all_vars = false;
                if (std::vector<SparqlParser::VarContext *> vars = select->var(); not vars.empty())
                    for (auto &var : vars)
                        query_variables.push_back(extractVariable(var));
                else
                    all_vars = true;

                std::queue<SparqlParser::TriplesBlockContext *> tripleBlocks;
                for (auto &block : select->whereClause()->groupGraphPattern()->triplesBlock())
                    tripleBlocks.push(block);
                while (not tripleBlocks.empty()) {
                    auto block = tripleBlocks.front();
                    tripleBlocks.pop();
                    SparqlParser::TriplesSameSubjectContext *triplesSameSubject = block->triplesSameSubject();

                    std::variant<Variable, Term> subj = parseVarOrTerm(triplesSameSubject->varOrTerm());
                    registerVariable(subj);
                    SparqlParser::PropertyListNotEmptyContext *propertyListNotEmpty = triplesSameSubject->propertyListNotEmpty();
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
                std::cout << "query variables" << query_variables << std::endl;
                std::cout << "variables" << variables << std::endl;
                std::cout << "bgps" << bgps << std::endl;
                if (std::set<Variable> var_set{query_variables.begin(), query_variables.end()};
                        not std::includes(variables.cbegin(), variables.cend(), var_set.cbegin(), var_set.cend()))
                    throw std::invalid_argument{"query variables must be a subset of BGP variables."};
            } else
                throw std::invalid_argument{"query could not be parsed."};

        }

        SelectModifier getSelectModifier() const {
            return select_modifier;
        }

        const std::vector<Variable> &getQueryVariables() const {
            return query_variables;
        }


        auto getSubscript() -> tensor::einsum::Subscript {
            using namespace tnt::tensor::einsum;
            using namespace tnt::util::types;
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

            return Subscript{ops_labels, result_labels};
        }

        auto getOperandKeys() -> std::vector<std::vector<std::optional<Term>>> {
            using namespace tnt::util::types;
            std::vector<std::vector<std::optional<Term>>> op_keys;
            for (const auto &bgp : bgps) {
                std::vector<std::optional<Term>> op_key{};
                for (const std::variant<Variable, Term> &res : bgp)
                    if (std::holds_alternative<Term>(res))
                        op_key.push_back({std::get<Term>(res)});
                    else
                        op_key.push_back(std::nullopt);
                op_keys.push_back(op_key);
            }
            return op_keys;
        }

    private:

        void registerVariable(std::variant<Variable, Term> &variant) {
            if (std::holds_alternative<Variable>(variant)) {
                Variable &var = std::get<Variable>(variant);
                if (not var._anonym)
                    variables.insert(var);
                else
                    anonym_variables.insert(var);
            }
        }


        auto parseGraphTerm(SparqlParser::GraphTermContext *termContext) -> std::variant<Variable, Term> {
            if (auto *iriRef = termContext->iriRef(); iriRef) {
                return URIRef{getFullIriString(iriRef)};

            } else if (auto *rdfLiteral = termContext->rdfLiteral(); rdfLiteral) {
                auto string_node = rdfLiteral->string();
                std::string literal_string;
                if (auto *stringLiteral1 = string_node->STRING_LITERAL1(); stringLiteral1) {
                    auto literal1 = stringLiteral1->getText();

                    static std::regex double_quote{"\"",std::regex::optimize};
                    static std::regex single_quote("\\'",std::regex::optimize);

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
                    if (tree::TerminalNode *plus = doubleNumberic->DOUBLE(); plus) {
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
                                "\"" + plus->getText() + "\"^^\"http://www.w3.org/2001/XMLSchema#integer>"};
                    } else {
                        return Literal{
                                "\"" + decimalNumeric->getText() +
                                "\"^^<http://www.w3.org/2001/XMLSchema#integer>"};
                    }
                }

            } else if (termContext->booleanLiteral()) {
                return Literal{
                        "\"" + termContext->getText() + "\"^^<http://www.w3.org/2001/XMLSchema#boolean>"};
            } else if (SparqlParser::BlankNodeContext *blankNode = termContext->blankNode();blankNode) {
                if (blankNode->BLANK_NODE_LABEL())
                    return Variable{termContext->getText()};
                else
                    return Variable{"__:" + std::to_string(next_anon_var_id++)};
            } else {
                // TODO: handle NIL value "( )"
            }
        }

        auto parseVarOrTerm(SparqlParser::VarOrTermContext *varOrTerm) -> std::variant<Variable, Term> {
            if (varOrTerm->var())
                return std::variant<Variable, Term>{extractVariable(varOrTerm->var())};
            else
                return parseGraphTerm(varOrTerm->graphTerm());
        }

        auto parseObject(SparqlParser::ObjectContext *obj) -> std::variant<Variable, Term> {
            SparqlParser::VarOrTermContext *varOrTerm = obj->graphNode()->varOrTerm();
            // TODO: consider obj->graphNode()->triplesNode()
            return parseVarOrTerm(varOrTerm);
        }

        auto parseVerb(SparqlParser::VerbContext *verb) -> std::variant<Variable, Term> {
            if (auto *varOrIRIref = verb->varOrIRIref(); varOrIRIref) {
                if (varOrIRIref->var()) {
                    return std::variant<Variable, Term>{extractVariable(varOrIRIref->var())};
                } else {
                    SparqlParser::IriRefContext *iriRef = varOrIRIref->iriRef();

                    return std::variant<Variable, Term>{URIRef{getFullIriString(iriRef)}};

                }
            } else { // is 'a'
                return std::variant<Variable, Term>{
                        URIRef{"<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"}};
            }
        }


        auto getFullIriString(SparqlParser::IriRefContext *iriRef) const -> std::string {
            if (tree::TerminalNode *complete_ref = iriRef->IRI_REF(); complete_ref) {
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

        auto getSelectModifier(SparqlParser::SelectQueryContext *select) -> SelectModifier {
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

        auto extractVariable(SparqlParser::VarContext *var) -> Variable {

            const std::string &data = var->getText();
            return Variable{std::string{data, 1, data.length() - 1}};
        }
    };
}


#endif //TNT_SPARQLPARSER_HPP
