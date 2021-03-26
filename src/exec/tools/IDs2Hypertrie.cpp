#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

#include <csv.hpp>

#include <tentris/store/RDF/TermStore.hpp>
#include <tentris/store/RDF/SerdParser.hpp>
#include <tentris/tensor/BoolHypertrie.hpp>
#include <boost/lexical_cast.hpp>
#include <tentris/util/LogHelper.hpp>

namespace tentris::IDs2Hypertrie {
	void loadIDsAndWriteOutStats(const std::string &csv_file_path);
}
int main(int argc, char *argv[]) {
	using namespace fmt::literals;
	if (argc != 2) {
		std::cerr << "Please provide exactly one CSV file with triple IDS only and no headings." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string csv_file_path{argv[1]};
	if (not std::filesystem::is_regular_file(csv_file_path)) {
		std::cerr << "{} is not a file."_format(csv_file_path) << std::endl;
		exit(EXIT_FAILURE);
	}

	tentris::IDs2Hypertrie::loadIDsAndWriteOutStats(csv_file_path);
}

namespace tentris::IDs2Hypertrie {
	using namespace tentris::store;
	using namespace fmt::literals;
	using namespace std::chrono;

	using key_part_type = size_t;
	using tr = hypertrie::Hypertrie_t<key_part_type,
			bool,
			hypertrie::internal::container::tsl_sparse_map,
			hypertrie::internal::container::tsl_sparse_set,
			false>;


	void loadIDsAndWriteOutStats(const std::string &csv_file_path) {
		hypertrie::Hypertrie<tr> trie(3);

		csv::CSVFormat format;
		format.delimiter('\t').quote(false);


		csv::CSVReader tsv_reader(csv_file_path, format);

		// Iterate through each line and split the content using delimiter
		unsigned long count = 0;
		auto start = steady_clock::now();

		try {
			hypertrie::BulkInserter<tr> bulk_inserter{trie, 0};

			for (csv::CSVRow &row: tsv_reader) { // Input iterator
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

		std::cerr << "## total ## \n"
				  << "triples processed: {}\n"_format(count)
				  << "triples loaded: {}\n"_format(trie.size())
				  << "hypertrie size estimation: {:d} kB\n"_format(tentris::logging::get_memory_usage())
				  << "duration: {} h {} min {}.{:03d} s = {} ms\n"_format(
						  (std::chrono::duration_cast<std::chrono::hours>(duration)).count(),
						  (std::chrono::duration_cast<std::chrono::minutes>(duration) % 60).count(),
						  (std::chrono::duration_cast<std::chrono::seconds>(duration) % 60).count(),
						  (std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000).count(),
						  std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	}

}