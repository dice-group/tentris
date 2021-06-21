#ifndef TENTRIS_GRAPHQLDATASTRUCTURES_HPP
#define TENTRIS_GRAPHQLDATASTRUCTURES_HPP

#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace tentris::store::graphql::internal {

	using OperandLabels = std::vector<char>;
	using OperandsLabels = std::vector<OperandLabels>;
	using ResultsLabels = std::vector<char>;
	using Feature = std::vector<std::string>;
	using Features = std::vector<Feature>;
	using FieldNames = std::map<char, std::string>;
	using LeafTypes = std::map<char, std::string>;
	using Path = std::vector<char>;
	using Paths = std::vector<Path>;
	using FragmentDependencies = std::map<char, std::map<std::string, std::vector<char>>>;

	struct FieldData {
		bool is_list = false;
		bool non_null = false;
		bool non_null_list_values = false;
		bool is_inverse = false;
		std::string type_name;
		std::string uri;
		std::map<std::string, std::string> arguments{};
	};

	struct ObjectData {
		std::set<std::string> implementations{};
		std::string uri;
		std::map<std::string, FieldData> fields_data{};
	};

	struct InterfaceData {
		std::string uri;
		std::map<std::string, FieldData> fields_data{};
	};

	// represnts a query starting from a root field
	struct ParsedSubGraphQL {
		// operands' subscripts
		OperandsLabels operands_labels{};
		// result's subscript
		ResultsLabels result_labels{};
		// list of fields names
		Features features{};
		// map from label to field names
		FieldNames field_names{};
		// inline framgent dependencies - label to type condition (uri) to dependent labels
		FragmentDependencies fragment_dependencies{};
		// the types of leaf fields
		LeafTypes leaf_types{};
		// the paths from the root to the leaves
		Paths paths{};
		// stores the labels whose fields are lists
		std::set<char> list_labels{};
		// stores the labels whose fields are non-null
		std::set<char> non_null_labels{};
		// stores the positions of operands that need to be filtered by id
		std::map<uint32_t, std::string> id_arguments{};
	};

	using ParsedGraphQL = std::vector<ParsedSubGraphQL>;

}// namespace tentris::store::graphql::internal

#endif//TENTRIS_GRAPHQLDATASTRUCTURES_HPP
