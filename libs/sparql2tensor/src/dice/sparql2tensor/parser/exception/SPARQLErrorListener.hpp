#ifndef DICE_SPARQL_SPARQLERRORLISTENER_HPP
#define DICE_SPARQL_SPARQLERRORLISTENER_HPP

#include <rdf4cpp/rdf.hpp>

#include <BaseErrorListener.h>

#include <robin_hood.h>


namespace dice::sparql2tensor::parser::exception {

	class SPARQLErrorListener : public antlr4::BaseErrorListener {

		void syntaxError(antlr4::Recognizer *recognizer,
						 antlr4::Token *offendingSymbol,
						 size_t line,
						 size_t charPositionInLine,
						 const std::string &msg,
						 std::exception_ptr e) override;

	};

}// namespace dice::sparql2tensor::parser::exception

#endif//TENTRIS_BINARIES_SPARQLERRORLISTENER_HPP
