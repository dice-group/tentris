#ifndef TNT_STORE_TRIPLESTORE
#define TNT_STORE_TRIPLESTORE

#include "RDFTerm2Id.hpp"
#include "RDFParser.hpp"


namespace tnt::store {
    class TripleStore {
        RDFTerm2Id TermIndex{};
    public:
        const RDFTerm2Id &getTermIndex() const {
            return TermIndex;
        }

        void loadRDF(std::string file_path) {

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
