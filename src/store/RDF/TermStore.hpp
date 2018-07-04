#ifndef TNT_STORE_RDFTERMINDEX
#define TNT_STORE_RDFTERMINDEX

#include "util/All.hpp"
#include "Term.hpp"
#include <map>
#include <memory>

namespace tnt::store {
    class TermStore {
    public:
        using key_part_t = tnt::util::types::key_part_t;

    public:
        class RevTermStore {
            friend class TermStore;

            TermStore &_original;

        protected:
            explicit RevTermStore(TermStore &rdf_term_index) : _original(rdf_term_index) {}

        public:
            const std::unique_ptr<Term> &at(const key_part_t &index) const {
                return _original._id_2_str.at(index);
            }

            TermStore &inverse() noexcept {
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
        std::map<key_part_t, std::unique_ptr<Term>> _id_2_str{};
        std::map<std::unique_ptr<Term>, key_part_t> _str_2_id{};
        key_part_t _next_id{};
        RevTermStore _inverse;
    public:

        TermStore() : RevTermStore{*this} {}

        const key_part_t &at(const std::unique_ptr<Term> &term) const {
            return _str_2_id.at(term);
        }

        const key_part_t &operator[](const std::unique_ptr<Term> &term) {
            try {
                return _str_2_id.at(term);
            } catch (...) {
                std::pair<std::map<key_part_t, std::unique_ptr<Term>>::iterator, bool> pair =
                        _id_2_str.insert(_next_id, std::move(term));
                _str_2_id.emplace(*pair.first);
                return pair.first->first;
            }
        }

        const key_part_t &operator[](const std::string &term) {
            return (*this)[parse(term)];
        }

        RevTermStore &inv() {
            return _inverse;
        }

        inline bool empty() const noexcept {
            return _str_2_id.empty();
        }


        inline size_t size() const noexcept {
            return _str_2_id.size();
        }

        void clear() noexcept {
            _str_2_id.clear();
            _id_2_str.clear();
        }
    };
};

#endif //TNT_STORE_RDFTERMINDEX
