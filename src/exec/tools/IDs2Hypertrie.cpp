#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

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

	std::string rdf_file{argv[1]};
	if (not std::filesystem::is_regular_file(rdf_file)) {
		std::cerr << "{} is not a file."_format(rdf_file) << std::endl;
		exit(EXIT_FAILURE);
	}

	tentris::tensor::BoolHypertrie hypertrie(3);

	std::ifstream file(rdf_file);

	std::string line = "";
	// Iterate through each line and split the content using delimeter
	unsigned int total = 0;
	unsigned int count = 0;
	unsigned int _1mios = 0;
	auto start = steady_clock::now();
	while (getline(file, line)) {
		++count;
		++total;
		using boost::lexical_cast;
		using key_part_type = const rdf_parser::store::rdf::Term *;
		std::vector<std::string> id_triple;
		boost::algorithm::split(id_triple, line, boost::algorithm::is_any_of(","));

		hypertrie.set(tentris::tensor::Key{(key_part_type) lexical_cast<uintptr_t>(id_triple[0]),
										   (key_part_type) lexical_cast<uintptr_t>(id_triple[1]),
										   (key_part_type) lexical_cast<uintptr_t>(id_triple[2])
		}, true);
		if (count == 1'000'000) {
			count = 0;
			++_1mios;
			std::cerr << "{:d} mio triples processed."_format(_1mios) << std::endl;
		}
	}
	auto end = steady_clock::now();
	file.close();
	std::cerr << "{:d} mio triples processed."_format(total) << std::endl;
	std::cerr << "hypertrie entries: {:d}."_format(total) << std::endl;
	std::cerr << "hypertrie size estimation: {:d} kB."_format(tentris::logging::get_memory_usage()) << std::endl;
	auto duration = end - start;

	std::cerr << "duration: {:d}.{:04d} s."_format(duration_cast<seconds>(duration).count(),
												   (duration_cast<milliseconds>(duration) % 1000).count()) << std::endl;
}
