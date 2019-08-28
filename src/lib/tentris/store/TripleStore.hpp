#ifndef TENTRIS_STORE_TRIPLESTORE
#define TENTRIS_STORE_TRIPLESTORE


#include <string>
#include <optional>
#include <vector>

#include <boost/bind.hpp>
#include <boost/coroutine2/all.hpp>
#include <serd-0/serd/serd.h>
#include <absl/hash/hash.h>
#include <tsl/hopscotch_map.h>

#include <boost/algorithm/string.hpp>

#include "tentris/store/RDF/TermStore.hpp"
#include "tentris/store/SPARQL/ParsedSPARQL.hpp"
#include "tentris/store/QueryExecutionPackageCache.hpp"
#include "tentris/store/QueryExecutionPackage.hpp"
#include "tentris/util/LogHelper.hpp"
#include "tentris/tensor/BoolHypertrie.hpp"


namespace {
	using namespace tentris::store::cache;
	using namespace tentris::store::sparql;
	using namespace ::tentris::logging;
	using namespace tentris::tensor;
}

namespace tentris::store {
	class BulkLoad;

	class TripleStore {
		TermStore termIndex{};

		BoolHypertrie trie{3};

		mutable QueryExecutionPackage_cache query_cache;

	public:
		explicit TripleStore(size_t cache_capacity = 500, size_t cache_bucket_size = 500,
							 std::chrono::system_clock::duration timeout = std::chrono::seconds(180)) :
				query_cache{trie, termIndex, cache_capacity, cache_bucket_size, timeout} {}


		TermStore &getTermIndex() {
			return termIndex;
		}

		const TermStore &getTermIndex() const {
			return termIndex;
		}

		friend class BulkLoad;

		void loadRDF(const std::string &file_path);

		void add(const std::tuple<std::string, std::string, std::string> &triple) {
			add(Term::make_term(std::get<0>(triple)),
				Term::make_term(std::get<1>(triple)),
				Term::make_term(std::get<2>(triple)));
		}

		inline void
		add(Term subject, Term predicate, Term object) {
			if (subject.type() != Term::NodeType::Literal and predicate.type() == Term::NodeType::URI) {
				auto subject_id = termIndex[std::move(subject)];
				auto predicate_id = termIndex[std::move(predicate)];
				auto object_id = termIndex[std::move(object)];
				trie.set({subject_id, predicate_id, object_id}, true);
			} else
				throw std::invalid_argument{
						"Subject or predicate of the triple have a term type that is not allowed there."};
		}

		bool contains(std::tuple<std::string, std::string, std::string> triple) {
			auto subject = termIndex.find(Term::make_term(std::get<0>(triple)));
			auto predicate = termIndex.find(Term::make_term(std::get<1>(triple)));
			auto object = termIndex.find(Term::make_term(std::get<2>(triple)));
			if (subject and predicate and object) {
				Key key{subject, predicate, object};
				return trie[key];
			}
			return false;
		}

		std::shared_ptr<QueryExecutionPackage> query(const std::string &sparql) const {
			std::shared_ptr<QueryExecutionPackage> query = query_cache.get(sparql);
			// logDebug("QueryExecutionPackage: {}"_format(query));
			return query;
		}

		size_t size() const {
			return trie.size();
		}


	};

	class BulkLoad {
		using prefixes_map_type = tsl::hopscotch_map<std::string, std::string, absl::Hash<std::string>>;
		TripleStore *triple_store;
		prefixes_map_type prefixes{};
		std::size_t load_log = 1'000'000;

		BulkLoad(TripleStore &triple_store, const std::string &file_path) : triple_store(&triple_store) {
			SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) this, nullptr, &serd_base_callback,
											 &serd_prefix_callback,
											 &serd_callback,
											 nullptr);
			serd_reader_read_file(sr, (uint8_t *) (file_path.data()));
			serd_reader_free(sr);
		}

	public:
		static void load(TripleStore &triple_store, const std::string &file_path) {
			BulkLoad{triple_store, file_path};
		}

	private:

		TripleStore &store() {
			return *triple_store;
		}

		auto getBNode(const SerdNode *node) const -> Term {
			return Term::make_bnode("_:{}"_format(std::string_view{(char *) (node->buf), size_t(node->n_bytes)}));
		}

		auto getURI(const SerdNode *node) const -> Term {
			return Term::make_uriref("<{}>"_format(std::string_view{(char *) (node->buf), size_t(node->n_bytes)}));
		}

		auto getPrefixedUri(const SerdNode *node) const -> Term {
			std::string_view uri_node_view{(char *) (node->buf), size_t(node->n_bytes)};

			std::vector<std::string> prefix_and_suffix{};
			boost::split(prefix_and_suffix, uri_node_view, [](char c) { return c == ':'; });

			assert(prefix_and_suffix.size() == 2);
			assert(prefixes.count(std::string{prefix_and_suffix[0]}));
			return Term::make_uriref(
					"<{}{}>"_format(prefixes.find(prefix_and_suffix[0])->second, prefix_and_suffix[1]));
		}

		auto getLiteral(const SerdNode *literal, const SerdNode *type_node, const SerdNode *lang_node) const -> Term {
			std::string literal_value = std::string{(char *) (literal->buf), size_t(literal->n_bytes)};
			if (type_node != nullptr)
				return Term::make_typed_literal(literal_value, {(char *) (type_node->buf), size_t(type_node->n_bytes)});
			else if (lang_node != nullptr)
				return Term::make_lang_literal(literal_value, {(char *) (lang_node->buf), size_t(lang_node->n_bytes)});
			else
				return Term::make_str_literal(literal_value);
		};

		static auto serd_base_callback(void *handle, const SerdNode *uri) -> SerdStatus {
			auto &load = *((BulkLoad *) handle);
			load.prefixes[""] = std::string((char *) (uri->buf), uri->n_bytes);
			return SERD_SUCCESS;
		}

		static auto serd_prefix_callback(void *handle, const SerdNode *name, const SerdNode *uri) -> SerdStatus {
			auto &load = *((BulkLoad *) handle);
			load.prefixes[std::string((char *) (name->buf), name->n_bytes)]
					= std::string((char *) (uri->buf), uri->n_bytes);
			return SERD_SUCCESS;
		}

		static auto
		serd_callback(void *handle, [[maybe_unused]] SerdStatementFlags flags, [[maybe_unused]] const SerdNode *graph,
					  const SerdNode *subject,
					  const SerdNode *predicate, const SerdNode *object, const SerdNode *object_datatype,
					  const SerdNode *object_lang) -> SerdStatus {
			auto &load = *((BulkLoad *) handle);
			Term subject_term;
			Term predicate_term;
			Term object_term;

			switch (subject->type) {
				case SERD_CURIE:
					subject_term = load.getPrefixedUri(subject);
					break;
				case SERD_URI:
					subject_term = load.getURI(subject);
					break;
				case SERD_BLANK: {
					subject_term = load.getBNode(subject);
				}
					break;
				default:
					return SERD_ERR_BAD_SYNTAX;
			}

			switch (predicate->type) {
				case SERD_CURIE:
					predicate_term = load.getPrefixedUri(predicate);
					break;
				case SERD_URI:
					predicate_term = load.getURI(predicate);
					break;
				default:
					return SERD_ERR_BAD_SYNTAX;
			}

			switch (object->type) {
				case SERD_CURIE:
					object_term = load.getPrefixedUri(object);
					break;
				case SERD_LITERAL:
					object_term = load.getLiteral(object, object_datatype, object_lang);
					break;
				case SERD_BLANK:
					object_term = load.getBNode(object);
					break;
				case SERD_URI:
					object_term = load.getURI(object);
					break;
				default:
					return SERD_ERR_BAD_SYNTAX;
			}
			auto &store = load.store();
			load.store().add(std::move(subject_term), std::move(predicate_term), std::move(object_term));
			if (store.size() % load.load_log == 0) {
				if ((store.size() / load.load_log) % 10 == 0)
					load.load_log *= 10;
				log("{} mio triples in store."_format(store.size() / 1'000'000));
			}
			return SERD_SUCCESS;
		}
	};

	void TripleStore::loadRDF(const std::string &file_path) {
		BulkLoad::load(*this, file_path);
	}
};
#endif //TENTRIS_STORE_TRIPLESTORE
