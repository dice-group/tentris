#ifndef HYPERTRIE_OPERATOR_HPP
#define HYPERTRIE_OPERATOR_HPP

#include <memory>
#include "einsum/internal/Subscript.hpp"
#include "einsum/internal/Entry.hpp"

namespace einsum::internal {


	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class CartesianOperator;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class JoinOperator;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class ResolveOperator;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class CountOperator;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class EntryGeneratorOperator;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class Operator {
	public:
		mutable Subscript::Type type = Subscript::Type::None;
	private:

		template<typename T>
		static constexpr Subscript::Type getType() {
			if (std::is_same_v<T, JoinOperator<key_part_type, map_type, set_type>>) {
				return Subscript::Type::Join;
			}
			if (std::is_same_v<T, ResolveOperator<key_part_type, map_type, set_type>>) {
				return Subscript::Type::Resolve;
			}
			if (std::is_same_v<T, CountOperator<key_part_type, map_type, set_type>>) {
				return Subscript::Type::Count;
			}
			if (std::is_same_v<T, CartesianOperator<key_part_type, map_type, set_type>>) {
				return Subscript::Type::Cartesian;
			}
			if (std::is_same_v<T, CartesianOperator<key_part_type, map_type, set_type>>) {
				return Subscript::Type::EntryGenerator;
			}
			return Subscript::Type::None;
		}

		/**
		 * Pointer to the actual operator instance.
		 */
		std::shared_ptr<void> operator_instance;

		/**
		 * Pointer to the next Function of the operator implementation.
		 * @param self pointer to the actual operator instance
		 * @return the next Entry. Afterwards the Iterator is automatically forwarded.
		 */
		UnsignedEntry<key_part_type> (*next_fp)(void *self);

		/**
		 * Pointer to the ended Function of the operator implementation.
		 * @param self actual operator instance
		 * @return if iteration ended
		 */
		bool (*ended_fp)(void *self);

		/**
		 * Pointer to the load Function of the operator implementation.
		 * @param self actual operator instance
		 * @param operands operands to be loaded
		 */
		void (*load_fp)(void *self, std::vector<const_BoolHypertrie<key_part_type, map_type, set_type>> operands);

		std::size_t (*hash_fp)(void *self);

	public:
		static Operator
		construct(std::shared_ptr<Subscript> subscript) {
			switch (subscript->type) {
				case Subscript::Type::Join:
					return {std::make_shared<JoinOperator<key_part_type, map_type, set_type >>(subscript)};
				case Subscript::Type::Resolve:
					return {std::make_shared<ResolveOperator<key_part_type, map_type, set_type >>(subscript)};
				case Subscript::Type::Count:
					return {std::make_shared<CountOperator<key_part_type, map_type, set_type >>(subscript)};
				case Subscript::Type::Cartesian:
					return {std::make_shared<CartesianOperator<key_part_type, map_type, set_type >>(subscript)};
				case Subscript::Type::EntryGenerator:
					return {std::make_shared<EntryGeneratorOperator<key_part_type, map_type, set_type >>(subscript)};
				default:
					throw std::invalid_argument{"subscript is of an undefined type."};
			}
		}


	private:
		template<typename T>
		Operator(std::shared_ptr<T> op) : type(getType<T>()), operator_instance(std::move(op)), next_fp(&T::next),
		                                  ended_fp(&T::ended),
		                                  load_fp(&T::load),
		                                  hash_fp(&T::hash) {}

	public:
		Operator() = default;

		Operator(Operator &) = default;

		Operator(Operator &&) noexcept = default;

		// TODO: remove because implicit
		Operator &operator=(Operator &op) noexcept {
			this->type = op.type;
			this->operator_instance = std::move(op.operator_instance);
			this->next_fp = op.next_fp;
			this->ended_fp = op.ended_fp;
			this->load_fp = op.load_fp;
			this->hash_fp = op.hash_fp;
			return *this;
		}

		Operator &operator=(Operator &&op) noexcept {
			this->type = op.type;
			this->operator_instance = std::move(op.operator_instance);
			this->next_fp = op.next_fp;
			this->ended_fp = op.ended_fp;
			this->load_fp = op.load_fp;
			this->hash_fp = op.hash_fp;
			return *this;
		}

		struct iterator {
		private:
			Operator *op;
		public:
			iterator() = default;

			explicit iterator(Operator &op) : op(&op) {}

			/**
			 * This is only a stub to fulfill the C++ iterator interface.
			 * The iterator is frowarded by using the operator* or by calling value().
			 * @return reference to self.
			 */
			iterator &operator++() { return *this; }

			/**
			 * Returns the next entry and forwards the iterator. equal to operator*
			 * @return
			 */
			inline UnsignedEntry<key_part_type> operator*() const {
				return std::invoke(op->next_fp, op->operator_instance.get());
			}

			/**
			 * Returns the next entry and forwards the iterator. equal to operator*
			 * @return
			 */
			inline UnsignedEntry<key_part_type> value() const {
				return std::invoke(op->next_fp, op->operator_instance.get());
			}

			/**
			 * Is true as long as there are more entrys retrievable via operator* or value.
			 * @return
			 */
			operator bool() {
				return not std::invoke(op->ended_fp, op->operator_instance.get());

			}

			/**
			 * Returns true if the iteration is at its end.
			 * @return
			 */
			inline bool ended() const { return std::invoke(op->ended_fp, op->operator_instance.get()); }

		};

		iterator begin() { return iterator{*this}; }

		bool end() { return false; }

		void load(std::vector<const_BoolHypertrie<key_part_type, map_type, set_type>> operands) {
			std::invoke(load_fp, operator_instance.get(), std::move(operands));
		}

		std::size_t hash() const {
			return std::invoke(hash_fp, operator_instance.get());
		}

		bool operator!=(const Operator &other) const { return hash() != other.hash(); };

	};


}

#endif //HYPERTRIE_OPERATOR_HPP

