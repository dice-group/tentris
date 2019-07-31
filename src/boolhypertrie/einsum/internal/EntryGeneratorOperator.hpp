#ifndef HYPERTRIE_ENTRYGENERATOROPERATOR_HPP
#define HYPERTRIE_ENTRYGENERATOROPERATOR_HPP
namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class EntryGeneratorOperator {
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;

		std::shared_ptr<Subscript> subscript;
		bool _ended = true;


	public:
		EntryGeneratorOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {}


		static Entry <key_part_type, value_type> next(void *self_raw) {
			auto &self = *static_cast<EntryGeneratorOperator *>(self_raw);
			self._ended = true;
			return Entry<key_part_type, value_type>{key_part_type(1),
			                                        Key<key_part_type>(self.subscript->resultLabelCount(),
			                                                           std::numeric_limits<key_part_type>::max())};
		}

		static bool ended(void *self_raw) {
			return static_cast<EntryGeneratorOperator *>(self_raw)->_ended;
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands) {
			static_cast<EntryGeneratorOperator *>(self_raw)->load_impl(std::move(operands));
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<EntryGeneratorOperator *>(self_raw)->subscript->hash();
		}

	private:
		inline void load_impl([[maybe_unused]]std::vector<const_BoolHypertrie_t> operands) {
			assert(operands.size() == 0); // no operand must be left
			_ended = false;
		}
	};
}
#endif //HYPERTRIE_ENTRYGENERATOROPERATOR_HPP
