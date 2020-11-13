#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

#include <csv.hpp>

#include <tentris/store/RDF/TermStore.hpp>
#include <tentris/util/All.hpp>
#include <tentris/store/RDF/SerdParser.hpp>
#include <tentris/tensor/BoolHypertrie.hpp>
#include <boost/lexical_cast.hpp>
#include <tentris/util/LogHelper.hpp>

int main(int argc, char *argv[]) {
	using namespace rdf_parser::Turtle;
	using namespace tentris::store;
	using namespace fmt::literals;
	using namespace std::chrono;
	if (argc != 2) {
		std::cerr << "Please provide exactly one CSV file with triple IDS only and no headings." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string csv_file_path{argv[1]};
	if (not std::filesystem::is_regular_file(csv_file_path)) {
		std::cerr << "{} is not a file."_format(csv_file_path) << std::endl;
		exit(EXIT_FAILURE);
	}

	using key_part_type = size_t;
	using tr = hypertrie::Hypertrie_t<key_part_type,
			bool,
			hypertrie::internal::container::tsl_sparse_map,
			hypertrie::internal::container::tsl_sparse_set,
			false>;

	hypertrie::Hypertrie<tr> trie(3);

	csv::CSVFormat format;
	format.delimiter(',').quote(false).no_header();

	csv::CSVReader csv_reader(csv_file_path, format);

	// Iterate through each line and split the content using delimiter
	unsigned int count = 0;
	auto start = steady_clock::now();

	try {
		hypertrie::BulkInserter<tr> bulk_inserter{trie, 0};

		for (csv::CSVRow& row: csv_reader) { // Input iterator
			row[0].get<size_t>();
			bulk_inserter.add({row[0].get<size_t>(),
							   row[1].get<size_t>(),
							   row[2].get<size_t>()});
			++count;

			if (bulk_inserter.size() == 1'000'000) {
				bulk_inserter.flush();
				std::cerr << "{:>10.3} mio triples processed.\n"_format(double(count) / 1'000'000);
				std::cerr << "{:>10.3} mio triples loaded.\n"_format(double(trie.size()) / 1'000'000);
			}
		}

		bulk_inserter.flush(true);

	} catch (...) {
		throw std::invalid_argument{"A parsing error occurred while parsing {}"_format(csv_file_path)};
	}
	auto end = steady_clock::now();
	auto duration = end - start;

	constexpr static auto uncompressed = hypertrie::internal::raw::NodeCompression::uncompressed;
	constexpr static auto compressed = hypertrie::internal::raw::NodeCompression::compressed;

	auto &storage = trie.context()->rawContext().storage;

	auto storage_3_uncompressed = storage.getNodeStorage<3UL, uncompressed>();
	auto storage_2_uncompressed = storage.getNodeStorage<2UL, uncompressed>();
	auto storage_2_compressed = storage.getNodeStorage<2UL, compressed>();
	auto storage_1_uncompressed = storage.getNodeStorage<1UL, uncompressed>();
	auto storage_1_compressed = storage.getNodeStorage<1UL, compressed>();


	{
		std::ofstream tsv_depth_3("depth_3_nodes_stats.tsv"); // Can also use ofstream, etc.
		auto csv_writer = csv::make_tsv_writer(tsv_depth_3);

		csv_writer << std::make_tuple("hash", "node_size", "dimension_0_size", "dimension_1_size", "dimension_2_size",
								  "reference_count");
		for (auto[hash, node] : storage_3_uncompressed) {
			csv_writer << std::make_tuple((std::string) hash, node->size(), node->edges(0).size(), node->edges(1).size(), node->edges(2).size(),
									  node->ref_count());
		}
	}

	{
		std::ofstream tsv_depth_2("depth_2_nodes_stats.tsv"); // Can also use ofstream, etc.
		auto csv_writer = csv::make_tsv_writer(tsv_depth_2);

		csv_writer << std::make_tuple("hash", "node_size", "dimension_0_size", "dimension_1_size", "reference_count");

		for (auto[hash, node] : storage_2_compressed) {
			csv_writer << std::make_tuple((std::string) hash, node->size(), 1, 1, node->ref_count());
		}

		for (auto[hash, node] : storage_2_uncompressed) {
			csv_writer << std::make_tuple((std::string) hash, node->size(), node->edges(0).size(), node->edges(1).size(), node->ref_count());
		}
	}

	{
		std::ofstream tsv_depth_1("depth_1_nodes_stats.tsv"); // Can also use ofstream, etc.
		auto csv_writer = csv::make_tsv_writer(tsv_depth_1);

		csv_writer << std::make_tuple("hash", "node_size", "dimension_0_size", "reference_count");

		for (auto[hash, node] : storage_1_compressed) {
			csv_writer << std::make_tuple((std::string) hash, node->size(), 1, node->ref_count());
		}

		for (auto[hash, node] : storage_1_uncompressed) {
			csv_writer << std::make_tuple((std::string) hash, node->size(), node->edges(0).size(), node->ref_count());
		}
	}


	std::cerr << "## total ## \n"
			  << "triples processed: {}\n"_format(count)
			  << "triples loaded: {}\n"_format(trie.size())
			  << "hypertrie size estimation: {:d} kB\n"_format(tentris::logging::get_memory_usage())
			  << "duration: {} h {} min {}.{:03d} s = {} ms\n"_format(
					  (duration_cast<hours>(duration)).count(),
					  (duration_cast<minutes>(duration) % 60).count(),
					  (duration_cast<seconds>(duration) % 60).count(),
					  (duration_cast<milliseconds>(duration) % 1000).count(),
					  duration_cast<milliseconds>(duration).count())
			  << "# hypertrie stats #\n"
			  << "depth 3 uncompressed nodes: {}\n"_format(storage_3_uncompressed.size())
			  << "depth 2 uncompressed nodes: {}\n"_format(storage.getNodeStorage<2UL, uncompressed>().size())
			  << "depth 2 compressed nodes: {}\n"_format(storage.getNodeStorage<2UL, compressed>().size())
			  << "depth 1 uncompressed nodes: {}\n"_format(storage.getNodeStorage<1UL, uncompressed>().size())
		//<< "depth 1 compressed nodes: {}\n"_format(storage.getNodeStorage<1UL, compressed>().size())
			;

}

