#ifndef HYPERTRIE_RESOLVEOPERATOR_HPP
#define HYPERTRIE_RESOLVEOPERATOR_HPP
namespace einsum::internal {

	template<typename value_type, typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class ResolveOperator {
		constexpr static const bool bool_value_type = std::is_same_v<value_type, bool>;
		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;
		using Operator_t = Operator<value_type, key_part_type, map_type, set_type>;

		std::shared_ptr<Subscript> subscript;
		LabelPossInOperand label_pos_in_result;
		Entry <key_part_type, value_type> *entry;
		bool ended_;

		typename const_BoolHypertrie_t::const_iterator operand_iter;

	public:
		ResolveOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {
			label_pos_in_result = this->subscript->operand2resultMapping_ResolveType();
			ended_ = true;
		}


		static void next(void *self_raw) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			++self.operand_iter;
			self.ended_ = not self.operand_iter;
			if (self.ended_)
				return;
			self.entry->value = key_part_type(1);
			const auto &operand_key = *self.operand_iter;
			for (auto i : range(operand_key.size()))
				self.entry->key[self.label_pos_in_result[i]] = operand_key[i];
			if constexpr (bool_value_type) {
				if (self.subscript->all_result_done) {
					self.ended_ = true;
					return;
				}
			}

			if constexpr (_debugeinsum_)
				fmt::print("[{}]->{} {}\n", fmt::join(self.entry->key, ","), self.entry->value, self.subscript);
		}

		static bool ended(void *self_raw) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			return self.ended_;
		}

		static void
		load(void *self_raw, std::vector<const_BoolHypertrie_t> operands, Entry <key_part_type, value_type> &entry) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			self.load_impl(std::move(operands), entry);
		}

		static std::size_t hash(void *self_raw) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			return self.subscript->hash();
		}

	private:
		inline void load_impl(std::vector<const_BoolHypertrie_t> operands, Entry <key_part_type, value_type> &entry) {
			if constexpr(_debugeinsum_) fmt::print("Resolve {}\n", subscript);
			this->entry = &entry;
			assert(operands.size() == 1); // only one operand must be left to be resolved
			operand_iter = std::move(operands[0].cbegin());
			assert(operand_iter);
			ended_ = not operand_iter;
			if (not ended_) {
				for (auto &key_part : entry.key)
					key_part = std::numeric_limits<key_part_type>::max();
				this->entry->value = key_part_type(1);
				const auto operand_key = *this->operand_iter;
				for (auto i : range(operand_key.size()))
					this->entry->key[this->label_pos_in_result[i]] = operand_key[i];
			}
		}

	};
}
#endif //HYPERTRIE_RESOLVEOPERATOR_HPP
