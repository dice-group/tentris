#ifndef TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
#define TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP

#include <algorithm>
#include <any>
#include <exception>
#include <ostream>

#include "tentris/store/AtomicGraphqlSchema.hpp"
#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/store/graphql/GraphQLParser.hpp"

namespace tentris::store {
	class TripleStore;
};

namespace tentris::store::cache {

	/**
     * A QueryExecutionPackage contains everything that is necessary to execute a given graphql query for a state of the
     * RDF graph.
     */
	struct GraphQLExecutionPackage {

	private:
		using const_BoolHypertrie = ::tentris::tensor::const_BoolHypertrie;
		using time_point_t = logging::time_point_t;
		using Subscript = ::tentris::tensor::Subscript;
		using Einsum = ::tentris::tensor::Einsum<::tentris::tensor::COUNTED_t>;

	private:
		std::string query_name;
		// a graphql query can generate multiple subscripts (multiple root fields)
		std::vector<std::shared_ptr<Subscript>> subscripts;
		// a vector containing the operands of each subscript
		std::vector<std::vector<const_BoolHypertrie>> all_operands{};
		// a vector containing the paths of the query (one list per root field)
		std::vector<std::vector<std::vector<std::pair<Subscript::Label, std::string>>>> all_paths{};
		// a vector containing the fragment labels (one set per root field)
		std::vector<std::set<Subscript::Label>> all_fragment_labels{};
		std::vector<Subscript::Label> opt_begin{'['};
		std::vector<Subscript::Label> opt_end{']'};

	public:
		GraphQLExecutionPackage() = delete;

		/**
         * @param request a graphql request consting of the request document and the operation name to be executed
         */
		explicit GraphQLExecutionPackage(const std::pair<std::string, std::string> &request);

	public:
		[[nodiscard]] std::vector<std::shared_ptr<Einsum>> generateEinsums(const time_point_t &timeout) {
			std::vector<std::shared_ptr<Einsum>> einsums{};
			for (auto i : iter::range(subscripts.size())) {
				auto einsum = std::make_shared<Einsum>(subscripts[i], all_operands[i], timeout);
				einsums.push_back(std::move(einsum));
			}
			return einsums;
		}

		[[nodiscard]] const std::vector<std::vector<std::pair<Subscript::Label, std::string>>> &getPath(std::size_t pos) {
			return all_paths[pos];
		}

		[[nodiscard]] const std::set<Subscript::Label> &getFragmentLabels(std::size_t pos) {
			return all_fragment_labels[pos];
		}

		[[nodiscard]] const std::vector<std::vector<const_BoolHypertrie>> &getOperands() const {
			return all_operands;
		}

		friend struct ::fmt::formatter<GraphQLExecutionPackage>;
	};

}// namespace tentris::store::cache

template<>
struct fmt::formatter<tentris::store::cache::GraphQLExecutionPackage> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const tentris::store::cache::GraphQLExecutionPackage &p, FormatContext &ctx) {
		return format_to(ctx.begin(),
						 " GraphQL Query Name:     {}\n"
						 " subscripts:  {}\n",
						 p.query_name, p.subscripts);
	}
};
#endif//TENTRIS_GRAPHQLEXECUTIONPACKAGE_HPP
