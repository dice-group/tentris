#ifndef TENTRIS_VARIABLE_HPP
#define TENTRIS_VARIABLE_HPP

#include<Dice/Sparql-Query/TripleVariable.hpp>

namespace tentris::store::sparql {

	using Variable=TripleVariable;

}
namespace std {
	template<>
	struct hash<tentris::store::sparql::Variable> {
		size_t operator()(const tentris::store::sparql::Variable &v) const noexcept {
			return Dice::hash::dice_hash(v.getName());
		}
	};
}


template<>
struct fmt::formatter<tentris::store::sparql::Variable> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::sparql::Variable &p, FormatContext &ctx) {
		return format_to(ctx.out(), "{}", p.getName());
	}
};

#endif //TENTRIS_VARIABLE_HPP
