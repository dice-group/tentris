#ifndef TENTRIS_HEADERS_HPP
#define TENTRIS_HEADERS_HPP

#include <pistache/http.h>

/**
 * Content-Type for SPARQL OUTPUT JSON FORMAT
 */
namespace tentris::http {
    class SPARQLJSON : public Pistache::Http::Header::Header {
    public:
        NAME("Content-Type")

        SPARQLJSON() {}

        void parse([[maybe_unused]] const std::string &str) override {
        }

        void write(std::ostream &os) const override {
            os << "application/sparql-results+json";
        }
    };
}
#endif //TENTRIS_HEADERS_HPP
