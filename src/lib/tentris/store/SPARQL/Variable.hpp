#ifndef TENTRIS_VARIABLE_HPP
#define TENTRIS_VARIABLE_HPP

#include <string>
#include <iostream>
#include <utility>

namespace tentris::store::sparql {

	class Variable {
	public:
		mutable std::string name;
		mutable bool is_anonym;

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
namespace std {
	template<>
	struct hash<tentris::store::sparql::Variable> {
		size_t operator()(const tentris::store::sparql::Variable &v) const noexcept {
			return robin_hood::hash_bytes(v.name.data(), v.name.size());
		}
	};
}


template<>
struct fmt::formatter<tentris::store::sparql::Variable> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::sparql::Variable &p, FormatContext &ctx) {
		return format_to(ctx.out(), "{}", p.name);
	}
};

#endif //TENTRIS_VARIABLE_HPP
