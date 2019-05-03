#ifndef TENTRIS_RESULT_HPP
#define TENTRIS_RESULT_HPP

#include "tentris/tensor/einsum/operator/GeneratorInterface.hpp"

namespace tentris::tensor {

	template<typename RESULT_TYPE, typename = typename std::enable_if<tentris::tensor::einsum::operators::is_binding<RESULT_TYPE>::value>::type>
	class Result {
	public:
		using binding_t = typename RESULT_TYPE::binding_t;
		using key_t = typename RESULT_TYPE::key_t;
		using count_t = typename RESULT_TYPE::count_t;
		using unordered_collection_t = typename RESULT_TYPE::unordered_collection_t;

	private:
		unordered_collection_t items;
		size_t size_ = 0;

	public:

		void insert(const binding_t &binding) {
			::tentris::tensor::einsum::operators::addToCollection<RESULT_TYPE>(items, binding);
		}

		bool contains(const binding_t &binding) {
			return bool(items.count(RESULT_TYPE::getKey(binding)));
		}

		bool empty() const noexcept { return items.empty(); }

		typename unordered_collection_t::iterator begin() noexcept { return items.begin(); }

		typename unordered_collection_t::iterator end() noexcept { return items.end(); }

		typename unordered_collection_t::const_iterator begin() const noexcept { return items.begin(); }

		typename unordered_collection_t::const_iterator end() const noexcept { return items.end(); }

		typename unordered_collection_t::const_iterator cbegin() const noexcept { return items.cbegin(); }

		typename unordered_collection_t::const_iterator cend() const noexcept { return items.cend(); }

		void clear() noexcept {
			size_ = 0;
			items.clear();
		}

		size_t size() const noexcept {
			return items.size();
		}

		size_t distinct_size() const noexcept {
			return items.size();
		}
	};

	template <>
	void Result<::tentris::tensor::einsum::operators::counted_binding>::insert(const ::tentris::tensor::einsum::operators::counted_binding::binding_t &binding) {
		size_ += ::tentris::tensor::einsum::operators::counted_binding::getCount(binding);
		::tentris::tensor::einsum::operators::addToCollection<::tentris::tensor::einsum::operators::counted_binding>(items, binding);
	}

	template <>
	size_t Result<::tentris::tensor::einsum::operators::counted_binding>::size()  const noexcept {
		return size_;
	}

}


#endif //TENTRIS_RESULT_HPP
