#ifndef TNT_QUERYEXECUTIONPACKAGE_HPP
#define TNT_QUERYEXECUTIONPACKAGE_HPP

#include <any>
#include <exception>
#include <ostream>

#include "tnt/store/RDF/TermStore.hpp"
#include "tnt/store/SPARQL/ParsedSPARQL.hpp"
#include "tnt/tensor/einsum/operator/CrossProduct.hpp"
#include "tnt/tensor/einsum/operator/Einsum.hpp"
#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/tensor/einsum/operator/OperatorNode.hpp"
#include "tnt/tensor/hypertrie/BoolHyperTrie.hpp"
#include "tnt/util/SynchronizedCachedFactory.hpp"

namespace {
	using namespace tnt::tensor::einsum::operators;
	using namespace tnt::store::sparql;
	using namespace tnt::util::types;
	using namespace tnt::tensor::einsum;
	using namespace tnt::tensor::hypertrie;
	using BoolHyperTrie = tnt::tensor::hypertrie::BoolHyperTrie;
}; // namespace

namespace tnt::store::cache {

	/**
	 * A QueryExecutionPackage contains everything that is necessary to execute a given sparql query for a state of the
	 * RDF graph.
	 */
	struct QueryExecutionPackage {
	private:
		ParsedSPARQL parsedSPARQL;

	public:
		std::timed_mutex processing;
		std::timed_mutex queuing;
		std::chrono::system_clock::time_point keep_result_timeout =
				std::numeric_limits<std::chrono::system_clock::time_point>::min();
		std::chrono::system_clock::time_point timeout;

	public:
		/**
		 * Indicates if the QueryExecutionPackage represents an distinct query or not. If it is distinct use only
		 * the methods with distinct in their names. Otherwise use only the methods with regular in their names
		 */
		bool is_distinct;
		bool is_trivial_emtpy;

	private:
		std::unique_ptr<OperatorNode<counted_binding>> regular_operator_tree;
		std::unique_ptr<OperatorNode<distinct_binding>> distinct_operator_tree;

	public:
		/**
		 *
		 * @param sparql_string sparql query to be parsed
		 * @param trie current try holding the data
		 * @param termIndex term store attached to the trie
		 * @throw std::invalid_argument the sparql query was not parsable
		 */
		QueryExecutionPackage(const std::string &sparql_string, BoolHyperTrie &trie, const TermStore &termIndex)
				: parsedSPARQL{sparql_string},
				  is_distinct{(parsedSPARQL.getSelectModifier() == SelectModifier::DISTINCT)} {

			const auto slice_keys = generateSliceKeys(parsedSPARQL.getBgps(), trie, termIndex);

			is_trivial_emtpy = slice_keys.empty();
			if (not is_trivial_emtpy) {

				const auto subscript = parsedSPARQL.getSubscript();

				const auto hypertries = std::vector<BoolHyperTrie *>(slice_keys.size(),
				                                                     &const_cast<BoolHyperTrie &>(trie));
				if (not is_distinct)
					regular_operator_tree = buildOperatorTree<counted_binding>(slice_keys, subscript, hypertries);
				else
					distinct_operator_tree = buildOperatorTree<distinct_binding>(slice_keys, subscript, hypertries);
			}
		}

		void done() {
			if (not is_distinct)
				regular_operator_tree->clearCacheDone();
			else
				distinct_operator_tree->clearCacheDone();

			processing.unlock();
		}// TODO: implement }

		void canceled() {
			if (not is_distinct)
				regular_operator_tree->clearCacheCanceled();
			else
				distinct_operator_tree->clearCacheCanceled();

			processing.unlock();
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
		template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
		std::unique_ptr<OperatorNode<RESULT_TYPE>> buildOperatorTree(const std::vector<SliceKey_t> &slice_keys,
		                                                             const std::shared_ptr<const Subscript> subscript,
		                                                             const std::vector<BoolHyperTrie *> &hypertries) {
			if (auto subsubscripts = subscript->getSubSubscripts(); not subsubscripts.empty()) {
				return std::unique_ptr<OperatorNode<RESULT_TYPE>>{
						new CrossProduct<RESULT_TYPE>{subscript, slice_keys, hypertries}};
			} else {
				return std::unique_ptr<OperatorNode<RESULT_TYPE>>{
						new Einsum<RESULT_TYPE>{subscript, slice_keys, hypertries}};
			}
		}

	public:
		const ParsedSPARQL &getParsedSPARQL() const { return parsedSPARQL; }

		const std::string &getSparqlStr() const { return parsedSPARQL.getSparqlStr(); }

		const OperatorNode<distinct_binding> &getDistinctOpTree() const {
			if (is_distinct)
				return *distinct_operator_tree;
			else
				throw std::domain_error("This Packackage holds a non-distinct Operator tree.");
		}

		const OperatorNode<counted_binding> &getRegularOpTree() const {
			if (not is_distinct)
				return *regular_operator_tree;
			else
				throw std::domain_error("This Packackage holds a distinct Operator tree.");
		}

		/**
		 * Returns an generator for the result if this->is_distinct
		 * @return result generator
		 */
		yield_pull<distinct_binding> getDistinctGenerator() const {
			if (is_distinct) {
				if (not is_trivial_emtpy)
					return distinct_operator_tree->get();
				else
					return yield_pull<distinct_binding>(
							[&]([[maybe_unused]] yield_push<distinct_binding> &yield) { return; });
			} else
				throw std::domain_error("This Packackage holds a non-distinct Operator tree.");
		}

		/**
		 * Returns an generator for the result if not this->is_distinct
		 * @return result generator
		 */
		yield_pull<counted_binding> getRegularGenerator() const {
			if (not is_distinct) {
				if (not is_trivial_emtpy)
					return regular_operator_tree->get();
				else
					return yield_pull<counted_binding>(
							[&]([[maybe_unused]] yield_push<counted_binding> &yield) { return; });
			} else
				throw std::domain_error("This Packackage holds a distinct Operator tree.");
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
		static std::vector<SliceKey_t> generateSliceKeys(const std::set<TriplePattern> &bgps,
		                                                 const BoolHyperTrie &trie, const TermStore &termIndex) {
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
					if (not std::get<bool>(trie.get(slice_key)))
						return {};
				} else
					slice_keys.push_back(slice_key);
			}

			return slice_keys;
		}
	};

} // namespace tnt::store::cache

template<>
struct fmt::formatter<tnt::store::cache::QueryExecutionPackage> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tnt::store::cache::QueryExecutionPackage &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
		                 " parsedSPARQL:     {}\n"
		                 " is_distinct:      {}\n"
		                 " is_trivial_emtpy: {}\n",
		                 p.parsedSPARQL, p.is_distinct, p.is_trivial_emtpy);
		// TODO: implement print for operator
	}
};

#endif // TNT_QUERYEXECUTIONPACKAGE_HPP
