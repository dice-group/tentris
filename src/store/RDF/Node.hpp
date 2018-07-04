#ifndef TEST_NODE_HPP
#define TEST_NODE_HPP

#include "../../util/All.hpp"
#include <string_view>

#include <stdexcept>
#include <string>
#include <optional>
#include <exception>
#include <regex>

namespace tnt::store {
    class Node {
    protected:
        std::string _identifier;

        explicit Node(std::string identifier) : _identifier(identifier) {};
    public:
        bool operator==(const Node &rhs) const {
            if (typeid(*this) == typeid(rhs))
                if (_identifier == rhs._identifier)
                    return true;
            return false;
        }

        const std::string &getIdentifier() const {
            return _identifier;
        }
    };

    class URIRef : public Node {

    public:
        explicit URIRef(std::string identifier) : Node{identifier} {};
    };

    class BNode : public Node {
    public:
        explicit BNode(std::string identifier) : Node{identifier} {};

    };

/**
 * Regex with groups [1]: literal string, [2]: language tag, [3]: type tag. [2] and [3] are exclusive.
 */
    static const std::regex literal_regex{"^<(.*)>(?:@(.*)|\\^\\^<(.*)>)$"};

    class Literal : public Node {
        std::string_view _literal = nullptr;
        std::string_view _lang = nullptr;
        std::string_view _type = nullptr;
    public:
        explicit Literal(std::string identifier) : Node{identifier} {

            std::match_results<std::string::const_iterator> mr;

            // check if the regex matched
            if (std::regex_match(_identifier, mr, literal_regex)) {
                // get a iterator to the beginning of the matched string
                const std::basic_string<char>::const_iterator &identifer_it = mr[0].first;

                if (const auto &literal_group = mr[1]; literal_group.matched) {
                    // extract the literal
                    _literal = {_identifier.data() + 1, (size_t) literal_group.length()};
                    // check if it has a language tag
                    if (const auto &lang_group = mr[2]; lang_group.matched) {
                        char *lang_raw = _identifier.data() + (lang_group.first - identifer_it);
                        _lang = {lang_raw, (size_t) lang_group.length()};
                        // check if it has a type tag
                    } else if (const auto &type_group = mr[3]; type_group.matched) {
                        char *type_raw = _identifier.data() + (type_group.first - identifer_it);
                        _type = {type_raw, (size_t) type_group.length()};
                    }
                    return;
                }
            }
            throw std::invalid_argument{"Literal string was malformed."};

        };

        Literal(std::string identifier, std::optional<std::string> lang, std::optional<std::string> type)
                : Node{{}} {

            if (lang) {
                _identifier = "\"" + identifier + "\"@" + *lang;
                _lang = {_identifier.data() + 1 + identifier.size() + 2, lang->size()};
            } else if (type) {
                _identifier = "\"" + identifier + "\"^^<" + *type + ">";
                _type = {_identifier.data() + 1 + identifier.size() + 3, type->size()};

            } else {
                _identifier = "\"" + identifier + "\"";
            }
            _literal = {_identifier.data() + 1, identifier.size()};
        }

        bool hasLang() {
            return _lang != nullptr;
        }

        bool hasType() {
            return _type != nullptr;
        }

        const std::string_view &getLang() const {
            return _lang;
        }

        const std::string_view &getType() const {
            return _type;
        }

    };
};

template<>
 struct std::hash<tnt::store::Node> {
    size_t operator()(const tnt::store::Node &v) const {
        std::hash<std::string> hasher;
        return hasher(v.getIdentifier());
    }
};
#endif //TEST_NODE_HPP
