#ifndef TNT_VARIABLE_HPP
#define TNT_VARIABLE_HPP

#include <string>
#include <iostream>

class Variable {
public:
    const std::string _var_name;
    const bool _anonym;

    explicit Variable(std::string var_name, bool anonym = false) : _var_name{var_name}, _anonym{anonym} {}

    inline bool operator==(const Variable &rhs) const {
        return _var_name == rhs._var_name;
    }

    inline bool operator!=(const Variable &rhs) const {
        return _var_name != rhs._var_name;
    }

    inline bool operator<(const Variable &rhs) const {
        return _var_name < rhs._var_name;
    }

    inline bool operator>(const Variable &rhs) const {
        return _var_name > rhs._var_name;
    }

    friend std::ostream &operator<<(std::ostream &os, const Variable &p) {
        os << "?" << p._var_name;
        return os;
    }
};

#endif //TNT_VARIABLE_HPP
