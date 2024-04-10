#include "MetallVariableBackend.hpp"
#include <dice/hash/DiceHash.hpp>

namespace dice::node_store {

	MetallVariableBackend::MetallVariableBackend(std::string_view name, bool anonymous, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: name_(name, allocator),
		  anonymous_(anonymous),
		  hash_(View(*this).hash()) {}
	MetallVariableBackend::MetallVariableBackend(rdf4cpp::rdf::storage::node::view::VariableBackendView view, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: name_(view.name, allocator),
		  anonymous_(view.is_anonymous),
		  hash_(View(*this).hash()) {}
	bool MetallVariableBackend::is_anonymous() const noexcept {
		return anonymous_;
	}
	std::string_view MetallVariableBackend::name() const noexcept {
		return name_;
	}
	MetallVariableBackend::operator rdf4cpp::rdf::storage::node::view::VariableBackendView() const noexcept {
		return {.name = name(),
				.is_anonymous = is_anonymous()};
	}
}// namespace dice::node_store
