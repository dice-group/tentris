#ifndef RDF4CPP_METALLVARIABLEBACKEND_HPP
#define RDF4CPP_METALLVARIABLEBACKEND_HPP

#include <rdf4cpp/rdf/storage/node/view/VariableBackendView.hpp>

#include <dice/node-store/metall_manager.hpp>

namespace dice::node_store {

	class MetallVariableBackend {
		metall_string name_;
		bool anonymous_;
		size_t hash_;

	public:
		using View = rdf4cpp::rdf::storage::node::view::VariableBackendView;

		explicit MetallVariableBackend(std::string_view name, bool anonymous, metall_manager::allocator_type<std::byte> const &allocator) noexcept;
		explicit MetallVariableBackend(rdf4cpp::rdf::storage::node::view::VariableBackendView, metall_manager::allocator_type<std::byte> const &allocator) noexcept;

		[[nodiscard]] bool is_anonymous() const noexcept;

		[[nodiscard]] std::string_view name() const noexcept;

		[[nodiscard]] size_t hash() const noexcept { return hash_; }

		explicit operator rdf4cpp::rdf::storage::node::view::VariableBackendView() const noexcept;
	};

}// namespace dice::node_store


#endif//RDF4CPP_METALLVARIABLEBACKEND_HPP
