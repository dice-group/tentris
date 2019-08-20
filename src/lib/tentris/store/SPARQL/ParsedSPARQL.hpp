#ifndef TENTRIS_SPARQLPARSER_HPP
#define TENTRIS_SPARQLPARSER_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <optional>
#include <exception>
#include <memory>
#include <tuple>

#include <antlr4-runtime.h>
#include <SparqlParser.h>
#include <SparqlLexer.h>
#include <SparqlBaseListener.h>

#include <boost/algorithm/string.hpp>

#include <einsum/internal/Subscript.hpp>

#include "tentris/util/All.hpp"
#include "tentris/store/RDF/Term.hpp"
#include "tentris/store/SPARQL/Variable.hpp"


namespace tentris::store::sparql {

	namespace {
		using Subscript =  einsum::internal::Subscript;
		using namespace tentris::util::types;
		using namespace tentris::store::rdf;
		using SparqlParser = tentris::a4grammar::sparql::SparqlParser;
		using namespace fmt::literals;
		using VarOrTerm = std::variant<Variable, Term>;
		using TriplePattern = std::vector<VarOrTerm>;
	}


	enum SelectModifier {
		NONE,
		DISTINCT,
		REDUCE
	};

	class LexerErrorListener : public antlr4::BaseErrorListener {
	public:
		LexerErrorListener() = default;

		void
		syntaxError([[maybe_unused]]antlr4::Recognizer *recognizer, [[maybe_unused]]antlr4::Token *offendingSymbol,
		            [[maybe_unused]]size_t line, [[maybe_unused]]size_t charPositionInLine, const std::string &msg,
		            [[maybe_unused]]std::exception_ptr e) override {
			throw std::invalid_argument{msg};
		}
	};

	class ParserErrorListener : public antlr4::BaseErrorListener {
	public:
		ParserErrorListener() = default;

		void
		syntaxError([[maybe_unused]]antlr4::Recognizer *recognizer, [[maybe_unused]]antlr4::Token *offendingSymbol,
		            [[maybe_unused]]size_t line, [[maybe_unused]]size_t charPositionInLine, const std::string &msg,
		            [[maybe_unused]]std::exception_ptr e) override {
			throw std::invalid_argument{msg};
		}
	};


	class ParsedSPARQL {


		// TODO: don't store all that stuff
		std::string sparql_str;
		std::istringstream str_stream;
		antlr4::ANTLRInputStream input;
		tentris::a4grammar::sparql::SparqlLexer lexer;
		antlr4::CommonTokenStream tokens;
		SparqlParser parser;
		SparqlParser::QueryContext *_query;

		std::map<std::string, std::string> prefixes{};
		SelectModifier select_modifier;
		std::vector<Variable> query_variables{};
		std::set<Variable> variables{};
		std::set<Variable> anonym_variables{};
		std::set<TriplePattern> bgps;
		uint next_anon_var_id = 0;
		std::shared_ptr<Subscript> subscript;

	public:

		explicit ParsedSPARQL(const std::string &sparqlstr) :
				sparql_str{sparqlstr},
				str_stream{sparql_str},
				input{str_stream},
				lexer{&input},
				tokens{&lexer},
				parser{&tokens} {
			// replace the error handler
			auto lexerErrorListener = LexerErrorListener{};
			lexer.removeErrorListeners();
			lexer.addErrorListener(&lexerErrorListener);

			auto parserErrorListener = ParserErrorListener{};
			parser.removeParseListeners();
			parser.removeErrorListeners();
			parser.addErrorListener(&parserErrorListener);
			// check that _query is present
			_query = parser.query();
			if (_query == nullptr)
				throw std::invalid_argument("The query was not parsable");
			if (_query) {
				const std::vector<SparqlParser::PrefixDeclContext *> &prefixDecl = _query->prologue()->prefixDecl();
				for (auto &prefix : prefixDecl)
					// remove < and > from <...>
					prefixes[prefix->PNAME_NS()->getText()] = std::string(prefix->IRI_REF()->getText(), 1,
					                                                      prefix->IRI_REF()->getText().size() - 2);


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

					VarOrTerm subj = parseVarOrTerm(triplesSameSubject->varOrTerm());
					registerVariable(subj);
					SparqlParser::PropertyListNotEmptyContext *propertyListNotEmpty = triplesSameSubject->propertyListNotEmpty();
					for (auto[pred_node, obj_nodes] : zip(propertyListNotEmpty->verb(),
					                                      propertyListNotEmpty->objectList())) {
						VarOrTerm pred = parseVerb(pred_node);
						registerVariable(pred);

						for (auto &obj_node : obj_nodes->object()) {
							VarOrTerm obj = parseObject(obj_node);
							registerVariable(obj);

							bgps.insert(TriplePattern{subj, pred, obj});
						}
					}
					if (auto *next_block = block->triplesBlock(); next_block)
						tripleBlocks.push(next_block);
				}
				if (all_vars)
					for (const auto &variable : variables)
						query_variables.push_back(variable);

				if (query_variables.empty())
					throw std::invalid_argument{"Empty query variables is not allowed."};



				using Label = Subscript::Label;
				// generate subscript
				std::map<Variable, Label> var_to_label{};
				Label next_label = 'a';
				for (const auto &var : variables) {
					var_to_label[var] = next_label++;
				}
				std::vector<std::vector<Label>> ops_labels{};
				for (const auto &bgp : bgps) {
					std::vector<Label> op_labels{};
					int count = 0;
					for (const std::variant<Variable, Term> &res : bgp)
						if (std::holds_alternative<Variable>(res)) {
							op_labels.push_back(var_to_label[std::get<Variable>(res)]);
							++count;
						}
					if (count) // removes operands without labels/variables
						ops_labels.push_back(op_labels);
				}

				std::vector<Label> result_labels{};
				for (const auto &query_variable : query_variables) {
					result_labels.push_back(var_to_label[query_variable]);
				}

				subscript = std::make_shared<Subscript>(ops_labels, result_labels);
			} else
				throw std::invalid_argument{"query could not be parsed."};

		}

		SelectModifier getSelectModifier() const {
			return select_modifier;
		}

		const std::vector<Variable> &getQueryVariables() const {
			return query_variables;
		}

		const std::set<Variable> &getVariables() const {
			return variables;
		}

		const std::set<Variable> &getAnonymVariables() const {
			return anonym_variables;
		}

		const std::string &getSparqlStr() const {
			return sparql_str;
		}

		const std::shared_ptr<Subscript> &getSubscript() const {
			return subscript;
		}

		const std::set<TriplePattern> &getBgps() const {
			return bgps;
		}

	private:

		void registerVariable(VarOrTerm &variant) {
			if (std::holds_alternative<Variable>(variant)) {
				auto &var = std::get<Variable>(variant);
				if (not var.is_anonym)
					variables.insert(var);
				else
					anonym_variables.insert(var);
			}
		}


		auto parseGraphTerm(
				SparqlParser::GraphTermContext *termContext) -> VarOrTerm {
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
			} else if (SparqlParser::BlankNodeContext *blankNode = termContext->blankNode();blankNode) {
				if (blankNode->BLANK_NODE_LABEL())
					return Variable{termContext->getText()};
				else
					return Variable{"__:" + std::to_string(next_anon_var_id++)};
			} else {
				throw std::logic_error{"Handling NIL not yet implemented."};
				// TODO: handle NIL value "( )"
			}
		}

		auto parseVarOrTerm(
				SparqlParser::VarOrTermContext *varOrTerm) -> VarOrTerm {
			if (varOrTerm->var())
				return VarOrTerm{extractVariable(varOrTerm->var())};
			else
				return parseGraphTerm(varOrTerm->graphTerm());
		}

		auto parseObject(SparqlParser::ObjectContext *obj) -> VarOrTerm {
			SparqlParser::VarOrTermContext *varOrTerm = obj->graphNode()->varOrTerm();
			// TODO: consider obj->graphNode()->triplesNode()
			return parseVarOrTerm(varOrTerm);
		}

		auto parseVerb(SparqlParser::VerbContext *verb) -> VarOrTerm {
			if (auto *varOrIRIref = verb->varOrIRIref(); varOrIRIref) {
				if (varOrIRIref->var()) {
					return VarOrTerm{extractVariable(varOrIRIref->var())};
				} else {
					SparqlParser::IriRefContext *iriRef = varOrIRIref->iriRef();

					return VarOrTerm{URIRef{getFullIriString(iriRef)}};

				}
			} else { // is 'a'
				return VarOrTerm{
						URIRef{"<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"}};
			}
		}


		auto getFullIriString(SparqlParser::IriRefContext *iriRef) const -> std::string {
			if (antlr4::tree::TerminalNode *complete_ref = iriRef->IRI_REF(); complete_ref) {
				return complete_ref->getText();
			} else {
				auto *prefixedName = iriRef->prefixedName();

				if (auto *pname_both = prefixedName->PNAME_LN();pname_both) {
					const std::string &pname_both_str = pname_both->getText();
					unsigned long i = pname_both_str.find(':') + 1;
					auto prefix_string = std::string{pname_both_str, 0, i};
					auto suffix_string = std::string{pname_both_str, i, pname_both_str.size() - i};
					try {
						return "<{}{}>"_format(prefixes.at(prefix_string), suffix_string);
					} catch (const std::out_of_range &exc) {
						throw std::invalid_argument{"Undefined prefix {} used."_format(prefix_string)};
					}
				} else {
					// TODO: this looks wrong
					auto *default_prefix = prefixedName->PNAME_NS();
					const std::string &resolvedPrefix = prefixes.at(default_prefix->getText());
					return {"<" + resolvedPrefix + ">"};
				}
			}
		}

		auto getSelectModifier(SparqlParser::SelectQueryContext *select) -> SelectModifier {
			auto *modifier = select->selectModifier();
			if (!modifier->children.empty()) {
				const std::string &modifier_str = modifier->children[0]->toString();
				if (modifier_str == "DISTINCT") {
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

	public:
		friend struct fmt::formatter<tentris::store::sparql::ParsedSPARQL>;
	};
}


template<>
struct fmt::formatter<tentris::store::sparql::ParsedSPARQL> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::sparql::ParsedSPARQL &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
		                 " prefixes:         {}\n"
		                 " select_modifier:  {}\n"
		                 " query_variables:  {}\n"
		                 " variables:        {}\n"
		                 " anonym_variables: {}\n"
		                 " bgps:             {}\n",
		                 p.prefixes, p.select_modifier, p.query_variables, p.variables, p.anonym_variables, p.bgps);
	}
};

#endif //TENTRIS_SPARQLPARSER_HPP

