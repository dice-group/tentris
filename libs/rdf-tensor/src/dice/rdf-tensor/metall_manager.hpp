#ifndef TENTRIS_METALL_MANAGER_HPP
#define TENTRIS_METALL_MANAGER_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-pointer-arithmetic"
#include <metall/basic_manager.hpp>
#pragma GCC diagnostic pop

namespace dice::rdf_tensor {
	using metall_manager = metall::basic_manager<>;
	using allocator_type = metall_manager::allocator_type<std::byte>;
}// namespace dice::rdf-tensor

#endif//TENTRIS_METALL_MANAGER_HPP
