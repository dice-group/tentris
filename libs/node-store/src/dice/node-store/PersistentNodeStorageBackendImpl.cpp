#include "PersistentNodeStorageBackendImpl.hpp"

#include <memory>
#include <mutex>
#include <utility>

namespace dice::node_store {
	using namespace rdf4cpp::rdf::storage::node;
	PersistentNodeStorageBackendImpl::PersistentNodeStorageBackendImpl(metall_manager::allocator_type<std::byte> const &allocator)
		: allocator(allocator),
		  bnode_storage_(allocator),
		  iri_storage_(allocator),
		  literal_storage_(allocator),
		  variable_storage_(allocator) {
		// some iri's like xsd:string are there by default
		using namespace rdf4cpp::rdf;
		for (const auto &[iri, id] : rdf4cpp::rdf::datatypes::registry::reserved_datatype_ids) {
			auto mem = iri_storage_.backend_allocator.allocate(1);
			iri_storage_.backend_allocator.construct(mem, iri, allocator);
			auto [iter, inserted_successfully] = iri_storage_.data2id.emplace(mem, id.to_underlying());
			assert(inserted_successfully);
			iri_storage_.id2data.emplace(id.to_underlying(), iter->first);
		}
	}

    template<typename Storage>
    static size_t lookup_size(Storage &storage) {
	    std::shared_lock l{storage.mutex};
	    return storage.id2data.size();
	}

    size_t PersistentNodeStorageBackendImpl::size() const noexcept {
	    return lookup_size(bnode_storage_) + lookup_size(iri_storage_) + lookup_size(literal_storage_) + lookup_size(variable_storage_);
	}

    bool PersistentNodeStorageBackendImpl::has_specialized_storage_for([[maybe_unused]] identifier::LiteralType type) {
	    return false;
	}

	/**
     * Synchronized lookup (and creation) of IDs by a provided view of a Node Backend.
     * @tparam Backend_t the Backend type. One of BNodeBackend, IRIBackend, LiteralBackend or VariableBackend
     * @tparam create_if_not_present enables code for creating non-existing Node Backends
     * @tparam NextIDFromView_func type of a function to generate the next ID which is assigned in case a new Node Backend is created
     * @param view contains the data of the requested Node Backend
     * @param storage the storage where the Node Backend is looked up
     * @param next_id_func function to generate the next ID which is assigned in case a new Node Backend is created
     * @return the NodeID for the looked up Node Backend. Result is null() if there was no matching Node Backend.
     */
	template<class Backend_t, bool create_if_not_present, class NextIDFromView_func = void *>
	inline identifier::NodeID lookup_or_insert_impl(typename Backend_t::View const &view,
													auto &storage,
													NextIDFromView_func next_id_func = nullptr) noexcept {
		std::shared_lock<std::shared_mutex> shared_lock{storage.mutex};
		auto found = storage.data2id.find(view);
		if (found == storage.data2id.end()) {
			if constexpr (create_if_not_present) {
				shared_lock.unlock();
				std::unique_lock<std::shared_mutex> unique_lock{storage.mutex};
				// update found (might have changed in the meantime)
				found = storage.data2id.find(view);
				if (found == storage.data2id.end()) {
					identifier::NodeID id = next_id_func(view);
					auto mem = storage.backend_allocator.allocate(1);
					storage.backend_allocator.construct(mem, view, storage.backend_allocator);
					auto [found2, inserted_successfully] = storage.data2id.emplace(mem, id);
					assert(inserted_successfully);
					storage.id2data.emplace(id, found2->first);
					return id;
				} else {
					unique_lock.unlock();
					return found->second;
				}
			} else {
				return {};
			}
		} else {
			shared_lock.unlock();
			return found->second;
		}
	}

	identifier::NodeID PersistentNodeStorageBackendImpl::find_or_make_id(view::LiteralBackendView const &view) noexcept {
		return lookup_or_insert_impl<MetallLiteralBackend, true>(
				view.get_lexical(), literal_storage_,
				[this](view::LexicalFormLiteralBackendView const &literal_view) {
					return identifier::NodeID{next_literal_id++,
											  identifier::iri_node_id_to_literal_type(literal_view.datatype_id)};
				});
	}

	identifier::NodeID PersistentNodeStorageBackendImpl::find_or_make_id(view::IRIBackendView const &view) noexcept {
		return lookup_or_insert_impl<MetallIRIBackend, true>(
				view, iri_storage_,
				[this]([[maybe_unused]] view::IRIBackendView const &view) {
					return next_iri_id++;
				});
	}

	identifier::NodeID PersistentNodeStorageBackendImpl::find_or_make_id(view::BNodeBackendView const &view) noexcept {
		return lookup_or_insert_impl<MetallBNodeBackend, true>(
				view, bnode_storage_,
				[this]([[maybe_unused]] view::BNodeBackendView const &view) {
					return next_bnode_id++;
				});
	}
	identifier::NodeID PersistentNodeStorageBackendImpl::find_or_make_id(view::VariableBackendView const &view) noexcept {
		return lookup_or_insert_impl<MetallVariableBackend, true>(
				view, variable_storage_,
				[this]([[maybe_unused]] view::VariableBackendView const &view) {
					return next_variable_id++;
				});
	}

	identifier::NodeID PersistentNodeStorageBackendImpl::find_id(const view::BNodeBackendView &view) const noexcept {
		return lookup_or_insert_impl<MetallBNodeBackend, false>(
				view, bnode_storage_);
	}
	identifier::NodeID PersistentNodeStorageBackendImpl::find_id(const view::IRIBackendView &view) const noexcept {
		return lookup_or_insert_impl<MetallIRIBackend, false>(
				view, iri_storage_);
	}
	identifier::NodeID PersistentNodeStorageBackendImpl::find_id(const view::LiteralBackendView &view) const noexcept {
		return lookup_or_insert_impl<MetallLiteralBackend, false>(
				view.get_lexical(), literal_storage_);
	}
	identifier::NodeID PersistentNodeStorageBackendImpl::find_id(const view::VariableBackendView &view) const noexcept {
		return lookup_or_insert_impl<MetallVariableBackend, false>(
				view, variable_storage_);
	}

	template<typename NodeTypeStorage>
	typename NodeTypeStorage::BackendView find_backend_view(NodeTypeStorage &storage, identifier::NodeID id) {
		std::shared_lock<std::shared_mutex> shared_lock{storage.mutex};
		return typename NodeTypeStorage::BackendView(*storage.id2data.at(id));
	}

	view::IRIBackendView PersistentNodeStorageBackendImpl::find_iri_backend_view(identifier::NodeID id) const {
		return find_backend_view(iri_storage_, id);
	}
	view::LiteralBackendView PersistentNodeStorageBackendImpl::find_literal_backend_view(identifier::NodeID id) const {
		return find_backend_view(literal_storage_, id);
	}
	view::BNodeBackendView PersistentNodeStorageBackendImpl::find_bnode_backend_view(identifier::NodeID id) const {
		return find_backend_view(bnode_storage_, id);
	}
	view::VariableBackendView PersistentNodeStorageBackendImpl::find_variable_backend_view(identifier::NodeID id) const {
		return find_backend_view(variable_storage_, id);
	}

}// namespace dice::node_store