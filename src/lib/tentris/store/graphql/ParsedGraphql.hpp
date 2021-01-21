#ifndef TENTRIS_PARSEDGRAPHQL_HPP
#define TENTRIS_PARSEDGRAPHQL_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <optional>
#include <exception>
#include <memory>
#include <tuple>
#include <regex>
#include <utility>

#include <boost/algorithm/string.hpp>
#include <Dice/einsum/internal/Subscript.hpp>
#include <robin_hood.h>

namespace tentris::store::graphql {

	namespace {
		using Subscript = einsum::internal::Subscript;
		using namespace fmt::literals;
	}

    class ParsedGraphql {

	private:

        std::string graphql_str;

	public:

        ParsedGraphql() = default;

        explicit ParsedGraphql(std::string graphql_str) :
                graphql_str{std::move(graphql_str)} {
		}

	};

}

#endif//TENTRIS_PARSEDGRAPHQL_HPP
