#include "MetallIRIBackend.hpp"
#include <dice/hash/internal/DiceHashPolicies.hpp>

namespace dice::node_store {

	MetallIRIBackend::MetallIRIBackend(std::string_view iri, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: iri(iri, allocator),
		  hash_(View(*this).hash()) {}
	MetallIRIBackend::MetallIRIBackend(rdf4cpp::rdf::storage::node::view::IRIBackendView iri, metall_manager::allocator_type<std::byte> const &allocator) noexcept
		: iri(iri.identifier, allocator),
		  hash_(View(*this).hash()) {}
	std::string_view MetallIRIBackend::identifier() const noexcept {
		return iri;
	}
	MetallIRIBackend::operator rdf4cpp::rdf::storage::node::view::IRIBackendView() const noexcept {
		return {.identifier = identifier()};
	}
}// namespace dice::node_store
