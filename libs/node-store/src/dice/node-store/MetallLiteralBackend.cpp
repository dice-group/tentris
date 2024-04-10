#include "MetallLiteralBackend.hpp"
#include <dice/hash/DiceHash.hpp>
#include <tuple>
namespace dice::node_store {

	MetallLiteralBackend::MetallLiteralBackend(std::string_view lexical, const rdf4cpp::rdf::storage::node::identifier::NodeID &datatype_id, std::string_view lang_tag, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: datatype_id_(datatype_id),
		  lexical(lexical, allocator),
		  lang_tag(lang_tag, allocator),
		  hash_(View(*this).hash()) {}
	MetallLiteralBackend::MetallLiteralBackend(rdf4cpp::rdf::storage::node::view::LiteralBackendView view, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: datatype_id_(view.datatype_id),
		  lexical(view.lexical_form, allocator),
		  lang_tag(view.language_tag, allocator),
		  hash_(View(*this).hash()) {}
	std::string_view MetallLiteralBackend::language_tag() const noexcept {
		return lang_tag;
	}
	const rdf4cpp::rdf::storage::node::identifier::NodeID &MetallLiteralBackend::datatype_id() const noexcept {
		return datatype_id_;
	}
	std::string_view MetallLiteralBackend::lexical_form() const noexcept {
		return lexical;
	}
	MetallLiteralBackend::operator rdf4cpp::rdf::storage::node::view::LiteralBackendView() const noexcept {
		return {.datatype_id = datatype_id(),
				.lexical_form = lexical_form(),
				.language_tag = language_tag()};
	}

}// namespace dice::node_store
