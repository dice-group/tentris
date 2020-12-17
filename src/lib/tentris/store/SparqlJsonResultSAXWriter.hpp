#ifndef TENTRIS_SPARQLJSONRESULTSAXWRITER_HPP
#define TENTRIS_SPARQLJSONRESULTSAXWRITER_HPP

#include <itertools.hpp>
#include <utility>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/SPARQL/Variable.hpp"
#include "tentris/util/LogHelper.hpp"

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/ostreamwrapper.h>

#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store {

	template<typename result_type>
	class SparqlJsonResultSAXWriter {
		using Term = rdf_parser::store::rdf::Term;
		using Variable = sparql::Variable;
		using Entry = ::tentris::tensor::EinsumEntry<result_type>;
		using Key = typename Entry::Key;
		using Value = typename Entry::value_type;

		std::size_t result_count = 0;
		std::size_t term_count_ = 0;

		std::vector<Variable> variables{};

		size_t buffer_size;
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer;


	public:
		explicit SparqlJsonResultSAXWriter(std::vector<Variable> variables, size_t buffer_size)
				: variables(std::move(variables)),
				  buffer_size(buffer_size),
				  buffer(nullptr, size_t(buffer_size * 1.3)),
				  writer(buffer) {
			writer.StartObject();
			writer.Key("head");
			{
				writer.StartObject();
				writer.Key("vars");
				{
					writer.StartArray();
					for (const auto &var : this->variables)
						writer.String(var.name);
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

		void add(const Entry &entry) {

			for (long i = 0; i < long(entry.value); ++i) {
				writer.StartObject();
				for (const auto &[term, var]: iter::zip(entry.key, variables)) {
					if (term == nullptr)
						continue;
					writer.Key(var.name);
					writer.StartObject();
					writer.Key("type");
					switch (term->type()) {
						case Term::NodeType::URIRef_:
							writer.String("uri");
							break;
						case Term::NodeType::BNode_:
							writer.String("bnode");
							break;
						case Term::NodeType::Literal_:
							writer.String("literal");
							break;
						default:
							logging::log("Incomplete term with no type (Literal, BNode, URI) detected.");
							assert(false);
					}
					writer.Key("value");

					auto value = term->value();
					writer.String(value.data(), value.size());

					if (term->isLiteral()) {
						const Literal &literal_term = term->castLiteral();
						if (literal_term.hasDataType()) {
							auto data_type = literal_term.dataType();
							writer.Key("datatype");
							writer.String(data_type.data(), data_type.size());
						} else if (literal_term.hasLang()) {
							auto lang = literal_term.lang();
							writer.Key("xml:lang");
							writer.String(lang.data(), lang.size());
						}
					}
					writer.EndObject();
					term_count_++;
				}
				writer.EndObject();
			}

			result_count += entry.value;
		}

		[[nodiscard]] std::size_t size() const {
			return buffer.GetSize();
		}

		[[nodiscard]] bool full() const {
			return buffer.GetSize() > this->buffer_size;
		};

		std::string_view string_view() {
			writer.Flush();
			return std::string_view(buffer.GetString(), buffer.GetSize());
		}

		void clear() {
			this->buffer.Clear();
		}
	};
}

#endif //TENTRIS_SPARQLJSONRESULTSAXWRITER_HPP
