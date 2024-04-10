#ifndef RDF4CPP_METALLBNODEBACKEND_HPP
#define RDF4CPP_METALLBNODEBACKEND_HPP

#include <rdf4cpp/rdf/storage/node/view/BNodeBackendView.hpp>

#include <dice/node-store/metall_manager.hpp>


namespace dice::node_store {

	class MetallBNodeBackend {
		metall_string identifier_;
		size_t hash_;

	public:
		using View = rdf4cpp::rdf::storage::node::view::BNodeBackendView;

		explicit MetallBNodeBackend(std::string_view identifier, metall_manager::allocator_type<std::byte> const &allocator) noexcept;
		MetallBNodeBackend(rdf4cpp::rdf::storage::node::view::BNodeBackendView view, metall_manager::allocator_type<std::byte> const &allocator) noexcept;
		[[nodiscard]] std::string_view identifier() const noexcept;

		[[nodiscard]] size_t hash() const noexcept { return hash_; }

		explicit operator View() const noexcept;
	};
}// namespace dice::node_store

#endif//RDF4CPP_METALLBNODEBACKEND_HPP
