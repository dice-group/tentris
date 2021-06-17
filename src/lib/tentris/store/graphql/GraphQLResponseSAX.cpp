#include "GraphQLResponseSAX.hpp"

namespace tentris::store::graphql {

	void GraphQLResponseSAX::add(const Entry &entry) {
		current_entry = &entry;
		if (last_entry) {
			Label updated_label;
			key_part_type updated_key_part;
			for (const auto &[pos, key_part] : iter::enumerate(entry.key)) {
				if (key_part != last_entry->key[pos]) {
					// in case there are two labels for a position, take the first
					updated_label = sub_query->result_labels[pos];
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
			write_leaf(sub_query->result_labels[leaf_pos], entry.key[leaf_pos]);
		}
		// no results at leaf fields but we still have new values in the inner fields
		if (empty_entry) {
			// find the last label that was assigned a value in the mapping
			char last_label{};
			for (const auto &[pos, key_part] : iter::enumerate(entry.key))
				if (key_part)
					last_label = sub_query->result_labels[pos];
			if (not resolved[last_label])
				open_field(last_label);
		}
		last_entry = std::make_unique<Entry>(entry);
	}

	void GraphQLResponseSAX::begin_root_field(const graphql::internal::ParsedSubGraphQL *sq) {
		// first root field -> create data object
		sub_query = sq;
		if (not has_data) {
			has_data = true;
			writer.Key("data");
			writer.StartObject();
		}
		std::size_t pos = 0;
		// iterate over the paths that were provided and gather info
		for (const auto &path : sub_query->paths) {
			Label parent_label{};
			for (const auto &label : path) {
				if (not resolved.contains(label)) {
					label_positions[label] = pos;
					// set dependencies - if a label has already a dependency push it to the next label
					if (parent_label) {
						if (label_last_child.contains(parent_label)) {
							label_last_neighbor[label] = label_last_child[parent_label];
							end_labels.erase(label_last_child[parent_label]);
						} else
							label_parent[label] = parent_label;
						label_last_child[parent_label] = label;
					}
					if (sub_query->leaf_types.contains(label))
						leaf_positions.emplace_back(pos);
					pos++;
					// do not add the label of the root field to the end labels
					if (parent_label)
						end_labels.insert(label);
					resolved[label] = false;
				}
				parent_label = label;
			}
		}
	}

	void GraphQLResponseSAX::end_root_field() {
		// close root field
		close_field(sub_query->paths.begin()->front());
		label_last_neighbor.clear();
		label_last_child.clear();
		end_labels.clear();
		label_parent.clear();
		leaf_positions.clear();
		array_counters.clear();
		resolved.clear();
		last_entry = nullptr;
		sub_query = nullptr;
	}

	// closes data object and writes errors if there are any
	void GraphQLResponseSAX::close() {
		// close data object
		writer.EndObject();
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
						writer.Uint(std::get<1>(part));
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
			if (sub_query->list_labels.contains(label))
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
			// if the field corresponding to the label is in an inline framgent do not print the field
			if (not sub_query->fragment_dependencies.contains(label) or
				AtomicTripleStore::getInstance().typeCondition(current_entry->key[sub_query->fragment_dependencies.at(label).first],
															   sub_query->fragment_dependencies.at(label).second)) {
				writer.Key(sub_query->field_names.at(label));
				if (sub_query->list_labels.contains(label)) {
					writer.StartArray();
					writer.EndArray();
				} else {
					if (sub_query->non_null_labels.contains(label))
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
		writer.Key(sub_query->field_names.at(label));
		if (sub_query->list_labels.contains(label)) {
			writer.StartArray();
			array_counters[label] = 0;
		}
	}

	void GraphQLResponseSAX::write_leaf(Label leaf_label, key_part_type key_part) {
		// if the leaf is already resolved then it should be a list type
		if (resolved[leaf_label]) {
			// todo: need to take care of IDs and error handling for actual lists
			if (sub_query->list_labels.contains(leaf_label))
				writer.String(key_part->value().data(), key_part->value().size());
			return;
		}
		open_field(leaf_label);
		// todo: add cases for other scalars -- need to keep track of label type in the graphql schema
		if (sub_query->leaf_types.at(leaf_label) == "String" or sub_query->leaf_types.at(leaf_label) == "ID")
			writer.String(key_part->value().data(), key_part->value().size());
		else if (sub_query->leaf_types.at(leaf_label) == "Int")
			writer.Int(std::stoi(key_part->value().data()));
		else if (sub_query->leaf_types.at(leaf_label) == "Float")
			writer.Double(std::stof(key_part->value().data()));
		else if (sub_query->leaf_types.at(leaf_label) == "Boolean")
			writer.Bool(strncmp(key_part->value().data(), "true", key_part->value().size()) == 0);
	}

}// namespace tentris::store::graphql