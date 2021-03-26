#ifndef TENTRIS_GRAPHQLRESPONSESAX_HPP
#define TENTRIS_GRAPHQLRESPONSESAX_HPP

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/ostreamwrapper.h>

#include "tentris/store/AtomicGraphqlSchema.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store::graphql {

	class GraphqlResponseSAX {

        using AtomicGraphqlSchema = tentris::store::AtomicGraphqlSchema;
        using EinsumMapping_t = tentris::tensor::EinsumMapping_t;
        using Entry = ::tentris::tensor::EinsumEntry<::tentris::tensor::COUNTED_t>;
        using Key = typename Entry::Key;
        using Label = ::tentris::tensor::Subscript::Label;
        using key_part_type = ::tentris::tensor::key_part_type;

        size_t buffer_size;
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer;

		std::map<Label, std::string> label_to_field{};
		std::map<Label, bool> label_is_list{};
		std::map<Label, Label> label_last_child{}; // for each label stores its last child
		std::map<Label, Label> label_last_neighbor{}; // for each label stores the last label in the same level
		std::map<Label, bool> resolved{};
        std::unique_ptr<EinsumMapping_t> last_mapping = nullptr;
		std::vector<Label> leaf_labels{};
		std::map<Label, Label> label_parent{};
		std::set<Label> end_labels{};

		bool has_data = false;

	public:

		explicit GraphqlResponseSAX(size_t buffer_size = 100'000'000UL)
			: buffer_size(buffer_size),
              buffer(nullptr, size_t(buffer_size * 1.3)),
              writer(buffer) {}

		void add(const Entry& entry, const EinsumMapping_t *mapping) {
			if (last_mapping) {
				for (const auto &iter : *mapping) {
					if (iter.second != (*last_mapping)[iter.first]) {
						if (is_leaf(iter.first))
							break;
						if (last_mapping)
							check_dependencies(label_last_child[iter.first]);
						auto resolved_iter = resolved.find(iter.first);
						resolved_iter++;
						for (; resolved_iter != resolved.end(); resolved_iter++)
							resolved_iter->second = false;
						break;
					}
				}
			}
			bool empty_entry = true;
			for (const auto &[pos, key_part] : iter::enumerate(entry.key)) {
				if (not key_part)
					continue;
				empty_entry = false;
				auto leaf_label = leaf_labels[pos];
				if (resolved[leaf_label]) {
                    if (not label_is_list[leaf_label]) {
                        std::cout << "multiple values at non list field" << std::endl;
                    }
					else {
                        writer.String(key_part->value().data(), key_part->value().size());
					}
					continue;
				}
				resolved[leaf_label] = true;
				if (label_parent.contains(leaf_label)) {
					if (not resolved[label_parent[leaf_label]])
                        construct_parent(label_parent[leaf_label]);
					else
                        writer.StartObject();
				}
				else
					check_dependencies(label_last_neighbor[leaf_label]);
                writer.Key(label_to_field[leaf_label]);
                writer.String(key_part->value().data(), key_part->value().size());
			}
			if (empty_entry) {
				char last_label{};
				for (auto iter = mapping->begin(); iter != mapping->end(); iter++) {
					if(iter->second)
						last_label = iter->first;
				}
                if (not resolved[last_label]) {
					if (label_parent.contains(last_label) and not resolved[label_parent[last_label]]) {
						construct_parent(label_parent[last_label]);
					}
					else {
						writer.StartObject();
					}
					writer.Key(label_to_field[last_label]);
					resolved[last_label] = true;
                    if(label_is_list[last_label])
                        writer.StartArray();
				}
                writer.StartObject();
			}
            last_mapping = std::make_unique<EinsumMapping_t>(*mapping);
		}

        void begin_root_field(const std::vector<std::vector<std::pair<char, std::string>>> &paths) {
			// first root field -> create data object
			if (not has_data) {
				has_data = true;
				writer.StartObject();
                writer.Key("data");
			}
			// iterate over the paths that were provided and gather info
			for (const auto &path : paths) {
				std::string parent_type{};
				Label parent_label{};
				for (auto iter = path.begin(); iter != path.end(); iter++) {
                    auto &label = iter->first;
                    auto &field_name = iter->second;
					if (not label_to_field.contains(label)) {
						label_to_field[label] = field_name;
						label_is_list[label] = AtomicGraphqlSchema::getInstance().fieldIsList(field_name, parent_type);
						// set dependencies - if a label has already a dependency push it to the next label
						if (parent_label) {
							if (label_last_child.contains(parent_label)) {
								label_last_neighbor[label] = label_last_child[parent_label];
								end_labels.erase(label_last_child[parent_label]);
							}
							else
								label_parent[label] = parent_label;
							label_last_child[parent_label] = label;
						}
                        end_labels.insert(label);
					}
                    parent_type = AtomicGraphqlSchema::getInstance().getFieldType(field_name, parent_type);
					parent_label = label;
					resolved[label] = false;
					// store leaf labels
					if(iter+1 == path.end())
						leaf_labels.push_back(label);
				}
			}
		}

		void end_root_field() {
			check_dependencies(label_to_field.begin()->first);
			label_to_field.clear();
			label_is_list.clear();
			label_last_neighbor.clear();
            label_last_child.clear();
		}

		// closes data object and writes errors if there are any
		void close() {
			if(has_data)
			    writer.EndObject();
		}

        [[nodiscard]] bool full() const {
            return buffer.GetSize() > this->buffer_size;
        };

        void clear() {
            this->buffer.Clear();
        }

        std::string_view string_view() {
            writer.Flush();
            return std::string_view(buffer.GetString(), buffer.GetSize());
        }

	private:

		void check_dependencies(Label dependent) {
			if(resolved[dependent]) {
				if(label_last_child.contains(dependent))
				    check_dependencies(label_last_child[dependent]);
				if(label_is_list[dependent])
					writer.EndArray();
                if(end_labels.contains(dependent))
                    writer.EndObject();
                return;
            }
			else {
				resolved[dependent] = true;
				// recursively check neighbor dependencies
				if(label_last_neighbor.contains(dependent))
                    check_dependencies(label_last_neighbor[dependent]);
				writer.Key(label_to_field[dependent]);
				if(label_is_list[dependent]) {
					writer.StartArray();
					writer.EndArray();
				}
				else
					writer.Null();
				if(end_labels.contains(dependent))
					writer.EndObject();
			}
		}

		void construct_parent(Label parent) {
            resolved[parent] = true;
			if(label_parent.contains(parent) and not resolved[label_parent[parent]])
				construct_parent(label_parent[parent]);
			else
				writer.StartObject();
            writer.Key(label_to_field[parent]);
            if(label_is_list[parent])
                writer.StartArray();
            writer.StartObject();
		}

		bool is_leaf(Label label) {
			return std::find(leaf_labels.begin(), leaf_labels.end(), label) != leaf_labels.end();
		}

	};

}

#endif//TENTRIS_GRAPHQLRESPONSESAX_HPP
