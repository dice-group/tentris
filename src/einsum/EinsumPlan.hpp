#ifndef SPARSETENSOR_EINSUM_EINSUMPLAN
#define SPARSETENSOR_EINSUM_EINSUMPLAN

#include <vector>
#include <memory>
#include "NewSubscript.hpp"
#include "Types.hpp"
#include "../util/All.hpp"
#include "../hypertrie/BoolHyperTrie.hpp"

namespace sparsetensor::operations {
    class EinsumPlan {
    protected:
        using BoolHyperTrie = sparsetensor::hypertrie::BoolHyperTrie;
        using Operands =  typename std::vector<BoolHyperTrie *>;
    public:
        class EinsumStep;


    private:
        const NewSubscript &_subscript;
    public:
        explicit EinsumPlan(const NewSubscript &subscript) : _subscript(subscript) {}

        EinsumStep getInitialStep() {
            return EinsumStep{_subscript, _subscript.getResultLabels()};
        }


        class EinsumStep {
        public:
            class Action;

            const NewSubscript &_subscript;
            const std::vector<label_t> &_result_labels;
        public:
            explicit EinsumStep(const NewSubscript &subscript, const std::vector<label_t> &result_labels)
                    : _subscript(subscript), _result_labels(result_labels) {}

            inline const vector<label_t> &getResultLabels() const {
                return _result_labels;
            }

            inline const size_t &getResultSize() const {
                return _result_labels.size();
            }

            Action getAction(Operands ops) {

                return Action{label_t{0}};
            }

            Action getAction(const label_t label) {
                return Action{label};
            }

            class Action {
            public:
                explicit Action(const label_t &label) {}
            };
        };
    };
};


#endif //SPARSETENSOR_EINSUM_EINSUMPLAN
