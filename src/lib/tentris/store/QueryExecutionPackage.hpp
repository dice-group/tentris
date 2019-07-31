#ifndef TENTRIS_QUERYEXECUTIONPACKAGE_HPP
#define TENTRIS_QUERYEXECUTIONPACKAGE_HPP

#include <any>
#include <exception>
#include <ostream>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/util/SynchronizedCachedFactory.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store::cache {

	namespace {
		using namespace tentris::store::sparql;
		using namespace tentris::util::types;
		using namespace tentris::tensor;
	}; // namespace

	/**
	 * A QueryExecutionPackage contains everything that is necessary to execute a given sparql query for a state of the
	 * RDF graph.
	 */
	struct QueryExecutionPackage {
	private:
		ParsedSPARQL parsedSPARQL;
		std::chrono::system_clock::time_point timeout;

	public:
		std::timed_mutex processing;
		std::timed_mutex queuing;
		std::chrono::system_clock::time_point keep_result_timeout =
				std::numeric_limits<std::chrono::system_clock::time_point>::min();

		/**
		 * Indicates if the QueryExecutionPackage represents an distinct query or not. If it is distinct use only
		 * the methods with distinct in their names. Otherwise use only the methods with regular in their names
		 */
		bool is_distinct;
		bool is_trivial_emtpy;

		size_t cache_bucket_size;

	private:
		std::shared_ptr<void> einsum_operator;

	public:
		/**
		 *
		 * @param sparql_string sparql query to be parsed
		 * @param trie current try holding the data
		 * @param termIndex term store attached to the trie
		 * @throw std::invalid_argument the sparql query was not parsable
		 */
		QueryExecutionPackage(const std::string &sparql_string, const_BoolHypertrie trie, const TermStore &termIndex,
		                      size_t cache_bucket_size)
				: parsedSPARQL{sparql_string},
				  is_distinct{(parsedSPARQL.getSelectModifier() == SelectModifier::DISTINCT)},
				  cache_bucket_size{cache_bucket_size} {

			const auto slice_keys = generateSliceKeys(parsedSPARQL.getBgps(), trie, termIndex);

			is_trivial_emtpy = slice_keys.empty();
			if (not is_trivial_emtpy) {

				const auto subscript = parsedSPARQL.getSubscript();

				// todo: do slicing here
				std::vector<const_BoolHypertrie> hypertries{};
				for (const auto &slice_key: slice_keys) {
					std::optional<const_BoolHypertrie> opt_slice = std::get<std::optional<const_BoolHypertrie>>(
							trie[slice_key]);
					if (opt_slice)
						hypertries.emplace_back(opt_slice.value());
					else {
						is_trivial_emtpy = true;
						return;
					}
				}
				if (not is_distinct)
					einsum_operator = buildOperatorTree<counted_binding>(subscript, hypertries);
				else
					einsum_operator = buildOperatorTree<distinct_binding>(subscript, hypertries);
			}
		}

		void done() {
			if (not is_trivial_emtpy and std::chrono::system_clock::now() > keep_result_timeout) {
				if (not is_distinct);// regular_operator_tree->clearCacheDone();
				else;// distinct_operator_tree->clearCacheDone();
			}
			keep_result_timeout = std::numeric_limits<std::chrono::system_clock::time_point>::min();

			processing.unlock();
		}

		void canceled() {
			if (not is_trivial_emtpy) {
				if (not is_distinct);//	regular_operator_tree->clearCacheCanceled();
				else;//distinct_operator_tree->clearCacheCanceled();
			}
			processing.unlock();
		}

		void setTimeout(const std::chrono::system_clock::time_point &timeout) {
			this->timeout = timeout;
			if (not is_trivial_emtpy) {
				if (not is_distinct);//regular_operator_tree->setTimeout(timeout);
				else;//distinct_operator_tree->setTimeout(timeout);
			}
		}

		std::chrono::system_clock::time_point getTimeout() const { return timeout; }

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
		std::shared_ptr<void> buildOperatorTree(
				const std::shared_ptr<Subscript> subscript,
				const std::vector<const_BoolHypertrie> &hypertries
		) {
			return std::make_shared<Einsum>(subscript, hypertries);
		}

	public:
		const ParsedSPARQL &getParsedSPARQL() const { return parsedSPARQL; }

		const std::string &getSparqlStr() const { return parsedSPARQL.getSparqlStr(); }

		const std::shared_ptr<void> &getDistinctOpTree() const {
			return einsum_operator;
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
		static std::vector<SliceKey_t>
		generateSliceKeys(const std::set<TriplePattern> &bgps, const const_BoolHypertrie &trie,
		                  const TermStore &termIndex) {
			std::vector<SliceKey_t> slice_keys{};
			for (const auto &op_key : bgps) {
				SliceKey_t slice_key(3, std::nullopt);
				bool no_slices = true;
				for (const auto[pos, op_key_part] : enumerate(op_key)) {
					if (std::holds_alternative<Term>(op_key_part))
						try {
							key_part_t ind = termIndex.at(std::get<Term>(op_key_part));
							slice_key[pos] = {ind};
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
		                 " is_trivial_emtpy: {}\n",
		                 p.parsedSPARQL, p.is_distinct, p.is_trivial_emtpy);
		// TODO: implement print for operator
	}
};

#endif // TENTRIS_QUERYEXECUTIONPACKAGE_HPP
