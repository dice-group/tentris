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

        boost::container::flat_map<Label, std::string> label_to_field{};
        boost::container::flat_map<Label, bool> label_is_list{};
        boost::container::flat_map<Label, Label> label_last_child{}; // for each label stores its last child
        boost::container::flat_map<Label, Label> label_last_neighbor{}; // for each label stores the last label in the same level
        boost::container::flat_map<Label, bool> resolved{};
        std::unique_ptr<EinsumMapping_t> last_mapping = nullptr;
        std::vector<Label> leaf_labels{};
        boost::container::flat_map<Label, Label> label_parent{};
        std::set<Label> end_labels{};

        bool has_data = false;

    public:

        explicit GraphqlResponseSAX(size_t buffer_size = 100'000'000UL)
            : buffer_size(buffer_size),
              buffer(nullptr, size_t(buffer_size * 1.3)),
              writer(buffer) {}

        void add(const Entry& entry, const EinsumMapping_t *mapping) {
            if (last_mapping) {
                Label updated_label;
                for (const auto &iter : *mapping) {
                    if (iter.second != (*last_mapping)[iter.first]) {
                        updated_label =  iter.first;
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
            for (const auto &[pos, key_part] : iter::enumerate(entry.key)) {
                if (not key_part)
                    continue;
                empty_entry = false;
                write_leaf(leaf_labels[pos], key_part);
            }
            // no results at leaf fields but we still have new values in the inner fields
            if (empty_entry) {
                // find the last label that was assigned a value in the mapping
                char last_label{};
                for (const auto & iter : *mapping)
                    if(iter.second)
                        last_label = iter.first;
				if (not resolved[last_label])
                    open_field(last_label);
            }
            last_mapping = std::make_unique<EinsumMapping_t>(*mapping);
        }

        void begin_root_field(const std::vector<std::vector<std::pair<char, std::string>>> &paths) {
            // first root field -> create data object
            if (not has_data) {
                has_data = true;
                writer.StartObject();
                writer.Key("data");
				writer.StartObject();
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
            return std::find(leaf_labels.begin(), leaf_labels.end(), label) != leaf_labels.end();
        }

    };

}

#endif//TENTRIS_GRAPHQLRESPONSESAX_HPP
