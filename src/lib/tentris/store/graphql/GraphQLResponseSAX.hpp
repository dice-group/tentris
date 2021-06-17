#ifndef TENTRIS_GRAPHQLRESPONSESAX_HPP
#define TENTRIS_GRAPHQLRESPONSESAX_HPP

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "tentris/store/AtomicTripleStore.hpp"
#include "tentris/store/graphql/internal/GraphQLDataStructures.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store::graphql {

	class GraphQLResponseSAX {

		using Entry = ::tentris::tensor::EinsumEntry<::tentris::tensor::COUNTED_t>;
		using Key = typename Entry::Key;
		using Label = ::tentris::tensor::Subscript::Label;
		using key_part_type = ::tentris::tensor::key_part_type;

		struct ErrorMessage {
			std::string message{};
			std::vector<std::variant<std::string, uint32_t>> path{};
		};

		size_t buffer_size;
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer;

		std::map<Label, Label> label_last_child{};   // for each label stores its last child
		std::map<Label, Label> label_last_neighbor{};// for each label stores the last label in the same level
		std::map<Label, bool> resolved{};
		std::map<Label, Label> label_parent{};
		std::map<Label, std::uint32_t> array_counters{};
		std::set<Label> end_labels{};
		std::vector<std::size_t> leaf_positions{};
		std::map<Label, std::size_t> label_positions{};
		std::unique_ptr<Entry> last_entry = nullptr;
		const Entry *current_entry = nullptr;
		std::vector<ErrorMessage> errors{};

		const graphql::internal::ParsedSubGraphQL *sub_query = nullptr;

		bool has_data = false;

	public:
		inline explicit GraphQLResponseSAX(size_t buffer_size)
			: buffer_size(buffer_size),
			  buffer(nullptr, size_t(buffer_size * 1.3)),
			  writer(buffer) {
			writer.StartObject();
		}

		void add(const Entry &entry);

		void begin_root_field(const graphql::internal::ParsedSubGraphQL *sub_query);

		void end_root_field();

		// closes data object and writes errors if there are any
		void close();

		[[nodiscard]] inline bool full() const {
			return buffer.GetSize() > this->buffer_size;
		};

		inline void clear() {
			this->buffer.Clear();
		}

		inline std::string_view string_view() {
			writer.Flush();
			return std::string_view(buffer.GetString(), buffer.GetSize());
		}

	private:
		/*
         * Closes the object corresponding to the field of the provided label
         * If the field is resolved, it makes sure its children field are closed
         * If the field is not resolved, it it makes sure that the previous fields of the level are closed
         * For fields that are not resolved it generates null / empty list values
         */
		void close_field(Label label);

		/*
        * Opens the object corresponding to the field of the provided label
        * Constructs the object of the parent field, if it is not resolved
        */
		void open_field(Label label);

		void write_leaf(Label leaf_label, key_part_type key_part);

		[[nodiscard]] inline bool is_leaf(Label label) {
			return (not label_last_child.contains(label));
		}

		inline void non_null_error(Label label) {
			ErrorMessage error{};
			error.message = fmt::format("Null value in non-nullable field: {}", sub_query->field_names.at(label));
			error.path.emplace_back(sub_query->field_names.at(label));
			if (array_counters.contains(label))
				error.path.emplace_back(array_counters[label]);
			auto parent_label = get_parent_label(label);
			while (parent_label) {
				error.path.insert(error.path.begin(), sub_query->field_names.at(parent_label));
				if (array_counters.contains(parent_label))
					error.path.insert(error.path.begin() + 1, array_counters[parent_label]);
				parent_label = get_parent_label(parent_label);
			}
			errors.push_back(std::move(error));
		}

		[[nodiscard]] inline Label get_parent_label(Label label) {
			if (label_parent.contains(label))
				return label_parent[label];
			else if (label_last_neighbor.contains(label)) {
				auto neighbor = label_last_neighbor[label];
				while (not label_parent.contains(neighbor))
					neighbor = label_last_neighbor[neighbor];
				return label_parent[neighbor];
			} else
				return 0;
		}
	};

}// namespace tentris::store::graphql

#endif//TENTRIS_GRAPHQLRESPONSESAX_HPP
