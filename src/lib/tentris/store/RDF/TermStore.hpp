#ifndef TENTRIS_STORE_RDFTERMINDEX
#define TENTRIS_STORE_RDFTERMINDEX

#include <tsl/sparse_set.h>
#include <memory>
#include <tuple>

#include "tentris/util/All.hpp"
#include <Dice/rdf_parser/RDF/Term.hpp>

namespace tentris::store::rdf {


	class TermStore {
	    using Term = rdf_parser::store::rdf::Term;
        using BNode = rdf_parser::store::rdf::BNode;
        using Literal = rdf_parser::store::rdf::Literal;
        using URIRef = rdf_parser::store::rdf::URIRef;
	public:
		using set_type = tsl::sparse_set<std::unique_ptr<Term>,
				TermHash,
				std::equal_to<>,
				std::allocator<std::unique_ptr<Term>>,
				tsl::sh::power_of_two_growth_policy<2>,
				tsl::sh::exception_safety::basic>;
		using const_iterator = set_type::const_iterator;

	private:

		set_type terms{};
	public:
		using ptr_type = Term const *;

		bool contains(const Term &term) const {
			auto term_hash = std::hash<Term>()(term);
			return contains(term, term_hash);
		}

		[[nodiscard]] bool contains(const Term &term, const std::size_t &term_hash) const {
			auto found = terms.find(term, term_hash);
			return found != terms.end();
		}

		[[nodiscard]] bool valid(ptr_type term) const {
			auto term_hash = std::hash<Term *>()(term);
			auto found = terms.find(term, term_hash);
			return found != terms.end();
		}

		[[nodiscard]] ptr_type get(const Term &term) const {
			auto term_hash = std::hash<Term>()(term);
			return get(term, term_hash);
		}

		[[nodiscard]] ptr_type get(const Term &term, const std::size_t &term_hash) const {
			auto found = terms.find(term, term_hash);
			if (found != terms.end())
				return (*found).get();
			else {
				throw std::out_of_range{"Term {} not in TermStore."};
			}
		}

		[[nodiscard]] ptr_type find(const Term &term, const std::size_t &term_hash) const {
			if (auto found = terms.find(term, term_hash); found != terms.end()) {
				return (*found).get();
			} else {
				return nullptr;
			}
		}

		[[nodiscard]] ptr_type find(const Term &term) const {
			auto term_hash = std::hash<Term>()(term);
			return find(term, term_hash);
		}

		ptr_type operator[](const Term &term) {
			auto term_hash = std::hash<Term>()(term);
			auto found = terms.find(term, term_hash);
			if (found != terms.end())
				return (*found).get();
			else {
				const auto &[iter, success] = terms.emplace(std::make_unique<Term>(term));
				assert(success);
				return (*iter).get();
			}
		}

		friend class fmt::formatter<TermStore>;

		std::size_t size() const {
			return terms.size();
		}

	};
};

template<>
struct fmt::formatter<tentris::store::rdf::TermStore> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::rdf::TermStore &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
						 " Entries:\n"
						 "   {}\n",
						 join(p.terms.begin(), p.terms.end(), "\n   "));
	}
};


#endif //TENTRIS_STORE_RDFTERMINDEX

