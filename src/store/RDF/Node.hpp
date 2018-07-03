#ifndef TEST_NODE_HPP
#define TEST_NODE_HPP

#include "../../util/All.hpp"
#include <string>

#include <stdexcept>
#include <string>
#include <optional>

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

class Literal : public Node {
public:
    explicit Literal(std::string identifier) : Node{identifier} {};

    explicit Literal(std::string identifier, std::optional<std::string> lang, std::optional<std::string> type)
            : Node{{}} {
        if (lang) {
            _identifier = "\"" + identifier + "\"@" + *lang;
        }
        if (type) {
            _identifier = "\"" + identifier + "\"^^" + *type;
        }
    }

};

#endif //TEST_NODE_HPP
