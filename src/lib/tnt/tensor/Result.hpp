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
		size_t size_ = 0;

	public:

		void insert(const binding_t &binding) {
			::tnt::tensor::einsum::operators::addToCollection<RESULT_TYPE>(items, binding);
		}

		bool contains(const binding_t &binding) {
			return bool(items.count(RESULT_TYPE::getKey(binding)));
		}

		bool empty() const noexcept { return items.empty(); }

		typename collection_t::iterator begin() noexcept { return items.begin(); }

		typename collection_t::iterator end() noexcept { return items.end(); }

		typename collection_t::const_iterator begin() const noexcept { return items.begin(); }

		typename collection_t::const_iterator end() const noexcept { return items.end(); }

		typename collection_t::const_iterator cbegin() const noexcept { return items.cbegin(); }

		typename collection_t::const_iterator cend() const noexcept { return items.cend(); }

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
	void Result<::tnt::tensor::einsum::operators::counted_binding>::insert(const ::tnt::tensor::einsum::operators::counted_binding::binding_t &binding) {
		size_ += ::tnt::tensor::einsum::operators::counted_binding::getCount(binding);
		::tnt::tensor::einsum::operators::addToCollection<::tnt::tensor::einsum::operators::counted_binding>(items, binding);
	}

	template <>
	size_t Result<::tnt::tensor::einsum::operators::counted_binding>::size()  const noexcept {
		return size_;
	}

}


#endif //TNT_RESULT_HPP
