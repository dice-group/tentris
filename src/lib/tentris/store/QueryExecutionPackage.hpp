#ifndef TENTRIS_QUERYEXECUTIONPACKAGE_HPP
#define TENTRIS_QUERYEXECUTIONPACKAGE_HPP

#include <any>
#include <exception>
#include <ostream>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store::cache {

	namespace {
		using namespace tentris::store::sparql;
		using namespace tentris::tensor;
	}; // namespace

	/**
	 * A QueryExecutionPackage contains everything that is necessary to execute a given sparql query for a state of the
	 * RDF graph.
	 */
	struct QueryExecutionPackage {
		using TimeoutType = std::chrono::system_clock::time_point;
	private:
		ParsedSPARQL parsedSPARQL;
		std::shared_ptr<Subscript> subscript;

	public:
		/**
		 * Indicates if the QueryExecutionPackage represents an distinct query or not. If it is distinct use only
		 * the methods with distinct in their names. Otherwise use only the methods with regular in their names
		 */
		bool is_distinct;
		bool is_trivial_empty;

		size_t cache_bucket_size;

	private:

		std::vector<const_BoolHypertrie> operands{};

	public:
		/**
		 *
		 * @param sparql_string sparql query to be parsed
		 * @param trie current try holding the data
		 * @param termIndex term store attached to the trie
		 * @throw std::invalid_argument the sparql query was not parsable
		 */
		QueryExecutionPackage(const std::string &sparql_string,
							  const const_BoolHypertrie &trie,
							  const TermStore &termIndex,
							  size_t cache_bucket_size)
				: parsedSPARQL{sparql_string},
				  subscript{parsedSPARQL.getSubscript()},
				  is_distinct{(parsedSPARQL.getSelectModifier() == SelectModifier::DISTINCT)},
				  cache_bucket_size{cache_bucket_size} {

			const auto slice_keys = generateSliceKeys(parsedSPARQL.getBgps(), trie, termIndex);

			is_trivial_empty = slice_keys.empty();
			if (not is_trivial_empty) {

				for (const auto &slice_key: slice_keys) {
					auto opt_slice = std::get<std::optional<const_BoolHypertrie>>(trie[slice_key]);
					if (opt_slice)
						operands.emplace_back(opt_slice.value());
					else {
						is_trivial_empty = true;
						return;
					}
				}
			}
		}

	private:
		/**
		 * Builds the operator tree for this query.
		 * @tparam RESULT_TYPE the type returned by the operand tree
		 * @param slice_keys slice keys to extract the operands from the hypertries. slice_keys and hypertries must be
		 * of equal length.
		 * @param subscript the subscript that spans the operator tree.
		 * @param hypertries a list of hypertries. typically this is a list containing the data base hypertrie multiple
		 * times.
		 * @return
		 */
		template<typename RESULT_TYPE>
		static std::shared_ptr<void> generateEinsum(const std::shared_ptr<Subscript> &subscript,
													const std::vector<const_BoolHypertrie> &hypertries) {
			return std::make_shared<Einsum<RESULT_TYPE>>(subscript, hypertries);
		}

	public:
		const ParsedSPARQL &getParsedSPARQL() const { return parsedSPARQL; }

		const std::string &getSparqlStr() const { return parsedSPARQL.getSparqlStr(); }

		std::shared_ptr<void> getEinsum() const {
			if (not is_distinct)
				return generateEinsum<COUNTED_t>(subscript, operands);
			else
				return generateEinsum<DISTINCT_t>(subscript, operands);
		}

		friend struct ::fmt::formatter<QueryExecutionPackage>;

	private:
		/**
		 * Calculates the slice keys for the BoolHyperTrie from a basic graph pattern. If the result is clearly empty
		 * an empty list of slice keys is returned.
		 * @param bgps basic graph pattern of the query
		 * @param trie BoolHyperTrie
		 * @param termIndex intex for trie
		 * @return slice keys for the BoolHyperTrie
		 */
		static std::vector<SliceKey>
		generateSliceKeys(const std::set<TriplePattern> &bgps, const const_BoolHypertrie &trie,
						  const TermStore &termIndex) {
			std::vector<SliceKey> slice_keys{};
			for (const auto &op_key : bgps) {
				SliceKey slice_key(3, std::nullopt);
				bool no_slices = true;
				for (const auto[pos, op_key_part] : enumerate(op_key)) {
					if (std::holds_alternative<Term>(op_key_part))
						try {
							Term const *term = termIndex.get(std::get<Term>(op_key_part));
							slice_key[pos] = term;
						} catch ([[maybe_unused]] std::out_of_range &exc) {
							// a keypart was not in the index so the result is zero anyways.
							return {};
						}
					else
						no_slices = false;
				}
				if (no_slices) {
					// one triple without variables was not in storeF
					if (not std::get<bool>(trie[slice_key]))
						return {};
				} else
					slice_keys.push_back(slice_key);
			}

			return slice_keys;
		}
	};

} // namespace tentris::store::cache

template<>
struct fmt::formatter<tentris::store::cache::QueryExecutionPackage> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::cache::QueryExecutionPackage &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
						 " parsedSPARQL:     {}\n"
						 " is_distinct:      {}\n"
						 " is_trivial_empty: {}\n",
						 p.parsedSPARQL, p.is_distinct, p.is_trivial_empty);
		// TODO: implement print for operator
	}
};

#endif // TENTRIS_QUERYEXECUTIONPACKAGE_HPP
