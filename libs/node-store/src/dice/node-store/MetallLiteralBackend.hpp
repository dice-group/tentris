#ifndef RDF4CPP_METALLLITERALBACKEND_HPP
#define RDF4CPP_METALLLITERALBACKEND_HPP

#include <rdf4cpp/rdf/storage/node/view/LiteralBackendView.hpp>

#include <dice/node-store/metall_manager.hpp>

namespace dice::node_store {

	class MetallLiteralBackend {
		rdf4cpp::rdf::storage::node::identifier::NodeID datatype_id_;
		metall_string lexical;
		metall_string lang_tag;
		size_t hash_;

	public:
		using View = rdf4cpp::rdf::storage::node::view::LiteralBackendView;

		MetallLiteralBackend(std::string_view lexical, const rdf4cpp::rdf::storage::node::identifier::NodeID &datatype_id, std::string_view lang_tag, metall_manager::allocator_type<std::byte> const &allocator) noexcept;
		MetallLiteralBackend(rdf4cpp::rdf::storage::node::view::LiteralBackendView view, metall_manager::allocator_type<std::byte> const &allocator) noexcept;

		[[nodiscard]] std::string_view lexical_form() const noexcept;

		[[nodiscard]] const rdf4cpp::rdf::storage::node::identifier::NodeID &datatype_id() const noexcept;

		[[nodiscard]] std::string_view language_tag() const noexcept;

		[[nodiscard]] size_t hash() const noexcept { return hash_; }

		explicit operator rdf4cpp::rdf::storage::node::view::LiteralBackendView() const noexcept;
	};

}// namespace dice::node_store


#endif//RDF4CPP_METALLLITERALBACKEND_HPP
