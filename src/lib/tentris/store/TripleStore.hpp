#ifndef TENTRIS_STORE_TRIPLESTORE
#define TENTRIS_STORE_TRIPLESTORE


#include <string>
#include <optional>
#include <vector>

#include <tsl/hopscotch_map.h>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/RDF/SerdParser.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/util/LogHelper.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"
#include "tentris/store/SPARQL/TriplePattern.hpp"
#include <Dice/rdf_parser/TurtleParser.hpp>



namespace tentris::store {

	class TripleStore {
		using BoolHypertrie = ::tentris::tensor::BoolHypertrie;
		using const_BoolHypertrie = ::tentris::tensor::const_BoolHypertrie;
		using TriplePattern = ::tentris::store::sparql::TriplePattern;
		using Variable = ::tentris::store::sparql::Variable;

		using TermStore = tentris::store::rdf::TermStore;
		TermStore termIndex{};

		BoolHypertrie trie{3};

	public:

		TermStore &getTermIndex() {
			return termIndex;
		}

		const TermStore &getTermIndex() const {
			return termIndex;
		}

		const_BoolHypertrie getBoolHypertrie() const {
			return trie;
		}

		void bulkloadRDF(const std::string &file_path, size_t bulk_size = 1'000'000) {
			using namespace ::fmt::literals;
			using namespace ::tentris::tensor;
			using namespace rdf_parser::Turtle;
			using namespace ::tentris::logging;

			try {
				hypertrie::BulkInserter<tr> bulk_inserter{trie, 0};
				// TurtleParser<FileParser> parser{file_path};
				unsigned long count = 0;
				for (const Triple &triple : rdf::SerdParser{file_path}) {
					if (not triple.subject().isLiteral() and triple.predicate().isURIRef()) {
						auto subject_id = termIndex[triple.subject()];
						auto predicate_id = termIndex[triple.predicate()];
						auto object_id = termIndex[triple.object()];
						bulk_inserter.add({subject_id, predicate_id, object_id});
					} else
						throw std::invalid_argument{
								"Subject or predicate of the triple have a term type that is not allowed there."};
					++count;

					if (trie.size() % bulk_size == 0) {
						bulk_inserter.flush();
						logDebug("{:>10.3} mio triples processed."_format(double(count)/1'000'000));
						logDebug("{:>10.3} mio triples loaded."_format(double(trie.size())/1'000'000));
					}
				}
				bulk_inserter.flush(true);
				log("{:>10.3} mio triples processed."_format(double(count)/1'000'000));
				log("{:>10.3} mio triples loaded."_format(double(trie.size())/1'000'000));
			} catch (...) {
				throw std::invalid_argument{"A parsing error occurred while parsing {}"_format(file_path)};
			}
		}

		void add(const std::tuple<std::string, std::string, std::string> &triple) {
			add(Term::make_term(std::get<0>(triple)),
				Term::make_term(std::get<1>(triple)),
				Term::make_term(std::get<2>(triple)));
		}

		std::variant<const_BoolHypertrie, bool> resolveTriplePattern(TriplePattern tp) {
			using namespace ::tentris::tensor;

			auto slice_count = 0;
			for (const auto &entry: tp)
				if (std::holds_alternative<Variable>(entry))
					++slice_count;

			SliceKey slice_key(3, std::nullopt);
			for (const auto &[pos, entry] : iter::enumerate(tp)) {
				if (std::holds_alternative<Term>(entry))
					try {
						auto term = termIndex.get(std::get<Term>(entry));
						slice_key[pos] = term;
					} catch ([[maybe_unused]] std::out_of_range &exc) {
						// a keypart was not in the index so the result is zero anyways.
						return (slice_count > 0)
							   ? std::variant<const_BoolHypertrie, bool>{
										const_BoolHypertrie()}
							   : std::variant<const_BoolHypertrie, bool>{false};
					}
			}
			return trie[slice_key];
		}

		inline void
		add(Term subject, Term predicate, Term object) {
			if (not subject.isLiteral() and predicate.isURIRef()) {
				auto subject_id = termIndex[std::move(subject)];
				auto predicate_id = termIndex[std::move(predicate)];
				auto object_id = termIndex[std::move(object)];
				trie.set({subject_id, predicate_id, object_id}, true);
			} else
				throw std::invalid_argument{
						"Subject or predicate of the triple have a term type that is not allowed there."};
		}

		bool contains(std::tuple<std::string, std::string, std::string> triple) {
			using namespace ::tentris::tensor;
			auto subject = termIndex.find(Term::make_term(std::get<0>(triple)));
			auto predicate = termIndex.find(Term::make_term(std::get<1>(triple)));
			auto object = termIndex.find(Term::make_term(std::get<2>(triple)));
			if (subject and predicate and object) {
				Key key{subject, predicate, object};
				return trie[key];
			}
			return false;
		}

		size_t size() const {
			return trie.size();
		}


	};
};
#endif //TENTRIS_STORE_TRIPLESTORE
