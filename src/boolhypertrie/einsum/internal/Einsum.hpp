#ifndef HYPERTRIE_EINSUM_HPP
#define HYPERTRIE_EINSUM_HPP

#include <utility>
#include <tsl/hopscotch_set.h>

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
		}
	};

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class Einsum<bool, key_part_type, map_type, set_type> {
		using value_type = bool;

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

		struct iterator {
		private:
			using Entry_t = Entry<key_part_type, bool>;
			using Key_t = typename Entry_t::key_type;
			typename Operator_t::iterator op_it;
			tsl::hopscotch_set<Key_t, ::einsum::internal::KeyHash<key_part_type>> found_entries{};
			Entry_t current_entry;
			bool ended_ = false;
		public:
			iterator() = default;

			explicit iterator(Einsum &einsum) : op_it(einsum.op.begin()) {
				if (op_it) {
					current_entry = op_it.value();
					found_entries.insert(current_entry.key);
				} else {
					ended_ = true;
				}
			}

			/**
			 * The iterator is frowarded by using the operator* or by calling value().
			 * @return reference to self.
			 */
			iterator &operator++() { return *this; }

			/**
			 * Returns the next entry and forwards the iterator. equal to operator*
			 * @return
			 */
			inline Entry<key_part_type, value_type> operator*() {
				return value();
			}

			/**
			 * Returns the next entry and forwards the iterator. equal to operator*
			 * @return
			 */
			inline Entry<key_part_type, value_type> value() {
				auto entry_to_return = std::move(current_entry);
				assert(not ended_);
				while (op_it) {
					current_entry = op_it.value();
					if (found_entries.find(current_entry.key) == found_entries.end()) {
						found_entries.insert(current_entry.key);
						break;
					}
				}
				ended_ = not op_it;

				return entry_to_return;
			}

			/**
			 * Is true as long as there are more entrys retrievable via operator* or value.
			 * @return
			 */
			operator bool() const {
				return not ended_;

			}

			/**
			 * Returns true if the iteration is at its end.
			 * @return
			 */
			inline bool ended() const { return not ended_; }

		};

		iterator begin() {
			op.load(operands);
			return iterator{*this};
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
