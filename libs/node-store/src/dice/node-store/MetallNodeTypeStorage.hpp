#ifndef RDF4CPP_METALLNODETYPESTORAGE_HPP
#define RDF4CPP_METALLNODETYPESTORAGE_HPP

#include <dice/sparse-map/sparse_map.hpp>


#include <rdf4cpp/rdf/storage/node/identifier/NodeID.hpp>

#include <shared_mutex>


namespace dice::node_store {
	/**
 * Storage for one of the Node Backend types. Includes a shared mutex to synchronize access and bidirectional mappings between the Backend type and identifier::NodeID.
 * @tparam BackendType_t one of BNodeBackend, IRIBackend, LiteralBackend and VariableBackend.
 */
	template<class BackendType_t>
	struct MetallNodeTypeStorage {
		using allocator_type = rdf_tensor::allocator_type;
		using Backend = BackendType_t;
		using Backend_allocator_type = metall_manager::allocator_type<Backend>;
		using Backend_ptr = typename Backend_allocator_type::pointer;
		using BackendView = typename Backend::View;
		struct BackendTypeHash {
			[[nodiscard]] size_t operator()(Backend_ptr const &x) const noexcept {
				return x->hash();
			}
			[[nodiscard]] size_t operator()(BackendView const &x) const noexcept {
				return x.hash();
			}
		};

		struct BackendTypeEqual {
			using is_transparent = void;

			bool operator()(Backend_ptr const &lhs, Backend_ptr const &rhs) const noexcept {
				if (bool(lhs) and bool(rhs))
					return lhs.get() == rhs.get();
				else
					return bool(lhs) == bool(rhs);
			}
			bool operator()(BackendView const &lhs, Backend_ptr const &rhs) const noexcept {
				if (rhs)
					return lhs == BackendView(*rhs);
				else
					return false;
			}
		};

		struct NodeIDHash {
			[[nodiscard]] size_t operator()(rdf4cpp::rdf::storage::node::identifier::NodeID const &x) const noexcept {
				return x.value();
			}
		};

		mutable std::shared_mutex mutex;
		dice::sparse_map::sparse_map<rdf4cpp::rdf::storage::node::identifier::NodeID, Backend_ptr, NodeIDHash, std::equal_to<>,
						metall_manager::allocator_type<std::pair<rdf4cpp::rdf::storage::node::identifier::NodeID, Backend_ptr>>>
				id2data;
		dice::sparse_map::sparse_map<Backend_ptr, rdf4cpp::rdf::storage::node::identifier::NodeID, BackendTypeHash, BackendTypeEqual,
						metall_manager::allocator_type<std::pair<Backend_ptr, rdf4cpp::rdf::storage::node::identifier::NodeID>>>
				data2id;

		Backend_allocator_type backend_allocator;

		explicit MetallNodeTypeStorage(rdf_tensor::allocator_type const &alloc) : mutex(), id2data(alloc), data2id(alloc), backend_allocator(alloc) {}
	};
}// namespace dice::node_store

#endif//RDF4CPP_METALLNODETYPESTORAGE_HPP
