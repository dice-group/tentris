#ifndef TENTRIS_GRAPHQLDATASTRUCTURES_HPP
#define TENTRIS_GRAPHQLDATASTRUCTURES_HPP

namespace tentris::store::graphql::internal {

    struct FieldData {
        bool is_list;
		bool non_null;
		bool non_null_list_values;
        std::string type_name;
        std::string uri;
    };

    struct ObjectData {
        std::string uri;
        std::map<std::string, FieldData> fields_data{};
    };

}

#endif//TENTRIS_GRAPHQLDATASTRUCTURES_HPP
