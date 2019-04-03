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

    };

}

template<>
struct fmt::formatter<tnt::store::sparql::Variable> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const tnt::store::sparql::Variable &p, FormatContext &ctx) {
        return format_to(ctx.begin(), "?{}", p.name);
    }
};

#endif //TNT_VARIABLE_HPP
