#ifndef TENTRIS_SPARQLJSONRESULTSAXWRITER_HPP
#define TENTRIS_SPARQLJSONRESULTSAXWRITER_HPP

#include <utility>

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cppitertools/itertools.hpp>
#include <rdf4cpp/rdf.hpp>

#include <dice/rdf-tensor/Query.hpp>

namespace dice::endpoint {

	class SparqlJsonResultSAXWriter {
		using Node = rdf4cpp::rdf::Node;
		using Literal = rdf4cpp::rdf::Literal;
		using IRI = rdf4cpp::rdf::IRI;
		using BlankNode = rdf4cpp::rdf::BlankNode;
		using Variable = rdf4cpp::rdf::query::Variable;
		using Entry = dice::rdf_tensor::Entry;

		std::size_t number_of_solutions_ = 0;
		std::size_t number_of_bindings_ = 0;

		size_t buffer_size;
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer;

		std::vector<std::string> variables_;

		inline static auto to_rapidjson(std::string_view view) {
			return rapidjson::GenericStringRef<char>(view.data() ? view.data() : "", view.size());
		}
	public:
		explicit SparqlJsonResultSAXWriter(const std::vector<Variable>& variables, size_t buffer_size)
			: buffer_size(buffer_size),
			  buffer(nullptr, size_t(buffer_size * 1.3)),
			  writer(buffer) {
			writer.StartObject();
			writer.Key("head");
			for (auto const &var : variables) {
				variables_.emplace_back(var.name());
			}
			{
				writer.StartObject();
				writer.Key("vars");
				{
					writer.StartArray();
					for (const auto &var : variables_)
						writer.String(to_rapidjson(var));
					writer.EndArray();
				}
				writer.EndObject();
			}
			writer.Key("results");
			writer.StartObject();
			writer.Key("bindings");
			writer.StartArray();
		}

		void close() {
			writer.EndArray();
			writer.EndObject();
			writer.EndObject();
		}

		void add(Entry const &entry) {

			for (size_t i = 0; i < size_t(entry.value()); ++i) {
				writer.StartObject();
				for (const auto &[term, var] : iter::zip(entry.key(), variables_)) {
					if (term.null())
						continue;
					writer.Key(to_rapidjson(var));
					writer.StartObject();
					writer.Key("type");
					if (term.is_iri()) {
						writer.String("uri");
						writer.Key("value");
						auto const &identifier = ((IRI) term).identifier();
						writer.String(identifier.data(), identifier.size());
					} else if (term.is_literal()) {
						writer.String("literal");

						auto literal = (Literal) term;

						static const IRI xsd_str{"http://www.w3.org/2001/XMLSchema#string"};
						auto datatype = literal.datatype();
						if (datatype != xsd_str) {
							auto const &lang = literal.language_tag();
							if (not lang.empty()) {
								writer.Key("xml:lang");
								writer.String(lang.data(), lang.size());
							} else {
								writer.Key("datatype");
								writer.String(datatype.identifier().data(), datatype.identifier().size());
							}
						}
						writer.Key("value");
						writer.String(to_rapidjson(literal.lexical_form()));

					} else if (term.is_blank_node()) {
						writer.String("bnode");
						writer.Key("value");
						auto const &identifier = ((BlankNode) term).identifier();
						writer.String(identifier.data(), identifier.size());
					} else {
						throw std::runtime_error("Node with incorrect type (none of Literal, BNode, URI) detected.");
					}

					writer.EndObject();
					number_of_bindings_++;
				}
				writer.EndObject();
			}

			number_of_solutions_ += entry.value();
		}

		[[nodiscard]] std::size_t size() const {
			return buffer.GetSize();
		}

		[[nodiscard]] std::size_t number_of_written_solutions() const {
			return number_of_solutions_;
		}

		[[nodiscard]] std::size_t number_of_written_bindings() const {
			return number_of_bindings_;
		}

		[[nodiscard]] bool full() const {
			return buffer.GetSize() > this->buffer_size;
		};

		std::string_view string_view() {
			writer.Flush();
			return {buffer.GetString(), buffer.GetSize()};
		}

		void clear() {
			this->buffer.Clear();
		}
	};
}// namespace dice::endpoint

#endif//TENTRIS_SPARQLJSONRESULTSAXWRITER_HPP
