#ifndef TENTRIS_TRIPLEPATTERN_HPP
#define TENTRIS_TRIPLEPATTERN_HPP

#include <array>
#include <variant>
#include <Dice/rdf_parser/RDF/Term.hpp>
#include "tentris/store/SPARQL/Variable.hpp"

namespace tentris::store::sparql {
	using VarOrTerm = std::variant<Variable, rdf_parser::store::rdf::Term>;
	using TriplePattern = std::array<VarOrTerm, 3>;
}

#endif //TENTRIS_TRIPLEPATTERN_HPP
