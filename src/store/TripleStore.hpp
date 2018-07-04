#ifndef TNT_STORE_TRIPLESTORE
#define TNT_STORE_TRIPLESTORE

#include "Node2Id.hpp"
#include "RDFParser.hpp"
#include "../tensor/hypertrie/BoolHyperTrie.hpp"


namespace tnt::store {
    class TripleStore {
        using key_part_t = tnt::util::types::key_part_t;
        using BoolHyperTrie =tnt::tensor::hypertrie::BoolHyperTrie;
        Node2Id TermIndex{};
        BoolHyperTrie trie{3};

    public:
        const Node2Id &getTermIndex() const {
            return TermIndex;
        }

        void loadRDF(std::string file_path) {
            size_t count = 0;
            for (auto &&triple : RDFParser{file_path}) {
                const key_part_t &subject_id = TermIndex[std::get<0>(triple)];
                const key_part_t &predicate_id = TermIndex[std::get<1>(triple)];
                const key_part_t &object_id = TermIndex[std::get<2>(triple)];
                trie.set({subject_id, predicate_id, object_id}, true);
                ++count;
            }
            std::cout << "Loaded " << count << " triples." << std::endl;
        }

        void add(std::tuple<std::string, std::string, std::string> triple) {

        }

        bool contains(std::tuple<std::string, std::string, std::string> triple) {

        }

        void query(std::string sparql) {

        }

    };
};

#endif //TNT_STORE_TRIPLESTORE
