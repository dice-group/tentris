#ifndef TENTRIS_GRAPHQLRESPONSE_HPP
#define TENTRIS_GRAPHQLRESPONSE_HPP

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/ostreamwrapper.h>

#include "tentris/store/graphql/GraphqlSchema.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store::graphql {

	class GraphqlResponse {

	private:

        using Entry = ::tentris::tensor::EinsumEntry<::tentris::tensor::COUNTED_t>;
        using Key = typename Entry::Key;
		using Label = ::tentris::tensor::Subscript::Label;
		using key_part_type = ::tentris::tensor::key_part_type;
		using SetFuncion = std::function<void(rapidjson::Pointer&, key_part_type, rapidjson::Document&)>;
		using JSONPath = std::vector<std::variant<std::string, std::size_t>>;

		// the json response
        rapidjson::Document response{};

		// helper fields
		// used to serialize the response
        rapidjson::StringBuffer buffer;
		// used to serialze the response
        rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
		// list containing the paths to leaf fields of the response -- used to construct json pointers
		// one path per einsum result label
		std::vector<JSONPath> paths_to_leaves{};
		// list of set functions -- one set function per einsum result label
		std::vector<SetFuncion> set_functions{};
		// stores the last mapping of the einsum -- used to updated the array counters
		std::unique_ptr<std::map<Label, key_part_type>> last_mapping = nullptr;
		// for each label stores its position in all leaf paths
		std::map<Label, std::pair<std::uint8_t, std::vector<std::uint8_t>>> label_positions{};
		// stores for each leaf field if it is nullable
		std::vector<bool> non_null{};
		std::uint32_t error_count = 0;

	public:

		explicit GraphqlResponse(const std::vector<std::vector<std::pair<char, std::string>>> &paths,
								 const GraphqlSchema &schema) {
			// iterate over the paths that were provided
			for(const auto &[i, path] : iter::enumerate(paths)) {
                JSONPath leaf_path{};
				std::string parent_type{};
                // iterate over the parts of the path
				// prepare vector representing the json path
				// store the positions of the vector in which each label appears
				for(const auto &path_part : path) {
					auto label = path_part.first;
					auto field_name = path_part.second;
					leaf_path.push_back(field_name);
					// found list type -- add array counter
					if(schema.fieldIsList(field_name, parent_type))
						leaf_path.emplace_back(std::size_t{0});
					// store the position of the label in the path
                    if(not label_positions.contains(label))
                        label_positions[label].first = leaf_path.size()-1;
                    label_positions[label].second.emplace_back(i);
					// leaf field -- need to assign set function and check nullability
					bool scalar = false;
                    if(schema.getFieldType(field_name, parent_type) == "String" or schema.getFieldType(field_name, parent_type) == "ID") {
						set_functions.emplace_back(GraphqlResponse::set_string);
						scalar = true;
					}
                    else if(schema.getFieldType(field_name, parent_type) == "Int") {
                        set_functions.emplace_back(GraphqlResponse::set_int);
                        scalar = true;
                    }
                    else if(schema.getFieldType(field_name, parent_type) == "Float") {
                        set_functions.emplace_back(GraphqlResponse::set_float);
                        scalar = true;
                    }
                    else if(schema.getFieldType(field_name, parent_type) == "Boolean") {
                        set_functions.emplace_back(GraphqlResponse::set_bool);
                        scalar = true;
                    }
					if(scalar)
						non_null.emplace_back(schema.fieldIsNonNull(field_name, parent_type));
					else
                        parent_type = schema.getFieldType(field_name, parent_type);
				}paths_to_leaves.emplace_back(std::move(leaf_path));
            }
		}

		[[nodiscard]] std::string_view string_view() {
			// validate the last added entries before printing
//			for(auto i : iter::range(paths_to_leaves.size()))
//				validate_nullability(i);
			buffer.Clear();
            response.Accept(writer);
			buffer.Flush();
            return std::string_view(buffer.GetString(), buffer.GetSize());
        }

		/*
		 * Adds a new result to the json response.
		 * It is also responsible for capturing errors (e.g., null values in non-null fields).
		 * @param entry: the result generated by einsum
		 * @param mapping: the complete mapping generated by einsum
		 */
		void add(const Entry& entry, const std::map<Label, key_part_type> *mapping) {
            if(last_mapping) {
				try {
					update_counters(mapping);
				} catch (...) {
                    return;
				}
			}
			for(const auto &[i, entry_part] : iter::enumerate(entry.key)) {
				if(not entry_part)
                    explore_inner_fields(i, mapping);
				else {
					auto pointer = rapidjson::Pointer(construct_path(paths_to_leaves[i]));
					set_functions[i](pointer, entry_part, response);
				}
			}
            last_mapping = std::make_unique<std::map<Label, key_part_type>>(*mapping);
		}

	private:

		// set functions -- one for each build-in graphql scalar type

		static void set_string(rapidjson::Pointer &pointer,
							   ::tentris::tensor::key_part_type key_part,
                               rapidjson::Document &response) {
            pointer.Create(response).SetString(key_part->value().data(), key_part->value().size());
		}

		static void set_int(rapidjson::Pointer &pointer,
                            ::tentris::tensor::key_part_type key_part,
                            rapidjson::Document &response) {
			pointer.Create(response).SetInt(std::stoi(key_part->value().data()));
		}

		static void set_float(rapidjson::Pointer &pointer,
                              ::tentris::tensor::key_part_type key_part,
                              rapidjson::Document &response) {
			pointer.Create(response).SetFloat(std::stof(key_part->value().data()));
		}

        static void set_bool(rapidjson::Pointer &pointer,
                              ::tentris::tensor::key_part_type key_part,
                              rapidjson::Document &response) {
            if(strcmp(key_part->value().data(), "true") == 0)
                pointer.Create(response).SetBool(true);
            else
                pointer.Create(response).SetBool(false);
        }

        /*
         * Updates the JSON arrays using the generated mapping (not only the result labels).
         * It is also responsible for identifying array errors (e.g., a non-array field generates multiple values).
         * @param mapping: the complete generated mapping
         */
        void update_counters(const std::map<Label, key_part_type> *mapping) {
			Label updated_label = 0;
            // find the first label that returned a different value
			for(const auto &[label, value] : *mapping) {
				if(last_mapping->contains(label) and value != (*last_mapping)[label]) {
                    updated_label = label;
					break;
				}
			}
			if(not updated_label)
				return;
			std::uint8_t array_pos = label_positions.at(updated_label).first;
			for(auto &lp_idx : label_positions[updated_label].second) {
				// check for null errors before updating
//				validate_nullability(lp_idx);
                // update the array index
				try {
					std::get<1>(paths_to_leaves[lp_idx][array_pos])++;
				} catch(...) {
                    rapidjson::Pointer("/error/"+std::to_string(error_count)+"/message")
                            .Create(response).SetString("Multiple values at non-list field", response.GetAllocator());
                    error_count++;
                    throw std::exception();
				}
				// reset subsequent array counters
				for(std::size_t i = array_pos+1; i < paths_to_leaves[lp_idx].size(); i++)
					if(std::holds_alternative<std::size_t>(paths_to_leaves[lp_idx][i]))
						std::get<1>(paths_to_leaves[lp_idx][i]) = 0;
			}
		}

		/*
		 * Looks into the results of the inner fields of the response
		 * It is called if a leaf field does not yield any resutls
		 */
		void explore_inner_fields(std::size_t leaf_path_idx, const std::map<Label, key_part_type> *mapping) {
			const auto &leaf_path = paths_to_leaves[leaf_path_idx];
			// find the most deep label whose value changed
			std::int32_t max_pos = -1;
			for(const auto &[label, positions] : label_positions) {
				if(std::find(positions.second.begin(), positions.second.end(), leaf_path_idx) == positions.second.end())
					continue;
				if(not mapping->contains(label) or not mapping->at(label))
					continue;
				if(last_mapping and mapping->at(label) == (*last_mapping)[label])
					continue;
				if(positions.first > max_pos)
					max_pos = positions.first;
			}
			if(max_pos < 0)
				return;
			// create default value for the next field in the response (null or empty list)
			JSONPath inner_path(leaf_path.begin(), leaf_path.begin()+max_pos+2);
			// check if the next field is list
			if (leaf_path.begin()+max_pos+2 != leaf_path.end() and not
				std::holds_alternative<std::string>(leaf_path[max_pos+2]))
				rapidjson::Pointer(construct_path(inner_path)).Create(response).SetArray();
			else
                rapidjson::Pointer(construct_path(inner_path)).Create(response);
		}

		static std::string construct_path(const JSONPath &vec_path) {
			std::string path{"/data"};
            for(auto token : vec_path) {
                if(std::holds_alternative<std::size_t>(token))
                    path += "/"+std::to_string(std::get<1>(token));
                else
                    path += "/"+std::get<0>(token);
            }
			return path;
		}

		void validate_nullability(std::uint8_t lp_idx) {
            if(non_null[lp_idx] and rapidjson::Pointer(construct_path(paths_to_leaves[lp_idx])).Get(response)->IsNull()) {
                rapidjson::Pointer("/error/" + std::to_string(error_count) + "/message")
                        .Create(response)
                        .SetString("Null value in non-null field", response.GetAllocator());
                for(const auto &[pos, path_part] : iter::enumerate(paths_to_leaves[lp_idx])) {
                    if(std::holds_alternative<std::string>(path_part))
                        rapidjson::Pointer("/error/" + std::to_string(error_count) + "/path/" + std::to_string(pos))
                                .Create(response)
                                .SetString(std::get<0>(path_part), response.GetAllocator());
                    else
                        rapidjson::Pointer("/error/" + std::to_string(error_count) + "/path/" + std::to_string(pos))
                                .Create(response)
                                .SetInt(std::get<1>(path_part));
                }
                error_count++;
            }
		}

	};

}

#endif//TENTRIS_GRAPHQLRESPONSE_HPP
