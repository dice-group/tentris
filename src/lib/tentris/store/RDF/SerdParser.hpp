#ifndef TENTRIS_SERDPARSER_H
#define TENTRIS_SERDPARSER_H

#include <fmt/format.h>
#include <fmt/core.h>
#include <Dice/RDF/Triple.hpp>
#include <Dice/RDF/Term.hpp>
#include <Dice/hash/DiceHash.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <serd/serd.h>
#include <boost/algorithm/string.hpp>
#include <tsl/hopscotch_map.h>
#include <atomic>
#include <memory>
#include <thread>
#include <utility>

namespace tentris::store::rdf {

	class BulkLoad {
		using Triple = Dice::rdf::Triple;
		using Term = Dice::rdf::Term;
		using BNode = Dice::rdf::BNode;
		using Literal = Dice::rdf::Literal;
		using URIRef = Dice::rdf::URIRef;
		using prefixes_map_type = tsl::hopscotch_map<std::string, std::string, Dice::hash::DiceHash<std::string>>;
		prefixes_map_type prefixes{};

	public:
        boost::lockfree::spsc_queue<Triple> result_queue{100000};
		bool parsing_done = false;

	public:
		static std::shared_ptr<BulkLoad> parse(const std::string &file_path) {

			auto bulk_load = std::make_shared<BulkLoad>();

			SerdReader *sr = serd_reader_new(SERD_TURTLE, (void *) bulk_load.get(), nullptr, &serd_base_callback,
											 &serd_prefix_callback,
											 &serd_callback,
											 nullptr);

			std::thread t([=]() {
				[[maybe_unused]] SerdStatus status = serd_reader_read_file(sr, (uint8_t *) (file_path.data()));
				bulk_load->parsing_done = true;
				serd_reader_free(sr);
			});
			t.detach();

			return bulk_load;

		}


	private:

		static auto getBNode(const SerdNode *node) -> Term {
			return BNode(std::string(std::string_view{(char *) (node->buf), size_t(node->n_bytes)}));
		}

		static auto getURI(const SerdNode *node) -> Term {
			return URIRef(std::string(std::string_view{(char *) (node->buf), size_t(node->n_bytes)}));
		}

		auto getPrefixedUri(const SerdNode *node) const -> Term {
			std::string_view uri_node_view{(char *) (node->buf), size_t(node->n_bytes)};

			std::vector<std::string> prefix_and_suffix{};
			boost::split(prefix_and_suffix, uri_node_view, [](char c) { return c == ':'; });

			assert(prefix_and_suffix.size() == 2);
			assert(prefixes.count(std::string{prefix_and_suffix[0]}));
			return URIRef(fmt::format("{}{}", prefixes.find(prefix_and_suffix[0])->second, prefix_and_suffix[1]));
		}

		static auto getLiteral(const SerdNode *literal, const SerdNode *type_node, const SerdNode *lang_node) -> Term {
			std::string literal_value = std::string{(char *) (literal->buf), size_t(literal->n_bytes)};
			if (type_node != nullptr)
				return Literal(literal_value, std::nullopt,
							   std::string{(char *) (type_node->buf), size_t(type_node->n_bytes)});
			else if (lang_node != nullptr)
				return Literal(literal_value, std::string{(char *) (lang_node->buf), size_t(lang_node->n_bytes)},
							   std::nullopt);
			else
				return Literal(literal_value, std::nullopt, std::nullopt);
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
			auto &bulk_load = *((BulkLoad *) handle);
			Term subject_term;
			Term predicate_term;
			Term object_term;

			switch (subject->type) {
				case SERD_CURIE:
					subject_term = bulk_load.getPrefixedUri(subject);
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
					predicate_term = bulk_load.getPrefixedUri(predicate);
					break;
				case SERD_URI:
					predicate_term = getURI(predicate);
					break;
				default:
					return SERD_ERR_BAD_SYNTAX;
			}

			switch (object->type) {
				case SERD_CURIE:
					object_term = bulk_load.getPrefixedUri(object);
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
			while (bulk_load.result_queue.write_available() == 0) {
				using namespace std::this_thread; // sleep_for, sleep_until
				using namespace std::chrono; // nanoseconds, steady_clock, seconds
				sleep_for(milliseconds(5));
			}
			bulk_load.result_queue.push({std::move(subject_term), std::move(predicate_term), std::move(object_term)});
			return SERD_SUCCESS;
		}
	};

	class SerdParser {
		using Triple = Dice::rdf::Triple;
		using Term = Dice::rdf::Term;
		using BNode = Dice::rdf::BNode;
		using Literal = Dice::rdf::Literal;
		using URIRef = Dice::rdf::URIRef;

		std::string file_name_;

	public:
		explicit SerdParser(std::string file_name) : file_name_(std::move(file_name)) {}


	public:
		class Iterator {

			bool done_;
			std::shared_ptr<BulkLoad> bulk_load;
			Triple result;

		public:
			explicit Iterator(const std::string &file_name) : done_(false), bulk_load(BulkLoad::parse(file_name)) {
				while (not bulk_load->result_queue.pop(result)) {
					if (bulk_load->parsing_done) {
						if (not bulk_load->result_queue.pop(result)) {
							done_ = true;
							return;
						}
					}
				}
			};

			void operator++() {
				while (not bulk_load->result_queue.pop(result)) {
					if (bulk_load->parsing_done) {
						if (not bulk_load->result_queue.pop(result)) {
							done_ = true;
						}
						return;
					}
				}
			}

			void operator++(int) { operator++(); }

			operator bool() const { return not done_; }

			const Triple &operator*() { return result; }
		};


		Iterator begin() { return Iterator(file_name_); }

		bool end() { return false; }

	};
}

#endif //TENTRIS_SERDPARSER_H
