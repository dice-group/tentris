#ifndef TENTRIS_JSONQUERYRESULT_HPP
#define TENTRIS_JSONQUERYRESULT_HPP

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
	class JsonQueryResult {
        using Term = rdf_parser::store::rdf::Term;
		using Variable = sparql::Variable;
		using Entry = ::tentris::tensor::EinsumEntry<result_type>;
		using Key = typename Entry::Key;
		using Value = typename Entry::value_type;

		std::size_t result_count = 0;

		std::vector<Variable> variables{};

		rapidjson::Document json_doc;
		rapidjson::Value * bindings;

	public:
		explicit JsonQueryResult(std::vector<Variable> variables) : variables(std::move(variables)) {
			json_doc.SetObject();
			rapidjson::Value &vars = rapidjson::Pointer("/head/vars").Create(json_doc);
			vars.SetArray();
			for (const auto &var : this->variables)
				vars.PushBack(rapidjson::StringRef(var.name), json_doc.GetAllocator());
			rapidjson::Value &bindings = rapidjson::Pointer("/results/bindings").Create(json_doc);
			bindings.SetArray();
			this->bindings = &bindings;
		}

		void add(const Entry &entry) {
			rapidjson::Document::AllocatorType& allocator = json_doc.GetAllocator();

			rapidjson::Value entry_obj(rapidjson::kObjectType);
			entry_obj.SetObject();
			for(const auto &[term, var]: iter::zip(entry.key, variables)){
				if (term == nullptr)
					continue;
				rapidjson::Value term_obj(rapidjson::kObjectType);
				term_obj.SetObject();

				switch (term->type()) {
					case Term::NodeType::URIRef_:
						term_obj.AddMember("type", "uri", allocator);
						break;
					case Term::NodeType::BNode_:
						term_obj.AddMember("type", "bnode", allocator);
						break;
					case Term::NodeType::Literal_:
						term_obj.AddMember("type", "literal", allocator);
						break;
					default:
						logging::log("Incomplete term with no type (Literal, BNode, URI) detected.");
						assert(false);
				}
				auto value = term->value();
				term_obj.AddMember("value", rapidjson::StringRef(value.data(), value.size()), allocator);

				if (term->isLiteral()) {
					const Literal & literal_term = term->castLiteral();
					if (literal_term.hasDataType()){
						auto data_type = literal_term.dataType();
						term_obj.AddMember("datatype", rapidjson::StringRef(data_type.data(), data_type.size()), allocator);
					}
					else if (literal_term.hasLang()){
						auto lang = literal_term.lang();
						term_obj.AddMember("xml:lang", rapidjson::StringRef(lang.data(), lang.size()), allocator);
					}
				}
				entry_obj.AddMember(rapidjson::StringRef(var.name.data(), var.name.size()), term_obj, allocator);
			}


			for (long i = 0; i < long(entry.value) - 1L; ++i) {
				rapidjson::Value entry_obj_copy(entry_obj, allocator);
				bindings->PushBack(entry_obj_copy,allocator);
			}
			bindings->PushBack(entry_obj,allocator);
			result_count+=entry.value;
		}

		[[nodiscard]] std::string string() const {
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			json_doc.Accept(writer);
			return std::string(buffer.GetString(), buffer.GetLength());
		}

		[[nodiscard]] std::size_t size() const {
			return result_count;
		}
	};
}

#endif //TENTRIS_JSONQUERYRESULT_HPP
