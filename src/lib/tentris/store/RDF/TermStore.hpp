#ifndef TENTRIS_STORE_RDFTERMINDEX
#define TENTRIS_STORE_RDFTERMINDEX

#include <map>
#include <unordered_map>
#include <tsl/hopscotch_map.h>
#include <memory>
#include <tuple>
#include <ostream>

#include "tentris/util/All.hpp"
#include "tentris/store/RDF/Term.hpp"

namespace {
	using namespace tentris::util::types;
}
template<>
struct std::hash<tentris::store::rdf::Term *> {
	size_t operator()(tentris::store::rdf::Term const *f) const {
		return std::hash<tentris::store::rdf::Term>()(*f);
	}
};

template<>
struct std::equal_to<std::shared_ptr<tentris::store::rdf::Term>> {
	bool operator()(const std::shared_ptr<tentris::store::rdf::Term> &lhs,
	                const std::shared_ptr<tentris::store::rdf::Term> &rhs) const {
		return std::equal_to<tentris::store::rdf::Term>()(*lhs.get(), *rhs.get());
	}
};

namespace tentris::store::rdf {


	class TermStore {
	public:
		friend struct fmt::formatter<tentris::store::rdf::TermStore>;

		class RevTermStore {
			friend class TermStore;

			TermStore &_original;

		protected:
			explicit RevTermStore(TermStore
			                      &rdf_term_index) : _original(rdf_term_index) {}

		public:
			const std::shared_ptr<Term> &at(const key_part_t &index) const {
				return _original._id2term.at(index);
			}

			TermStore &inv() noexcept {
				return _original;
			}

			TermStore &inv() const noexcept {
				return _original;
			}

			inline bool empty() const noexcept {
				return _original.empty();
			}


			inline size_t size() const noexcept {
				return _original.size();
			}

			void clear() noexcept {
				_original.clear();
			}

			friend struct fmt::formatter<tentris::store::rdf::TermStore::RevTermStore>;

		};

	private:
		struct TermPtrComp {
			bool operator()(const std::shared_ptr<Term> &a, const std::shared_ptr<Term> &b) const {
				return a->getIdentifier().compare(b->getIdentifier()) < 0;
			}

			bool operator()(const std::shared_ptr<Term> &a, const Term &b) const {
				return a->getIdentifier().compare(b.getIdentifier()) < 0;
			}

			bool operator()(const Term &a, const std::shared_ptr<Term> &b) const {
				return a.getIdentifier().compare(b->getIdentifier()) < 0;
			}
		};

		tsl::hopscotch_map<key_part_t, std::shared_ptr<Term>> _id2term{};
		tsl::hopscotch_map<std::shared_ptr<Term>, key_part_t, std::hash<std::shared_ptr<Term>>,
				std::equal_to<std::shared_ptr<Term>>,
				std::allocator<std::pair<std::shared_ptr<Term>, key_part_t>>,
				30, true
		> _term2id{};
		key_part_t _next_id{};
		RevTermStore _inverse;
	public:

		TermStore() : _inverse{*this} {}

		const key_part_t &at(const std::shared_ptr<Term> &term) const {

			return _term2id.at(term);
		}

		const key_part_t &at(const Term &term) const {
			std::shared_ptr<Term> temp_term = std::shared_ptr<Term>{new Term{term}};

			const unsigned long &id_ = _term2id.at(temp_term);
			return id_;
		}

		bool contains(const std::shared_ptr<Term> &term) const {
			return _term2id.find(term) != _term2id.end();
		}

		const key_part_t &operator[](std::shared_ptr<Term> term) {
			auto found = _term2id.find(term);
			if (found != _term2id.end())
				return found->second;
			else {
				auto pair = _term2id.insert(std::make_pair(std::move(term), _next_id));
				auto &entry = *pair.first;
				const key_part_t &id = entry.second;
				std::shared_ptr<Term> pTerm = entry.first;
				_id2term.insert({id, pTerm});
				++_next_id;
				return id;
			}
		}

		const key_part_t &operator[](const std::string &term) {
			return (*this)[parseTerm(term)];
		}

		RevTermStore &inv() {
			return _inverse;
		}

		const RevTermStore &inv() const {
			return _inverse;
		}

		inline bool empty() const noexcept {
			return _term2id.empty();
		}


		inline size_t size() const noexcept {
			return _term2id.size();
		}

		void clear() noexcept {
			_id2term.clear();
			_term2id.clear();
		}

		friend struct fmt::formatter<tentris::store::rdf::TermStore>;

	};
};

template<>
struct fmt::formatter<tentris::store::rdf::TermStore> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::rdf::TermStore &p, FormatContext &ctx) {
		auto entries = values(p._id2term);
		return format_to(ctx.begin(),
		                 " Entries:\n"
		                 "   {}\n",
		                 join(entries.begin(), entries.end(), "\n   "));
	}
};

template<>
struct fmt::formatter<tentris::store::rdf::TermStore::RevTermStore> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::rdf::TermStore::RevTermStore &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
		                 "{}",
		                 p.inv());
	}
};

#endif //TENTRIS_STORE_RDFTERMINDEX

