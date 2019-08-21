#ifndef TEST_NODE_HPP
#define TEST_NODE_HPP

#include "tentris/util/All.hpp"
#include <string_view>
#include <absl/hash/hash.h>

#include <stdexcept>
#include <string>
#include <optional>
#include <exception>
#include <regex>
#include <utility>


namespace tentris::store::rdf {

	namespace {
		const static std::regex is_bnode_regex{"^_:(?:.*)$", std::regex::optimize};

		const static std::regex is_uri_regex{"^<(?:.*)>$", std::regex::optimize};

		const static std::regex is_literal_regex{"^\"(?:[^]*)\"(?:@(?:.*)|\\^\\^<(?:.*)>|)$", std::regex::optimize};

		/**
		 * Regex with groups [1]: literal string, [2]: language tag, [3]: type tag. [2] and [3] are exclusive.
		 */
		const static std::regex literal_regex{"^\"([^]*)\"(?:@(.*)|\\^\\^<(.*)>|)$", std::regex::optimize};
	}


	struct unbound_string_view {
		std::ptrdiff_t start = 0;
		std::size_t count = 0;

		[[nodiscard]] std::string_view string_view(const std::string &str) const {
			return {str.data() + start, count};
		}
	};

	class Term {
	public:
		enum NodeType {
			None = 0,
			URI,
			BNode,
			Literal

		};

	protected:
		std::string _identifier{};
		NodeType _node_type{};
		unbound_string_view _value{};
		unbound_string_view _lang{};
		unbound_string_view _data_type{};

	public:
		explicit Term(std::string identifier) : _identifier(std::move(identifier)) {}

		Term(std::string identifier, NodeType nodeType) : _identifier(std::move(identifier)),
														  _node_type(nodeType) {}

	public:
		Term() = default;

		Term(Term &) = default;
		Term(const Term &) = default;
		Term(Term &&) = default;

		Term &operator=(const Term &) = default;

		Term &operator=(Term &&) = default;


		[[nodiscard]] const std::string &getIdentifier() const {
			return _identifier;
		}

		inline const NodeType &type() const {
			return _node_type;
		}

		[[nodiscard]] inline std::string_view value() const {
			return _value.string_view(_identifier);
		}

		[[nodiscard]] inline std::string_view dataType() const {
			return _data_type.string_view(_identifier);
		}

		[[nodiscard]] inline std::string_view lang() const {
			return _lang.string_view(_identifier);
		}

		[[nodiscard]] inline bool hasDataType() const {
			return _data_type.count != 0;
		}

		[[nodiscard]] inline bool hasLang() const {
			return _lang.count != 0;
		}

		inline bool operator==(const Term &rhs) const {
			return _identifier == rhs._identifier;
		}

		inline bool operator!=(const Term &rhs) const {
			return _identifier != rhs._identifier;
		}

		inline bool operator<(const Term &rhs) const {
			return _identifier < rhs._identifier;
		}

		inline bool operator>(const Term &rhs) const {
			return _identifier > rhs._identifier;
		}

		friend bool operator==(const Term &lhs, const std::unique_ptr<Term> &rhs) {
			return lhs == *rhs;
		}

		friend bool operator==(const std::unique_ptr<Term> &lhs, const Term &rhs) {
			return *lhs == rhs;
		}

		friend bool operator==(const std::unique_ptr<Term> &lhs, const std::unique_ptr<Term> &rhs) {
			return *lhs == *rhs;
		}

		friend bool operator==(const Term *lhs, const std::unique_ptr<Term> &rhs) {
			return *lhs == *rhs;
		}

		friend bool operator==(const std::unique_ptr<Term> &lhs, const Term *rhs) {
			return *lhs == *rhs;
		}

		static Term make_literal(const std::string &identifier) {

			Term term{identifier, Literal};

			std::match_results<std::string::const_iterator> mr;
			match_regex:
			// check if the regex matched
			if (std::regex_match(term._identifier, mr, literal_regex)) {
				// get a iterator to the beginning of the matched string
				const std::basic_string<char>::const_iterator &identifer_it = mr[0].first;
				if (const auto &type_group = mr[3]; type_group.matched) {
					if (type_group.str() == "http://www.w3.org/2001/XMLSchema#string") {
						term._identifier = std::string{term._identifier, 0,
													   size_t(type_group.first - identifer_it - 3)};
						goto match_regex;
					} else {
						auto type_start = (type_group.first - identifer_it);
						auto type_count = (std::size_t) type_group.length();
						term._data_type = {type_start, type_count};
					}
				} else if (const auto &lang_group = mr[2]; lang_group.matched) {
					auto lang_start = (lang_group.first - identifer_it);
					auto lang_count = (size_t) lang_group.length();
					term._lang = {lang_start, lang_count};
					// check if it has a type tag
				}

				const auto &literal_group = mr[1];
				assert(literal_group.matched);
				term._value = {1, (size_t) literal_group.length()};
				return term;
			}
			throw std::invalid_argument{"Literal string was malformed."};
		}

		static Term make_lang_literal(const std::string &value, const std::string &lang) {

			Term term{fmt::format(R"("{}"@{})", value, lang), Literal};
			term._value = {1, value.size()};
			term._lang = {(ptrdiff_t) (1 + value.size() + 2), lang.size()};
			return term;
		}

		static Term make_str_literal(const std::string &value) {

			Term term{fmt::format(R"("{}")", value), Literal};
			term._value = {1, value.size()};
			return term;
		}

		static Term make_typed_literal(const std::string &value, const std::string &type) {
			bool is_string_type = (type != "http://www.w3.org/2001/XMLSchema#string");

			Term term{
					(is_string_type)
					? fmt::format(R"("{}"^^<{}>)", value, type)
					: fmt::format(R"("{}")", value),
					Literal};
			if (not is_string_type) {
				term._data_type = {(std::ptrdiff_t) (1 + value.size() + 4), type.size()};
			}
			term._value = {1, value.size()};
			return term;
		}

		static Term make_bnode(const std::string &identifier) {

			Term term{identifier, BNode};

			term._value = {2, term._identifier.size() - 2};
			return term;
		}

		static Term make_uriref(const std::string &identifier) {

			Term term{identifier, URI};

			term._value = {1, term._identifier.size() - 2};
			return term;
		}

		static Term make_term(const std::string &identifier) {
			if (std::regex_match(identifier, is_literal_regex))
				return make_literal(identifier);
			else if (std::regex_match(identifier, is_uri_regex))
				return make_uriref(identifier);
			else if (std::regex_match(identifier, is_bnode_regex))
				return make_bnode(identifier);
			throw std::invalid_argument{"RDF term string was malformed."};
		}

		[[nodiscard]] std::size_t hash() const {
			return absl::Hash<std::string>()(_identifier);
		}
	};


};


template<>
struct std::hash<tentris::store::rdf::Term> {
	size_t operator()(const tentris::store::rdf::Term &v) const {
		return v.hash();
	}
};

template<>
struct std::hash<tentris::store::rdf::Term *> {
	size_t operator()(const tentris::store::rdf::Term *&v) const {
		return v->hash();
	}
};

namespace tentris::store::rdf {
	struct TermHash {
		size_t operator()(const tentris::store::rdf::Term &v) const {
			return v.hash();
		}

		size_t operator()(const std::unique_ptr<tentris::store::rdf::Term> &v) const {
			return v->hash();
		}

		size_t operator()(const tentris::store::rdf::Term *&v) const {
			return v->hash();
		}
	};
}

template<>
struct fmt::formatter<tentris::store::rdf::Term> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::rdf::Term &p, FormatContext &ctx) {
		return format_to(ctx.begin(), p.getIdentifier());
	}
};

#endif //TEST_NODE_HPP
