#ifndef TENTRIS_OPERATORNODE_HPP
#define TENTRIS_OPERATORNODE_HPP

#include "tentris/tensor/Result.hpp"
#include "tentris/tensor/einsum/operator/GeneratorInterface.hpp"

#include <chrono>

namespace tentris::tensor::einsum::operators {

	class CancelProcessing : public std::exception {
		const char *what() const throw() override {
			return "canceled evaluation";
		}
	};

	enum OperatorType {
		EINSUM,
		CROSSPRODUCT
	};

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	class OperatorNode {
	protected:
		std::chrono::system_clock::time_point timeout;
		size_t cache_bucket_size;

		explicit OperatorNode(size_t cache_bucket_size) : cache_bucket_size{cache_bucket_size} {}

	public:
		virtual ~OperatorNode() = default;

	public:


		OperatorType type;

		virtual yield_pull <RESULT_TYPE> get() const = 0;

		virtual const Result <RESULT_TYPE> &getFullResult() const = 0;

		virtual void clearCacheCanceled() const = 0;

		virtual void clearCacheDone() const = 0;

		virtual void setTimeout(const std::chrono::system_clock::time_point &timeout) {
			this->timeout = timeout;
			this->cache_bucket_size = cache_bucket_size;
		}
	};
} // namespace tentris::tensor::einsum::operators

#endif // TENTRIS_OPERATORNODE_HPP
