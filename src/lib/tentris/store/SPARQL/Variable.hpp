#ifndef TENTRIS_VARIABLE_HPP
#define TENTRIS_VARIABLE_HPP

#include <string>
#include <iostream>
#include <utility>

namespace tentris::store::sparql {

	class Variable {
	public:
		const std::string name;
		const bool is_anonym;

		explicit Variable(std::string var_name, bool anonym = false) : name{std::move(var_name)}, is_anonym{anonym} {}

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
struct fmt::formatter<tentris::store::sparql::Variable> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::sparql::Variable &p, FormatContext &ctx) {
		return format_to(ctx.begin(), "{}", p.name);
	}
};

#endif //TENTRIS_VARIABLE_HPP
