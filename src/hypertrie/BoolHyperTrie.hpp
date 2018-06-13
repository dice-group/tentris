#ifndef SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP
#define SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP

#include "PosCalc.hpp"
#include "Types.hpp"
#include "../tensor/Types.hpp"
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>
#include <limits>
#include <ostream>

using std::ostream;


using std::vector;
using std::unordered_map;
using std::variant;
using std::map;
using std::optional;
using std::tuple;
using sparsetensor::tensor::key_pos_t;
using sparsetensor::tensor::key_part_t;


namespace sparsetensor::hypertrie {


    class BoolHyperTrie {
        using Key_t = sparsetensor::tensor::Key_t;

    public:
        using inner_childs = std::map<key_part_t, BoolHyperTrie *>;
        using leaf_childs = std::set<key_part_t>;

        explicit BoolHyperTrie(key_pos_t depth) : depth(depth), edges_by_pos{
                (depth > 0) ? std::variant<vector<inner_childs>, leaf_childs>{vector<inner_childs>(depth)}
                            : std::variant<vector<inner_childs>, leaf_childs>{leaf_childs{}}} {
        }

        /**
         * Depth is the length of the keys.
         */
        key_pos_t depth{};

        /**
         * Number of entries.
         */
        uint64_t leafcount{};

    private:
        /**
         * edges_by_pos: subkey_pos -> edges;<br/>
         * edges: key_part -> subtrie OR value
         */
        std::variant<vector<inner_childs>, leaf_childs> edges_by_pos;


        // Done
        inner_childs &getEdges(const key_pos_t &subkey_pos) {
            vector<inner_childs> &anAuto = std::get<vector<inner_childs>>(this->edges_by_pos);
            return anAuto.at(subkey_pos);
        }

        // Done
        inline leaf_childs &getLeafs() {
            return std::get<leaf_childs>(this->edges_by_pos);
        }

        // Done
        inline void addChildAsLink(const key_pos_t &pos, key_part_t key_part, BoolHyperTrie *&subhypertrie) {
            inner_childs &edges = getEdges(pos);
            edges.emplace(std::move(key_part), std::move(subhypertrie));
        }

        inline BoolHyperTrie *insertNewChild(const key_pos_t &pos, const key_part_t &key_part) {

            inner_childs &edges = getEdges(pos);
            // no value was passed. So create a new sub HyperTrie of depth this->depth -1 and add it.
            BoolHyperTrie *child_ = new BoolHyperTrie(this->depth - (key_pos_t) 1);
            edges.insert(std::make_pair(key_part, child_));
            // return new value
            return child_;
        }

        // Done
        BoolHyperTrie *getInnerChild(const key_pos_t &subkey_pos, const key_part_t &key_part) {
            inner_childs &edges = getEdges(subkey_pos);
            // find child
            const auto &child_ = edges.find(key_part);
            // return it if it exist
            if (child_ != edges.end()) {
                return child_->second;
            } else {
                throw "The requested child doesn't exist.";
            }
        }

    public:

        /**
         * Get an value or SubTrie by a key.
         * @param key Vector of uint64 coordinates.
         * @return a SubTrie or a value depending on the length of the key.
         */
        variant<BoolHyperTrie *, bool> get(const Key_t &key) {
            vector<std::optional<key_part_t>> intern_key(this->depth);
            for (key_pos_t key_pos = 0; key_pos < key.size(); ++key_pos) {
                intern_key[key_pos] = {key[key_pos]};
            }
            return get(intern_key);
        }


        /**
         * Get an value or SubTrie by a key.
         * @param key Vector of optional uint64 key_parts. If a key_pos is an std::nullopt the key_part for that position is not set resulting in a slice.
         * @return a SubTrie or a value depending if the key contains slices.
         */
        variant<BoolHyperTrie *, bool> get(const vector<std::optional<key_part_t>> &key) {
            if (this->empty()) {
                throw "hypertrie is emtpy.";
            }

            // extract non_slice_key_parts
            map<key_pos_t, key_part_t> non_slice_key_parts{};
            for (key_pos_t key_pos = 0; key_pos < size(key); ++key_pos) {
                if (key[key_pos]) {
                    non_slice_key_parts[key_pos] = *key[key_pos];
                }
            }

            if (non_slice_key_parts.empty()) {
                throw "empty slices are currently not supported.";
                //return {variant<HyperTrie<bool> *, T>{this}}; // TODO: is there a problem with this?
            } else {
                // get child while there are non slice parts in the subkey.
                BoolHyperTrie *current_subtrie = this;
                PosCalc *posCalc = PosCalc::getInstance(this->depth);
                while (not non_slice_key_parts.empty()) {
                    if (non_slice_key_parts.size() == 1) {
                        const auto&[key_pos, key_part] = *non_slice_key_parts.cbegin();

                        if (current_subtrie->depth == 1) {
                            std::set<key_part_t> &entries = current_subtrie->getLeafs();
                            if (entries.find(key_part) != entries.end()) {
                                return true;
                            } else {
                                throw "No value stored for given Key.";
                            }
                        } else {
                            key_pos_t subkey_pos = posCalc->key_to_subkey_pos(key_pos);

                            return current_subtrie->getInnerChild(subkey_pos, non_slice_key_parts[key_pos]);
                        }
                    } else {
                        key_pos_t min_card_key_pos = current_subtrie->getMinCardKeyPos(non_slice_key_parts, posCalc);
                        key_pos_t min_card_subkey_pos = posCalc->key_to_subkey_pos(min_card_key_pos);

                        current_subtrie = current_subtrie->getInnerChild(min_card_subkey_pos,
                                                                         non_slice_key_parts[min_card_key_pos]);

                        non_slice_key_parts.erase(min_card_key_pos);
                        posCalc = posCalc->use(min_card_key_pos);
                    }
                }
            }
        }

        /**
         * Returns the smallest key that is used at the given position.
         * @param key_pos key position.
         * @return samllest key at given position or the maximum key_part_t value if no entry is in this HyperTrie.
         */
        inline key_part_t getMinKeyPart(const key_pos_t &key_pos) {
            try {
                if (this->depth == 1) {
                    return *std::get<leaf_childs>(this->edges_by_pos).cbegin();
                } else {
                    return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).cbegin()->first;
                }
            } catch (...) {}
            return KEY_PART_MAX;
        }

        /**
         * Returns the largest key that is used at the given position.
         * @param key_pos key position.
         * @return largest key at given position or the minimum key_part_t value if no entry is in this HyperTrie.
         */
        inline key_part_t getMaxKeyPart(const key_pos_t &key_pos) const {
            try {
                if (this->depth == 1) {
                    return *std::get<leaf_childs>(this->edges_by_pos).crbegin();
                } else {
                    return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).crbegin()->first;
                }
            } catch (...) {}
            return KEY_PART_MIN;
        }

        /**
         * Looksup the number of children at the given Position.
         * @param key_pos position to check amount of children.
         * @return number of children for given position.
         */
        inline size_t getCard(const key_pos_t &key_pos) const {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).size();
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).size();
            }
        }

        // Todo: correct
        inline vector<size_t> getCards(const vector<key_pos_t> &key_poss) const {
            vector<size_t> cards(key_poss.size());
            for (size_t i = 0; i < key_poss.size(); ++i) {
                cards[i] = getCard(key_poss.at(i));
            }
            return cards;
        }

        key_pos_t getMinCardKeyPos(const map<key_pos_t, key_part_t> &non_slice_key_parts,
                                   const PosCalc *posCalc) const {
            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;
            for (const auto &[key_pos, key_part] : non_slice_key_parts) {
                size_t card = getCard(posCalc->key_to_subkey_pos(key_pos));
                if (card < min_card) {
                    min_card = card;
                    min_card_key_pos = key_pos;
                }
            }
            return min_card_key_pos;
        }

    private:
        void del_rek(const Key_t &key, std::unordered_map<subkey_mask_t, BoolHyperTrie *> &finished_subtries,
                     PosCalc *pos_calc) {
            throw "del_rek not yet implemented.";
        }

        /**
         * Inserts and links the HyperTrie nodes recursively.
         * @param key key to save the value for
         * @param new_value the new value to be safed.
         * @param has_old_value if there was an value for this key before.
         * @param value_diff difference beween former value and current value
         * @param finished_subtries map of finished sub HyperTries
         * @param pos_calc PosCalc object for the current sub HyperTrie
         */
        void set_rek(const Key_t &key, std::unordered_map<subkey_mask_t, BoolHyperTrie *> &finished_subtries,
                     PosCalc *pos_calc) {
            // update this node
            this->leafcount += 1;

            // add it to the finished ( means updated ) nodes.
            finished_subtries[pos_calc->getSubKeyMask()] = this;

            // subtrie has only one position left: insert value
            if (pos_calc->subkey_length == 1) {
                key_part_t key_part = key.at(pos_calc->subkey_to_key_pos(0));

                leaf_childs &leafs = this->getLeafs();

                leafs.emplace(key_part);

            } else { // depth > 1 -> inner node
                // a child must be set or updated for every subkey_pos available.
                for (const key_pos_t key_pos : pos_calc->getKeyPoss()) {
                    key_part_t key_part = key.at(key_pos);

                    // get pos_calc for next child and check if it was already updated earlier.
                    PosCalc *next_pos_calc = pos_calc->use(key_pos);
                    const auto &finished_child = finished_subtries.find(next_pos_calc->getSubKeyMask());

                    try {
                        // get the child at the current position.
                        BoolHyperTrie *child_ = getInnerChild(pos_calc->key_to_subkey_pos(key_pos), key_part);
                        // the child exists ...
                        // ... and the subtrie starting with the child was not already finished:
                        if (finished_child == finished_subtries.end()) {
                            // call this function for the child
                            child_->set_rek(key, finished_subtries, next_pos_calc);
                        }
                    } catch (...) {
                        // the child does not exist ...
                        // ... and the subtrie starting with the child was already finished:
                        if (finished_child != finished_subtries.end()) {
                            // set the child at this node
                            BoolHyperTrie *child = finished_child->second;
                            this->addChildAsLink(pos_calc->key_to_subkey_pos(key_pos), key_part, child);

                        } else { // ... and the subtrie starting with the child was not already finished:
                            // set a new child and call this function for the child
                            BoolHyperTrie *new_child = this->insertNewChild(pos_calc->key_to_subkey_pos(key_pos),
                                                                            key_part);
                            new_child->set_rek(key, finished_subtries, next_pos_calc);
                        }
                    }
                }
            }
        }

    public:

        /**
         * Set a value for a key.
         * @param key key to the value
         * @param value value to be set
         */
        void set(const Key_t &key, const bool &value) {
            if (key.size() != this->depth) {
                throw "Key length must match HyperTrie->depth";
            }
            // check if there is already another value for this subkey.
            try {
                get(key); // fails if key is not stored
                if (value) {
                    // cache for already created sub HyperTries.
                    std::unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

                    // get pos_calc for this.
                    subkey_mask_t subkey_mask(key.size());
                    PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

                    // remove subkey recursively
                    del_rek(key, finished_subtries, pos_calc);
                    return;
                } else {
                    throw "Delete not supported yet.";
                }
            } catch (...) {
                if (value) {
                    // cache for already created sub HyperTries.
                    std::unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

                    // get pos_calc for this.
                    subkey_mask_t subkey_mask(key.size());
                    PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

                    // store subkey recursively
                    set_rek(key, finished_subtries, pos_calc);
                } else {
                    return;
                }
            }


        }

        void del(const Key_t &coords) {
            throw "Not yet implemented.";
        }

        inline bool empty() const noexcept {
            return leafcount == 0;
        }

        variant<leaf_childs::iterator, inner_childs::iterator> begin_(const key_pos_t &key_pos) {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).begin();
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).begin();
            }
        }

        variant<leaf_childs::iterator, inner_childs::iterator> end_(const key_pos_t &key_pos) {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).end();
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).end();
            }
        }

        variant<leaf_childs::const_iterator, inner_childs::const_iterator> cbegin_(const key_pos_t &key_pos) {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).cbegin();
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).cbegin();
            }
        }

        variant<leaf_childs::const_iterator, inner_childs::const_iterator> cend_(const key_pos_t &key_pos) {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).cend();
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).cend();
            }
        }

        variant<leaf_childs::iterator, inner_childs::iterator> lower_bound(key_part_t min_key_part = KEY_PART_MIN,
                                                                           key_pos_t key_pos = 0) {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).lower_bound(min_key_part);
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).lower_bound(min_key_part);
            }
        }

        variant<leaf_childs::iterator, inner_childs::iterator>
        upper_bound(key_part_t max_key_part = KEY_PART_MAX, key_pos_t key_pos = 0) {
            if (this->depth == 1) {
                return std::get<leaf_childs>(this->edges_by_pos).upper_bound(max_key_part);
            } else {
                return std::get<vector<inner_childs>>(this->edges_by_pos).at(key_pos).upper_bound(max_key_part);
            }
        }

        friend ostream &operator<<(ostream &out, BoolHyperTrie &trie) {
            out << "<BoolHyperTrie: depth=" << int(trie.depth) << ", leafcount=" << int(trie.leafcount) << ">";
            return out;
        }

    };
}

#endif //SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP


