#include "GraphQLResponseSAX.hpp"

namespace tentris::store::graphql {

	void GraphQLResponseSAX::add(const Entry &entry) {
		if (last_entry) {
			Label updated_label;
			key_part_type updated_key_part;
			for (const auto &[pos, key_part] : iter::enumerate(entry.key)) {
				if (key_part != last_entry->key[pos]) {
					// in case there are two labels for a position, take the first
					updated_label = labels_in_entry[pos].front();
					updated_key_part = key_part;
					break;
				}
			}
			if (not is_leaf(updated_label) and updated_key_part) {
				// close the children of the field corresponding to the first updated label
				close_field(label_last_child[updated_label]);
				// update the resolved map the updated_key part is not null
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
				if (key_part)
					last_label = labels_in_entry[pos].front();
			if (not resolved[last_label])
				open_field(last_label);
		}
		last_entry = std::make_unique<Entry>(entry);
	}

	void GraphQLResponseSAX::begin_root_field(const std::vector<std::vector<std::pair<char, std::string>>> &paths,
											  const std::set<char> &frag_labels) {
		// first root field -> create data object
		if (not has_data) {
			has_data = true;
			writer.Key("data");
			writer.StartObject();
		}
		fragment_labels = frag_labels;
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
					label_is_non_null[label] = AtomicGraphqlSchema::getInstance().fieldIsNonNull(field_name, parent_type);
					// set dependencies - if a label has already a dependency push it to the next label
					if (parent_label) {
						if (label_last_child.contains(parent_label)) {
							label_last_neighbor[label] = label_last_child[parent_label];
							end_labels.erase(label_last_child[parent_label]);
						} else
							label_parent[label] = parent_label;
						label_last_child[parent_label] = label;
					}
					if (AtomicGraphqlSchema::getInstance().fieldIsScalar(field_name, parent_type)) {
						if (AtomicGraphqlSchema::getInstance().getFieldType(field_name, parent_type) == "ID") {
							labels_in_entry.back().push_back(label);
							leaf_positions.push_back(pos - 1);
						} else {
							labels_in_entry.emplace_back(std::vector<Label>{label});
							leaf_positions.push_back(pos);
							pos++;
						}
					} else {
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

	void GraphQLResponseSAX::end_root_field() {
		close_field(label_to_field.begin()->first);
		label_to_field.clear();
		label_is_list.clear();
		label_last_neighbor.clear();
		label_last_child.clear();
		fragment_labels.clear();
	}

	// closes data object and writes errors if there are any
	void GraphQLResponseSAX::close() {
		if (not errors.empty()) {
			writer.Key("errors");
			writer.StartArray();
			for (const auto &error : errors) {
				writer.StartObject();
				writer.Key("message");
				writer.String(error.message);
				writer.Key("path");
				writer.StartArray();
				for (const auto &part : error.path) {
					if (std::holds_alternative<std::string>(part))
						writer.String(std::get<0>(part));
					else
						writer.Int(std::get<1>(part));
				}
				writer.EndArray();
				writer.EndObject();
			}
			writer.EndArray();
		}
		writer.EndObject();
	}

	void GraphQLResponseSAX::close_field(Label label) {
		// the label is resolved
		if (resolved[label]) {
			// close its children if there are any
			if (label_last_child.contains(label))
				close_field(label_last_child[label]);
			// if the field is an array, close it
			if (label_is_list[label])
				writer.EndArray();
		}
		// the label is not resolved
		else {
			resolved[label] = true;
			// if it is the first child start the parent object
			if (label_parent.contains(label)) {
				if (not resolved[label_parent[label]])
					open_field(label_parent[label]);
				writer.StartObject();
			}
			// close its neighbor, if there is one
			else if (label_last_neighbor.contains(label))
				close_field(label_last_neighbor[label]);
			// if the field corresponding to the label does not appear in a fragment create default value (empty list / null)
			if (not fragment_labels.contains(label)) {
				writer.Key(label_to_field[label]);
				if (label_is_list[label]) {
					writer.StartArray();
					writer.EndArray();
				} else {
					if (label_is_non_null[label])
						non_null_error(label);
					writer.Null();
				}
			}
		}
		// if it the last inner field of a parent field, close the object of the parent field
		if (end_labels.contains(label))
			writer.EndObject();
	}

	void GraphQLResponseSAX::open_field(Label label) {
		if (label_parent.contains(label)) {
			// create object for parent if it is not resolved
			if (not resolved[label_parent[label]])
				open_field(label_parent[label]);
			else
				array_counters[label_parent[label]]++;
			// start parent object
			writer.StartObject();
		} else if (label_last_neighbor.contains(label))
			close_field(label_last_neighbor[label]);
		resolved[label] = true;
		writer.Key(label_to_field[label]);
		if (label_is_list[label]) {
			writer.StartArray();
			array_counters[label] = 0;
		}
	}

	void GraphQLResponseSAX::write_leaf(Label leaf_label, key_part_type key_part) {
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

}// namespace tentris::store::graphql