#ifndef SPARSETENSOR_HYPERTRIE_DIAGONAL_HPP
#define SPARSETENSOR_HYPERTRIE_DIAGONAL_HPP

#include "../einsum/Types.hpp"
#include <cstdint>
#include <variant>
#include <optional>
#include <tuple>
#include <vector>
#include "HyperTrie.hpp"
#include "../einsum/Subscript.hpp"
#include "Types.hpp"


using ::std::variant;
using ::std::optional;
using ::std::tuple;
using ::std::vector;
using sparsetensor::einsum::label_pos_t;
using sparsetensor::tensor::key_pos_t;
using sparsetensor::tensor::key_part_t;

namespace sparsetensor::hypertrie {

    template<typename T>
    class Diagonal {
    public:

        HyperTrie<T> *hyperTrie;
        key_pos_t min_card_key_pos{};
        const vector<label_pos_t> label_poss;
        key_part_t min_key = KEY_PART_MIN;
        key_part_t max_key = KEY_PART_MAX;

        class Iterator;

        Diagonal(HyperTrie<T> *hyperTrie, vector<label_pos_t> label_poss) : hyperTrie(hyperTrie),
                                                                             label_poss(label_poss) {
            min_card_key_pos = hyperTrie->getMinCardKeyPos();
        }

        ~Diagonal() {
            //delete iter;
        }


        size_t estimCard() {
            return hyperTrie->getCard(min_card_key_pos);
        }

        key_part_t min() {
            return hyperTrie->getMinKeyPart(min_card_key_pos);
        }

        key_part_t max() {
            return hyperTrie->getMaxKeyPart(min_card_key_pos);;
        }

        optional<variant<HyperTrie<T> *, T>> find(const key_part_t &key_part) {
            const vector<optional<key_part_t>> &key = vector<optional<key_part_t >>(hyperTrie->depth);
            for (const auto &label_pos :label_poss) {
                key[label_pos] = {key_part};
            }

            return hyperTrie->get(key);

        }

        void setLowerBound(key_part_t min_key) {
            this->min_key = min_key;
        }

        void setUpperBound(key_part_t max_key) {
            this->max_key = max_key;
        }

        tuple<Iterator, Iterator> getIterator() {
            Iterator iter{hyperTrie, label_poss, min_card_key_pos, min_key, max_key};
            Iterator iter_end{iter.iter_end};

            return {iter, iter_end};
        }

        class Iterator {
        public:

            typedef typename ::map<key_part_t, variant<HyperTrie<T> *, T>>::iterator map_iterator;
        private:

            /**
             * Initializes this->key_poss by decreasing the key_pos of all key_poss after min_card_key_pos by one. This is due
             * to the fact that sub-HyperTries these key_poss refer to is obtained by resloving the key_pos min_card_key_pos.
             * @param key_poss key_poss of partent
             * @param min_card_key_pos key_pos_t that is removed in child
             */
            void
            init_poss(const vector<key_pos_t> &key_poss, const key_pos_t &min_card_key_pos) {// init key_poss
                bool seen_key_pos = false;
                for (const key_pos_t &key_pos : key_poss)
                    if (key_pos != min_card_key_pos)
                        if (not seen_key_pos)
                            this->key_poss.push_back(key_pos);
                        else
                            this->key_poss.push_back(key_pos_t(key_pos - 1));
                    else
                        seen_key_pos = true;
            }

        public:
            Iterator(HyperTrie<T> *&hyperTrie,
                     const vector<key_pos_t> &key_poss,
                     key_pos_t min_card_key_pos,
                     key_part_t min_key_part = KEY_PART_MIN,
                     key_part_t max_key_part = KEY_PART_MAX
            ) {
                init_poss(key_poss, min_card_key_pos);

                // init subkey
                subkey.resize(this->key_poss.size());

                map<key_part_t, std::variant<HyperTrie<T> *, T>>
                        *edges = hyperTrie->edges_by_pos.at(min_card_key_pos);
                this->iter = edges->lower_bound(min_key_part);

                this->iter_end = edges->upper_bound(max_key_part);
                this->current_key_part = min_key_part;
                operator++();
            }

            Iterator(map_iterator iter_end) :
                    iter(iter_end),
                    iter_end(iter_end),
                    end(true) {}

        private:
            map_iterator iter;

            map_iterator iter_end;
            key_part_t current_key_part{};
            bool end{};
            vector<key_pos_t> key_poss{};
            vector<std::optional<key_part_t>> subkey{};

            variant<HyperTrie<T> *, T> current_sub_trie{};

        public:

            Iterator &operator++() {
                while (iter != iter_end) {
                    // get next current_key_part
                    current_key_part = iter->first;
                    // get next result for min-card position
                    variant<HyperTrie<T> *, T> &child = iter->second;
                    if (subkey.size() == 0) {
                        current_sub_trie = &child;
                        iter++;
                        break;
                    } else {
                        // write the current_key_part to the relevant positions of the subkey
                        for (const key_pos_t &key_pos : key_poss) {
                            subkey[key_pos] = current_key_part;
                        }

                        // check if the same current_key_part exists also for the other relevant key_pos
                        optional<variant<HyperTrie<T> *, T>> &&result = std::get<HyperTrie<T> *>(child)->get(subkey);
                        if (result) {
                            current_sub_trie = *result;
                            iter++;
                            break;
                        } else {
                            iter++;
                            continue;
                        }
                    }

                }
                return *this;
            }

            Iterator operator++(int) {
                Iterator it_copy{*this};
                operator++();
                return it_copy;
            }

            tuple<key_part_t, variant<HyperTrie<T> *, T>> operator*() {
                return {current_key_part, current_sub_trie};
            }

            bool operator==(const Iterator &rhs) const {
                if (rhs.end && end)
                    return true;
                else if (rhs.end && rhs.current_key_part <= current_key_part)
                    return true;
                else
                    return current_key_part == rhs.current_key_part;
            }

            bool operator!=(const Iterator &rhs) const { // todo: check if it is right
                if (rhs.end && end)
                    return false;
                else if (rhs.end && rhs.current_key_part <= current_key_part)
                    return false;
                else
                    return current_key_part != rhs.current_key_part;
            }
        };

    };
}


#endif //TSPARSETENSOR_HYPERTRIE_DIAGONAL_HPP
