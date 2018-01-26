//
// Created by me on 1/26/18.
//

#ifndef LIBSPARSETENSOR_JOINITERATOR_HPP
#define LIBSPARSETENSOR_JOINITERATOR_HPP

#include "HyperTrie.hpp"
#include <iterator>
#include <tuple>
#include <variant>
#include <vector>
#include <einsum/Subscript.hpp>
#include <einsum/EvalPlan.hpp>
#include <optional>


namespace HyperTrie {
    using std::tuple;
    using std::set;
    using std::optional;

    using HyperTrie::key_pos_t;
    using HyperTrie::key_part_t;

/**
 * This class represents all sub-HyperTries that arise from matching one or multiple dimensions of one or multiple
 * HyperTries together with their respective key part.
 */
    template<typename T>
    class JoinIterator : public std::iterator<std::input_iterator_tag,
            tuple<key_part_t, vector<variant<HyperTrie *, T>>>> {
    public:
        JoinIterator(const vector<HyperTrie *> &HyperTries, PlanStep *planStep, label_t label)
                : hyperTries(
                HyperTries), planStep(planStep), label(label) {}

    private:
        vector<HyperTrie *> hyperTries;
        PlanStep *planStep;
        label_t label;
        key_part_t key_part{};
    public:

        JoinIterator &operator++() {
            const set<op_pos_t> &op_poss = planStep->operandsWithLabel(label);
            const std::optional<label_pos_t> &key_pos = planStep->labelPosInResult(label);
            
            if(size(op_poss) == 1){
                const op_pos_t &op_pos = *begin(op_poss);
                const HyperTrie * op = hyperTries.at(op_pos);
                if (op->leafcount > 0){
                    const vector<label_pos_t> &label_poss = planStep->labelPossInOperand(op_pos, label);
                    // TODO: 3
                }

                
                
            }


            return *this;
        }

        JoinIterator operator++(int) {
            JoinIterator it_copy{*this};
            operator++();
            return it_copy;
        }

        bool operator==(const JoinIterator &rhs) const {
            // TODO: does this make sense?
            return key_part == rhs.key_part;
        }

        bool operator!=(const JoinIterator &rhs) const {
            // TODO: does this make sense?
            return key_part != rhs.key_part;
        }


    };

}

#endif //LIBSPARSETENSOR_JOINITERATOR_HPP
