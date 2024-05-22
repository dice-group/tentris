#include "MetallBNodeBackend.hpp"
#include <dice/hash/DiceHash.hpp>
namespace dice::node_store {

	MetallBNodeBackend::MetallBNodeBackend(std::string_view identifier, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: identifier_(identifier, allocator),
		  hash_(View(*this).hash()) {}
	MetallBNodeBackend::MetallBNodeBackend(rdf4cpp::rdf::storage::node::view::BNodeBackendView view, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: identifier_(view.identifier, allocator),
		  hash_(View(*this).hash()) {}
	std::string_view MetallBNodeBackend::identifier() const noexcept {
		return identifier_;
	}
	MetallBNodeBackend::operator View() const noexcept {
		return {.identifier = identifier(), .scope = nullptr};
	}
}// namespace dice::node_store