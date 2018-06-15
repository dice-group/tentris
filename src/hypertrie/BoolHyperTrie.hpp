#ifndef SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP
#define SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP

#include "PosCalc.hpp"
#include "Types.hpp"
#include "../tensor/Types.hpp"
#include "../container/VecMap.hpp"
#include "../container/VecSet.hpp"
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>
#include <limits>
#include <ostream>

namespace sparsetensor::hypertrie {
    using sparsetensor::tensor::Key_t;
    using std::vector;
    using std::unordered_map;
    using std::variant;
    using std::map;
    using std::optional;
    using std::tuple;
    using sparsetensor::tensor::key_pos_t;
    using sparsetensor::tensor::key_part_t;
    using std::ostream;

    class BoolHyperTrie {
        // TODO: write destructor
        // TODO: implement del


    public:
        /**
         * Inner edges are encoded by mapping a key_part to an subhypertrie. Only key_parts that map to an non-zero
         * subhypertrie are stored. A inner_edges is used for every position of the stored keys.
         */
        using inner_edges = sparsetensor::container::VecMap<key_part_t, BoolHyperTrie *>;
        /**
         * Leaf edges are encoded by storing only the key_parts that map to a true. All other key parts map to zero.
         */
        using leaf_edges = sparsetensor::container::VecSet<key_part_t>;

        /**
         * The default constructor creates a empty BoolHyperTrie with depth = 1.
         */
        BoolHyperTrie() : _depth(1), _subtries{leaf_edges{}} {}

        /**
         * Creates a empty BoolHyperTrie with given depth.
         * @param depth depth of the BoolHyperTrie
         */
        explicit BoolHyperTrie(key_pos_t depth) : _depth(depth), _subtries{} {
            if (depth > 1) {
                _subtries = vector<inner_edges>(depth);
            } else {
                _subtries = leaf_edges{};
            }
        }


    private:
        /**
         * The depth defines the length of keys.
         */
        key_pos_t _depth{};

    public:
        /**
         * Get the depth of the BoolHyperTrie.
         * @return depth of this
         */
        inline const key_pos_t &depth() const noexcept {
            return _depth;
        }

    private:
        /**
         * Number of entries.
         */
        uint64_t _leafcount{};

    public:
        /**
         * Get the number of none-zero elements stored.
         * @return number of stored none-zero elements
         */
        inline const uint64_t &size() const noexcept {
            return _leafcount;
        }

    private:
        /**
         * Stores the subtries. For depth > 1 a vector of length equal to depth stores for every position in the keys to
         * the non-zero elements a map that holds the subtries for that position. The map stores then for every key_part
         * used at any key to a non-zero element at that position the proper subBoolHyperTrie.
         */
        variant<vector<inner_edges>, leaf_edges> _subtries{};


        /**
         * Returns the inner edges for a given position of this BoolHyperTrie.
         * @param subkey_pos key_position in this hypertrie. May differ from the position in an parent BoolHyperTrie.
         * @return a map of inner_edges.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth() 
         */
        inner_edges &getInnerEdges(const key_pos_t &subkey_pos) {
            std::get<vector<inner_edges>>(this->_subtries).at(subkey_pos);
        }

        /**
         * Returns leaf edges.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is not 1
         * @return a set of leaf edges.
         */
        inline leaf_edges &getLeafEdges() {
            return std::get<leaf_edges>(this->_subtries);
        }

        /**
         * Adds an Child BoolHyperTrie at the given position and for the given key_part.
         * @param pos position of the key_part
         * @param key_part a key_part that must not be in use at this position and BoolHyperTrie.
         * @param subhypertrie A pointer to a BoolHyperTrie. For consistency it must have a depth = --this->depth(). The
         * argument is consumed and thus must not be used again after calling this function.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth()
         */
        inline void addChildAsPointer(const key_pos_t &pos, key_part_t key_part, BoolHyperTrie *&subhypertrie) {
            inner_edges &edges = getInnerEdges(pos);
            edges.setItem(key_part, subhypertrie);
        }

        /**
         * Creates a new Child BoolHyperTrie at the given position and for the given key_part.
         * @param key_pos position of the key_part
         * @param key_part a key_part that must not be in use at this position and BoolHyperTrie.
         * @return a pointer to the created child BoolHyperTrie
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth()
         */
        inline BoolHyperTrie *insertNewChild(const key_pos_t &key_pos, const key_part_t &key_part) {
            inner_edges &edges = getInnerEdges(key_pos);
            // no value was passed. So create a new sub HyperTrie of _depth this->_depth -1 and add it.
            BoolHyperTrie *child_ = new BoolHyperTrie(this->_depth - (key_pos_t) 1);
            edges.setItem(key_part, child_);
            // return new value
            return child_;
        }

        /**
         * Get an child of an inner node at the given position and for the given key_part.
         * @param key_pos position of the key_part
         * @param key_part key_part where to look for the child
         * @return the child
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth() or the key doesn't exist
         */
        BoolHyperTrie *getInnerChild(const key_pos_t &key_pos, const key_part_t &key_part) {
            inner_edges &edges = getInnerEdges(key_pos);
            // find child
            return edges.at(key_part);
        }

    public:

        /**
         * Get an value or sub BoolHyperTrie by a key.
         * @param key key to the requested part
         * @return a sub BoolHyperTrie or a value depending on the length of the key.
         * @throws the entry doesn't exist
         */
        variant<BoolHyperTrie *, bool> get(const Key_t &key) {
            vector<optional<key_part_t>> intern_key(this->_depth);
            for (key_pos_t key_pos = 0; key_pos < key.size(); ++key_pos) {
                intern_key[key_pos] = {key[key_pos]};
            }
            return get(intern_key);
        }


        /**
         * Get an value or sub BoolHyperTrie by a key.
         * @param key Vector of optional key_parts. If a key_pos is an std::nullopt the key_part for that position is
         * not set resulting in a slice.
         * @return a sub BoolHyperTrie or a value depending if the key contains slices.
         */
        variant<BoolHyperTrie *, bool> get(const vector<optional<key_part_t>> &key) {
            if (this->empty()) {
                throw "hypertrie is emtpy.";
            }

            // extract non_slice_key_parts
            map<key_pos_t, key_part_t> non_slice_key_parts{};
            for (key_pos_t key_pos = 0; key_pos < key.size(); ++key_pos) {
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
                PosCalc *posCalc = PosCalc::getInstance(this->_depth);
                while (not non_slice_key_parts.empty()) {
                    if (non_slice_key_parts.size() == 1) {
                        const auto&[key_pos, key_part] = *non_slice_key_parts.cbegin();

                        if (current_subtrie->_depth == 1) {
                            leaf_edges &entries = current_subtrie->getLeafEdges();
                            if (entries.contains(key_part))
                                return true;
                            else
                                throw "No value stored for given Key.";
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
         * @param key_pos position of the key_part
         * @return smallest key_part at given position or the maximum key_part_t value if no entry is in this BoolHyperTrie.
         */
        inline key_part_t getMinKeyPart(const key_pos_t &key_pos = 0) {
            try {
                if (this->_depth == 1) {
                    return std::get<leaf_edges>(this->_subtries).min();
                } else {
                    return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).minKey();
                }
            } catch (...) {}
            return KEY_PART_MAX;
        }

        /**
         * Returns the largest key that is used at the given position.
         * @param key_pos position of the key_part
         * @return largest key_part at given position or the minimum key_part_t value if no entry is in this BoolHyperTrie.
         */
        inline key_part_t getMaxKeyPart(const key_pos_t &key_pos = 0) const {
            try {
                if (this->_depth == 1) {
                    return std::get<leaf_edges>(this->_subtries).max();
                } else {
                    return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).maxKey();
                }
            } catch (...) {}
            return KEY_PART_MIN;
        }

        /**
         * Lookup the number of children at the given position.
         * @param key_pos position to check amount of children.
         * @return number of children for given position.
         */
        inline size_t getCard(const key_pos_t &key_pos) const {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).size();
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).size();
            }
        }

        /**
         * Get the cardinalities (numbers of direct children) for the given key positions.
         * @param key_poss vector of key positions
         * @return cardinalities at the requested positions
         */
        inline vector<size_t> getCards(const vector<key_pos_t> &key_poss) const {
            vector<size_t> cards(key_poss.size());
            for (size_t i = 0; i < key_poss.size(); ++i) {
                cards[i] = getCard(key_poss.at(i));
            }
            return cards;
        }

        /**
         * Given a map that has key_pos keys, return the key_pos out of them with the minimum cardinality.
         * @param map_from_key_pos a map that has key_pos as keys
         * @param posCalc a position calculator
         * @return the minimum key position.
         */
        key_pos_t getMinCardKeyPos(const map<key_pos_t, key_part_t> &map_from_key_pos,
                                   const PosCalc *posCalc) const {
            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;
            for (const auto &[key_pos, key_part] : map_from_key_pos) {
                size_t card = getCard(posCalc->key_to_subkey_pos(key_pos));
                if (card < min_card) {
                    min_card = card;
                    min_card_key_pos = key_pos;
                }
            }
            return min_card_key_pos;
        }

    private:
        void del_rek(const Key_t &key, unordered_map<subkey_mask_t, BoolHyperTrie *> &finished_subtries,
                     PosCalc *pos_calc) {
            throw "del_rek not yet implemented.";
        }

        /**
         * Inserts and links the HyperTrie nodes recursively.
         * @param key key to save the value for
         * @param finished_subtries map of finished sub HyperTries
         * @param pos_calc PosCalc object for the current sub HyperTrie
         */
        void set_rek(const Key_t &key, unordered_map<subkey_mask_t, BoolHyperTrie *> &finished_subtries,
                     PosCalc *pos_calc) {
            // update this node
            this->_leafcount += 1;

            // add it to the finished ( means updated ) nodes.
            finished_subtries[pos_calc->getSubKeyMask()] = this;

            // subtrie has only one position left: insert value
            if (pos_calc->subkey_length == 1) {
                key_part_t key_part = key.at(pos_calc->subkey_to_key_pos(0));

                leaf_edges &leafs = this->getLeafEdges();

                leafs.add(key_part);

            } else { // _depth > 1 -> inner node
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
                            this->addChildAsPointer(pos_calc->key_to_subkey_pos(key_pos), key_part, child);

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
         * @throws "Key length must match HyperTrie->_depth";
         * @throws "Delete not supported yet."
         */
        void set(const Key_t &key, const bool &value) {
            if (key.size() != this->_depth) {
                throw "Key length must match HyperTrie->_depth";
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
                    unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

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

        /**
         * Returns if there are no nonzero elements in the trie.
         * @return true if it is empty
         */
        inline bool empty() const noexcept {
            return _leafcount == 0;
        }

        variant<leaf_edges::iterator, inner_edges::iterator> begin_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).begin();
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).begin();
            }
        }

        variant<leaf_edges::iterator, inner_edges::iterator> end_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).end();
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).end();
            }
        }

        variant<leaf_edges::const_iterator, inner_edges::const_iterator> cbegin_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).cbegin();
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).cbegin();
            }
        }

        variant<leaf_edges::const_iterator, inner_edges::const_iterator> cend_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).cend();
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).cend();
            }
        }

        variant<leaf_edges::iterator, inner_edges::iterator> lower_bound(key_part_t min_key_part = KEY_PART_MIN,
                                                                         key_pos_t key_pos = 0) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).lower_bound(min_key_part);
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).lower_bound(min_key_part);
            }
        }

        variant<leaf_edges::iterator, inner_edges::iterator>
        upper_bound(key_part_t max_key_part = KEY_PART_MAX, key_pos_t key_pos = 0) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).upper_bound(max_key_part);
            } else {
                return std::get<vector<inner_edges>>(this->_subtries).at(key_pos).upper_bound(max_key_part);
            }
        }

        friend ostream &operator<<(ostream &out, BoolHyperTrie &trie) {
            out << "<BoolHyperTrie: _depth=" << int(trie._depth) << ", leafcount=" << int(trie._leafcount) << ">";
            return out;
        }

    };
}

#endif //SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP


