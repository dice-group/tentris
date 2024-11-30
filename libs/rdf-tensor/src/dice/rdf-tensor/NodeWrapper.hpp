#ifndef TENTRIS_NODEWRAPPER_HPP
#define TENTRIS_NODEWRAPPER_HPP

#include <dice/hash/DiceHash.hpp>
#include <rdf4cpp/rdf.hpp>
//#include <dice/rdf-tensor/check_no_throw.hpp>

namespace dice::rdf_tensor {
	using namespace rdf4cpp::rdf;

	class NodeWrapper : public Node {
	protected:
		explicit NodeWrapper(NodeBackendHandle id) noexcept : Node(id) {}

	public:
		NodeWrapper() noexcept = default;

		NodeWrapper(Node node) noexcept : Node(node) {}

		bool operator==(const NodeWrapper &other) const noexcept {
			return this->backend_handle().raw() == other.backend_handle().raw();
		}

		bool operator!=(const NodeWrapper &other) const noexcept {
			return this->backend_handle().raw() != other.backend_handle().raw();
		}

		auto operator<=>(const NodeWrapper &other) const noexcept {
			return this->backend_handle().raw() <=> other.backend_handle().raw();
		};

		operator std::optional<Node>() const noexcept {
			return static_cast<Node>(*this);
		};
	};
};// namespace dice::rdf-tensor

template<typename Policy>
struct dice::hash::dice_hash_overload<Policy, dice::rdf_tensor::NodeWrapper> {
	inline static std::size_t dice_hash(dice::rdf_tensor::NodeWrapper const &x) noexcept {
		return Policy::hash_fundamental(x.backend_handle().raw());
	}
};

template<>
struct std::hash<dice::rdf_tensor::NodeWrapper> {
	size_t operator()(dice::rdf_tensor::NodeWrapper const &x) const noexcept {
		return x.backend_handle().raw();
	}
};

#endif//TENTRIS_NODEWRAPPER_HPP
