#ifndef TNT_VARIABLE_HPP
#define TNT_VARIABLE_HPP

#include <string>
#include <iostream>

namespace tnt::store::sparql {

    class Variable {
    public:
        const std::string name;
        const bool is_anonym;

        explicit Variable(std::string var_name, bool anonym = false) : name{var_name}, is_anonym{anonym} {}

        inline bool operator==(const Variable &rhs) const {
            return name == rhs.name;
        }

        inline bool operator!=(const Variable &rhs) const {
            return name != rhs.name;
        }

        inline bool operator<(const Variable &rhs) const {
            return name < rhs.name;
        }

        inline bool operator>(const Variable &rhs) const {
            return name > rhs.name;
        }

        friend std::ostream &operator<<(std::ostream &os, const Variable &p) {
            os << "?" << p.name;
            return os;
        }
    };

}
#endif //TNT_VARIABLE_HPP
