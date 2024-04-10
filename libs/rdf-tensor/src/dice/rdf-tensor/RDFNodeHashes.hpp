#ifndef TENTRIS_NODEHASHES_HPP
#define TENTRIS_NODEHASHES_HPP

#include <dice/hash/DiceHash.hpp>
#include <rdf4cpp/rdf.hpp>

namespace dice::hash {
	template<typename Policy>
	struct dice_hash_overload<Policy, rdf4cpp::rdf::Node> {
		inline static std::size_t dice_hash(rdf4cpp::rdf::Node const &x) noexcept {
			return Policy::hash_fundamental(x.backend_handle().raw());
		}
	};

	template<typename Policy>
	struct dice_hash_overload<Policy, rdf4cpp::rdf::query::Variable> {
		inline static std::size_t dice_hash(rdf4cpp::rdf::query::Variable const &x) noexcept {
			return Policy::hash_fundamental(x.backend_handle().raw());
		}
	};

	template<typename Policy>
	struct dice_hash_overload<Policy, rdf4cpp::rdf::Literal> {
		inline static std::size_t dice_hash(rdf4cpp::rdf::Literal const &x) noexcept {
			return Policy::hash_fundamental(x.backend_handle().raw());
		}
	};

	template<typename Policy>
	struct dice_hash_overload<Policy, rdf4cpp::rdf::IRI> {
		inline static std::size_t dice_hash(rdf4cpp::rdf::IRI const &x) noexcept {
			return Policy::hash_fundamental(x.backend_handle().raw());
		}
	};

	template<typename Policy>
	struct dice_hash_overload<Policy, rdf4cpp::rdf::BlankNode> {
		inline static std::size_t dice_hash(rdf4cpp::rdf::BlankNode const &x) noexcept {
			return Policy::hash_fundamental(x.backend_handle().raw());
		}
	};
}// namespace dice::hash
#endif//TENTRIS_NODEHASHES_HPP
