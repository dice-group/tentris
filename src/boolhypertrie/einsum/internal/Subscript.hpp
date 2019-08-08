#ifndef SPARSETENSOR_EINSUM_SUBSCRIPT_HPP
#define SPARSETENSOR_EINSUM_SUBSCRIPT_HPP


#include <vector>
#include <tuple>
#include <map>
#include <numeric>
#include <algorithm>
#include <memory>
#include <ostream>

#include <fmt/core.h>
#include <fmt/format.h>

#include "einsum/internal/util/UndirectedGraph.hpp"
#include "einsum/internal/RawSubscript.hpp"


#include <vector>
#include <tuple>
#include <boost/container_hash/hash.hpp>
#include <tsl/hopscotch_set.h>
#include <tsl/hopscotch_map.h>

namespace einsum::internal {

	using DependencyGraph = util::UndirectedGraph<Label>;
	using ConnectedComponent = std::set<Label>;
	using ConnectedComponents = std::vector<ConnectedComponent>;

	using CartesianOperandPos = OperandPos;
	using OriginalOperandPos = OperandPos;
	using OriginalResultPos = OperandPos;
	using OriginalOperandPoss = std::vector<OriginalOperandPos>;
	using OriginalResultPoss = std::vector<OriginalResultPos>;

	/**
	 * Representation of the subscript of a expression in einstein summation convention.
	 * This provides also  bracketing of independently computable parts and resulting in a
	 * cross product of the bracketed parts.
	 */
	class Subscript {
	public:
		enum class Type {
			None = 0, Join, Cartesian, Resolve, Count, EntryGenerator, CarthesianMapping
		};
		using Label = char;

		class CartesianSubSubscripts {

		private:
			mutable std::vector<std::shared_ptr<Subscript>> sub_subscripts;
			mutable std::vector<OriginalOperandPoss> original_operand_poss_of_sub_subscript;
			mutable std::vector<OriginalResultPoss> original_result_poss_of_sub_subscript;
			mutable std::shared_ptr<Subscript> subscript;

		public:
			CartesianSubSubscripts() = default;

			CartesianSubSubscripts(const Subscript &original_subscript) {
				OperandsSc operands_labels{};
				for (const auto &connected_component: original_subscript.connected_components) {
					auto[cartesian_sub_subscript, original_op_poss, original_result_poss] =
					extractCartesianSubSubscript(original_subscript, connected_component);
					operands_labels.push_back(cartesian_sub_subscript->raw_subscript.result);
					sub_subscripts.emplace_back(std::move(cartesian_sub_subscript));
					original_operand_poss_of_sub_subscript.emplace_back(std::move(original_op_poss));
					original_result_poss_of_sub_subscript.emplace_back(std::move(original_result_poss));
				}
				ResultSc result_labels = original_subscript.raw_subscript.result;
				subscript = std::make_shared<Subscript>(operands_labels, result_labels, Type::CarthesianMapping);
			}

			const std::shared_ptr<Subscript> &getSubscript() const {
				return subscript;
			}

			const std::vector<std::shared_ptr<Subscript>> &getSubSubscripts() const {
				return sub_subscripts;
			}

			const OriginalOperandPoss &getOriginalOperandPoss(CartesianOperandPos cart_op_pos) const {
				return original_operand_poss_of_sub_subscript[cart_op_pos];
			}

			const std::vector<OriginalResultPoss> &getOriginalResultPoss() const {
				return original_result_poss_of_sub_subscript;
			}

		private:
			static std::tuple<std::shared_ptr<Subscript>, OriginalOperandPoss, OriginalResultPoss>
			extractCartesianSubSubscript(const Subscript &subscripts, const ConnectedComponent &label_subset) {
				OperandsSc operands_labels{};
				OriginalOperandPoss original_op_poss{};
				OriginalResultPoss original_result_poss{};
				for (const auto &[parent_op_pos, parent_op_labels] :
						iter::enumerate(subscripts.raw_subscript.operands)) {
					OperandSc op_labels{};
					for (const Label label : parent_op_labels)
						if (label_subset.count(label))
							op_labels.emplace_back(label);

					if (not op_labels.empty()) {
						operands_labels.push_back(op_labels);
						original_op_poss.push_back(parent_op_pos);
					}
				}
				ResultSc result_labels{};
				for (const auto[result_pos, label] : iter::enumerate(subscripts.raw_subscript.result))
					if (label_subset.count(label)) {
						result_labels.emplace_back(label);
						original_result_poss.push_back(result_pos);
					}

				return {std::make_shared<Subscript>(operands_labels, result_labels),
				        original_op_poss,
				        original_result_poss};
			}

			std::size_t size() const noexcept {
				return sub_subscripts.size();
			}
		};


	private:
		mutable tsl::hopscotch_map<Label, std::shared_ptr<Subscript>> sub_subscripts{};

		RawSubscript raw_subscript{};

		tsl::hopscotch_set<Label> operands_label_set{};

		tsl::hopscotch_set<Label> result_label_set{};

		DependencyGraph dependency_graph{};

		ConnectedComponents connected_components{};

		// Join
		mutable tsl::hopscotch_map<Label, LabelPossInOperands> label_poss_in_operands{};
		// Join & resolve
		mutable tsl::hopscotch_map<Label, LabelPos> label_poss_in_result{};
		// Resolve
		LabelPossInOperand operand2result_mapping_resolveType{};

		// Join
		tsl::hopscotch_map<Label, std::vector<OperandPos>> poss_of_operands_with_labels{};

		// Cartesian
		CartesianSubSubscripts cartesian_sub_subscripts;
	public:
		std::shared_ptr<Subscript> removeLabel(Label label) const {
			auto iterator = sub_subscripts.find(label);
			if (iterator != sub_subscripts.end())
				return iterator->second;
			else
				return sub_subscripts.insert(
								{label, std::make_shared<Subscript>(raw_subscript.removeLabel(label))})
						.first->second;
		}

		const tsl::hopscotch_set<Label> &getOperandsLabelSet() const {
			return operands_label_set;
		}

		const tsl::hopscotch_set<Label> &getResultLabelSet() const {
			return result_label_set;
		}

		const std::vector<OperandPos> &getPossOfOperandsWithLabel(Label label) const {
			auto iterator = poss_of_operands_with_labels.find(label);
			if (iterator != poss_of_operands_with_labels.end())
				return iterator->second;
			else
				throw std::invalid_argument("label is not used in operands.");
		}


		/**
		 * for Join
		 * @param label
		 * @return
		 */
		const LabelPossInOperands &getLabelPossInOperands(const Label label) const {
			assert(operands_label_set.count(label));
			auto iterator = label_poss_in_operands.find(label);
			if (iterator != label_poss_in_operands.end())
				return iterator->second;
			else
				return label_poss_in_operands.insert(
								{label, raw_subscript.getLabelPossInOperands(label)})
						.first->second;
		}

		bool isResultLabel(const Label label) const noexcept {
			return result_label_set.count(label);
		}

		LabelPos getLabelPosInResult(const Label label) const {
			auto iterator = label_poss_in_result.find(label);
			if (iterator != label_poss_in_result.end())
				return iterator->second;
			else
				throw std::invalid_argument("label is not in result.");
		}

		[[nodiscard]] auto operandsCount() const noexcept {
			return raw_subscript.operandsCount();
		}

		[[nodiscard]] auto labelCount(const OperandPos operand_pos) const {
			return raw_subscript.labelCount(operand_pos);
		}

		[[nodiscard]] auto resultLabelCount() const noexcept {
			return raw_subscript.resultLabelCount();
		}

		[[nodiscard]] auto getOperandLabels(const OperandPos label_pos) const {
			assert(operandsCount() > label_pos);
			return raw_subscript.operands[label_pos];
		}

		auto operand2resultMapping_ResolveType() const noexcept {
			assert(type == Type::Resolve);
			return operand2result_mapping_resolveType;
		}

	public:
		Type type;
		mutable bool all_result_done;

		Subscript() = default;

		Subscript(const Subscript &) = default;

		Subscript(Subscript &) = default;

		Subscript(Subscript &&) = default;

		Subscript &operator=(const Subscript &) = default;

		Subscript &operator=(Subscript &) = default;

		Subscript(const std::string &subscript_str) : Subscript(from_string(subscript_str)) {}

		bool calcAllResultDone(const tsl::hopscotch_set<Label> &operand_labels,
		                       const tsl::hopscotch_set<Label> &result_labels) {
			for (auto result_label : result_labels) {
				if (operand_labels.count(result_label))
					return false;
			}
			return true;
		}

		Subscript(RawSubscript raw_subscript, Type type = Type::None)
				: raw_subscript(raw_subscript),
				  operands_label_set(raw_subscript.getOperandsLabelSet()),
				  result_label_set(raw_subscript.getResultLabelSet()),
				  dependency_graph(calcDependencyGraph(raw_subscript)),
				  connected_components(dependency_graph.getConnectedComponents()),
				  type((type == Type::CarthesianMapping) ? Type::CarthesianMapping : calcState(raw_subscript,
				                                                                               operands_label_set,
				                                                                               result_label_set,
				                                                                               connected_components)),
				  all_result_done(calcAllResultDone(operands_label_set, result_label_set)) {
			switch (this->type) {

				case Type::Join: {
					label_poss_in_result = raw_subscript.getLabelPossInResult();

					for (const auto[op_pos, labels] : iter::enumerate(raw_subscript.operands))
						for (const Label label : labels)
							poss_of_operands_with_labels[label].push_back(op_pos);
					break;
				}

				case Type::Cartesian: {
					cartesian_sub_subscripts = {*this};
					break;
				}
				case Type::Resolve: {
					label_poss_in_result = raw_subscript.getLabelPossInResult();

					operand2result_mapping_resolveType.reserve(resultLabelCount());
					for (auto label: getOperandLabels(0))
						operand2result_mapping_resolveType.emplace_back(
								getLabelPosInResult(label)
						);
					break;
				}
				case Type::Count:
					break;
				case Type::EntryGenerator:
					break;
				case Type::None:
					break;
				case Type::CarthesianMapping:
					break;
			}
		}


		Subscript(OperandsSc operands, ResultSc result, Type type = Type::None) : Subscript{
				RawSubscript{std::move(operands), std::move(result)}, type} {}

		std::string to_string() const {
			std::vector<std::string> operand_strings;
			operand_strings.reserve(raw_subscript.operands.size());
			for (const auto &operand : raw_subscript.operands) {
				operand_strings.push_back(fmt::format("{}", fmt::join(operand, "")));
			}
			return fmt::format("{}->{}", fmt::join(operand_strings, ","), fmt::join(raw_subscript.result, ""));
		}

		static Subscript from_string(const std::string &subscript_str) {
			auto iter = subscript_str.cbegin();
			auto end = subscript_str.end();
			std::map<char, Label> char_mapping{};
			OperandsSc operands_sc{};
			ResultSc result_sc{};
			Label next_label = 'a';

			while (*iter != '-') {
				OperandSc &operand_sc = operands_sc.emplace_back();
				while (*iter != ',' and *iter != '-') {
					if (not char_mapping.count(*iter)) {
						char_mapping[*iter] = next_label++;
					}
					operand_sc.push_back(char_mapping[*iter]);
					++iter;
				}
				if (*iter == '-')
					break;
				++iter;
			}
			if (operands_sc.empty())
				operands_sc.emplace_back();
			iter = iter + 2;
			while (iter != end) {
				assert(char_mapping.count(*iter));
				result_sc.push_back(char_mapping[*iter]);
				++iter;
			}
			return {std::move(operands_sc), std::move(result_sc)};
		}

		const RawSubscript &getRawSubscript() const {
			return raw_subscript;
		}

	public:
		const CartesianSubSubscripts &getCartesianSubscript() const {
			return cartesian_sub_subscripts;
		}

	private:
		Type calcState(const RawSubscript &raw_subscript,
		               const tsl::hopscotch_set<Label> &operands_label_set,
		               const tsl::hopscotch_set<Label> &result_label_set,
		               const ConnectedComponents &connected_components) {
			switch (raw_subscript.operandsCount()) {
				case 0:
					return Type::EntryGenerator;
				case 1:
					// single operand
					if (auto operand_label_count = raw_subscript.labelCount(0);
							operand_label_count == operands_label_set.size()) {
						// at least one label occures multiple times and thus there is a diagonal
						// check if  all operand labels are either in result or all are not in result
						// none of both may also be the case
						bool all_in_result = true;
						bool none_in_result = true;
						for (auto label : operands_label_set) {
							if (result_label_set.count(label))
								none_in_result = false;
							else
								all_in_result = false;
						}
						if (all_in_result)
							return Type::Resolve;
						else if (none_in_result)
							return Type::Count;
					}
					break;
				default: {
					// multiple operands
					if (connected_components.size() > 1) {
						// more than one connected component means that there is a Cartesian product
						return Type::Cartesian;
					}
					break;
				}
			}
			return Type::Join;
		}

		static DependencyGraph calcDependencyGraph(const RawSubscript &raw_subscript) {
			DependencyGraph label_dependency_graph{};
			for (const auto &operand : raw_subscript.operands)
				label_dependency_graph.addCompleteGraph(operand);
			return label_dependency_graph;
		}

	public:
		bool operator!=(const Subscript &other) const { return hash() != other.hash(); };

		[[nodiscard]] inline size_t hash() const { return raw_subscript.hash; }

		[[nodiscard]] bool valid() const {
			for (auto result_label : raw_subscript.result)
				if (not operands_label_set.count(result_label))
					return false;
			return true;
		}


	};

	using CartesianSubSubscripts = Subscript::CartesianSubSubscripts;
}

std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<einsum::internal::Subscript> &sub_script) {
	stream << sub_script->to_string();
	return stream;
}

std::ostream &operator<<(std::ostream &stream, einsum::internal::Subscript const *const sub_script) {
	stream << sub_script->to_string();
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const einsum::internal::Subscript &sub_script) {
	stream << sub_script.to_string();
	return stream;
}


template<>
struct std::hash<einsum::internal::Subscript> {
	size_t operator()(const einsum::internal::Subscript &s) const { return s.hash(); }
};

#endif //SPARSETENSOR_EINSUM_SUBSCRIPT_HPP


