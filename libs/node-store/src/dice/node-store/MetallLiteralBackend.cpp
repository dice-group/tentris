#include "MetallLiteralBackend.hpp"
#include <dice/hash/DiceHash.hpp>
#include <tuple>
namespace dice::node_store {
	MetallLiteralBackend::MetallLiteralBackend(rdf4cpp::rdf::storage::node::view::LexicalFormLiteralBackendView view, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: hash_(view.hash()),
          datatype_id_(view.datatype_id),
		  lexical(view.lexical_form, allocator),
		  lang_tag(view.language_tag, allocator),
		  needs_escape_(view.needs_escape) {}
	std::string_view MetallLiteralBackend::language_tag() const noexcept {
		return lang_tag;
	}
	const rdf4cpp::rdf::storage::node::identifier::NodeID &MetallLiteralBackend::datatype_id() const noexcept {
		return datatype_id_;
	}
	std::string_view MetallLiteralBackend::lexical_form() const noexcept {
		return lexical;
	}
    bool MetallLiteralBackend::needs_escape() const noexcept {
	    return needs_escape_;
	}
	MetallLiteralBackend::operator rdf4cpp::rdf::storage::node::view::LexicalFormLiteralBackendView() const noexcept {
		return {.datatype_id = datatype_id(),
				.lexical_form = lexical_form(),
				.language_tag = language_tag(),
		        .needs_escape = needs_escape()};
	}

}// namespace dice::node_store
