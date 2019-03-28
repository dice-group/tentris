#ifndef TNT_STORE_RDFTERMINDEX
#define TNT_STORE_RDFTERMINDEX

#include <map>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <ostream>

#include "tnt/util/All.hpp"
#include "tnt/store/RDF/Term.hpp"

namespace {
    using namespace tnt::util::types;
}

namespace tnt::store::rdf {
    class TermStore {
    public:
        friend struct fmt::formatter<tnt::store::rdf::TermStore>;

        class RevTermStore {
            friend class TermStore;

            TermStore &_original;

        protected:
            explicit RevTermStore(TermStore
                                  &rdf_term_index) : _original(rdf_term_index) {}

        public:
            const std::unique_ptr<Term> &at(const key_part_t &index) const {
                return *(_original._id2term.at(index));
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

            friend struct fmt::formatter<tnt::store::rdf::TermStore::RevTermStore>;

        };

    private:
        struct TermPtrComp {
            bool operator()(const std::unique_ptr<Term> &a, const std::unique_ptr<Term> &b) const {
                return a->getIdentifier().compare(b->getIdentifier()) < 0;
            }

            bool operator()(const std::unique_ptr<Term> &a, const Term &b) const {
                return a->getIdentifier().compare(b.getIdentifier()) < 0;
            }

            bool operator()(const Term &a, const std::unique_ptr<Term> &b) const {
                return a.getIdentifier().compare(b->getIdentifier()) < 0;
            }
        };

        std::unordered_map<key_part_t, std::unique_ptr<Term> *> _id2term{};
        std::unordered_map<std::unique_ptr<Term>, key_part_t> _term2id{};
        key_part_t _next_id{};
        RevTermStore _inverse;
    public:

        TermStore() : _inverse{*this} {}

        const key_part_t &at(const std::unique_ptr<Term> &term) const {
            return _term2id.at(term);
        }

        const key_part_t &at(const Term &term) const {
            std::unique_ptr<Term> temp_term = std::unique_ptr<Term>{new Term{term}};

            const unsigned long &id_ = _term2id.at(temp_term);
            return id_;
        }

        bool contains(const std::unique_ptr<Term> &term) const {
            return _term2id.find(term) != _term2id.end();
        }

        const key_part_t &operator[](std::unique_ptr<Term> term) {
            try {
                return _term2id.at(term);
            } catch (...) {
                auto pair =
                        _term2id.insert(std::make_pair(std::move(term), _next_id));
				auto &entry = *pair.first;
                const key_part_t &id = entry.second;
                std::unique_ptr<Term> *pTerm = const_cast<std::unique_ptr<Term> *>(&entry.first);
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

        friend struct fmt::formatter<tnt::store::rdf::TermStore>;

    };
};

template<>
struct fmt::formatter<tnt::store::rdf::TermStore> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const tnt::store::rdf::TermStore &p, FormatContext &ctx) {
        auto entries = values(p._id2term);
        return format_to(ctx.begin(),
                         " Entries:\n"
                         "   {}\n",
                         join(entries.begin(), entries.end(), "\n   "));
    }
};

template<>
struct fmt::formatter<tnt::store::rdf::TermStore::RevTermStore> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const tnt::store::rdf::TermStore::RevTermStore &p, FormatContext &ctx) {
        return format_to(ctx.begin(),
                         "{}",
                         p.inv());
    }
};

#endif //TNT_STORE_RDFTERMINDEX
