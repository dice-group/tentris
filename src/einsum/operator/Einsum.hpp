#ifndef SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
#define SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP


#include "../../tensor/MapTensor.hpp"
#include "../../einsum/EvalPlan.hpp"
#include "../../hypertrie/Join.hpp"

#include <parallel/numeric>
#include "../ShapeCalc.hpp"
#include <parallel/algorithm>
#include <iostream>
#include <thread>
#include <omp.h>


using sparsetensor::tensor::Tensor;
using sparsetensor::tensor::HyperTrieTensor;
using sparsetensor::tensor::MapTensor;
using sparsetensor::hypertrie::Join;


namespace sparsetensor::einsum::operators {

    template<typename T>
    class Einsum {
        EvalPlan plan;

        MapTensor<T> *result;
    public:
        Subscript subscript;

        Einsum() : plan({}),
                   subscript({}),
                   result(nullptr) {}

        Einsum(const Subscript &subscript) : plan(EvalPlan{subscript}), subscript(subscript) {

        }

        void rekEinsum(const vector<HyperTrieTensor<T> *> &operands) {
            vector<variant<HyperTrie<T> *, T>> hypertrie_operands{};
            for (HyperTrieTensor<T> *operand : operands) {
                if (operand->ndim == 0) {
                    hypertrie_operands.push_back({operand->trie->leafsum});
                } else {
                    hypertrie_operands.push_back({operand->trie});
                }
            }

            Key_t result_key = Key_t(this->result->ndim, 50);
            auto[step, label] = plan.firstStep(hypertrie_operands);
//            std::cout << "Initial Label: " << label << std::endl;
//            std::cout << "Initial Step: " << step << std::endl;
//            std::cout << "####### start ####" << std::endl;
            rekEinsum(hypertrie_operands, result_key, step, label);
        }


        void rekEinsum(const vector<variant<HyperTrie<T> *, T>> &operands, const Key_t &result_key,
                       PlanStep &step, const label_t &label) {
//            std::cout << "Current Key: " << result_key << std::endl;
//            std::cout << "Current Label: " << label << std::endl;
//            std::cout << "Current Step: " << step << std::endl;
//            std::cout << "Current Tensors: \n";
//            for (const auto &operand : operands) {
//                if (std::holds_alternative<HyperTrie<T> *>(operand))
//                    std::cout << *std::get<HyperTrie<T> *>(operand) << "\n";
//                else
//                    std::cout << std::get<T>(operand) << "\n";
//            }

//            std::cout << "\n";

            if (not step.all_done) {
                Join<T> join{operands, step, label, result_key};
                // TODO: parallelize



                const typename Join<T>::Iterator &join_end = join.end();
//                #pragma omp parallel for private(threadID)
                // __gnu_parallel::for_each
                std::for_each(join.begin(), join.end(), [&](const auto &entry){
                    auto tid = omp_get_thread_num();
                    printf("Hello World from thread = %d\n", tid);
                    std::cout<<std::this_thread::get_id()<<std::endl;
                    const auto &[next_operands, next_result_key] = entry;
//                    std::cout << "####### rek ####" << label << std::endl;
                    auto[next_step, next_label] = plan.nextStep<T>(operands, step, label);
                    rekEinsum(next_operands, next_result_key, next_step, next_label);
                });
//                for(typename Join<T>::Iterator join_it = join.begin(); join_it != join_end; ++join_it){
//                    const auto &[next_operands, next_result_key] = *join_it;
//                    threadID = omp_get_thread_num();
//                    #pragma omp critical
//                    {
//                        printf("Thread %d reporting\n", threadID);
//                    }
//
////                    std::cout << "####### rek ####" << label << std::endl;
//                    auto[next_step, next_label] = plan.nextStep<T>(operands, step, label);
//                    rekEinsum(next_operands, next_result_key, next_step, next_label);
//                }
            } else {
//                std::cout << "####### calc ####" << label << std::endl;
                T result_value = 1;
                for (auto &&operand : operands) {
                    result_value *= std::get<T>(operand);
                }
                const T res_val = result->get(result_key) + result_value;
//                std::cout << "## add " << result_value << "to" << result_key << "results in:" << res_val << std::endl;
                result->set(result_key, res_val);
            }
        }


        MapTensor<T> *getResult(const vector<HyperTrieTensor<T> *> &operands) {
            const shape_t &result_shape = calcShape<T, HyperTrieTensor>(operands, subscript);
            result = new MapTensor<T>(result_shape);
            rekEinsum(operands);
            return result;
        }
    };


}
#endif //SPARSETENSOR_EINSUM_OPERATOR_EINSUM_HPP
