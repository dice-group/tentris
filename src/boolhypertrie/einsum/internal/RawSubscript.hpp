#ifndef HYPERTRIE_RAWSUBSCRIPT_HPP
#define HYPERTRIE_RAWSUBSCRIPT_HPP

#include <vector>
#include <tuple>
#include <boost/container_hash/hash.hpp>
#include <tsl/hopscotch_set.h>
#include <tsl/hopscotch_map.h>

namespace einsum::internal {
	using Label = char;
	using OperandSc = std::vector<Label>;
	using ResultSc = OperandSc;
	using OperandsSc = std::vector<OperandSc>;
	using OperandPos = uint8_t;
	using LabelPos = uint8_t ;
	using LabelPossInOperand = std::vector<LabelPos>;
	using LabelPossInOperands = std::vector<LabelPossInOperand>;

	class RawSubscript {
	public:
		mutable OperandsSc operands{};
		mutable ResultSc result{};
		mutable std::size_t hash{};

	public:
		RawSubscript() = default;

		RawSubscript(RawSubscript &) = default;

		RawSubscript(const RawSubscript &) = default;

		RawSubscript(RawSubscript &&) = default;

		RawSubscript &operator=(const RawSubscript &) = default;

		RawSubscript &operator=(RawSubscript &) = default;

		RawSubscript &operator=(RawSubscript &&) = default;

		explicit RawSubscript(const OperandsSc &operands, const ResultSc &result) : operands(operands), result(result),
		                                                              hash(boost::hash_value(operands) +
		                                                                   boost::hash_value(result)) {
		}

		RawSubscript(const std::tuple<OperandsSc, ResultSc> &raw_subscript) : RawSubscript(std::get<0>(raw_subscript),
		                                                                            std::get<1>(raw_subscript)) {}

		RawSubscript(const std::pair<OperandsSc, ResultSc> &raw_subscript) : RawSubscript(std::get<0>(raw_subscript),
		                                                                           std::get<1>(raw_subscript)) {}

		[[nodiscard]] auto operandsCount() const noexcept {
			return operands.size();
		}

		[[nodiscard]] auto labelCount(OperandPos operand_pos) const {
			return operands[operand_pos].size();
		}

		[[nodiscard]] auto resultLabelCount() const noexcept {
			return result.size();
		}

		[[nodiscard]] auto getOperandsLabelSet() const noexcept {
			tsl::hopscotch_set<Label> operand_labels{};
			for (const auto &operand : operands)
				for (auto label : operand)
					operand_labels.insert(label);
			return operand_labels;
		}

		[[nodiscard]] auto getResultLabelSet() const noexcept {
			tsl::hopscotch_set<Label> result_labels{};
			for (auto label : result)
				result_labels.insert(label);
			return result_labels;
		}

		[[nodiscard]] auto getLabelPossInOperands(Label label) const noexcept {
			LabelPossInOperands label_poss_in_operands{};
			label_poss_in_operands.resize(operands.size());
			for (auto i : range(operands.size())) {

				auto &label_poss_in_operand = label_poss_in_operands[i];
				const auto &operand = operands[i];

				for (const auto &[label_pos, current_label] : enumerate(operand))
					if (current_label == label)
						label_poss_in_operand.push_back(label_pos);

			}

			return label_poss_in_operands;
		}

		[[nodiscard]] auto getLabelPossInResult() const noexcept {
			tsl::hopscotch_map<Label, LabelPos> label_poss_in_result{};
			for (auto[pos, label]: iter::enumerate(result))
				label_poss_in_result.insert({label, pos});
			return label_poss_in_result;
		}

		[[nodiscard]] auto removeLabel(Label label) const noexcept {
			assert(getOperandsLabelSet().count(label));
			OperandsSc next_operands{};
			for (const auto &operand: operands) {
				OperandSc new_operand{};
				for (auto current_label: operand)
					if (current_label != label)
						new_operand.push_back(current_label);
				if (not new_operand.empty()) {
					next_operands.push_back(std::move(new_operand));
				}
			}
			return RawSubscript(next_operands, result);
		}


		bool operator!=(const RawSubscript &other) { return hash != other.hash; };
	};
}

template<>
struct std::hash<einsum::internal::RawSubscript> {
	size_t operator()(const einsum::internal::RawSubscript &s) const { return s.hash; }
};

#endif //HYPERTRIE_RAWSUBSCRIPT_HPP
