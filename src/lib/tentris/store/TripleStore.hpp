#ifndef TENTRIS_STORE_TRIPLESTORE
#define TENTRIS_STORE_TRIPLESTORE

#include <string>
#include <optional>
#include <vector>

#include <tsl/hopscotch_map.h>

#include <Dice/RDF/ParseTerm.hpp>
#include <Dice/RDF/Triple.hpp>
#include <Dice/SPARQL/TriplePattern.hpp>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/RDF/SerdParser.hpp"
#include "tentris/util/LogHelper.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"


namespace tentris::store {

	class TripleStore {
		using BoolHypertrie = ::tentris::tensor::BoolHypertrie;
		using const_BoolHypertrie = ::tentris::tensor::const_BoolHypertrie;
		using Term = Dice::rdf::Term;
		using BNode = Dice::rdf::BNode;
		using Literal = Dice::rdf::Literal;
		using URIRef = Dice::rdf::URIRef;
		using Triple = Dice::rdf::Triple;
		using TriplePattern = Dice::sparql::TriplePattern;
		using Variable = Dice::sparql::Variable;

		using TermStore = tentris::store::rdf::TermStore;
		TermStore termIndex{};

		BoolHypertrie trie{3};

		// for graphql

		Term rdf_type_term = URIRef("http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
		// cache for hypertries -- used for checking the type conditions of inline fragments
		std::map<std::string, const_BoolHypertrie> type_tries_cache{};

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

					if (bulk_inserter.size() == bulk_size) {
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
			add(Dice::rdf::parse_term(std::get<0>(triple)),
				Dice::rdf::parse_term(std::get<1>(triple)),
				Dice::rdf::parse_term(std::get<2>(triple)));
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

        const_BoolHypertrie resolveGQLObjectType(const std::string &object_type) {
            using namespace ::tentris::tensor;
			auto object_type_term = URIRef(object_type);
            try {
                SliceKey slice_key{std::nullopt, termIndex.get(rdf_type_term), termIndex.get(object_type_term)};
                return std::get<const_BoolHypertrie>(trie[slice_key]);
			}
			catch ([[maybe_unused]] std::out_of_range &exc) {
				// a keypart was not in the index so the result is zero anyways.
				return const_BoolHypertrie(1);
			}
        }

        const_BoolHypertrie resolveGQLField(const std::string& field) {
            using namespace ::tentris::tensor;
            auto term = URIRef(field);
            try {
                SliceKey slice_key{std::nullopt, termIndex.get(term), std::nullopt};
                return std::get<const_BoolHypertrie>(trie[slice_key]);
            }
            catch ([[maybe_unused]] std::out_of_range &exc) {
                // a keypart was not in the index so the result is zero anyways.
                return const_BoolHypertrie(2);
            }
        }

		const_BoolHypertrie resolveGQLArgumentID(const std::string &id, const const_BoolHypertrie &field_operand) {
            using namespace ::tentris::tensor;
			// create uri from id -- remove quotation marks
            auto term = URIRef(id);
            try {
                SliceKey slice_key{termIndex.get(term)};
                if(std::get<bool>(field_operand[slice_key])) {
					BoolHypertrie filtered(1);
					filtered.set({termIndex.get(term)}, true);
					return std::move(filtered);
				}
				else
					return const_BoolHypertrie(1);
            }
            catch ([[maybe_unused]] std::out_of_range &exc) {
                // a keypart was not in the index so the result is zero anyways.
                return const_BoolHypertrie(1);
            }
		}

        const_BoolHypertrie resolveGQLArgument(const std::string &argument,
											   const std::string &value,
                                               const std::string &type) {
            using namespace ::tentris::tensor;
            auto argument_uri = URIRef(argument);
            try {
                SliceKey slice_key{std::nullopt, termIndex.get(argument_uri), std::nullopt};
				if(type == "String") {
					auto value_literal = Literal(value, std::nullopt, std::nullopt);
					slice_key[2] = termIndex.get(value_literal);
				}
				// todo: other scalar values
                return std::get<const_BoolHypertrie>(trie[slice_key]);
            }
            catch ([[maybe_unused]] std::out_of_range &exc) {
                // a keypart was not in the index so the result is zero anyways.
                return const_BoolHypertrie(1);
            }
        }

		bool typeCondition(const tensor::key_part_type value, const std::string &type) {
			tensor::Key key{value};
			try {
				return type_tries_cache.at(type)[key];
			} catch (...) {
                Term type_term = URIRef(type);
                tensor::SliceKey skey{std::nullopt, termIndex.get(rdf_type_term), termIndex.get(type_term)};
                try {
					auto type_trie = std::get<0>(trie[skey]);
					bool result = type_trie[key];
					type_tries_cache[type] = type_trie;
					return result;
				} catch (...) {
                    type_tries_cache[type] = const_BoolHypertrie{1};
					return false;
				}
			}
		}

		inline void
		add(const Term& subject, const Term& predicate, const Term& object) {
			if (not subject.isLiteral() and predicate.isURIRef()) {
				auto subject_id = termIndex[subject];
				auto predicate_id = termIndex[predicate];
				auto object_id = termIndex[object];
				trie.set({subject_id, predicate_id, object_id}, true);
			} else
				throw std::invalid_argument{
						"Subject or predicate of the triple have a term type that is not allowed there."};
		}

		bool contains(std::tuple<std::string, std::string, std::string> triple) {
			using namespace ::tentris::tensor;
			auto subject = termIndex.find(Dice::rdf::parse_term(std::get<0>(triple)));
			auto predicate = termIndex.find(Dice::rdf::parse_term(std::get<1>(triple)));
			auto object = termIndex.find(Dice::rdf::parse_term(std::get<2>(triple)));
			if (subject and predicate and object) {
				Key key{subject, predicate, object};
				return trie[key];
			}
			return false;
		}

		[[nodiscard]] size_t size() const {
			return trie.size();
		}


	};
};
#endif //TENTRIS_STORE_TRIPLESTORE
