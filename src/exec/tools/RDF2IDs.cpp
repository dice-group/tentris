#include <iostream>
#include <filesystem>
#include <chrono>

#include <tentris/store/RDF/SerdParser.hpp>
#include <tentris/store/RDF/TermStore.hpp>
#include <tentris/util/All.hpp>

int main(int argc, char *argv[]) {
	using namespace rdf_parser::Turtle;
	using namespace tentris::store;
	using namespace fmt::literals;
	using namespace std::chrono;

	if (argc != 2) {
		std::cerr << "Please provide exactly one triple file as commandline argument." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string rdf_file{argv[1]};
	if (not std::filesystem::is_regular_file(rdf_file)) {
		std::cerr << "{} is not a file."_format(rdf_file) << std::endl;
		exit(EXIT_FAILURE);
	}

	fmt::print(stderr, "To store the result pipe stdout (not stderr!) to a file. Output format is TSV (tab-separated file, extension: .tsv).\n");

	rdf::TermStore ts{};
	unsigned long count = 0;

	try {
		auto start = steady_clock::now();
		fmt::print("S\tP\tO\n");
		for (const Triple &triple : rdf::SerdParser{rdf_file}) {
			std::array<uintptr_t, 3> id_triple{
					(uintptr_t) ts[triple.subject()],
					(uintptr_t) ts[triple.predicate()],
					(uintptr_t) ts[triple.object()]};
			fmt::print("{}\t{}\t{}\n", id_triple[0], id_triple[1], id_triple[2]);
			++count;
			if (count % 1'000'000 == 0) {
				fmt::print(stderr, "triples processed: {:>10.3}\n", double(count) / 1'000'000);
			}
		}
		auto end = steady_clock::now();
		auto duration = end - start;

		fmt::print(stderr, "total triples processed: {}\n", count);
		fmt::print(stderr, "duration: {} h {} min {}.{:03d} s = {} ms\n",
				(std::chrono::duration_cast<std::chrono::hours>(duration)).count(),
				(std::chrono::duration_cast<std::chrono::minutes>(duration) % 60).count(),
				(std::chrono::duration_cast<std::chrono::seconds>(duration) % 60).count(),
				(std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000).count(),
				std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	} catch (...) {
		throw std::invalid_argument{
				"A parsing error occurred while parsing {}. Error occurred at {}th triple."_format(rdf_file, count)};
	}
}
