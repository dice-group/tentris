#ifndef TENTRIS_GRAPHQLRESPONSEDOM_HPP
#define TENTRIS_GRAPHQLRESPONSEDOM_HPP

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

#include "tentris/store/AtomicGraphqlSchema.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store::graphql {

	class GraphqlResponseDOM {

	private:

		using AtomicGraphqlSchema = tentris::store::AtomicGraphqlSchema;
		using Entry = ::tentris::tensor::EinsumEntry<::tentris::tensor::COUNTED_t>;
		using Key = typename Entry::Key;
		using Label = ::tentris::tensor::Subscript::Label;
		using key_part_type = ::tentris::tensor::key_part_type;

		// stores the last mapping of the einsum -- used to updated the array counters
		std::unique_ptr<std::map<Label, key_part_type>> last_mapping = nullptr;
		// stores for each inner field its children fields
		std::map<Label, std::vector<std::string>> label_children{};
		// stores for each inner field the type of its children fields
		std::map<Label, std::vector<bool>> label_children_types{};
		// stores for each inner field its path
		std::map<Label, std::vector<std::string>> label_paths{};
		// stores the paths that lead to leaves
		std::vector<std::vector<std::string>> leaf_paths{};
		// stores for each leaf path the inner field labels that it contains
		std::vector<std::vector<Label>> labels_in_leaves{};

		rapidjson::Document json_response{};
		rapidjson::GenericValue<rapidjson::UTF8<char>> *root_field = nullptr;

	public:

        explicit GraphqlResponseDOM() {
            json_response.SetObject();
            json_response.AddMember("data", rapidjson::kNullType, json_response.GetAllocator());
			root_field = &json_response["data"];
        }

        void begin_root_field(const std::vector<std::vector<std::pair<char, std::string>>> &paths) {
			// iterate over the paths that were provided
            for(const auto &[i, path] : iter::enumerate(paths)) {
                std::string parent_type{};
				std::vector<std::string> label_path{};
                std::vector<Label> labels_in_leaf{};
                // prepare vector representing the json path
                // store the positions of the vector in which each label appears
                for(auto iter = path.begin(); iter != path.end(); iter++) {
					auto label = iter->first;
					labels_in_leaf.push_back(label);
					auto field_name = iter->second;
					label_path.push_back(field_name);
					parent_type = AtomicGraphqlSchema::getInstance().getFieldType(field_name, parent_type);
					if(iter+1 != path.end()) {
                        if(not label_paths.contains(label))
                            label_paths[label] = label_path;
						auto &child_field = (iter+1)->second;
						if(std::find(label_children[label].begin(), label_children[label].end(), child_field)
							== label_children[label].end()) {
							label_children[label].push_back(child_field);
							label_children_types[label].push_back(
									AtomicGraphqlSchema::getInstance().fieldIsList(child_field, parent_type));
						}
					}
					else {
						leaf_paths.emplace_back(std::move(label_path));
						labels_in_leaves.emplace_back(std::move(labels_in_leaf));
					}
				}
            }
            auto root_field_name = label_paths.begin()->second.front();
            if(root_field->IsNull())
                root_field->SetObject();
            rapidjson::Value root_field_value(root_field_name.c_str(), json_response.GetAllocator());
            if(tentris::store::AtomicGraphqlSchema::getInstance().fieldIsList(root_field_name))
                root_field->AddMember(root_field_value, rapidjson::kArrayType, json_response.GetAllocator());
            else
                root_field->AddMember(root_field_value, rapidjson::kNullType, json_response.GetAllocator());
        }

		void end_root_field() {
			last_mapping = nullptr;
			label_children_types.clear();
			label_paths.clear();
			label_children.clear();
			leaf_paths.clear();
		}

        [[nodiscard]] inline std::string to_string() {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            json_response.Accept(writer);
            return std::string(buffer.GetString(), buffer.GetSize());
        }

        void add(const Entry& entry, const std::map<Label, key_part_type> *mapping) {
			// update inner fields
			for(auto iter = mapping->cbegin(); iter != mapping->cend(); iter++) {
				if(not last_mapping or iter->second != (*last_mapping)[iter->first]) {
					for( ; iter != mapping->cend(); iter++)
						if(label_children.contains(iter->first)) // skip leaf fields
					        init_field(label_paths[iter->first], label_children[iter->first], label_children_types[iter->first]);
					break;
				}
			}
			// set value for leaf field
			for(const auto &[pos, key_part] : iter::enumerate(entry.key)) {
				if(key_part) {
					set_value(leaf_paths[pos], key_part);
				}
			}
            last_mapping = std::make_unique<std::map<Label, key_part_type>>(*mapping);
		}

	private:

		void init_field(const std::vector<std::string> &path,
						const std::vector<std::string> &fields,
						const std::vector<bool> &field_list_type) {
            rapidjson::GenericValue<rapidjson::UTF8<char>> *sub_tree = root_field;
			for(auto &path_part : path) {
                if(sub_tree->IsArray())
                    sub_tree = &(*sub_tree)[sub_tree->Size()-1];
				sub_tree = &(*sub_tree)[path_part];
			}
			rapidjson::Value new_object(rapidjson::kObjectType);
			for(const auto &[pos, field] : iter::enumerate(fields)) {
                rapidjson::Value key(field.c_str(), json_response.GetAllocator());
				if(field_list_type[pos])
                    new_object.AddMember(key, rapidjson::kArrayType, json_response.GetAllocator());
				else
                    new_object.AddMember(key, rapidjson::kNullType, json_response.GetAllocator());
			}
			if(sub_tree->IsArray())
				sub_tree->PushBack(new_object.Move(), json_response.GetAllocator());
			else
				*sub_tree = new_object.Move();
		}

		void set_value(const std::vector<std::string> &path,
					   key_part_type value) {
            rapidjson::GenericValue<rapidjson::UTF8<char>> *sub_tree = root_field;
            for(auto &path_part : path) {
                if(sub_tree->IsArray())
                    sub_tree = &(*sub_tree)[sub_tree->Size()-1];
				sub_tree = &(*sub_tree)[path_part];
            }
            rapidjson::Value new_object(std::string(value->value()), json_response.GetAllocator());
            if(sub_tree->IsArray())
                sub_tree->PushBack(new_object.Move(), json_response.GetAllocator());
			else
				*sub_tree = new_object.Move();
 		}

	};

}

#endif//TENTRIS_GRAPHQLRESPONSEDOM_HPP
