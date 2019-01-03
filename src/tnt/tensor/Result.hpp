#ifndef TNT_RESULT_HPP
#define TNT_RESULT_HPP

#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"

namespace tnt::tensor {

	template<typename RESULT_TYPE, typename = typename std::enable_if<tnt::tensor::einsum::operators::is_binding<RESULT_TYPE>::value>::type>
	class Result {
	public:
		using binding_t = typename RESULT_TYPE::binding_t;
		using key_t = typename RESULT_TYPE::key_t;
		using count_t = typename RESULT_TYPE::count_t;
		using collection_t = typename RESULT_TYPE::collection_t;

	private:
		collection_t items;

	public:

		void insert(const binding_t &binding){
			RESULT_TYPE::addToCollection(items, binding);
		}

		bool contains(const binding_t &binding){
			return bool(items.count(RESULT_TYPE::getKey(binding)));
		}

		typename collection_t::iterator begin(){return items.begin();}
		typename collection_t::iterator end(){return items.end();}
		typename collection_t::const_iterator cbegin() const noexcept {return items.cbegin();}
		typename collection_t::const_iterator cend() const noexcept {return items.cend();}
	};


}


#endif //TNT_RESULT_HPP
