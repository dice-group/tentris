#ifndef TNT_OPERATORNODE_HPP
#define TNT_OPERATORNODE_HPP

#include "tnt/tensor/einsum/operator/GeneratorInterface.hpp"
#include "tnt/tensor/Result.hpp"

namespace tnt::tensor::einsum::operators {

	enum OperatorType {
		EINSUM,
		CROSSPRODUCT
	};

	template<typename RESULT_TYPE, typename = typename std::enable_if<is_binding<RESULT_TYPE>::value>::type>
	class OperatorNode {
	public:
		OperatorType type;

		virtual yield_pull <RESULT_TYPE> get() const = 0;

		virtual const Result <RESULT_TYPE> &getFullResult() const = 0;

		virtual void clearCacheCanceled() const = 0;

		virtual void clearCacheDone() const = 0;
	};
}


#endif //TNT_OPERATORNODE_HPP
