#include <gtest/gtest.h>

#include <fstream>
#include <regex>
#include <experimental/filesystem>

#include "tnt/store/SPARQL/ParsedSPARQL.hpp"

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
    for (const auto&[line_number, query] : enumerate(queries, 1)) {
        std::cout << " # " << std::setw(3) << line_number << " " << query << std::endl;
        try {
            ParsedSPARQL{query};
            std::cout << " #     " << "OK" << std::endl;
        } catch (std::exception ex) {
            no_errors = false;
            std::cout << " #     " << "Failed" << std::endl;
            std::cout << ex.what() << std::endl;
        }
    }
    return no_errors;
}

TEST(TestSPARQLParser, ParseBenchmarkQueries) {

    const std::string sp2b_file = "queries/sp2b.txt";
    const std::string dbpedia_file = "queries/DBpedia.txt";
    ASSERT_TRUE(std::experimental::filesystem::is_regular_file(sp2b_file));
    ASSERT_TRUE(std::experimental::filesystem::is_regular_file(dbpedia_file));

    bool no_errors = true;
    no_errors &= parse_queries(sp2b_file);
    no_errors &= parse_queries(dbpedia_file);
    ASSERT_TRUE(no_errors);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
