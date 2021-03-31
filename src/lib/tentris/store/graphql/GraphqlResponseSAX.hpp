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
        using Entry = ::tentris::tensor::EinsumEntry<::tentris::tensor::COUNTED_t>;
        using Key = typename Entry::Key;
        using Label = ::tentris::tensor::Subscript::Label;
        using key_part_type = ::tentris::tensor::key_part_type;

        size_t buffer_size;
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer;

        boost::container::flat_map<Label, std::string> label_to_field{};
        boost::container::flat_map<Label, bool> label_is_list{};
        boost::container::flat_map<Label, Label> label_last_child{}; // for each label stores its last child
        boost::container::flat_map<Label, Label> label_last_neighbor{}; // for each label stores the last label in the same level
        boost::container::flat_map<Label, bool> resolved{};
        boost::container::flat_map<Label, Label> label_parent{};
        std::set<Label> end_labels{};
        std::vector<std::vector<Label>> labels_in_entry{};
		std::vector<std::size_t> leaf_positions{};
        std::unique_ptr<Entry> last_entry = nullptr;

        bool has_data = false;

    public:

        explicit GraphqlResponseSAX(size_t buffer_size)
            : buffer_size(buffer_size),
              buffer(nullptr, size_t(buffer_size * 1.3)),
              writer(buffer) {
			writer.StartObject();
		}

        void add(const Entry& entry) {
            if (last_entry) {
                Label updated_label;
                for (const auto &[pos, key_part] : iter::enumerate(entry.key)) {
                    if (key_part != last_entry->key[pos]) {
                        // in case there are two labels for a position, take the first
                        updated_label =  labels_in_entry[pos].front();
                        break;
                    }
                }
                if (not is_leaf(updated_label)) {
                    // close the children of the field corresponding to the first updated label
                    close_field(label_last_child[updated_label]);
                    // update the resolved map
                    auto resolved_iter = resolved.find(updated_label);
                    resolved_iter++;
                    for (; resolved_iter != resolved.end(); resolved_iter++)
                        resolved_iter->second = false;
                }
            }
            bool empty_entry = true;
            for (auto leaf_pos : leaf_positions) {
                if (not entry.key[leaf_pos])
                    continue;
                empty_entry = false;
                write_leaf(labels_in_entry[leaf_pos].back(), entry.key[leaf_pos]);
            }
            // no results at leaf fields but we still have new values in the inner fields
            if (empty_entry) {
                // find the last label that was assigned a value in the mapping
                char last_label{};
                for (const auto &[pos, key_part] : iter::enumerate(entry.key))
                    if(key_part)
                        last_label = labels_in_entry[pos].front();
				if (not resolved[last_label])
                    open_field(last_label);
            }
            last_entry = std::make_unique<Entry>(entry);
        }

        void begin_root_field(const std::vector<std::vector<std::pair<char, std::string>>> &paths) {
            // first root field -> create data object
            if (not has_data) {
                has_data = true;
                writer.Key("data");
				writer.StartObject();
            }
			std::size_t pos = 0;
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
                        if (AtomicGraphqlSchema::getInstance().fieldIsScalar(field_name, parent_type)) {
                            if (AtomicGraphqlSchema::getInstance().getFieldType(field_name, parent_type) == "ID") {
								labels_in_entry.back().push_back(label);
								leaf_positions.push_back(pos-1);
							}
							else {
								labels_in_entry.emplace_back(std::vector<Label>{label});
                                leaf_positions.push_back(pos);
								pos++;
							}
                        }
						else {
							labels_in_entry.emplace_back(std::vector<Label>{label});
							pos++;
						}
                        end_labels.insert(label);
                        resolved[label] = false;
                    }
                    parent_type = AtomicGraphqlSchema::getInstance().getFieldType(field_name, parent_type);
                    parent_label = label;
                }
            }
        }

        void end_root_field() {
            close_field(label_to_field.begin()->first);
            label_to_field.clear();
            label_is_list.clear();
            label_last_neighbor.clear();
            label_last_child.clear();
        }

        // closes data object and writes errors if there are any
        void close() {
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

        /*
         * Closes the object corresponding to the field of the provided label
         * If the field is resolved, it recursively ensures that its children field are closed
         * If the field is not resolved, it recursively ensures that the previous fields of the level are closed
         * For fields that are not resolved it generates null / empty list values
         */
        void close_field(Label label) {
            // the label is resolved
            if(resolved[label]) {
                // close its children if there are any
                if(label_last_child.contains(label))
                    close_field(label_last_child[label]);
                // if the field is an array, close it
                if(label_is_list[label])
                    writer.EndArray();
            }
                // the label is not resolved
            else {
                resolved[label] = true;
				// if is the first child start the parent object
				if (label_parent.contains(label))
					writer.StartObject();
                // close its neighbor, if there is one
                else if (label_last_neighbor.contains(label))
                    close_field(label_last_neighbor[label]);
                // create default value (empty list / null)
                writer.Key(label_to_field[label]);
                if (label_is_list[label]) {
                    writer.StartArray();
                    writer.EndArray();
                }
                else // todo: nullability check
                    writer.Null();
            }
            // if it the last inner field of a parent field, close the object of the parent field
            if(end_labels.contains(label))
                writer.EndObject();
        }

        /*
         * Opens the object corresponding to the field of the provided label
         * Constructs the object of the parent field, if it is not resolved
         */
        void open_field(Label label) {
            if (label_parent.contains(label)) {
				// create object for parent if it is not resolved
				if (not resolved[label_parent[label]])
					open_field(label_parent[label]);
				// start parent object
                writer.StartObject();
			}
            else if (label_last_neighbor.contains(label))
                close_field(label_last_neighbor[label]);
            resolved[label] = true;
            writer.Key(label_to_field[label]);
            if(label_is_list[label])
                writer.StartArray();
        }

        void write_leaf(Label leaf_label, key_part_type key_part) {
            // if the leaf is already resolved then it should be a list type
            if (resolved[leaf_label]) {
                // todo: need to take care of IDs and error handling for actual lists
                if (label_is_list[leaf_label])
                    writer.String(key_part->value().data(), key_part->value().size());
                return;
            }
            open_field(leaf_label);
            // todo: add cases for other scalars -- need to keep track of label type in the graphql schema
            writer.String(key_part->value().data(), key_part->value().size());
        }

        [[nodiscard]] bool is_leaf(Label label) {
            return (not label_last_child.contains(label));
        }

    };

}

#endif//TENTRIS_GRAPHQLRESPONSESAX_HPP
