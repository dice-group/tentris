#include <dice/node-store/PersistentNodeStorageBackend.hpp>

int main() {
	dice::rdf_tensor::metall_manager x{metall::create_only, "abc"};
	dice::node_store::PersistentNodeStorageBackendImpl y{x.get_allocator()};
}
