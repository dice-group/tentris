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
	using namespace tentris::util::types;
	using namespace tentris::store::sparql;
	using namespace ::tentris::logging;
	using namespace tentris::tensor;
}

namespace tentris::store {
	class TripleStore {
		TermStore termIndex{};

		BoolHypertrie trie{3};
		tsl::hopscotch_map<std::string, std::string, absl::Hash<std::string>> prefixes{};
		mutable QueryExecutionPackage_cache query_cache;
		std::size_t load_log = 1'000'000;

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

		void loadRDF(std::string file_path);

		void add(std::tuple<std::string, std::string, std::string> triple) {
			add(parseTerm(std::get<0>(triple)), parseTerm(std::get<1>(triple)), parseTerm(std::get<2>(triple)));
		}

		void add(std::tuple<std::shared_ptr<Term>, std::shared_ptr<Term>, std::shared_ptr<Term>> triple) {
			add(std::move(std::get<0>(triple)), std::move(std::get<1>(triple)), std::move(std::get<2>(triple)));
		}

		inline void
		add(std::shared_ptr<Term> &&subject, std::shared_ptr<Term> &&predicate, std::shared_ptr<Term> &&object) {
			if (subject->type() != Term::NodeType::Literal and predicate->type() == Term::NodeType::URI) {
				const key_part_t subject_id = termIndex[std::move(subject)];
				const key_part_t predicate_id = termIndex[std::move(predicate)];
				const key_part_t object_id = termIndex[std::move(object)];
				trie.set({subject_id, predicate_id, object_id}, true);
			} else
				throw std::invalid_argument{
						"Subject or predicate of the triple have a term type that is not allowed there."};
		}

		bool contains(std::tuple<std::string, std::string, std::string> triple) {
			const std::shared_ptr<Term> &subject = parseTerm(std::get<0>(triple));
			const std::shared_ptr<Term> &predicate = parseTerm(std::get<1>(triple));
			const std::shared_ptr<Term> &object = parseTerm(std::get<2>(triple));
			if (termIndex.contains(subject) and termIndex.contains(predicate) and termIndex.contains(object)) {
				BoolHypertrie::Key key{termIndex.at(subject), termIndex.at(predicate), termIndex.at(object)};
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

		friend auto
		serd_callback(void *handle, [[maybe_unused]] SerdStatementFlags flags, [[maybe_unused]] const SerdNode *graph,
					  const SerdNode *subject,
					  const SerdNode *predicate, const SerdNode *object, const SerdNode *object_datatype,
					  const SerdNode *object_lang) -> SerdStatus;

		friend
		auto serd_prefix_callback(void *handle, const SerdNode *name, const SerdNode *uri) -> SerdStatus;
		friend
		auto serd_base_callback (void *handle, const SerdNode *uri) -> SerdStatus;
	};

	auto getBNode(const SerdNode *node) -> std::shared_ptr<Term> {
		return std::make_shared<BNode>("_:{}"_format(std::string_view{(char *) (node->buf), size_t(node->n_bytes)}));
	}

	auto getURI(const SerdNode *node) -> std::shared_ptr<Term> {
		return std::make_shared<URIRef>("<{}>"_format(std::string_view{(char *) (node->buf), size_t(node->n_bytes)}));
	}

	auto getURIwithPrefix(const SerdNode *node, const tsl::hopscotch_map<std::string, std::string, absl::Hash<std::string>> &prefixes) -> std::shared_ptr<Term> {
		std::string_view uri_node_view{(char *) (node->buf), size_t(node->n_bytes)};

		std::vector<std::string> prefix_and_suffix{};
		boost::split(prefix_and_suffix, uri_node_view, [](char c){return c == ':';});

		assert(prefix_and_suffix.size() == 2);
		assert(prefixes.count(std::string{prefix_and_suffix[0]}));
		return std::make_shared<URIRef>("<{}{}>"_format(prefixes.find(prefix_and_suffix[0])->second, prefix_and_suffix[1]));
	}

	auto getLiteral(const SerdNode *literal, const SerdNode *type_node,
					const SerdNode *lang_node) -> std::shared_ptr<Term> {
		std::optional<std::string> type = (type_node != nullptr)
										  ? std::optional<std::string>{{(char *) (type_node->buf),
																			   size_t(type_node->n_bytes)}}
										  : std::nullopt;
		std::optional<std::string> lang = (lang_node != nullptr)
										  ? std::optional<std::string>{{(char *) (lang_node->buf),
																			   size_t(lang_node->n_bytes)}}
										  : std::nullopt;
		return std::make_shared<Literal>(std::string{(char *) (literal->buf), size_t(literal->n_bytes)}, lang, type);
	};

	auto serd_base_callback (void *handle, const SerdNode *uri) -> SerdStatus{
			auto * store = (tentris::store::TripleStore *) handle;
			store->prefixes[""] = std::string((char *) (uri->buf), uri->n_bytes);
		return SERD_SUCCESS;
	}

	auto serd_prefix_callback(void *handle, const SerdNode *name, const SerdNode *uri) -> SerdStatus {
		auto *store = (tentris::store::TripleStore *) handle;
		store->prefixes[std::string((char *) (name->buf), name->n_bytes)] = std::string((char *) (uri->buf),
																						uri->n_bytes);
		return SERD_SUCCESS;
	}

	auto serd_callback(void *handle, [[maybe_unused]] SerdStatementFlags flags, [[maybe_unused]] const SerdNode *graph,
					   const SerdNode *subject,
					   const SerdNode *predicate, const SerdNode *object, const SerdNode *object_datatype,
					   const SerdNode *object_lang) -> SerdStatus {
		auto *store = (tentris::store::TripleStore *) handle;
		std::shared_ptr<Term> subject_term;
		std::shared_ptr<Term> predicate_term;
		std::shared_ptr<Term> object_term;

		switch (subject->type) {
			case SERD_CURIE:
				subject_term = getURIwithPrefix(subject, store->prefixes);
				break;
			case SERD_URI:
				subject_term = getURI(subject);
				break;
			case SERD_BLANK: {
				subject_term = getBNode(subject);
			}
				break;
			default:
				return SERD_ERR_BAD_SYNTAX;
		}

		switch (predicate->type) {
			case SERD_CURIE:
				predicate_term = getURIwithPrefix(predicate, store->prefixes);
				break;
			case SERD_URI:
				predicate_term = getURI(predicate);
				break;
			default:
				return SERD_ERR_BAD_SYNTAX;
		}

		switch (object->type) {
			case SERD_CURIE:
				object_term = getURIwithPrefix(object, store->prefixes);
				break;
			case SERD_LITERAL:
				object_term = getLiteral(object, object_datatype, object_lang);
				break;
			case SERD_BLANK:
				object_term = getBNode(object);
				break;
			case SERD_URI:
				object_term = getURI(object);
				break;
			default:
				return SERD_ERR_BAD_SYNTAX;
		}
		store->add(std::move(subject_term), std::move(predicate_term), std::move(object_term));
		if (store->size() % store->load_log == 0) {
			if ((store->size() / store->load_log) % 10 == 0)
				store->load_log *= 10;
			log("{} mio triples loaded."_format(store->size() / 1'000'000));
		}
		return SERD_SUCCESS;
	}


	void TripleStore::loadRDF(std::string file_path) {
		SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) this, nullptr, &serd_base_callback, &serd_prefix_callback,
										 &serd_callback,
										 nullptr);
		serd_reader_read_file(sr, (uint8_t *) (file_path.data()));
	}

};
#endif //TENTRIS_STORE_TRIPLESTORE
