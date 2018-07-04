#ifndef TNT_STORE_RDFTERMINDEX
#define TNT_STORE_RDFTERMINDEX

#include "../util/All.hpp"
#include "RDF/Node.hpp"
#include <map>
#include <memory>

namespace tnt::store {
    class Node2Id {
    public:
        using key_part_t = tnt::util::types::key_part_t;

    public:
        class Id2Node {
            friend class Node2Id;
            Node2Id &_original;

        protected:
            explicit Id2Node(Node2Id &rdf_term_index) : _original(rdf_term_index) {}

        public:
            const std::unique_ptr<Node> &at(const key_part_t &index) const {
                return _original._id_2_str.at(index);
            }

            Node2Id &inverse() noexcept {
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
        std::map<key_part_t, std::unique_ptr<Node>> _id_2_str{};
        std::map<std::unique_ptr<Node>, key_part_t> _str_2_id{};
        key_part_t _next_id{};
        Id2Node _inverse;
    public:

        Node2Id() : _inverse{*this} {}

        const key_part_t &at(const std::unique_ptr<Node> &term) const {
            return _str_2_id.at(term);
        }

        const key_part_t &operator[](const std::unique_ptr<Node> &term) {
            try {
                return _str_2_id.at(term);
            } catch (...) {
                std::pair<std::map<key_part_t, std::unique_ptr<Node>>::iterator, bool> pair =
                        _id_2_str.insert(_next_id, std::move(term));
                _str_2_id.emplace(*pair.first);
                return pair.first->first;
            }
        }

        const key_part_t &operator[](const std::string &term) {
            return (*self)[parse(term)];
        }

        Id2Node &inv() {
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
