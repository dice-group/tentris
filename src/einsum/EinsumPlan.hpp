#ifndef SPARSETENSOR_EINSUM_EINSUMPLAN
#define SPARSETENSOR_EINSUM_EINSUMPLAN

#include <vector>
#include <memory>

#include "Subscript.hpp"
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
        const Subscript &_subscript;
    public:
        explicit EinsumPlan(const Subscript &subscript) : _subscript(subscript) {}

        EinsumStep getInitialStep() {
            return EinsumStep{_subscript, _subscript.getResultLabels()};
        }

        inline const Subscript &getSubscript() const {
            return _subscript;
        }


        class EinsumStep {
        public:
            class Action;

            const Subscript &_subscript;
            const std::vector<label_t> &_result_labels;
        public:
            explicit EinsumStep(const Subscript &subscript, const std::vector<label_t> &result_labels)
                    : _subscript(subscript), _result_labels(result_labels) {}

            inline const std::vector<label_t> &getResultLabels() const {
                return _result_labels;
            }

            inline size_t getResultSize() const {
                return _result_labels.size();
            }

            Action getAction(Operands ops) {

                return Action{*this, label_t{0}};
            }

            Action getAction(const label_t label) {
                return Action{*this, label};
            }

            class Action {
                const EinsumStep &_step;
            public:
                bool all_done = true;

                // TODO: implement
                explicit Action(const EinsumStep &step, const label_t &label) : _step{_step} {}

                // TODO: implement
                Action nextAction(const label_t &label) {
                    return Action{_step, label};
                }
            };
        };
    };
};


#endif //SPARSETENSOR_EINSUM_EINSUMPLAN
