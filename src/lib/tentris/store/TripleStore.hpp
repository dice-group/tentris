#ifndef TENTRIS_STORE_TRIPLESTORE
#define TENTRIS_STORE_TRIPLESTORE


#include <string>
#include <optional>
#include <vector>

#include <boost/bind.hpp>
#include <boost/coroutine2/all.hpp>
#include <absl/hash/hash.h>
#include <tsl/hopscotch_map.h>

#include <boost/algorithm/string.hpp>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/util/LogHelper.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"
#include "tentris/store/SPARQL/TriplePattern.hpp"
#include <Dice/rdf_parser/TurtleParser.hpp>


namespace {
	using namespace tentris::store::sparql;
	using namespace ::tentris::logging;
	using namespace tentris::tensor;
}

namespace tentris::store {
	class BulkLoad;

	class TripleStore {
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

		void loadRDF(const std::string &file_path) {
			using namespace rdf_parser::Turtle;

			try {
				TurtleParser<> parser{file_path};
				for (const Triple &triple : parser) {
					add(triple.subject(), triple.predicate(), triple.object());
				}
			} catch (...) {
				throw std::invalid_argument{"A parsing error occured while parsing {}"_format(file_path)};
			}
		}

		void add(const std::tuple<std::string, std::string, std::string> &triple) {
			add(Term::make_term(std::get<0>(triple)),
				Term::make_term(std::get<1>(triple)),
				Term::make_term(std::get<2>(triple)));
		}

		std::variant<std::optional<const_BoolHypertrie>, bool> resolveTriplePattern(TriplePattern tp) {
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
							   ? std::variant<std::optional<const_BoolHypertrie>, bool>{
										std::optional<const_BoolHypertrie>()}
							   : std::variant<std::optional<const_BoolHypertrie>, bool>{false};
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
