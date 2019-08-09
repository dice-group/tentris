#ifndef HYPERTRIE_COUNTOPERATOR_HPP
#define HYPERTRIE_COUNTOPERATOR_HPP
namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class CountOperator {
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;

		std::shared_ptr<Subscript> subscript;
		Entry<key_part_type, value_type> *entry;
		value_type count;
		bool _ended;


	public:
		CountOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {}


		static void next(void *self_raw) {
			auto &self = *static_cast<CountOperator *>(self_raw);
			self._ended = true;
			self.entry->key = self.count;
			if constexpr (_debugeinsum_) fmt::print("[{}]->{} {}\n", fmt::join(self.entry);
		}

		static bool ended(void *self_raw) {
			return static_cast<CountOperator *>(self_raw)->_ended;
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands) {
			static_cast<CountOperator *>(self_raw)->load_impl(operands);
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<CountOperator *>(self_raw)->subscript->hash();
		}

	private:
		inline void load_impl(std::vector<const_BoolHypertrie_t> operands, Entry<key_part_type, value_type> &entry) {
			this->entry = &entry;
			assert(operands.size() == 1); // only one operand must be left to be resolved
			count = operands[0].size();
			_ended = not count;
		}

	};
}
#endif //HYPERTRIE_COUNTOPERATOR_HPP
