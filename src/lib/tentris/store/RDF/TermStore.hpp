#ifndef TENTRIS_STORE_RDFTERMINDEX
#define TENTRIS_STORE_RDFTERMINDEX

#include <map>
#include <unordered_map>
#include <tsl/hopscotch_set.h>
#include <memory>
#include <tuple>
#include <ostream>

#include "tentris/util/All.hpp"
#include "tentris/store/RDF/Term.hpp"


namespace tentris::store::rdf {


	class TermStore {
	public:
		using set_type = tsl::hopscotch_set<std::unique_ptr<Term>, TermHash, std::equal_to<>>;
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
			if (auto found = terms.find(term, term_hash); found != terms.end()){
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

