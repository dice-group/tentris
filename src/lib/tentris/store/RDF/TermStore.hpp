#ifndef TENTRIS_STORE_RDFTERMINDEX
#define TENTRIS_STORE_RDFTERMINDEX

#include <tsl/sparse_set.h>
#include <memory>
#include <tuple>

#include <Dice/RDF/Term.hpp>

#include <Dice/hash/DiceHash.hpp>

namespace tentris::store::rdf {


	/**
	 * A hash for Terms that returns for Term* the hash hash(Term) instead of hash(Term*).
	 */
	struct TermHash {
	private:
		using Term = Dice::rdf::Term;
	public:
		size_t operator()(Term const  &term) const {
			return ::Dice::hash::dice_hash(term);
		}

		size_t operator()(std::unique_ptr<Term> const &term_ptr) const {
			return ::Dice::hash::dice_hash(*term_ptr);
		}

		size_t operator()(Term const *const term_ptr) const {
			return ::Dice::hash::dice_hash(*term_ptr);
		}
	};


	class TermStore {
		using Term = Dice::rdf::Term;
		using BNode = Dice::rdf::BNode;
		using Literal = Dice::rdf::Literal;
		using URIRef = Dice::rdf::URIRef;
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

		[[nodiscard]] bool contains(const Term &term) const {
			auto term_hash = TermHash()(term);
			return contains(term, term_hash);
		}

		[[nodiscard]] bool contains(const Term &term, const std::size_t &term_hash) const {
			auto found = terms.find(term, term_hash);
			return found != terms.end();
		}

		[[nodiscard]] bool valid(ptr_type term_ptr) const {
			auto term_hash = TermHash()(term_ptr);
			auto found = terms.find(*term_ptr, term_hash);
			return found != terms.end();
		}

		[[nodiscard]] ptr_type get(const Term &term) const {
			auto term_hash = TermHash()(term);
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
			auto term_hash = TermHash()(term);
			return find(term, term_hash);
		}

		ptr_type operator[](const Term &term) {
			auto term_hash = TermHash()(term);
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

		[[nodiscard]] std::size_t size() const {
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

