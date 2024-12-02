#ifndef TENTRIS_PERSISTENTNODESTORAGEBACKEND_HPP
#define TENTRIS_PERSISTENTNODESTORAGEBACKEND_HPP

#include "dice/node-store/PersistentNodeStorageBackendImpl.hpp"

namespace dice::node_store {

	class PersistentNodeStorageBackend : public rdf4cpp::rdf::storage::node::INodeStorageBackend {
		PersistentNodeStorageBackendImpl *impl_;

	public:
		explicit PersistentNodeStorageBackend(PersistentNodeStorageBackendImpl *impl);

		~PersistentNodeStorageBackend() override = default;

		[[nodiscard]] size_t size() const noexcept override;
		[[nodiscard]] bool has_specialized_storage_for(rdf4cpp::rdf::storage::node::identifier::LiteralType datatype) const noexcept override;

		rdf4cpp::rdf::storage::node::identifier::NodeID find_or_make_id(const rdf4cpp::rdf::storage::node::view::BNodeBackendView &view) noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_or_make_id(const rdf4cpp::rdf::storage::node::view::IRIBackendView &view) noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_or_make_id(const rdf4cpp::rdf::storage::node::view::LiteralBackendView &view) noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_or_make_id(const rdf4cpp::rdf::storage::node::view::VariableBackendView &view) noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_id(const rdf4cpp::rdf::storage::node::view::BNodeBackendView &view) const noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_id(const rdf4cpp::rdf::storage::node::view::IRIBackendView &view) const noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_id(const rdf4cpp::rdf::storage::node::view::LiteralBackendView &view) const noexcept override;
		rdf4cpp::rdf::storage::node::identifier::NodeID find_id(const rdf4cpp::rdf::storage::node::view::VariableBackendView &view) const noexcept override;
		rdf4cpp::rdf::storage::node::view::IRIBackendView find_iri_backend_view(rdf4cpp::rdf::storage::node::identifier::NodeID id) const override;
		rdf4cpp::rdf::storage::node::view::LiteralBackendView find_literal_backend_view(rdf4cpp::rdf::storage::node::identifier::NodeID id) const override;
		rdf4cpp::rdf::storage::node::view::BNodeBackendView find_bnode_backend_view(rdf4cpp::rdf::storage::node::identifier::NodeID id) const override;
		rdf4cpp::rdf::storage::node::view::VariableBackendView find_variable_backend_view(rdf4cpp::rdf::storage::node::identifier::NodeID id) const override;
		bool erase_iri(rdf4cpp::rdf::storage::node::identifier::NodeID id) override;
		bool erase_literal(rdf4cpp::rdf::storage::node::identifier::NodeID id) override;
		bool erase_bnode(rdf4cpp::rdf::storage::node::identifier::NodeID id) override;
		bool erase_variable(rdf4cpp::rdf::storage::node::identifier::NodeID id) override;
	};
}// namespace dice::node_store

#endif//TENTRIS_PERSISTENTNODESTORAGEBACKEND_HPP
