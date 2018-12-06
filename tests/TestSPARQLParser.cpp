#include <gtest/gtest.h>

#include <fstream>
#include <regex>
#include <filesystem>

#include <tnt/store/SPARQL/ParsedSPARQL.hpp>


std::vector<std::string> load_queries(std::string query_file_path) {
    std::vector<std::string> queries{};
    std::ifstream query_file(query_file_path);
    std::string query;
    while (std::getline(query_file, query)) {
        static std::regex white_spaces{"\\s+"};
        if (not std::regex_match(query, white_spaces))
            queries.push_back(query);
    }
    return queries;
}

bool parse_queries(const std::string &file) {
    std::vector<std::string> queries = load_queries(file);

    using namespace tnt::store::sparql;
    bool no_errors = true;
    std::cout << " ### " << file << std::endl;
    std::set<int> lines_with_errors{};
    for (const auto&[line_number, query] : enumerate(queries, 1)) {
        std::cout << " # " << std::setw(3) << line_number << " " << query << std::endl;
        try {
            ParsedSPARQL{query};
            std::cout << " #     " << "OK" << std::endl;
        } catch (std::exception ex) {
            lines_with_errors.insert(line_number);
            no_errors = false;
            std::cout << " #     " << "Failed" << std::endl;
            std::cout << ex.what() << std::endl;
        }
    }
    std::cout << " ### " << "failed queries: " << lines_with_errors << std::endl;
    return no_errors;
}

TEST(TestSPARQLParser, ParseBenchmarkQueries) {
    namespace fs = std::filesystem;
    const std::string sp2b_file = "queries/sp2b.txt";
    const std::string dbpedia_file = "queries/DBpedia.txt";
    const std::string swdf_file = "queries/swdf.txt";
    ASSERT_TRUE(fs::is_regular_file(sp2b_file));
    ASSERT_TRUE(fs::is_regular_file(dbpedia_file));
    ASSERT_TRUE(fs::is_regular_file(swdf_file));

    bool no_errors = true;
    no_errors &= parse_queries(sp2b_file);
    no_errors &= parse_queries(dbpedia_file);
    no_errors &= parse_queries(swdf_file);
    ASSERT_TRUE(no_errors);
}

TEST(TestSPARQLParser, DISABLED_ParseSingleQuery) {

    const std::string query = "PREFIX  dc:   <http://purl.org/dc/elements/1.1/>  PREFIX  :     <http://dbpedia.org/resource/>  PREFIX  rdfs: <http://www.w3.org/2000/01/rdf-schema#>  PREFIX  dbpedia2: <http://dbpedia.org/property/>  PREFIX  foaf: <http://xmlns.com/foaf/0.1/>  PREFIX  owl:  <http://www.w3.org/2002/07/owl#>  PREFIX  xsd:  <http://www.w3.org/2001/XMLSchema#>  PREFIX  dbpedia: <http://dbpedia.org/>  PREFIX  rdf:  <http://www.w3.org/1999/02/22-rdf-syntax-ns#>  PREFIX  skos: <http://www.w3.org/2004/02/skos/core#>  "
                              " SELECT  *  WHERE    { ?data rdf:type <http://dbpedia.org/ontology/FormulaOneRacer> .      ?wins <http://dbpedia.org/ontology/wins> 10    }";
    using namespace tnt::store::sparql;
    ParsedSPARQL q{query};
    std::cout << q << std::endl;
    wait(0);

}


