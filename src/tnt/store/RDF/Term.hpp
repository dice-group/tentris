#ifndef TEST_NODE_HPP
#define TEST_NODE_HPP

#include "tnt/util/All.hpp"
#include <string_view>

#include <stdexcept>
#include <string>
#include <optional>
#include <exception>
#include <regex>

namespace tnt::store {

    namespace {
        static const std::regex is_bnode_regex{"^_:(?:.*)$", std::regex::optimize};

        static const std::regex is_uri_regex{"^<(?:.*)>$", std::regex::optimize};

        static const std::regex is_literal_regex{"^\"(?:[^]*)\"(?:@(?:.*)|\\^\\^<(?:.*)>|)$", std::regex::optimize};
    }

    class Term {
    public:
        enum NodeType {
            URI,
            BNode,
            Literal

        };

    protected:
        std::string _identifier;
        NodeType _node_type;
        std::string_view _value = nullptr;
        std::string_view _lang = nullptr;
        std::string_view _type = nullptr;


        explicit Term(std::string identifier) : _identifier(identifier) {};

        explicit Term(NodeType node_type) : _node_type{node_type} {};

        Term(std::string identifier, NodeType node_type) : _identifier{identifier}, _node_type{node_type} {};
    public:
        Term(const Term &term) = default;

        const std::string &getIdentifier() const {
            return _identifier;
        }

        inline const NodeType &type() {
            return _node_type;
        }

        inline const std::string_view &get_value() const {
            return _value;
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
    };

    class URIRef : public Term {

    public:
        explicit URIRef(std::string identifier) : Term{identifier, NodeType::URI} {
            _value = std::string_view{_identifier.data() + 1, _identifier.size() - 2};
        };
    };

    class BNode : public Term {
    public:
        explicit BNode(std::string identifier) : Term{identifier, NodeType::BNode} {
            _value = std::string_view{_identifier.data() + 2, _identifier.size() - 2};
        };

    };

/**
 * Regex with groups [1]: literal string, [2]: language tag, [3]: type tag. [2] and [3] are exclusive.
 */
    static const std::regex literal_regex{"^\"([^]*)\"(?:@(.*)|\\^\\^<(.*)>|)$", std::regex::optimize};

    class Literal : public Term {

    public:
        explicit Literal(std::string identifier) : Term{identifier, NodeType::Literal} {

            std::match_results<std::string::const_iterator> mr;
            match_regex:
            // check if the regex matched
            if (std::regex_match(_identifier, mr, literal_regex)) {
                // get a iterator to the beginning of the matched string
                const std::basic_string<char>::const_iterator &identifer_it = mr[0].first;
                if (const auto &type_group = mr[3]; type_group.matched){
                    if(type_group.str() == "http://www.w3.org/2001/XMLSchema#string") {
                        _identifier = std::string{_identifier, 0, size_t(type_group.first - identifer_it - 3)};
                        goto match_regex;
                    } else{
                        char *type_raw = _identifier.data() + (type_group.first - identifer_it);
                        _type = {type_raw, (size_t) type_group.length()};
                    }
                } else if (const auto &lang_group = mr[2]; lang_group.matched) {
                    char *lang_raw = _identifier.data() + (lang_group.first - identifer_it);
                    _lang = {lang_raw, (size_t) lang_group.length()};
                    // check if it has a type tag
                }

                const auto &literal_group = mr[1];
                assert(literal_group.matched);
                _value = {_identifier.data() + 1, (size_t) literal_group.length()};
                return;
            }
            throw std::invalid_argument{"Literal string was malformed."};

        }

        Literal(std::string identifier, std::optional<std::string> lang, std::optional<std::string> type)
                : Term{NodeType::Literal} {

            if (lang) {
                _identifier = "\"" + identifier + "\"@" + *lang;
                _lang = {_identifier.data() + 1 + identifier.size() + 2, lang->size()};
            } else if (type and type != "http://www.w3.org/2001/XMLSchema#string") {
                // string tags shall not be stored as they are implicit
                _identifier = "\"" + identifier + "\"^^<" + *type + ">";
                _type = {_identifier.data() + 1 + identifier.size() + 4, type->size()};
            } else {
                _identifier = "\"" + identifier + "\"";
            }
            _value = {_identifier.data() + 1, identifier.size()};
        }

        bool hasLang() const {
            return _lang != nullptr;
        }

        bool hasType() const {
            return _type != nullptr;
        }

        const std::string_view &getLang() const {
            return _lang;
        }

        const std::string_view &getType() const {
            return _type;
        }
    };

    std::unique_ptr<Term> parseTerm(const std::string &term) {
        if (std::regex_match(term, is_literal_regex))
            return std::unique_ptr<Term>{new Literal{term}};
        else if (std::regex_match(term, is_uri_regex))
            return std::unique_ptr<Term>{new URIRef{term}};
        else if (std::regex_match(term, is_bnode_regex))
            return std::unique_ptr<Term>{new BNode{term}};
        throw std::invalid_argument{"RDF term string was malformed."};
    }
};


template<>
struct std::hash<tnt::store::Term> {
    size_t operator()(const tnt::store::Term &v) const {
        std::hash<std::string> hasher;
        return hasher(v.getIdentifier());
    }
};

std::ostream &operator<<(std::ostream &os, const tnt::store::Term &p) {
    os << p.getIdentifier();
    return os;
}

#endif //TEST_NODE_HPP
