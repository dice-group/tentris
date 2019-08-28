#ifndef TENTRIS_QUERYRESULT_HPP
#define TENTRIS_QUERYRESULT_HPP

#include <itertools.hpp>
#include <utility>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/SPARQL/Variable.hpp"
#include "tentris/util/LogHelper.hpp"

#include "tentris/tensor/BoolHypertrie.hpp"

namespace tentris::store {
	namespace {
		using namespace tentris::tensor;
		using namespace ::tentris::logging;
	}

	thread_local static const std::string json_head = R"({"head":{"vars":[)";
	thread_local static const std::string json_mid = R"(]},"results":{"bindings":[)";
	thread_local static const std::string json_tail = R"(]}}\n)";

	template<typename result_type>
	class QueryResult {
		using Term = rdf::Term;
		using Variable = sparql::Variable;
		using Entry = EinsumEntry<result_type>;
		using Key = typename Entry::key_t;
		using Value = typename Entry::value_t;

		std::size_t json_size = json_head.size() + json_mid.size() + json_tail.size();

		std::size_t result_count = 0;

		std::vector<Variable> variables{};

		struct JsonBindings {
			Value count{};
			std::string json{};
		};

		tsl::sparse_map<Key, Value> entries{};

	public:
		explicit QueryResult(std::vector<Variable> variables) : variables(std::move(variables)) {
			if (not this->variables.emtpy())
				json_size += (this->variables.size() - 1); // for commata
			json_size += (this->variables.size()) * 2; // for quotation marks
			for (const Variable &var: variables)
				json_size += var.name.size();
		}

		void add_result(Entry entry) {
			auto entry_json_size = jsonSize(entry.value);
			auto calc_size = [](auto count, auto json_binding_size) { return json_binding_size * count; };
			auto found = entries.find(entry.key);
			if (found != entries.end()) {
				JsonBindings &bindings = found.value();
				auto old_count = bindings.count;
				auto old_size = calc_size(old_count, bindings.json.size());
				auto new_count = old_count + entry.value;
				auto new_size = calc_size(new_count, bindings.json.size());
				bindings.count = new_count;
				json_size += new_size - old_size;
				result_count += new_count - old_count;
			} else {
				auto[iter, valid] = entries.emplace({entry.key, {entry.value, key2jsonStr(entry.key)}});
				JsonBindings &bindings = iter.value();
				auto size = calc_size(entry.value, bindings.json.size());
				json_size += size;
				result_count += entry.value;
			}
		}

		[[nodiscard]] std::size_t jsonSize() const {
			return json_size + (result_count - 1);
		}

	private:
		[[nodiscard]] static std::string key2jsonStr(const Key &key, const std::vector<Variable> &variables) {
			std::string json{};
			json.reserve(variables.size() * 50);
			json += "{";
			bool firstKey = true;
			for (const auto[term, var] : zip(key, variables)) {
				if (term == nullptr)
					continue;
				if (firstKey) {
					firstKey = false;
				} else {
					json += ",";
				}

				json += fmt::format(R"("{}":{)", var.name);

				const Term::NodeType termType = term->type();
				switch (termType) {
					case Term::URI:
						json += R"("type":"uri")";
						break;
					case Term::BNode:
						json += R"("type":"bnode")";
						break;
					case Term::Literal:
						json += R"("type":"literal")";
						break;
					case Term::None:
						log("Uncomplete term with no type (Literal, BNode, URI) detected.");
						assert(false);
				}

				json += R"(,"value":")" << escapeJsonString(term->value());
				if (termType == Term::Literal) {
					if (term->hasDataType())
						json += fmt::format(R"(","datatype":{}")", term->dataType());
					else if (term->hasLang())
						json += fmt::format(R"(","xml:lang":"{})", term->lang());
				}
				json += R"("})";
			}
			json += R"(})";
		};
	public:
		[[nodiscard]] std::string str() const {

			std::string result{};
			result.reserve(jsonSize());


			result += json_head;
			if (not variables.empty())
				result += fmt::format(R"("{}")", fmt::join(variables, R"(",")"));
			result += json_mid;
			bool first_bindings = true;

			for (const auto &[key, value] : entries) {
				std::string binding_str = toJson(key);
				for (auto _ : iter::range(value)) {
					if (first_bindings)
						first_bindings = false;
					else
						result += ',';
					result += binding_str;
				}
			}
			result += json_tail;
		}

		[[nodiscard]] std::size_t size() const {

		}
	};
}

#endif //TENTRIS_QUERYRESULT_HPP
