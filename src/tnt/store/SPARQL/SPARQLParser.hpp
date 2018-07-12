#ifndef TNT_SPARQLPARSER_HPP
#define TNT_SPARQLPARSER_HPP

// ---> move to own library to remove it from global namespace
#include "tnt/store/SPARQL/antlr/SparqlParser.cpp"
#include "tnt/store/SPARQL/antlr/SparqlLexer.cpp"
#include "tnt/store/SPARQL/antlr/SparqlBaseListener.cpp"
// <--- move to own library to remove it from global namespace
#include "tnt/tensor/einsum/Subscript.hpp"
#include "tnt/store/RDF/Term.hpp"
#include <antlr4-runtime.h>
#include "tnt/util/All.hpp"

#include <sstream>
#include <string>
#include <iostream>
#include <memory>


namespace tnt::store::sparql {
    enum SelectModifier {
        NONE,
        DISTINCT,
        REDUCE
    };

    class Variable {
    public:
        const std::string _var_name;

        Variable(std::string var_name) : _var_name{var_name} {}

        inline bool operator==(const Variable &rhs) const {
            return _var_name == rhs._var_name;
        }

        inline bool operator!=(const Variable &rhs) const {
            return _var_name != rhs._var_name;
        }

        inline bool operator<(const Variable &rhs) const {
            return _var_name < rhs._var_name;
        }

        inline bool operator>(const Variable &rhs) const {
            return _var_name > rhs._var_name;
        }

        friend std::ostream &operator<<(std::ostream &os, const Variable &p) {
            os << p._var_name;
            return os;
        }
    };

    class SPARQLParser {

        std::istringstream str_stream;
        ANTLRInputStream input;
        SparqlLexer lexer;
        CommonTokenStream tokens;
        SparqlParser parser;
        SparqlParser::QueryContext *query;

    public:

        SPARQLParser(std::string sparql_str) : str_stream{sparql_str},
                                               input{str_stream},
                                               lexer{&input},
                                               tokens{&lexer},
                                               parser{&tokens},
                                               query{parser.query()} {}

        Term parseGraphTerm(SparqlParser::GraphTermContext *termContext) {
            if (termContext->iriRef() != nullptr) {
                return URIRef{termContext->getText()};
            } else if (termContext->rdfLiteral() != nullptr) {
                return Literal{termContext->getText()};
            } else if (termContext->numericLiteral() != nullptr) {
                // TODO: handle raw numeric Literals
            } else if (termContext->booleanLiteral() != nullptr) {
                // TODO: handle boolean variables
            } else if (termContext->blankNode() != nullptr) {
                // TODO: handle anonymous variables
                return BNode(termContext->getText());
            } else {
                // TODO: handle NIL value "( )"
            }
        }

        auto getSubscript() -> tensor::einsum::Subscript {
            using namespace tnt::store::sparql::detail;
            using namespace antlr4;
            SelectModifier p_select_modifier;
            std::set<Variable> query_variables{};
            std::set<Variable> variables{};
            std::set<std::vector<std::variant<Variable, Term>>> bgps;
            if (query != nullptr) {
                SparqlParser::SelectQueryContext *select = query->selectQuery();
                p_select_modifier = getSelectModifier(select);
                std::vector<SparqlParser::VarContext *> vars = select->var();
                for (auto &var : vars) {
                    query_variables.insert(extractVariable(var));
                }

                std::vector<SparqlParser::TriplesBlockContext *> triplesBlock = select->whereClause()->groupGraphPattern()->triplesBlock();
                for (auto &block : triplesBlock) {
                    SparqlParser::TriplesSameSubjectContext *triplesSameSubject = block->triplesSameSubject();

                    std::variant<Variable, Term> subj = parseVarOrTerm(triplesSameSubject->varOrTerm());
                    if (std::holds_alternative<Variable>(subj)) variables.insert(std::get<Variable>(subj));

                    SparqlParser::PropertyListNotEmptyContext *propertyListNotEmpty = triplesSameSubject->propertyListNotEmpty();
                    for (auto[pred_node, obj_nodes] : zip(propertyListNotEmpty->verb(),
                                                          propertyListNotEmpty->objectList())) {
                        std::variant<Variable, Term> pred = parseVerb(pred_node);
                        if (std::holds_alternative<Variable>(pred)) variables.insert(std::get<Variable>(pred));
                        for (auto &obj_node : obj_nodes->object()) {
                            std::variant<Variable, Term> obj = parseObject(obj_node);
                            if (std::holds_alternative<Variable>(obj)) variables.insert(std::get<Variable>(obj));

                            bgps.insert(std::vector<std::variant<Variable, Term>>{subj, pred, obj});
                        }
                    }
                }
            }

            std::cout << "query variables" << query_variables << std::endl;
            std::cout << "variables" << variables << std::endl;
//            std::cout << "bgps" << bgps << std::endl;
        }

        auto parseVarOrTerm(SparqlParser::VarOrTermContext *varOrTerm) -> std::variant<Variable, Term> {
            if (varOrTerm->var() != nullptr)
                return std::variant<Variable, Term>{extractVariable(varOrTerm->var())};
            else
                return std::variant<Variable, Term>{parseGraphTerm(varOrTerm->graphTerm())};
        }

        auto parseObject(SparqlParser::ObjectContext *obj) -> std::variant<Variable, Term> {
            SparqlParser::VarOrTermContext *varOrTerm = obj->graphNode()->varOrTerm();
            // TODO: consider obj->graphNode()->triplesNode()
            return parseVarOrTerm(varOrTerm);
        }

        auto parseVerb(SparqlParser::VerbContext *verb) -> std::variant<Variable, Term> {
            if (SparqlParser::VarOrIRIrefContext *varOrIRIref = verb->varOrIRIref(); varOrIRIref != nullptr) {
                if (varOrIRIref->var() != nullptr)
                    return std::variant<Variable, Term>{extractVariable(varOrIRIref->var())};
                else
                    return std::variant<Variable, Term>{URIRef(varOrIRIref->getText())};
            } else { // is 'a'
                return std::variant<Variable, Term>(URIRef{"<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"});
            }
        }

        auto getSelectModifier(SparqlParser::SelectQueryContext *select) -> SelectModifier {
            SparqlParser::SelectModifierContext *modifier = select->selectModifier();
            if (modifier->children.size() != 0) {
                const std::__cxx11::string &string = modifier->children[0]->toString();
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
            return std::string{data, 1, data.length() - 1};
        }
    };


}


#endif //TNT_SPARQLPARSER_HPP
