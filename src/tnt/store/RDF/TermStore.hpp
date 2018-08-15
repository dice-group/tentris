#ifndef TNT_STORE_RDFTERMINDEX
#define TNT_STORE_RDFTERMINDEX

#include <map>
#include <memory>
#include <tuple>
#include <ostream>

#include "tnt/util/All.hpp"
#include "tnt/store/RDF/Term.hpp"

namespace {
    using namespace tnt::util::types;
}

namespace tnt::store {
    class TermStore {
    public:
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

            inline bool empty() const noexcept {
                return _original.empty();
            }


            inline size_t size() const noexcept {
                return _original.size();
            }

            void clear() noexcept {
                _original.clear();
            }
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

        std::map<key_part_t, std::unique_ptr<Term> *> _id2term{};
        std::map<std::unique_ptr<Term>, key_part_t, TermPtrComp> _term2id{};
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
                std::pair<std::map<std::unique_ptr<Term>, key_part_t>::iterator, bool> pair =
                        _term2id.insert(std::make_pair(std::move(term), _next_id));
                std::pair<const std::unique_ptr<Term>, key_part_t> &entry = *pair.first;
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

        friend std::ostream &operator<<(std::ostream &os, const TermStore &store) {
            for (const auto &str_p: values(store._id2term)) {
                os << *str_p << "\n";
            }
            os << std::endl;
            return os;
        }
    };
};

#endif //TNT_STORE_RDFTERMINDEX
