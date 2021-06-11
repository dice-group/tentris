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
    using FieldName = std::string;
    using Argument = std::pair<FieldName, std::string>;
    using FieldsNameArguments = std::vector<std::variant<FieldName, Argument>>;
    using Path = std::vector<std::pair<char, std::string>>;
    using Paths = std::vector<Path>;

	struct FieldData {
		bool is_list = false;
		bool non_null = false;
		bool non_null_list_values = false;
		bool is_inverse = false;
		std::string type_name;
		std::string uri;
	};

	struct ObjectData {
		std::vector<std::string> implementations{};
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
		FieldsNameArguments fields_name_arguments{};
		// list of paths
		Paths paths{};
		// the labels that appear in inline fragments
		std::set<char> fragment_labels{};
	};

	using ParsedGraphQL = std::vector<ParsedSubGraphQL>;

}// namespace tentris::store::graphql::internal

#endif//TENTRIS_GRAPHQLDATASTRUCTURES_HPP
