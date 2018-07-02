#ifndef TNT_STORE_RDFTERMINDEX
#define TNT_STORE_RDFTERMINDEX

#include "../util/All.hpp"
#include <map>

namespace tnt::store {
    class RDFTerm2Id {
        using id = tnt::util::types::key_part_t;

    public:
        class Id2RDFTerm {
            RDFTerm2Id &_original;

        public:
            explicit Id2RDFTerm(RDFTerm2Id &rdf_term_index) : _original(rdf_term_index) {}

            const std::string &at(const id &index) const {
                return _original._id_2_str.at(index);
            }

            RDFTerm2Id &inverse() {
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
        std::map<id, std::string> _id_2_str{};
        std::map<std::string, id> _str_2_id{};
        id _next_id{};
        Id2RDFTerm _inverse;
    public:

        RDFTerm2Id() : _inverse{*this} {}

        const id &at(const std::string &term) const {
            return _str_2_id.at(term);
        }

        const id &operator[](const std::string &term) {
            try {
                return _str_2_id.at(term);
            } catch (...) {
                std::pair<std::map<id, std::string>::iterator, bool> pair = _id_2_str.insert(
                        std::tuple{_next_id, term});
                _str_2_id.emplace(*pair.first);
                return pair.first->first;
            }
        }

        Id2RDFTerm &inv() {
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
