#ifndef HYPERTRIE_EINSUM_HPP
#define HYPERTRIE_EINSUM_HPP

#include <utility>

#include "einsum/internal/Operator.hpp"
#include "einsum/internal/CartesianOperator.hpp"
#include "einsum/internal/JoinOperator.hpp"
#include "einsum/internal/ResolveOperator.hpp"
#include "einsum/internal/CountOperator.hpp"
#include "einsum/internal/EntryGeneratorOperator.hpp"

namespace einsum::internal {
	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class Einsum {

		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;
		using Join_t = Join<key_part_type, map_type, set_type>;
		using Operator_t = Operator<value_type, key_part_type, map_type, set_type>;

		std::shared_ptr<Subscript> subscript{};
		std::vector<const_BoolHypertrie_t> operands{};
		Operator_t op{};

	public:
		Einsum(std::shared_ptr<Subscript> subscript, const std::vector<const_BoolHypertrie_t> &operands)
				: subscript(std::move(subscript)), operands(operands), op{Operator_t::construct(this->subscript)} {}

		const std::shared_ptr<Subscript> &getSubscript() const {
			return subscript;
		}

		const std::vector<const_BoolHypertrie_t> &getOperands() const {
			return operands;
		}

		const Operator_t &getOp() const {
			return op;
		}

		using iterator = typename Operator_t::iterator;

		iterator begin() {
			op.load(operands);
			return op.begin();
		}

		bool end() const {
			return false;
		}

		void clear() {
			std::logic_error("not yet implemented.");
		}
	};
}
#endif //HYPERTRIE_EINSUM_HPP
