#ifndef DICE_SPARQL_PROLOGUEVISITOR_HPP
#define DICE_SPARQL_PROLOGUEVISITOR_HPP

#include <rdf4cpp/rdf.hpp>

#include <SparqlParser/SparqlParserBaseVisitor.h>

#include <robin_hood.h>


namespace dice::sparql2tensor::parser::visitors {

	using namespace dice::sparql_parser::base;

	class PrologueVisitor : public SparqlParserBaseVisitor {
		rdf4cpp::rdf::IRIFactory *prefixes_;

	public:
		PrologueVisitor(rdf4cpp::rdf::IRIFactory &prefixes) noexcept;

		std::any visitPrologue(SparqlParser::PrologueContext *) override;

		std::any visitBaseDecl(SparqlParser::BaseDeclContext *) override;

		std::any visitPrefixDecl(SparqlParser::PrefixDeclContext *) override;
	};

}// namespace dice::sparql2tensor::parser::visitors

#endif//DICE_SPARQL_PROLOGUEVISITOR_HPP
