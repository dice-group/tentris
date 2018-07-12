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

        Term parseSubject(SparqlParser::GraphTermContext *termContext) {
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
            std::set<std::vector<std::variant<Term, Variable>>> bgps;
            if (query != nullptr) {
                SparqlParser::SelectQueryContext *select = query->selectQuery();
                p_select_modifier = getSelectModifier(select);
                std::vector<SparqlParser::VarContext *> vars = select->var();
                for (auto &var : vars) {
                    query_variables.insert(extractVariable(var));
                }
                SparqlParser::WhereClauseContext *where = select->whereClause();
                SparqlParser::GroupGraphPatternContext *groupGraphPattern = where->groupGraphPattern();
                std::vector<SparqlParser::TriplesBlockContext *> triplesBlock = groupGraphPattern->triplesBlock();
                for (auto &block : triplesBlock) {
                    SparqlParser::TriplesSameSubjectContext *triplesSameSubject = block->triplesSameSubject();
                    SparqlParser::VarOrTermContext *subj_node = triplesSameSubject->varOrTerm();

                    std::variant<Variable, Term> subj = (subj_node->var() != nullptr) ?
                                                           std::variant<Variable, Term>{extractVariable(subj_node->var())} :
                                                           std::variant<Variable, Term>{
                                                                   parseSubject(subj_node->graphTerm())};

                    SparqlParser::PropertyListNotEmptyContext *propertyListNotEmpty = triplesSameSubject->propertyListNotEmpty();
                    const std::vector<SparqlParser::VerbContext *> &vector = propertyListNotEmpty->verb();
                    const std::vector<SparqlParser::ObjectListContext *> &obj_nodess = propertyListNotEmpty->objectList();


                    for (auto[pred_node, obj_nodes] : zip(propertyListNotEmpty->verb(),
                                                          propertyListNotEmpty->objectList())) {
                        for (auto obj_node : obj_nodes->object()) {

                            std::cout << pred_node->getText() << " " << obj_node->getText() << std::endl;
                        }
                    }

//                    std::cout << block->toStringTree() << std::endl;
                }

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
