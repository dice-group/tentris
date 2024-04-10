#ifndef TENTRIS_METALL_ALLOCATOR_HPP
#define TENTRIS_METALL_ALLOCATOR_HPP


#ifndef BOOST_BIND_GLOBAL_PLACEHOLDERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#endif

#include <dice/rdf-tensor/metall_manager.hpp>

#include <metall/container/string.hpp>


namespace dice::node_store {

	using metall_manager = rdf_tensor::metall_manager;
	using metall_string = metall::container::basic_string<char, std::char_traits<char>, metall_manager::allocator_type<char>>;
}
#endif//TENTRIS_METALL_ALLOCATOR_HPP
