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
#include <regex>

#include <Sparql/SparqlParser.h>
#include <Sparql/SparqlLexer.h>
#include <Sparql/SparqlBaseListener.h>

#include <robin_hood.h>

#include <boost/algorithm/string.hpp>

#include <Dice/einsum/internal/Subscript.hpp>
#include <utility>

#include <Dice/RDF/Term.hpp>
#include <Dice/RDF/ParseTerm.hpp>
#include <Dice/RDF/Triple.hpp>
#include <Dice/SPARQL/TriplePattern.hpp>


namespace tentris::store::sparql {

	namespace {
		using Subscript =  einsum::internal::Subscript;
		namespace parser = Dice::sparql_parser::base;
		using SparqlParser = parser::SparqlParser;
		using namespace fmt::literals;
	}


	enum SelectModifier {
		NONE,
		DISTINCT,
		REDUCE
	};

	class LexerErrorListener : public antlr4::BaseErrorListener {
		using Variable = Dice::sparql::Variable;
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
		using Term = Dice::rdf::Term;
		using BNode = Dice::rdf::BNode;
		using Literal = Dice::rdf::Literal;
		using URIRef = Dice::rdf::URIRef;
		using TriplePattern = Dice::sparql::TriplePattern;
		using VarOrTerm = Dice::sparql::VarOrTerm;
		using Variable = Dice::sparql::Variable;

		using SparqlLexer = parser::SparqlLexer;
		using ANTLRInputStream =antlr4::ANTLRInputStream;
		using CommonTokenStream = antlr4::CommonTokenStream;
		using QueryContext = SparqlParser::QueryContext;
		std::string sparql_str;

		SelectModifier select_modifier = NONE;

		robin_hood::unordered_map<std::string, std::string> prefixes{};
		std::vector<Variable> query_variables{};
		robin_hood::unordered_set<Variable> variables{};
		robin_hood::unordered_set<Variable> anonym_variables{};
		std::vector<TriplePattern> bgps;
		uint next_anon_var_id = 0;
		std::shared_ptr<Subscript> subscript;

	public:

		ParsedSPARQL() = default;


		explicit ParsedSPARQL(std::string sparqlstr) :
				sparql_str{std::move(sparqlstr)} {
			namespace ranges = std::ranges;

			std::istringstream str_stream{sparql_str};
			ANTLRInputStream input{str_stream};
			SparqlLexer lexer{&input};
			CommonTokenStream tokens{&lexer};
			SparqlParser parser{&tokens};
			// replace the error handler
			auto lexerErrorListener = LexerErrorListener{};
			lexer.removeErrorListeners();
			lexer.addErrorListener(&lexerErrorListener);

			auto parserErrorListener = ParserErrorListener{};
			parser.removeParseListeners();
			parser.removeErrorListeners();
			parser.addErrorListener(&parserErrorListener);
			// check that _query is present
			QueryContext *_query = parser.query();
			if (_query == nullptr)
				throw std::invalid_argument("The query was not parsable");
			else {
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
					for (auto[pred_node, obj_nodes] : iter::zip(propertyListNotEmpty->verb(),
																propertyListNotEmpty->objectList())) {
						VarOrTerm pred = parseVerb(pred_node);
						registerVariable(pred);

						for (auto &obj_node : obj_nodes->object()) {
							VarOrTerm obj = parseObject(obj_node);
							registerVariable(obj);
							if(ranges::find(bgps, TriplePattern{subj, pred, obj}) == bgps.end())
								bgps.push_back(TriplePattern{subj, pred, obj});
						}
					}
					if (auto *next_block = block->triplesBlock(); next_block)
						tripleBlocks.push(next_block);
				}
				for (const auto &variable : query_variables)
					variables.insert(variable);
				if (all_vars)
					for (const auto &variable : variables)
						if (not anonym_variables.contains(variable))
							query_variables.push_back(variable);



				using Label = Subscript::Label;
				// generate subscript
				robin_hood::unordered_map<Variable, Label> var_to_label{};
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
			}
		}

		[[nodiscard]] SelectModifier getSelectModifier() const {
			return select_modifier;
		}

		[[nodiscard]] const std::vector<Variable> &getQueryVariables() const {
			return query_variables;
		}

		[[nodiscard]] const robin_hood::unordered_set<Variable> &getVariables() const {
			return variables;
		}

		[[nodiscard]] const robin_hood::unordered_set<Variable> &getAnonymVariables() const {
			return anonym_variables;
		}

		[[nodiscard]] const std::string &getSparqlStr() const {
			return sparql_str;
		}

		[[nodiscard]] const std::shared_ptr<Subscript> &getSubscript() const {
			return subscript;
		}

		[[nodiscard]] const std::vector<TriplePattern> &getBgps() const {
			return bgps;
		}

	private:

		void registerVariable(VarOrTerm &variant) {
			if (std::holds_alternative<Variable>(variant)) {
				auto &var = std::get<Variable>(variant);
				variables.insert(var);
				if (var.isAnon())
					anonym_variables.insert(var);
			}
		}


		auto parseGraphTerm(
				SparqlParser::GraphTermContext *termContext) -> VarOrTerm {
			if (auto *iriRef = termContext->iriRef(); iriRef) {
				return URIRef(getIriString(iriRef));

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
					return Literal{literal_string, std::nullopt, getIriString(type)};
				} else {
					return Literal{literal_string, std::nullopt, std::nullopt};
				}

			} else if (auto *numericLiteral = termContext->numericLiteral();numericLiteral) {

				if (auto *decimalNumeric = numericLiteral->decimalNumeric(); decimalNumeric) {

					if (auto *plus = decimalNumeric->DECIMAL(); plus) {
						return Literal{plus->getText(), std::nullopt, "http://www.w3.org/2001/XMLSchema#decimal"};
					} else {
						return Literal{decimalNumeric->getText(), std::nullopt,
									   "http://www.w3.org/2001/XMLSchema#decimal"};
					}
				} else if (auto *doubleNumberic = numericLiteral->doubleNumberic();doubleNumberic) {
					if (antlr4::tree::TerminalNode *plus = doubleNumberic->DOUBLE(); plus) {
						return Literal{plus->getText(), std::nullopt, "http://www.w3.org/2001/XMLSchema#double"};
					} else {
						return Literal{doubleNumberic->getText(), std::nullopt,
									   "http://www.w3.org/2001/XMLSchema#double"};
					}
				} else {
					auto *integerNumeric = numericLiteral->integerNumeric();
					if (auto *plus = integerNumeric->INTEGER(); plus) {
						return Literal{plus->getText(), std::nullopt, "http://www.w3.org/2001/XMLSchema#integer"};
					} else {
						return Literal{integerNumeric->getText(), std::nullopt,
									   "http://www.w3.org/2001/XMLSchema#integer"};
					}
				}

			} else if (termContext->booleanLiteral()) {
				return Literal{termContext->getText(), std::nullopt, "http://www.w3.org/2001/XMLSchema#boolean"};
			} else if (SparqlParser::BlankNodeContext *blankNode = termContext->blankNode();blankNode) {
				if (blankNode->BLANK_NODE_LABEL())
					return Variable{termContext->getText(), true};
				else
					return Variable{"__:" + std::to_string(next_anon_var_id++), true};
			} else if (not termContext->NIL()) {
				return Dice::rdf::parse_term(termContext->getText());
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

					return VarOrTerm{URIRef{getIriString(iriRef)}};

				}
			} else { // is 'a'
				return VarOrTerm{URIRef{"http://www.w3.org/1999/02/22-rdf-syntax-ns#type"}};
			}
		}

		auto getIriString(SparqlParser::IriRefContext *iriRef) const -> std::string {
			if (antlr4::tree::TerminalNode *complete_ref = iriRef->IRI_REF(); complete_ref) {
				auto fullIri = complete_ref->getText();
				return std::string{fullIri, 1, fullIri.size() - 2};
			} else {
				auto *prefixedName = iriRef->prefixedName();

				if (auto *pname_both = prefixedName->PNAME_LN();pname_both) {
					const std::string &pname_both_str = pname_both->getText();
					unsigned long i = pname_both_str.find(':') + 1;
					auto prefix_string = std::string{pname_both_str, 0, i};
					auto suffix_string = std::string{pname_both_str, i, pname_both_str.size() - i};
					try {
						return "{}{}"_format(prefixes.at(prefix_string), suffix_string);
					} catch (const std::out_of_range &exc) {
						throw std::invalid_argument{"Undefined prefix {} used."_format(prefix_string)};
					}
				} else {
					// TODO: this looks wrong
					auto *default_prefix = prefixedName->PNAME_NS();
					const std::string &resolvedPrefix = prefixes.at(default_prefix->getText());
					return resolvedPrefix;
				}
			}
		}


		auto getFullIriString(SparqlParser::IriRefContext *iriRef) const -> std::string {
			return {"<" + getIriString(iriRef) + ">"};
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
		return format_to(ctx.out(),
						 " prefixes:         {}\n"
						 " select_modifier:  {}\n"
						 " query_variables:  {}\n"
						 " variables:        {}\n"
						 " anonym_variables: {}\n"
						 " bgps:             {}\n",
						 fmt::join(p.prefixes, ","),
						 p.select_modifier,
						 fmt::join(p.query_variables, ","),
						 fmt::join(p.variables, ","),
						 fmt::join(p.anonym_variables, ","),
						 fmt::join(p.bgps, ",")
						 );
	}
};

#endif //TENTRIS_SPARQLPARSER_HPP

