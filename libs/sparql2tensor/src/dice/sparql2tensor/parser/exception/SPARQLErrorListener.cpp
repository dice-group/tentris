#include "dice/sparql2tensor/parser/exception/SPARQLErrorListener.hpp"

namespace dice::sparql2tensor::parser::exception {

	void SPARQLErrorListener::syntaxError([[maybe_unused]] antlr4::Recognizer *recognizer,
										  [[maybe_unused]] antlr4::Token *offendingSymbol,
										  size_t line,
										  size_t charPositionInLine,
										  const std::string &msg,
										  [[maybe_unused]] std::exception_ptr e) {
		std::ostringstream error_msg;
		error_msg << "Syntax error: " << msg << ". At line: " << line << " and position: " << charPositionInLine;
		throw std::runtime_error(error_msg.str());
	}

} // namespace dice::sparql2tensor::parser::exception
