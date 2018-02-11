#ifndef SPARSETENSOR_HYPERTRIE_DIAGONAL_HPP
#define SPARSETENSOR_HYPERTRIE_DIAGONAL_HPP

#include "../einsum/Types.hpp"
#include "HyperTrie.hpp"
#include "Types.hpp"
#include <cstdint>
#include <variant>
#include <optional>
#include <tuple>
#include <vector>
#include <parallel/numeric>


using std::variant;
using std::optional;
using std::tuple;
using std::vector;
using sparsetensor::einsum::label_pos_t;
using sparsetensor::tensor::key_pos_t;
using sparsetensor::tensor::key_part_t;

namespace sparsetensor::hypertrie {

    /**
     * Represents a diagonal of a hypertrie. A diagonal is when one or more key_parts are resolved with the same value.
     * @tparam T content type of the HyperTrie
     */
    template<typename T>
    class Diagonal {

        /**
         * Pointer to the hypertrie that is diagonalized.
         */
        HyperTrie<T> *hypertrie;
        /**
         * The key_pos_t of the hypertrie with minimal cardinality.
         */
        key_pos_t min_card_key_pos;
        /**
         * The key_pos_t's that the hypertrie is diagonalized by.
         */
        const vector<label_pos_t> key_poss;
        /**
         * minimal key part in diagonal.
         */
        key_part_t min_key_part = KEY_PART_MIN;

        /**
         * maximal key part in diagonal.
         */
        key_part_t max_key_part = KEY_PART_MAX;

        /**
         * estimated cardinality.
         */
        size_t estimated_card = SIZE_MAX;

    public:
        class Iterator;

        explicit Diagonal(HyperTrie<T> *hyperTrie, vector<label_pos_t> key_poss) :
                hypertrie(hyperTrie),
                key_poss(key_poss),
                min_card_key_pos(hyperTrie->getMinCardKeyPos()) {
            min_key_part = hypertrie->getMinKeyPart(min_card_key_pos);
            max_key_part = hypertrie->getMaxKeyPart(min_card_key_pos);
            estimated_card = hypertrie->getCard(min_card_key_pos);
        }

        /**
         * estimated cardinality.
         * @return estimated cardinality.
         */
        const size_t &estimCard() const {
            return estimated_card;
        }

        /**
         * minimal key part in diagonal.
         * @return minimal key part in diagonal.
         */
        const key_part_t &min() const {
            return min_key_part;
        }

        /**
         * maximal key part in diagonal.
         * @return maximal key part in diagonal.
         */
        const key_part_t &max() const {
            return max_key_part;
        }

        /**
         * Find value for key_part binding in diagonal.
         * @param key_part binding for key_poss
         * @return a optional Hypertrie or value variant
         */
        optional<variant<HyperTrie<T> *, T>> find(const key_part_t &key_part) const {
            if (key_part < this->min_key_part || key_part > this->max_key_part) {
                return std::nullopt;
            }

            vector<optional<key_part_t>> key(hypertrie->depth);
            for (const auto &label_pos :key_poss) {
                key.at(label_pos) = {key_part};
            }

            return hypertrie->get(key);
        }

        /**
         * Set the lower bound for key parts.
         * @param min_key_t lower bound for key parts
         */
        void setLowerBound(const key_part_t min_key_t) {
            this->min_key_part = min_key_t;
        }

        /**
         * Set the upper bound for key parts.
         * @param min_key_t lower bound for key parts
         */
        void setUpperBound(const key_part_t max_key_t) {
            this->max_key_part = max_key_t;
        }

        /**
         * Constant Iterator over key_part bindings and variants of subhypertries or values.
         * @return const iterator
         */
        Iterator begin() const {
            return Iterator{*this};
        }

        /**
         * Constant Iterator end over key_part bindings and variants of subhypertries or values.
         * @return end of const iterator
         */
        Iterator end() const {
            return Iterator{*this, true};
        }

        class Iterator {
            friend class Diagonal<T>;

            /**
             * key pos iterator over the hypertrie -> subhypertrie = *hypertrie_iter
             */
            typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator hypertrie_iter;

            /**
             * max key_part candidate
             */
            const key_part_t &max_key_part;

            /**
             * If the end is reached.
             */
            bool ended = false;
            /**
             * All relevant key_pos_t in the subkey where the min card key_pos_t is already resolved.
             */
            vector<key_pos_t> key_poss_in_subkey;
            /**
             * Subkey to query the subhypertie
             */
            vector<optional<key_part_t>> subkey;

            /**
             * the current keypart to be returned
             */
            key_part_t current_key_part{};
            /**
             * the current subsubhypertrie to be returned
             */
            variant<HyperTrie<T> *, T> *current_subsubhypertrie;

            /**
             * Assume you've got a subtrie where a key_part_t at min_card_key_pos was resolved. Then this calculates the
             * new key_pos_t of the desired diagonal.
             * @param key_poss key_poss of hypertrie
             * @param min_card_key_pos key_pos_t in hypertrie with minimal cardinality
             */
            static vector<key_pos_t> calcKeyPossInSubkey(const vector<key_pos_t> &key_poss,
                                                         const key_pos_t &min_card_key_pos) {
                vector<key_pos_t> key_poss_in_subkey{};
                bool seen_key_pos = false;
                for (const key_pos_t &key_pos : key_poss)
                    if (key_pos == min_card_key_pos) {
                        seen_key_pos = true;
                    } else {
                        if (not seen_key_pos)
                            key_poss_in_subkey.push_back(key_pos);
                        else
                            key_poss_in_subkey.push_back(key_pos - uint8_t(1));
                    }
                return key_poss_in_subkey;
            }

        public:
            Iterator(const Diagonal<T> &diagonal,
                     const bool ended = false) :
                    key_poss_in_subkey(calcKeyPossInSubkey(diagonal.key_poss, diagonal.min_card_key_pos)),
                    subkey(vector<optional<key_part_t>>(diagonal.key_poss.size() - 1)),
                    hypertrie_iter(diagonal.hypertrie->lower_bound(diagonal.min_key_part, diagonal.min_card_key_pos)),
                    max_key_part(diagonal.max_key_part),
                    current_subsubhypertrie(nullptr) {
                this->ended = ended;
                if (not ended)
                    this->current_key_part = diagonal.min_key_part;
                else
                    this->current_key_part = std::max(diagonal.max_key_part, diagonal.max_key_part + 1);
                operator++();
            }


            Iterator &operator++() {
                if (not ended) {
                    while (this->current_key_part < max_key_part) {
                        auto &[current_key_part, current_subhypertrie] = *(this->hypertrie_iter);
                        if (subkey.size() == 0) { // there is no subkey ( = diagonal along only one key_pos_t)
                            this->current_key_part = current_key_part;
                            this->current_subsubhypertrie = &current_subhypertrie;
                            ++hypertrie_iter;
                            return *this;
                        } else { // there is a subkey ( = diagonal along multiple key_pos_t's)
                            // write the current_key_part to the relevant positions of the subkey
                            for (const key_pos_t &key_pos : key_poss_in_subkey) {
                                subkey.at(key_pos) = current_key_part;
                            }

                            // check if the same current_key_part exists also for the other relevant key_pos
                            ++hypertrie_iter;
                            this->current_key_part = current_key_part;
                            try {
                                variant<HyperTrie<T> *, T> &subsubhypertrie = std::get<HyperTrie<T> *>(current_subhypertrie)->get(subkey);
                                this->current_subsubhypertrie = &subsubhypertrie;
                                return *this;
                            }
                            catch (...) {
                                continue;
                            }
                        }
                    }
                    ended = true;
                }

                return *this;
            }

            Iterator operator++(int) {
                operator++();
                return *this;
            }

            tuple<key_part_t &, variant<HyperTrie<T> *, T> &> operator*() {
                return {current_key_part, *current_subsubhypertrie};
            }

            bool operator==(const Iterator &rhs) const {
                // if both ended they are equal
                if (rhs.ended && ended)
                    return true;
                    // if rhs ended and lhs's key_pos is greater then rhs's
                else if (rhs.ended && rhs.current_key_part <= current_key_part)
                    return true;
                    // the same the other way around
                else if (ended && current_key_part <= rhs.current_key_part)
                    return true;
                    // both key_parts are equal
                else
                    return current_key_part == rhs.current_key_part;
            }

            bool operator!=(const Iterator &rhs) const {
                return not this->operator==(rhs);
            }

            inline const bool &hasEnded() const {
                return ended;
            }
        };
    };
}


#endif //TSPARSETENSOR_HYPERTRIE_DIAGONAL_HPP
