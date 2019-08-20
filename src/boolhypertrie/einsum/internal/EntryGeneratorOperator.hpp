#ifndef HYPERTRIE_ENTRYGENERATOROPERATOR_HPP
#define HYPERTRIE_ENTRYGENERATOROPERATOR_HPP
namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class EntryGeneratorOperator {
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;

		std::shared_ptr<Subscript> subscript;
		Entry<key_part_type, value_type> *entry;
		bool _ended = true;


	public:
		EntryGeneratorOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {}


		static void next(void *self_raw) {
			auto &self = *static_cast<EntryGeneratorOperator *>(self_raw);
			self._ended = true;

			if constexpr (_debugeinsum_) fmt::print("[{}]->{} {}\n", fmt::join(self.entry->key, ","), self.entry->value, self.subscript);
		}

		static bool ended(void *self_raw) {
			return static_cast<EntryGeneratorOperator *>(self_raw)->_ended;
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands, Entry<key_part_type, value_type> &entry) {
			static_cast<EntryGeneratorOperator *>(self_raw)->load_impl(std::move(operands), entry);
		}

		static std::size_t hash(void *self_raw) {
			return static_cast<EntryGeneratorOperator *>(self_raw)->subscript->hash();
		}

	private:
		inline void load_impl([[maybe_unused]]std::vector<const_BoolHypertrie_t> operands, Entry<key_part_type, value_type> &entry) {
			if constexpr(_debugeinsum_) fmt::print("EntryGen {}\n", subscript);
			this->entry = &entry;
			this->entry->value = key_part_type(1);
			assert(operands.size() == 0); // no operand must be left
			_ended = false;
			if(not _ended)
				for(auto &key_part : entry.key)
					key_part = std::numeric_limits<key_part_type>::max();
		}
	};
}
#endif //HYPERTRIE_ENTRYGENERATOROPERATOR_HPP
