#ifndef HYPERTRIE_COUNTOPERATOR_HPP
#define HYPERTRIE_COUNTOPERATOR_HPP
namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class CountOperator {
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;

		std::shared_ptr<Subscript> subscript;
		Entry<key_part_type, value_type> *entry;
		bool _ended;


	public:
		CountOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {}


		static void next(void *self_raw) {
			auto &self = *static_cast<CountOperator *>(self_raw);
			self._ended = true;
			if constexpr (_debugeinsum_) fmt::print("[{}]->{} {}\n", fmt::join(self.entry->key, ","), self.entry->value, self.subscript);
		}

		static bool ended(void *self_raw) {
			return static_cast<CountOperator *>(self_raw)->_ended;
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands, Entry<key_part_type, value_type> &entry) {
			static_cast<CountOperator *>(self_raw)->load_impl(operands, entry);
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<CountOperator *>(self_raw)->subscript->hash();
		}

	private:
		inline void load_impl(std::vector<const_BoolHypertrie_t> operands, Entry<key_part_type, value_type> &entry) {
			this->entry = &entry;
			assert(operands.size() == 1); // only one operand must be left to be resolved
			this->entry->value = operands[0].size();
			_ended = not this->entry->value;
		}

	};
}
#endif //HYPERTRIE_COUNTOPERATOR_HPP
