#ifndef TENTRIS_GRAPHQLDATASTRUCTURES_HPP
#define TENTRIS_GRAPHQLDATASTRUCTURES_HPP

namespace tentris::store::graphql::internal {

    struct FieldData {
        bool is_list = false;
        bool non_null = false;
        bool non_null_list_values = false;
		bool is_inverse = false;
        std::string type_name;
        std::string uri;
    };

    struct ObjectData {
		bool is_interface = false;
        std::string uri;
        std::map<std::string, FieldData> fields_data{};
    };

}

#endif//TENTRIS_GRAPHQLDATASTRUCTURES_HPP
