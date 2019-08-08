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
		bool ended_;

		typename const_BoolHypertrie_t::const_iterator operand_iter;

	public:
		ResolveOperator(std::shared_ptr<Subscript> subscript)
				: subscript(std::move(subscript)) {
			label_pos_in_result = this->subscript->operand2resultMapping_ResolveType();
			ended_ = true;
		}


		static Entry <key_part_type, value_type> next(void *self_raw) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			auto entry = Entry<key_part_type, value_type>{key_part_type(1),
			                                              Key<key_part_type>(self.subscript->resultLabelCount(),
			                                                                 std::numeric_limits<key_part_type>::max())};
			const auto operand_key = *self.operand_iter;
			for (auto i : range(operand_key.size()))
				entry.key[self.label_pos_in_result[i]] = operand_key[i];
			if constexpr (bool_value_type) {
				if (self.subscript->all_result_done) {
					self.ended_ = true;
					return entry;
				}
			}
			++self.operand_iter;
			self.ended_ = not self.operand_iter;
			if constexpr (_debugeinsum_) fmt::print("[{}]->{} {}\n", fmt::join(entry.key, ","), entry.value, self.subscript);
			return entry;
		}

		static bool ended(void *self_raw) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			return self.ended_;
		}

		static void load(void *self_raw, std::vector<const_BoolHypertrie_t> operands) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			self.load_impl(std::move(operands));
		}

		static std::size_t hash(void *self_raw) {
			auto &self = *static_cast<ResolveOperator *>(self_raw);
			return self.subscript->hash();
		}

	private:
		inline void load_impl(std::vector<const_BoolHypertrie_t> operands) {
			if(_debugeinsum_) fmt::print("Resolve {}\n", subscript);
			assert(operands.size() == 1); // only one operand must be left to be resolved
			operand_iter = operands[0].cbegin();
			assert(operand_iter);
			ended_ = not operand_iter;
		}

	};
}
#endif //HYPERTRIE_RESOLVEOPERATOR_HPP
