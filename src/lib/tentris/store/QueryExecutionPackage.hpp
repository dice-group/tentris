#ifndef TENTRIS_QUERYEXECUTIONPACKAGE_HPP
#define TENTRIS_QUERYEXECUTIONPACKAGE_HPP

#include <any>
#include <exception>
#include <ostream>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store {
	class TripleStore;
};

namespace tentris::store::cache {

	/**
	 * A QueryExecutionPackage contains everything that is necessary to execute a given sparql query for a state of the
	 * RDF graph.
	 */
	struct QueryExecutionPackage {
		using const_BoolHypertrie = ::tentris::tensor::const_BoolHypertrie;
		using time_point_t = logging::time_point_t;
		using SelectModifier = sparql::SelectModifier;
		using Variable = sparql::Variable;
		using ParsedSPARQL = sparql::ParsedSPARQL;
		using Subscript = ::tentris::tensor::Subscript;

	private:
		std::string sparql_string;
		std::shared_ptr<Subscript> subscript;
		SelectModifier select_modifier;
		std::vector<Variable> query_variables;

	public:
		/**
		 * Indicates if the QueryExecutionPackage represents an distinct query or not. If it is distinct use only
		 * the methods with distinct in their names. Otherwise use only the methods with regular in their names
		 */

		bool is_trivial_empty = false;

	private:

		std::vector<const_BoolHypertrie> operands{};

	public:
		QueryExecutionPackage() = delete;

		/**
		 *
		 * @param sparql_string sparql query to be parsed
		 * @param trie current try holding the data
		 * @param termIndex term store attached to the trie
		 * @throw std::invalid_argument the sparql query was not parsable
		 */
		explicit QueryExecutionPackage(const std::string &sparql_string) : sparql_string{sparql_string} {
			using namespace logging;
			logDebug(fmt::format("Parsing query: {}", sparql_string));
			ParsedSPARQL parsed_sparql{sparql_string};
			subscript = parsed_sparql.getSubscript();
			select_modifier = parsed_sparql.getSelectModifier();
			logDebug(fmt::format("Parsed subscript: {} [distinct = {}]",
								 subscript,
								 select_modifier == SelectModifier::DISTINCT));
			query_variables = parsed_sparql.getQueryVariables();

			auto &triple_store = AtomicTripleStore::getInstance();

			logDebug(fmt::format("Slicing TPs"));
			for ([[maybe_unused]] const auto &[op_pos, tp]: iter::enumerate(parsed_sparql.getBgps())) {
				logDebug(fmt::format("Slice key {}: ⟨{}⟩", op_pos, fmt::join(tp, ", ")));
				std::variant<const_BoolHypertrie, bool> op = triple_store.resolveTriplePattern(tp);
				if (std::holds_alternative<bool>(op)) {
					is_trivial_empty = not std::get<bool>(op);
					logTrace(fmt::format("Operand {} is {}", op_pos, is_trivial_empty));
				} else {
					auto bht = std::get<const_BoolHypertrie>(op);
					if (not bht.empty()) {
						logTrace(fmt::format("Operand {} size {}", op_pos, bht.size()));
						operands.emplace_back(bht);
					} else {
						is_trivial_empty = true;
						operands.clear();
					}
				}
				if (is_trivial_empty) {
					logDebug(fmt::format("Query is trivially empty, i.e. the lastly sliced operand {} is emtpy.", op_pos));
					break;
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
													const std::vector<const_BoolHypertrie> &hypertries,
													const time_point_t &timeout) {
			using namespace tensor;
			return std::make_shared<Einsum<RESULT_TYPE>>(subscript, hypertries, timeout);
		}

	public:
		std::shared_ptr<void> getEinsum(const time_point_t &timeout = time_point_t::max()) const {
			using namespace tensor;
			if (select_modifier == SelectModifier::NONE)
				return generateEinsum<COUNTED_t>(subscript, operands, timeout);
			else
				return generateEinsum<DISTINCT_t>(subscript, operands, timeout);
		}

		const std::string &getSparqlStr() const {
			return sparql_string;
		}

		const std::shared_ptr<Subscript> &getSubscript() const {
			return subscript;
		}

		SelectModifier getSelectModifier() const {
			return select_modifier;
		}

		const std::vector<Variable> &getQueryVariables() const {
			return query_variables;
		}

		friend struct ::fmt::formatter<QueryExecutionPackage>;
	};
} // namespace tentris::store::cache

template<>
struct fmt::formatter<tentris::store::cache::QueryExecutionPackage> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::cache::QueryExecutionPackage &p, FormatContext &ctx) {
		using SelectModifier = tentris::store::sparql::SelectModifier;
		return format_to(ctx.begin(),
						 " SPARQL:     {}\n"
						 " subscript:  {}\n"
						 " is_distinct:      {}\n"
						 " is_trivial_empty: {}\n",
						 p.sparql_string, p.subscript, p.select_modifier == SelectModifier::DISTINCT,
						 p.is_trivial_empty);
	}
};

#endif // TENTRIS_QUERYEXECUTIONPACKAGE_HPP

